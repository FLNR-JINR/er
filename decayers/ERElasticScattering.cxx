/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERElasticScattering.h"

#include <iostream>

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TMath.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"
#include "ERMCEventHeader.h"

using namespace std;
using namespace TMath;

ERElasticScattering::ERElasticScattering(TString name):
  ERDecay(name),
  fThetaFileName(""),
  fTheta1(0), 
  fTheta2(180.),
  fPhi1(0),
  fPhi2(360.),
  fTargetIonName(""),
  fTargetIonPDG(NULL)
{
}
//-------------------------------------------------------------------------------------------------
ERElasticScattering::~ERElasticScattering() {
}
//-------------------------------------------------------------------------------------------------
void ERElasticScattering::SetTargetIon(Int_t A, Int_t Z, Int_t Q){
  FairRunSim* run = FairRunSim::Instance();
  fTargetIonName = fName + TString("_TargetIon");
  FairIon* ion = new FairIon(fTargetIonName,A,Z,Q);
  run->AddNewIon(ion);
}
//-------------------------------------------------------------------------------------------------
Bool_t ERElasticScattering::Init(){
  if (!ERDecay::Init())
    return kFALSE;

  fTargetIonPDG = TDatabasePDG::Instance()->GetParticle(fTargetIonName);
  if ( ! fTargetIonPDG ) {
    LOG(FATAL) << "Target ion not found in pdg database!" << endl;
    return kFALSE;
  }

  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERElasticScattering::Stepping() {
  if(!fDecayFinish && gMC->TrackPid() == fInputIonPDG->PdgCode() && TString(gMC->CurrentVolName()).Contains(fVolumeName)){
    gMC->SetMaxStep(fStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() > fDecayPosZ){

      TLorentzVector fInputIonV;
      gMC->TrackMomentum(fInputIonV);
      Float_t iM = fInputIonPDG->Mass();
      Float_t tM = fTargetIonPDG->Mass();
      Float_t iM2 = pow(iM,2);
      Float_t tM2 = pow(tM,2);
      
      Float_t inputIonT = sqrt(pow(fInputIonV.P(),2)+iM2) - iM;

      LOG(INFO) << "ElasticScattering: " << fName << FairLogger::endl;
      LOG(INFO) << "  Input ion with Ekin = " << inputIonT
                << ", mass = " << iM 
                << " mom = " <<  fInputIonV.Px() << "," <<  fInputIonV.Py() << "," << fInputIonV.Pz() << FairLogger::endl;

      Float_t invariant = pow((iM+tM),2)+2*tM*inputIonT;
      Double_t shorty=pow(invariant-iM2-tM2,2);
      Float_t Pcm = sqrt((shorty-4*iM2*tM2)/(4*invariant));

      LOG(INFO) << "  CM momentum: " << Pcm << FairLogger::endl;

      Float_t theta = ThetaGen();
      Float_t phi = fRnd->Uniform(fPhi1*DegToRad(),fPhi2*DegToRad());

      LOG(INFO) << "  Theta = : " << theta*RadToDeg() << ", phi = " << phi*RadToDeg() << FairLogger::endl;

      TLorentzVector out1V (Pcm*sin(theta)*cos(phi),
                            Pcm*sin(theta)*sin(phi),
                            Pcm*sin(theta),
                            sqrt(pow(Pcm,2) + iM2));
      TLorentzVector out2V ( -out1V.Px(),
                             -out1V.Py(),
                             -out1V.Pz(),
                              sqrt(pow(Pcm,2) + tM2));

      LOG(INFO) << "  CM out1 state(px,py,pz,E) = : "<<out1V.Px()<<","<<out1V.Py()<<","<<out1V.Pz()
                << out1V.E() <<  FairLogger::endl;
      LOG(INFO) << "  CM out2 state(px,py,pz,E) = : "<<out2V.Px()<<","<<out2V.Py()<<","<<out2V.Pz()
                << out2V.E() <<  FairLogger::endl;

      LOG(INFO) << "  Boosting with beta = " << fInputIonV.Beta() 
                << ", gamma = " << fInputIonV.Gamma() << FairLogger::endl;

      out1V.Boost(fInputIonV.BoostVector());
      out2V.Boost(fInputIonV.BoostVector());
      
      AddParticleToStack(fInputIonPDG->PdgCode(),curPos,out1V);
      AddParticleToStack(fTargetIonPDG->PdgCode(),curPos,out2V);

      fDecayFinish = kTRUE;
      gMC->StopTrack();
      gMC->SetMaxStep(10000.);
    }
  }
  return kTRUE;
}


Float_t ERElasticScattering::ThetaGen(){

  Float_t theta = acos(fRnd->Uniform(cos(fTheta1*DegToRad()),cos(fTheta2*DegToRad())));

  return theta;
}


ClassImp(ERElasticScattering)
