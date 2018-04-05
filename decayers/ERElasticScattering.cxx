/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERElasticScattering.h"

#include <fstream>

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TMath.h"
#include "TVectorD.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"
#include "ERMCEventHeader.h"

using TMath::DegToRad;
using TMath::RadToDeg;

//-------------------------------------------------------------------------------------------------
TGraph* thetaCDFGr = NULL;
TGraph* thetaInvCDFGr = NULL;

Double_t ThetaCDF(Double_t *x, Double_t *par)
{
  return thetaCDFGr->Eval(x[0]);
}

Double_t ThetaInvCDF(Double_t *x, Double_t *par)
{
  return thetaInvCDFGr->Eval(x[0]);
}

//-------------------------------------------------------------------------------------------------
ERElasticScattering::ERElasticScattering(TString name):
  ERDecay(name),
  fThetaFileName(""),
  fTheta1(0), 
  fTheta2(180.),
  fPhi1(0),
  fPhi2(360.),
  fTargetIonName(""),
  fTargetIonPDG(NULL),
  fThetaInvCDF(NULL),
  fCDFmin(0.),
  fCDFmax(1.)
{
}

//-------------------------------------------------------------------------------------------------
ERElasticScattering::~ERElasticScattering() {
}

//-------------------------------------------------------------------------------------------------
void ERElasticScattering::SetTargetIon(Int_t A, Int_t Z, Int_t Q) {
  FairRunSim* run = FairRunSim::Instance();
  fTargetIonName = fName + TString("_TargetIon");
  FairIon* ion = new FairIon(fTargetIonName,A,Z,Q);
  run->AddNewIon(ion);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERElasticScattering::Init() {
  if (!ERDecay::Init())
    return kFALSE;

  fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
  if ( ! fTargetIonPDG ) {
    LOG(FATAL) << "Target ion not found in pdg database!" << FairLogger::endl;
    return kFALSE;
  }

  if (fThetaFileName != ""){
    LOG(INFO) << "ElasticScattering " << fName << " initialize from theta distribution file" << FairLogger::endl;   
    
    TString path = TString(gSystem->Getenv("VMCWORKDIR")) + "/input/" + fThetaFileName;
    std::ifstream f;
    f.open(path.Data());
    if (!f.is_open()) {
      LOG(FATAL) << "Can't open file " << path << FairLogger::endl;
      return kFALSE;  
    }

    Int_t nPoints = std::count(std::istreambuf_iterator<char>(f), 
                               std::istreambuf_iterator<char>(), '\n');
    f.seekg(0, std::ios::beg);
    TVectorD tet(nPoints);
    TVectorD sigma(nPoints);
    
    LOG(DEBUG2) << "nPoints = " << nPoints << FairLogger::endl;

    Int_t i = 0;
    while(!f.eof()) {
      // Костыль
      if (i == nPoints) break;
      f >> tet(i) >> sigma(i);
      LOG(DEBUG2) << i << ": " << tet(i) << "\t" << sigma(i) << FairLogger::endl;
      i++;
    }

    thetaCDFGr = new TGraph(tet, sigma);
    thetaInvCDFGr = new TGraph(sigma, tet);

    TF1* thetaCDF = new TF1("thetaCDF", ThetaCDF, 0., 180., 0);
    fThetaInvCDF = new TF1("thetaInvCDF", ThetaInvCDF, 0., 1., 0);

    fCDFmin = thetaCDF->Eval(fTheta1);
    fCDFmax = thetaCDF->Eval(fTheta2);
  }

  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERElasticScattering::Stepping() {
  if (!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName)){
    gMC->SetMaxStep(fStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() > fDecayPosZ) {

      TLorentzVector fInputIonV;
      gMC->TrackMomentum(fInputIonV);
      Float_t iM = fInputIonPDG->Mass();
      Float_t tM = fTargetIonPDG->Mass();
      Float_t iM2 = pow(iM,2);
      Float_t tM2 = pow(tM,2);
      
      Float_t inputIonT = sqrt(pow(fInputIonV.P(),2)+iM2) - iM;

      LOG(DEBUG) << "ElasticScattering: " << fName << FairLogger::endl;
      LOG(DEBUG) << "  Input ion with Ekin = " << inputIonT
                 << ", mass = " << iM 
                 << " mom = " << fInputIonV.Px() << "," << fInputIonV.Py() << "," << fInputIonV.Pz() << FairLogger::endl;

      Float_t invariant = pow((iM+tM),2)+2*tM*inputIonT;
      Double_t shorty = pow(invariant-iM2-tM2,2);
      Float_t Pcm = sqrt((shorty-4*iM2*tM2)/(4*invariant));

      LOG(DEBUG) << "  CM momentum: " << Pcm << FairLogger::endl;
      LOG(DEBUG) << "  CM Ekin: " << sqrt(pow(Pcm,2)+iM2) - iM<< FairLogger::endl;

      Float_t theta = ThetaGen();
      Float_t phi = fRnd->Uniform(fPhi1*DegToRad(), fPhi2*DegToRad());

      if (fThetaFileName != "") {
        LOG(DEBUG) << "  CM [CDFmin,CDFmax] = [" << fCDFmin << "," << fCDFmax << "]" << FairLogger::endl;
      }

      LOG(DEBUG) << "  CM Theta = " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;

      TLorentzVector out1V (Pcm*sin(theta)*cos(phi),
                            Pcm*sin(theta)*sin(phi),
                            Pcm*cos(theta),
                            sqrt(pow(Pcm,2) + iM2));
      TLorentzVector out2V ( -out1V.Px(),
                             -out1V.Py(),
                             -out1V.Pz(),
                              sqrt(pow(Pcm,2) + tM2));

      LOG(DEBUG) << "  CM out1 state(px,py,pz,E) = "<<out1V.Px()<<","<<out1V.Py()<<","<<out1V.Pz()
                 << "," << out1V.E() <<  FairLogger::endl;
      LOG(DEBUG) << "  CM out2 state(px,py,pz,E) = "<<out2V.Px()<<","<<out2V.Py()<<","<<out2V.Pz()
                 << "," << out2V.E() <<  FairLogger::endl;

      LOG(DEBUG) << "  CM out1 Ekin = "<< sqrt(pow(out1V.P(),2)+iM2) - iM << FairLogger::endl;
      LOG(DEBUG) << "  CM out2 Ekin = "<< sqrt(pow(out2V.P(),2)+tM2) - tM << FairLogger::endl;

      LOG(DEBUG) << "  Boosting with beta = " << fInputIonV.Beta() 
                << ", gamma = " << fInputIonV.Gamma() << FairLogger::endl;

      TLorentzVector targetV(0,0,0,tM);
      TLorentzVector cmV = targetV + fInputIonV;

      out1V.Boost(cmV.BoostVector());
      out2V.Boost(cmV.BoostVector());

      LOG(DEBUG) << "  Lab theta = " << out1V.Theta()*RadToDeg() << " phi = " << out1V.Phi()*RadToDeg() << FairLogger::endl;      
      LOG(DEBUG) << "  Lab out1 T = "<< sqrt(pow(out1V.P(),2)+iM2) - iM <<  FairLogger::endl;
      LOG(DEBUG) << "  Lab out2 T = "<< sqrt(pow(out2V.P(),2)+tM2) - tM <<  FairLogger::endl;

      curPos[2] += 0.0007; //TODO

      AddParticleToStack(fInputIonPDG->PdgCode(),curPos,out1V);
      AddParticleToStack(fTargetIonPDG->PdgCode(),curPos,out2V);

      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
    }
  }
  return kTRUE;
}

Float_t ERElasticScattering::ThetaGen() {
  Float_t theta = 0.;
  if (fThetaFileName == "")
    theta = acos(fRnd->Uniform(cos(fTheta1*DegToRad()), cos(fTheta2*DegToRad())));
  else{
    theta = fThetaInvCDF->Eval(fRnd->Uniform(fCDFmin,fCDFmax))*DegToRad();
  }
  return theta;
}

ClassImp(ERElasticScattering)
