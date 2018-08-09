#include "ERN15B11ElasticScattering.h"

// STD
#include <iostream>

//ROOT
#include <TMath.h>
#include <TLorentzVector.h>
#include <TF1.h>
#include <TLorentzRotation.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TVirtualMC.h>

// FairRoot
#include <FairRunSim.h>
#include <FairLogger.h>

using TMath::DegToRad;
using TMath::RadToDeg;

TGraph* ERN15B11ElasticScattering::fThetaCDFGr = NULL;
TGraph* ERN15B11ElasticScattering::fThetaInvCDFGr = NULL;

//-------------------------Globals----------------------------------
Bool_t test = kFALSE;
//------------------------------------------------------------------

ERN15B11ElasticScattering::ERN15B11ElasticScattering(TString name):
    ERDecay(name),
    fThetaFileName(""),
    fTheta1(0.),
    fTheta2(180.),
    fPhi1(0),
    fPhi2(360.),
    fTargetIonName(""),
    fTargetIonPDG(NULL),
    fThetaInvCDF(NULL),
    fCDFmin(0.),
    fCDFmax(1.),
    fTheta1B11(0.),
    fTheta2B11(0.),
    fCDFminB11(0.),
    fCDFmaxB11(0.),
    fDetPos(0.),
    fIonMass(0.),
    fInteractNumInTarget(0),
    fProbab(0.)
{
    fThetaCDF = NULL;
    fThetaInvCDF = NULL;
}

ERN15B11ElasticScattering::~ERN15B11ElasticScattering()
{
}

Double_t ERN15B11ElasticScattering::ThetaCDF(Double_t *x, Double_t *par)
{
    return fThetaCDFGr->Eval(x[0]);
}

Double_t ERN15B11ElasticScattering::ThetaInvCDF(Double_t *x, Double_t *par)
{
    return fThetaInvCDFGr->Eval(x[0]);
}

void ERN15B11ElasticScattering::SetTargetIon(Int_t A, Int_t Z, Int_t Q)
{
    FairRunSim* run = FairRunSim::Instance();
    fTargetIonName = fName + TString("_TargetIon");
    FairIon* ion = new FairIon(fTargetIonName,A,Z,Q);
    run->AddNewIon(ion);
}

Bool_t ERN15B11ElasticScattering::Init()
{
    if (!ERDecay::Init())
    {
        return kFALSE;
    }

    fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
    if ( ! fTargetIonPDG )
    {
        LOG(FATAL) << "Target ion not found in pdg database!" << FairLogger::endl;
        return kFALSE;
    }

    SetIonMass(fInputIonPDG->Mass());
    SetTargetMass(fTargetIonPDG->Mass());
    RangeCalculate (fInputIonPDG->Mass(), fTargetIonPDG->Mass());

    return kTRUE;
}

Bool_t ERN15B11ElasticScattering::Stepping()
{
    if (!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName))
    {
        gMC->SetMaxStep(fStep);
        TLorentzVector curPos;
        gMC->TrackPosition(curPos);
        //fDecayPosZ = -0.0007/2.;
        if (curPos.Z() > fDecayPosZ)
        {
            TLorentzVector fInputIonV;
            gMC->TrackMomentum(fInputIonV);
            Double_t iM = GetIonMass();
            Double_t tM = GetTargetMass();
            Double_t iM2 = pow(iM,2);
            Double_t tM2 = pow(tM,2);

            // Input MC ion mass identify
            Double_t mcIonMass = fInputIonV.E() / fInputIonV.Gamma();
            if (fabs(mcIonMass-iM) > 1e-12)
            {
                //std::cout.precision(12);
                //std::cout << "PDG Input ion mass: " << fInputIonPDG->Mass() << " ";
                //std::cout << "Input Ion mcMass: " << mcIonMass << std::endl;
                SetIonMass(mcIonMass);
                iM = mcIonMass;
                RangeCalculate(iM, tM); // For angles drawing ranges calculate
                //std::cout << "N15 mass: " << iM << ", B11 mass: " << tM << std::endl;
                //std::cout.precision(3);
            }

            Double_t inputIonT = sqrt(pow(fInputIonV.P(),2)+iM2) - iM;

            LOG(DEBUG) << "ElasticScattering: " << fName << FairLogger::endl;
            LOG(DEBUG) << "  Input ion with Ekin = " << inputIonT
                        << ", mass = " << iM
                        << " mom = " << fInputIonV.Px() << "," << fInputIonV.Py() << "," << fInputIonV.Pz() << FairLogger::endl;

            Double_t invariant = pow((iM+tM),2)+2*tM*inputIonT;
            Double_t shorty = pow(invariant-iM2-tM2,2);
            Double_t Pcm = sqrt((shorty-4*iM2*tM2)/(4*invariant));

            LOG(DEBUG) << "  CM momentum: " << Pcm << FairLogger::endl;
            LOG(DEBUG) << "  CM Ekin: " << sqrt(pow(Pcm,2)+iM2) - iM << FairLogger::endl;



            Double_t theta = ThetaGen();
            Double_t phi = fRnd->Uniform(fPhi1, fPhi2);
            LOG(DEBUG) << "PhiRange: " << fPhi1*RadToDeg() << ":" << fPhi2*RadToDeg() << FairLogger::endl;

            // In case of B11 registration
            if (theta > fTheta2*DegToRad() || theta < fTheta1*DegToRad())
            {
                phi = phi + 180.*DegToRad();
                if (!test) LOG(INFO) << "\n\n\n************* BADB11 !!! **************\n\n\n";
            }

            if (fThetaFileName != "")
            {
                LOG(DEBUG) << "  CM [CDFmin,CDFmax] = [" << fCDFmin << "," << fCDFmax << "]" << FairLogger::endl;
            }

            LOG(DEBUG) << "  CM Theta = " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;

            TLorentzVector out1V (Pcm*sin(theta)*cos(phi), Pcm*sin(theta)*sin(phi), Pcm*cos(theta), sqrt(pow(Pcm,2) + iM2));
            TLorentzVector out2V (-out1V.Px(), -out1V.Py(), -out1V.Pz(), sqrt(pow(Pcm,2) + tM2));

            LOG(DEBUG) << "  CM out1 state(px,py,pz,E) = "<<out1V.Px()<<","<<out1V.Py()<<","<<out1V.Pz()
                        << "," << out1V.E() << FairLogger::endl;
            LOG(DEBUG) << "  CM out2 state(px,py,pz,E) = "<<out2V.Px()<<","<<out2V.Py()<<","<<out2V.Pz()
                        << "," << out2V.E() << FairLogger::endl;

            LOG(DEBUG) << "  CM out1 Ekin = "<< sqrt(pow(out1V.P(),2)+iM2) - iM << FairLogger::endl;
            LOG(DEBUG) << "  CM out2 Ekin = "<< sqrt(pow(out2V.P(),2)+tM2) - tM << FairLogger::endl;

            LOG(DEBUG) << "  Boosting with beta = " << fInputIonV.Beta()
                        << ", gamma = " << fInputIonV.Gamma() << FairLogger::endl;

            TLorentzVector targetV(0.,0.,0.,tM);
            TLorentzVector cmV = targetV + fInputIonV;

            // we use second case
            out1V.RotateY(cmV.Theta());
            out1V.RotateZ(cmV.Phi());
            out1V.Boost(cmV.BoostVector());

            out2V.RotateY(cmV.Theta());
            out2V.RotateZ(cmV.Phi());
            out2V.Boost(cmV.BoostVector());

            //out1V.Boost(cmV.BoostVector());
            //out2V.Boost(cmV.BoostVector());

            LOG(INFO) << "InfoInputIon: theta = " << fInputIonV.Theta() << ", phi = " << fInputIonV.Phi() << FairLogger::endl;


            LOG(DEBUG) << "  Lab theta = " << out1V.Theta()*RadToDeg() << " phi = " << out1V.Phi()*RadToDeg() << FairLogger::endl;
            LOG(DEBUG) << "  Lab out1 T = "<< sqrt(pow(out1V.P(),2)+iM2) - iM <<  FairLogger::endl;
            LOG(DEBUG) << "  Lab out2 T = "<< sqrt(pow(out2V.P(),2)+tM2) - tM <<  FairLogger::endl;
            LOG(DEBUG) << "  Lab theta B11 = " << out2V.Theta()*RadToDeg() << FairLogger::endl;

            //curPos[2] += 0.0007; //TODO

            AddParticleToStack(fInputIonPDG->PdgCode(),curPos,out1V);
            AddParticleToStack(fTargetIonPDG->PdgCode(),curPos,out2V);

            fDecayFinish = kTRUE;
            gMC->StopTrack();
            gMC->SetMaxStep(10000.);

            // Interactions number counter
            fInteractNumInTarget++;
        }
    }

    return kTRUE;
}

Double_t ERN15B11ElasticScattering::ThetaGen()
{
    Double_t theta = 0.;
    if (fThetaFileName == "")
    {
        theta = acos(fRnd->Uniform(cos(fTheta1*DegToRad()), cos(fTheta2*DegToRad())));
    }
    else
    {
        Double_t dF1 = fabs(fCDFmax-fCDFmin);
        Double_t dF2 = fabs(fCDFmaxB11-fCDFminB11);
        Double_t dLength = dF1+dF2;
        fProbab = dLength;

        Double_t Rnd = fRnd->Uniform(0., 1.)*dLength;
        //LOG(INFO) << "ProbabN15: " << dF1/dLength << "\t" << "ProbabB11: " << dF2/dLength << FairLogger::endl;
        Double_t curCDF;
        if (Rnd > dF1)
        {
            curCDF = fCDFminB11 + Rnd - dF1;
            //LOG(INFO) << "B11 was found" << FairLogger::endl;
            test = kTRUE;
        }
        else
        {
            curCDF = fCDFmin + Rnd;
            test = kFALSE;
        }

        theta = fThetaInvCDF->Eval(curCDF)*DegToRad();
    }
    return theta;
}

void ERN15B11ElasticScattering::RangeCalculate(Double_t iM, Double_t tM)
{
    Double_t radAngle = fDetPos*DegToRad();
    Double_t ratio = iM/tM;
    Double_t ratio2 = ratio*ratio;

    Double_t thetaCMN15 = acos( -ratio*sin(radAngle)*sin(radAngle) + cos(radAngle)*sqrt(1 - ratio2*sin(radAngle)*sin(radAngle)) );
    Double_t thetaCMB11 = 180. - 2.*fDetPos;

    fTheta1 = thetaCMN15*RadToDeg() - 0.652;
    fTheta2 = thetaCMN15*RadToDeg() + 0.652;

    LOG(INFO) << "Thetas: " << fTheta1 << " : " << fTheta2 << FairLogger::endl;

    fTheta1B11 = thetaCMB11 - 0.521;
    fTheta2B11 = thetaCMB11 + 0.521;

    fPhi1 = -0.1055/*-asin( 2./218./sin(radAngle) )*/;
    fPhi2 = 0.1055/*asin( 2./218./sin(radAngle) )*/;
}


ClassImp(ERN15B11ElasticScattering)
