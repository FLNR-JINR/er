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
#include <TLorentzRotation.h>
#include <TVectorD.h>
#include <TGraph.h>
#include <TVirtualMC.h>

#include "G4IonTable.hh"

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
  ERDecay(name)
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

void ERElasticScattering::SetThetaRange(Double_t th1, Double_t th2, ERInteractionParticipant DetIonType) {
  fThetaMinCM = th1; fThetaMaxCM = th2;
  fDetectionIonType = DetIonType;
}

void ERElasticScattering::SetLabThetaRange(Double_t thetaCenter, Double_t dTheta, ERInteractionParticipant DetIonType, Bool_t relMod, Double_t BeamAvE) {
  fThetaRangeCenter = thetaCenter; fThetaRangedTheta = dTheta;
  fDetectionIonType = DetIonType;
  fRelativisticMode = relMod;
  fBeamAverageEnergy = BeamAvE;
  if (fRelativisticMode == kTRUE && fBeamAverageEnergy == 0.) {
    LOG(FATAL) << "ERElasticScattering::SetLabThetaRange: In the relativistic case (4-th param = kTRUE) "
               << "the average energy of the beam (5-th param) can't be zero." 
               << "For the relativistic case this energy must be set correct." << FairLogger::endl;
  }
  if (relMod == kFALSE && fBeamAverageEnergy != 0.) {
    LOG(WARNING) << "ERElasticScattering::SetLabThetaRange: The average energy of the beam (5-th param) is not zero, " 
                 << "but in non relativistic case (4-th param = kFALSE) "
                 << "the average energy of the beam is not used! This energy should be set to zero!" << FairLogger::endl;
  }
}

Double_t ERElasticScattering::GetThetaCMMean() const {
  if (fInteractNumInTarget == 0)
    return 0.;
  return fThetaCMSum / fInteractNumInTarget;
}

Bool_t ERElasticScattering::Init() {
  if (!ERDecay::Init()) {
    return kFALSE;
  }

  fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
  if ( ! fTargetIonPDG ) {
    LOG(FATAL) << "ERElasticScattering::Init: Target ion was not found in pdg database!" << FairLogger::endl;
  }

  DefineOfIonsMasses();
  LOG(DEBUG) << "ERElasticScattering::Init()" << FairLogger::endl;
  LOG(DEBUG) << "Traget Ions Mass is " << GetTargetIonMass()
             << ", Prigectile Ions Mass is " << GetProjectileIonMass() << FairLogger::endl;
    
  if (fRelativisticMode)
    ThetaRangesLab2CMRelativistic();
  else
    ThetaRangesLab2CM();

  if (!ThetaCDFRead()) {
    LOG(FATAL) << "The input file which contains the CDF function can't be read!" << FairLogger::endl;
    return kFALSE;
  }
  return kTRUE;
}

Bool_t ERElasticScattering::Stepping() {
  if (!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName)) {
    gMC->SetMaxStep(fStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() >= fDecayPosZ) {
      TLorentzVector fProjectileIonV;
      gMC->TrackMomentum(fProjectileIonV);
      Double_t pM = GetProjectileIonMass();
      Double_t tM = GetTargetIonMass();
      Double_t pM2 = pow(pM, 2);
      Double_t tM2 = pow(tM, 2);

      Double_t projectileIonIonT = sqrt(pow(fProjectileIonV.P(), 2)+pM2) - pM;

      LOG(DEBUG) << "ElasticScattering: " << fName << FairLogger::endl;
      LOG(DEBUG) << "  ProjectileIon ion with Ekin = " << projectileIonIonT
                  << ", mass = " << pM
                  << " mom = (" << fProjectileIonV.Px() << "," << fProjectileIonV.Py() << "," << fProjectileIonV.Pz() << ")" << FairLogger::endl;

      Double_t invariant = pow((pM+tM), 2) + 2*tM*projectileIonIonT;
      Double_t shorty = pow(invariant-pM2-tM2, 2);
      Double_t Pcm = sqrt( (shorty-4*pM2*tM2) / (4*invariant) );

      LOG(DEBUG) << "  CM momentum: " << Pcm << FairLogger::endl;
      LOG(DEBUG) << "  CM Ekin: " << sqrt(pow(Pcm,2)+pM2) - pM << FairLogger::endl;

      // Generate random angles theta and phi
      Double_t theta = ThetaGen();
      Double_t phi = fRnd->Uniform(fPhiMin*DegToRad(), fPhiMax*DegToRad());
     
      // In case of target ion registration
      if (fDetectionIonType == kTARGET) {
        phi = phi + 180.*DegToRad();
        fThetaCMSum += theta*RadToDeg();
      }
      else
        fThetaCMSum += theta*RadToDeg();

      if (fThetaFileName != "") {
        LOG(DEBUG) << "  CM [CDFmin,CDFmax] = [" << fCDFmin << "," << fCDFmax << "]" << FairLogger::endl;
      }
      TLorentzVector out1V (Pcm*sin(theta)*cos(phi), Pcm*sin(theta)*sin(phi), Pcm*cos(theta), sqrt(pow(Pcm,2) + pM2));
      TLorentzVector out2V (-out1V.Px(), -out1V.Py(), -out1V.Pz(), sqrt(pow(Pcm,2) + tM2));
      LOG(DEBUG) << "BEFORE BOOST=======================================================" << FairLogger::endl;
      LOG(DEBUG) << "  CM Theta = " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;
      LOG(DEBUG) << "  CM out1 state(px,py,pz,E) = "<<out1V.Px()<<", "<<out1V.Py()<<", "<<out1V.Pz()
                << ", " << out1V.E() << FairLogger::endl;
      LOG(DEBUG) << "  CM out2 state(px,py,pz,E) = "<<out2V.Px()<<", "<<out2V.Py()<<", "<<out2V.Pz()
                << ", " << out2V.E() << FairLogger::endl;
      LOG(DEBUG) << "  CM out1 Ekin = "<< sqrt(pow(out1V.P(),2)+pM2) - pM << FairLogger::endl;
      LOG(DEBUG) << "  CM out2 Ekin = "<< sqrt(pow(out2V.P(),2)+tM2) - tM << FairLogger::endl;

      TLorentzVector targetV(0,0,0,tM);
      TLorentzVector cmV = targetV + fProjectileIonV;
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
      LOG(DEBUG) << "  Lab theta projectile ion = " << out1V.Theta()*RadToDeg() << " phi = " << out1V.Phi()*RadToDeg() << FairLogger::endl;
      LOG(DEBUG) << "  Lab out1 T = "<< sqrt(pow(out1V.P(),2)+pM2) - pM <<  FairLogger::endl;
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

void  ERElasticScattering::ThetaRangesLab2CM() {
  Double_t pM = GetProjectileIonMass();
  Double_t tM = GetTargetIonMass();
  LOG(DEBUG) << "ERElasticScattering::ThetaRangesLab2CM(pM = " << pM << ", tM = " << tM << ")" << FairLogger::endl;
  Double_t rAng = fThetaRangeCenter*DegToRad();
  Double_t ratio = pM/tM;
  Double_t ratio2 = ratio*ratio;
  Double_t rdThetaLab = fThetaRangedTheta*DegToRad(); // Detectors rdThetaLab
  if (fDetectionIonType == kPROJECTILE) {
    // Projectile Ion
    if (pM != tM) {
      fThetaMinCM = RadToDeg()*acos( -ratio*sin(rAng-rdThetaLab)*sin(rAng-rdThetaLab)
                + cos(rAng-rdThetaLab)*sqrt(1.-ratio2*sin(rAng-rdThetaLab)*sin(rAng-rdThetaLab)) );
      fThetaMaxCM = RadToDeg()*acos( -ratio*sin(rAng+rdThetaLab)*sin(rAng+rdThetaLab)
                + cos(rAng+rdThetaLab)*sqrt(1.-ratio2*sin(rAng+rdThetaLab)*sin(rAng+rdThetaLab)) );
    }
    else {
      fThetaMinCM = RadToDeg()*(2.*rAng - rdThetaLab);
      fThetaMaxCM = RadToDeg()*(2.*rAng + rdThetaLab);
    }

    LOG(DEBUG) << "  Projectile CMTheta1: " << fThetaMinCM << ", Projectile CMTheta2: " << fThetaMaxCM
               << ", average value: " << 0.5*(fThetaMaxCM-fThetaMinCM) + fThetaMinCM << FairLogger::endl;
  }
  else if (fDetectionIonType == kTARGET) {
    // Target Ion
    fThetaMinCM = 180. - 2.*RadToDeg()*rAng - RadToDeg()*rdThetaLab;
    fThetaMaxCM = 180. - 2.*RadToDeg()*rAng + RadToDeg()*rdThetaLab;
    LOG(DEBUG) << "  B11: CMTheta1: " << fThetaMinCM << ", CMTheta2: " << fThetaMaxCM
               << ", average value: " << 0.5*(fThetaMaxCM-fThetaMinCM) + fThetaMinCM << FairLogger::endl;
  }
  else {
    LOG(FATAL) << "Unknown fDetectionIonType!" << FairLogger::endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// It is a method to recalculation theta ranges from Lab to CM.
/// Input formula is tan(thetaLab) = MomOfProjectilePerpendicularComponent / MomOfProjectileParallelComponent.
/// Where MomOfProjectilePerpendicularComponent = Pcm*Sin(thetaCM)*sqrt(1-V^2),
/// and MomOfProjectileParallelComponent = Pcm*Cos(thetaCM) + V*sqrt(pM^2+Pcm^2).
/// Where V is a velocity of central of mass relativity of the Lab.
/// Pcm is momentum of CM relativity of Lab. 
/// Here we use inverse formula to obtain thetaCM from thetaLab.
/// For comfortable calculations we introduced corresponding notations.
/// y = tan(thetaLab),
/// z = V*sqrt(pM^2+Pcm^2),
/// t = 1-V^2,
/// x = Cos(thetaCM).
/// a = Pcm^2*(y^2+t),
/// b = 2*y^2*Pcm*z*x,
/// c = y^2*z^2 - Pcm^2*t = 0,
/// From input formula we obtain the equation 
/// a*x^2 + b*x + c = 0
/// From the equation  
/// D = b^2 - a*c, 
/// x1,2 = (-b +- sqrt(D)) / 2 / a,
/// for projectile ion Cos(thetaCM) = (-b + sqrt(D)) / 2 / a,
/// for target ion Cos(thetaCM) = -Cos(thetaCM) = (b - sqrt(D)) / 2 / a.
void ERElasticScattering::ThetaRangesLab2CMRelativistic() {
  Double_t pM = GetProjectileIonMass();
  Double_t tM = GetTargetIonMass();
  /// For a target ion in the calculations below projectile mass has to be changed with target mass
  if (fDetectionIonType == kTARGET) 
    pM = tM;

  Double_t pMom = sqrt(fBeamAverageEnergy*fBeamAverageEnergy - pM*pM);
  Double_t VelocityOfCMRelOfLab = pMom / (fBeamAverageEnergy + tM);
  if (VelocityOfCMRelOfLab > 1.) {
    LOG(FATAL) << "In ERElasticScattering::ThetaRangesLab2CMRelativistic() the velocity of CM can't be > 1." << FairLogger::endl;
  }
  /// Absolute value of the CM momentum is the same 
  /// for both particles and also before and after the ellastic scattering.
  Double_t MomInCM = VelocityOfCMRelOfLab*tM / sqrt(1. - VelocityOfCMRelOfLab*VelocityOfCMRelOfLab);
  Double_t yMin = tan(fThetaRangeCenter*DegToRad()-fThetaRangedTheta*DegToRad());
  Double_t yMax = tan(fThetaRangeCenter*DegToRad()+fThetaRangedTheta*DegToRad());
  Double_t z = VelocityOfCMRelOfLab*sqrt(pM*pM + MomInCM*MomInCM);
  Double_t t = 1.-VelocityOfCMRelOfLab*VelocityOfCMRelOfLab;
  Double_t B1Min = t*((MomInCM*MomInCM-z*z)*yMin*yMin + MomInCM*MomInCM*t);
  Double_t B1Max = t*((MomInCM*MomInCM-z*z)*yMax*yMax + MomInCM*MomInCM*t);
  if (B1Min < 0. || B1Max < 0.) {
    LOG(FATAL) << "In ERElasticScattering::ThetaRangesLab2CMRelativistic() B1 can't be < 0." << FairLogger::endl;
  }
  B1Min = sqrt(B1Min);
  B1Max = sqrt(B1Max);
  Double_t B2Min = yMin*yMin*z;
  Double_t B2Max = yMax*yMax*z;
  Double_t B3Min = MomInCM*(yMin*yMin + t);
  Double_t B3Max = MomInCM*(yMax*yMax + t);
  Double_t CosThetaCMMin;
  Double_t CosThetaCMMax;

  if (fDetectionIonType == kPROJECTILE) {
    CosThetaCMMin = (-B2Min+B1Min) / B3Min;
    CosThetaCMMax = (-B2Max+B1Max) / B3Max;
  }
  else if (fDetectionIonType == kTARGET) {
    CosThetaCMMin = (B2Min-B1Min) / B3Min;
    CosThetaCMMax = (B2Max-B1Max) / B3Max;
  }
  else {
    LOG(FATAL) << "In ERElasticScattering::ThetaRangesLab2CMRelativistic() unknown fDetectionIonType!" << FairLogger::endl;
  }

  fThetaMinCM = acos(CosThetaCMMin)*RadToDeg();
  fThetaMaxCM = acos(CosThetaCMMax)*RadToDeg();
}

Bool_t ERElasticScattering::ThetaCDFRead() {
  if (fThetaFileName != "") {
    LOG(INFO) << "ElasticScattering " << fName << " initialized from theta distribution file" << FairLogger::endl;

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
      // Костыль
      if (i == nPoints) break;
      f >> tet(i) >> sigma(i);
      LOG(DEBUG2) << i << ": " << tet(i) << "\t" << sigma(i) << FairLogger::endl;
      i++;
    }

    thetaCDFGr = new TGraph(tet, sigma);
    thetaInvCDFGr = new TGraph(sigma, tet);

    fThetaCDF = new TF1("thetaCDF", ThetaCDF, 0., 180., 0);
    fThetaInvCDF = new TF1("thetaInvCDF", ThetaInvCDF, 0., 1., 0);

    fCDFmin = fThetaCDF->Eval(fThetaMinCM);
    fCDFmax = fThetaCDF->Eval(fThetaMaxCM);

    delete thetaCDFGr;
    delete fThetaCDF;
  }
  return kTRUE;
}

Double_t ERElasticScattering::ThetaGen() {
  Double_t theta;
  if (fThetaFileName == "") {
    theta = acos(fRnd->Uniform(cos(fThetaMinCM*DegToRad()), cos(fThetaMaxCM*DegToRad())));
  }
  else {
    theta = fThetaInvCDF->Eval(fRnd->Uniform(fCDFmin, fCDFmax))*DegToRad();
  }
  return theta;
}

Bool_t ERElasticScattering::DefineOfIonsMasses() {
  G4ParticleTable* table = G4ParticleTable::GetParticleTable();
  if (! table ) {
    LOG(FATAL) << "G4 Particle Table is not found!" << FairLogger::endl;
    return kFALSE;
  }

  SetProjectileIonMass(1e-3*((G4ParticleDefinition*)table->FindParticle((G4int)fInputIonPDG->PdgCode()))->GetPDGMass());
  SetTargetIonMass(1e-3*((G4ParticleDefinition*)table->FindParticle((G4int)fTargetIonPDG->PdgCode()))->GetPDGMass());

  if (! GetProjectileIonMass() ) {
    LOG(FATAL) << "Can't difine Mass for projectile ion!" << FairLogger::endl;
    return kFALSE;
  }

  if (! GetTargetIonMass() ) {
    LOG(FATAL) << "Can't difine Mass for target ion!" << FairLogger::endl;
    return kFALSE;
  }

  return kTRUE;
}

ClassImp(ERElasticScattering)
