/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERElasticScattering.h"

#include <iostream>

#include <TF1.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TLorentzRotation.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TVirtualMC.h>

#include <FairRunSim.h>
#include <FairLogger.h>

using TMath::DegToRad;
using TMath::RadToDeg;

TGraph* thetaCDFGr = NULL;
TGraph* thetaInvCDFGr = NULL;

//-------------------------Globals----------------------------------
Double_t ThetaCDF(Double_t *x, Double_t *par) {
  return thetaCDFGr->Eval(x[0]);
}

Double_t ThetaInvCDF(Double_t *x, Double_t *par) {
  return thetaInvCDFGr->Eval(x[0]);
}
//------------------------------------------------------------------

ERElasticScattering::ERElasticScattering(TString name):
  ERDecay(name),
  fThetaFileName(""),
  fTargetIonName(""),
  fRegisterIonStatus(IonStatus::ProjectileIon),
  fThetaMin(0.),
  fThetaMax(180.),
  fThetaRangeCenter(0.),
  fThetaRangedTheta(0.),
  fPhiMin(0),
  fPhiMax(360.),
  fTargetIonPDG(NULL),
  fThetaInvCDF(NULL),
  fCDFmin(0.),
  fCDFmax(1.),
  fProjectileIonMass(0.),
  fInteractNumInTarget(0),
  fThetaLabRangeIsSet(kFALSE)
{
}

ERElasticScattering::~ERElasticScattering() {
  if(fThetaInvCDF)
    delete fThetaInvCDF;
  if (thetaInvCDFGr)
    delete thetaInvCDFGr;
}

void ERElasticScattering::SetTargetIon(Int_t A, Int_t Z, Int_t Q) {
  FairRunSim* run = FairRunSim::Instance();
  fTargetIonName = fName + TString("_TargetIon");
  FairIon* ion = new FairIon(fTargetIonName,A,Z,Q);
  run->AddNewIon(ion);
}

void ERElasticScattering::SetLabThetaRange(Double_t thetaCenter, Double_t dTheta, IonStatus regIonSt) {
  fThetaRangeCenter = thetaCenter; fThetaRangedTheta = dTheta;
  fRegisterIonStatus = regIonSt;
  fThetaLabRangeIsSet = kTRUE;
}

Bool_t ERElasticScattering::Init() {
  if (!ERDecay::Init()) {
    return kFALSE;
  }

  fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
  if ( ! fTargetIonPDG ) {
    LOG(FATAL) << "Target ion not found in pdg database!" << FairLogger::endl;
    return kFALSE;
  }

  SetProjectileIonMass(fInputIonPDG->Mass());
  SetTargetIonMass(fTargetIonPDG->Mass());

  if (fThetaLabRangeIsSet)
    ThetaRangesLab2CM(GetProjectileIonMass(), GetTargetIonMass());

  if (fThetaFileName != "") {
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
    while (!f.eof()) {
      if (i == nPoints) break;
      f >> tet(i) >> sigma(i);
      LOG(DEBUG2) << i << ": " << tet(i) << "\t" << sigma(i) << FairLogger::endl;
      i++;
    }

    thetaCDFGr = new TGraph(tet, sigma);
    thetaInvCDFGr = new TGraph(sigma, tet);

    fThetaCDF = new TF1("thetaCDF", ThetaCDF, 0., 180., 0);
    fThetaInvCDF = new TF1("thetaInvCDF", ThetaInvCDF, 0., 1., 0);

    fCDFmin = fThetaCDF->Eval(fThetaMin);
    fCDFmax = fThetaCDF->Eval(fThetaMax);

    delete thetaCDFGr;
    delete fThetaCDF;

    std::cerr << "ERElasticScattering::Init" << std::endl;
    std::cerr << "fThetaMin: " << fThetaMin << ", fThetaMax: " << fThetaMax << std::endl;
    std::cerr << "fCDFmin: " << fCDFmin << ", CDFmax: " << fCDFmax << std::endl;
  }
  return kTRUE;
}

Bool_t ERElasticScattering::Stepping() {
  if (!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName)) {
    gMC->SetMaxStep(fStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() >= fDecayPosZ) {
      TLorentzVector fInputIonV;
      gMC->TrackMomentum(fInputIonV);
      Double_t iM = GetProjectileIonMass();
      Double_t tM = GetTargetIonMass();
      Double_t iM2 = pow(iM, 2);
      Double_t tM2 = pow(tM, 2);

      Double_t inputIonT = sqrt(pow(fInputIonV.P(), 2)+iM2) - iM;

      LOG(DEBUG) << "ElasticScattering: " << fName << FairLogger::endl;
      LOG(DEBUG) << "  Input ion with Ekin = " << inputIonT
                  << ", mass = " << iM
                  << " mom = (" << fInputIonV.Px() << "," << fInputIonV.Py() << "," << fInputIonV.Pz() << ")" << FairLogger::endl;

      Double_t invariant = pow((iM+tM), 2) + 2*tM*inputIonT;
      Double_t shorty = pow(invariant-iM2-tM2, 2);
      Double_t Pcm = sqrt( (shorty-4*iM2*tM2) / (4*invariant) );

      LOG(DEBUG) << "  CM momentum: " << Pcm << FairLogger::endl;
      LOG(DEBUG) << "  CM Ekin: " << sqrt(pow(Pcm,2)+iM2) - iM << FairLogger::endl;

      // Generate random angles theta and phi
      Double_t theta = ThetaGen();
      Double_t phi = fRnd->Uniform(fPhiMin*DegToRad(), fPhiMax*DegToRad());

      // In case of target ion registration
      if (fRegisterIonStatus == IonStatus::TargetIon) {
        phi = phi + 180.*DegToRad();
      }

      if (fThetaFileName != "") {
        LOG(DEBUG) << "  CM [CDFmin,CDFmax] = [" << fCDFmin << "," << fCDFmax << "]" << FairLogger::endl;
      }

      TLorentzVector out1V (Pcm*sin(theta)*cos(phi), Pcm*sin(theta)*sin(phi), Pcm*cos(theta), sqrt(pow(Pcm,2) + iM2));
      TLorentzVector out2V (-out1V.Px(), -out1V.Py(), -out1V.Pz(), sqrt(pow(Pcm,2) + tM2));
      LOG(DEBUG) << "BEFORE BOOST=======================================================" << FairLogger::endl;
      LOG(DEBUG) << "  CM Theta = " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;
      LOG(DEBUG) << "  CM out1 state(px,py,pz,E) = "<<out1V.Px()<<", "<<out1V.Py()<<", "<<out1V.Pz()
                << ", " << out1V.E() << FairLogger::endl;
      LOG(DEBUG) << "  CM out2 state(px,py,pz,E) = "<<out2V.Px()<<", "<<out2V.Py()<<", "<<out2V.Pz()
                << ", " << out2V.E() << FairLogger::endl;
      LOG(DEBUG) << "  CM out1 Ekin = "<< sqrt(pow(out1V.P(),2)+iM2) - iM << FairLogger::endl;
      LOG(DEBUG) << "  CM out2 Ekin = "<< sqrt(pow(out2V.P(),2)+tM2) - tM << FairLogger::endl;

      TLorentzVector targetV(0,0,0,tM);
      TLorentzVector cmV = targetV + fInputIonV;
      TVector3 cmVBoost = cmV.BoostVector();
      LOG(DEBUG) << "  tM in targetV(0, 0, 0, tM): " << tM << FairLogger::endl;
      LOG(DEBUG) << "  cmV components: (" << cmV.Px() << ", " << cmV.Py() << ", " << cmV.Pz() << ", " << cmV.E() << ")" << FairLogger::endl;
      LOG(DEBUG) << "  Boosting with beta = " << cmV.Beta()
                << ", gamma = " << cmV.Gamma() << FairLogger::endl;
      LOG(DEBUG) << "  Module of cmV.BoostVector: " << sqrt(cmVBoost.Px()*cmVBoost.Px() + cmVBoost.Py()*cmVBoost.Py() + cmVBoost.Pz()*cmVBoost.Pz()) << FairLogger::endl;
      LOG(DEBUG) << "  cmV.BoostVector components: (" << cmVBoost.Px() << ", " << cmVBoost.Py() << ", " << cmVBoost.Pz() << ")" << FairLogger::endl;

      theta = cmV.Theta();
      phi = cmV.Phi();
      LOG(DEBUG) << "  Rotation angles: theta = " << theta*RadToDeg() << ", Phi = " << phi*RadToDeg() << FairLogger::endl;

      out1V.RotateZ(-phi);
      out1V.RotateY(theta);
      out1V.RotateZ(phi);
      out1V.Boost(cmV.BoostVector());

      out2V.RotateZ(-phi);
      out2V.RotateY(theta);
      out2V.RotateZ(phi);
      out2V.Boost(cmV.BoostVector());

      LOG(DEBUG) << "AFTER BOOST=======================================================" << FairLogger::endl;
      LOG(DEBUG) << "  Lab theta primary ion = " << out1V.Theta()*RadToDeg() << " phi = " << out1V.Phi()*RadToDeg() << FairLogger::endl;
      LOG(DEBUG) << "  Lab out1 T = "<< sqrt(pow(out1V.P(),2)+iM2) - iM <<  FairLogger::endl;
      LOG(DEBUG) << "  Lab out2 T = "<< sqrt(pow(out2V.P(),2)+tM2) - tM <<  FairLogger::endl;
      LOG(DEBUG) << "  Lab theta target ion = " << out2V.Theta()*RadToDeg() << " phi = " << out2V.Phi()*RadToDeg() << FairLogger::endl;
      LOG(DEBUG) << "  Lab out1 state(px,py,pz,E) = " << out1V.Px() << "," << out1V.Py() << "," << out1V.Pz()
                << "," << out1V.E() << FairLogger::endl;
      LOG(DEBUG) << "  Lab out2 state(px,py,pz,E) = "<<out2V.Px()<<","<<out2V.Py()<<","<<out2V.Pz()
                << "," << out2V.E() << FairLogger::endl;

      AddParticleToStack(fInputIonPDG->PdgCode(), curPos,out1V);
      AddParticleToStack(fTargetIonPDG->PdgCode(), curPos,out2V);

      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);

      // Interactions numbers counter
      fInteractNumInTarget++;
    }
  }
  return kTRUE;
}

void  ERElasticScattering::ThetaRangesLab2CM(Double_t pM, Double_t tM) {
  Double_t rAng = fThetaRangeCenter*DegToRad();
  Double_t ratio = pM/tM;
  Double_t ratio2 = ratio*ratio;
  Double_t rdTheta = fThetaRangedTheta*TMath::DegToRad(); // Detectors rdTheta
  if (fRegisterIonStatus == IonStatus::ProjectileIon) {
    // Projectile Ion
    if (pM != tM) {
      fThetaMin = TMath::RadToDeg()*acos( -ratio*sin(rAng-rdTheta)*sin(rAng-rdTheta)
                + cos(rAng-rdTheta)*sqrt(1.-ratio2*sin(rAng-rdTheta)*sin(rAng-rdTheta)) );
      fThetaMax = TMath::RadToDeg()*acos( -ratio*sin(rAng+rdTheta)*sin(rAng+rdTheta)
                + cos(rAng+rdTheta)*sqrt(1.-ratio2*sin(rAng+rdTheta)*sin(rAng+rdTheta)) );
    }
    else {
      fThetaMin = TMath::RadToDeg()*(2.*rAng - rdTheta);
      fThetaMax = TMath::RadToDeg()*(2.*rAng + rdTheta);
    }

    LOG(DEBUG) << "  N15: CMTheta1: " << fThetaMin << ", CMTheta2: " << fThetaMax
              << ", average value: " << 0.5*(fThetaMax-fThetaMin) + fThetaMin << FairLogger::endl;
  }
  else if (fRegisterIonStatus == IonStatus::TargetIon) {
    // Target Ion
    fThetaMin = 180. - 2.*TMath::RadToDeg()*rAng - TMath::RadToDeg()*rdTheta;
    fThetaMax = 180. - 2.*TMath::RadToDeg()*rAng + TMath::RadToDeg()*rdTheta;
    LOG(DEBUG) << "  B11: CMTheta1: " << fThetaMin << ", CMTheta2: " << fThetaMax
                << ", average value: " << 0.5*(fThetaMax-fThetaMin) + fThetaMin << FairLogger::endl;
  }
  else {
    LOG(FATAL) << "Incorrect third param in ERElasticScattering::SetLabThetaRange" << FairLogger::endl;
  }
  std::cerr << "ERElasticScattering::ThetaRangesLab2CM" << std::endl;
  std::cerr << "fThetaMin: " << fThetaMin << ", fThetaMax: " << fThetaMax << std::endl;
  std::cerr << "fCDFmin: " << fCDFmin << ", CDFmax: " << fCDFmax << std::endl;
}

Double_t ERElasticScattering::ThetaGen() {
  Double_t theta = 0.;
  std::cerr << "ERElasticScattering::ThetaGen" << std::endl;
  std::cerr << "fThetaMin: " << fThetaMin << ", fThetaMax: " << fThetaMax << std::endl;
  std::cerr << "fCDFmin: " << fCDFmin << ", CDFmax: " << fCDFmax << std::endl;

  if (fThetaFileName == "") {
    theta = acos(fRnd->Uniform(cos(fThetaMin*DegToRad()), cos(fThetaMax*DegToRad())));
  }
  else {
    theta = fThetaInvCDF->Eval(fRnd->Uniform(fCDFmin, fCDFmax))*DegToRad();
  }
  std::cerr << "Returning theta: " << theta << std::endl;
  return theta;
}

ClassImp(ERElasticScattering)
