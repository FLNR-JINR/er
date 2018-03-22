/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecayEXP1803.h"

#include <iostream>
using namespace std;

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TRandom.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"
#include "ERMCEventHeader.h"

ERDecayEXP1803::ERDecayEXP1803():
  ERDecay("EXP1803"),
  fDecayFinish(kFALSE),
  fTargetReactZ(0.),
  fMinStep(0.01),
  f6He(NULL),
  f2H (NULL),
  f3He(NULL),
  f5H (NULL),
  f3H (NULL),
  fn  (NULL),
  fIon3He(NULL),
  f5HMass(0.),
  fIs5HUserMassSet(false),
  fIs5HExcitationSet(false)
{
  fRnd = new TRandom3();
  // fRnd->SetSeed();
  fRnd2 = new TRandom3();
  fRnd2->SetSeed();
  fReactionPhaseSpace = new TGenPhaseSpace();
  fDecayPhaseSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  fUnstableIon5H = new FairIon("5H",  1, 5, 1);
  fIon3He        = new FairIon("3He", 2, 3, 2);
  run->AddNewIon(fUnstableIon5H);
  run->AddNewIon(fIon3He);

  fADInput = NULL;
  fADFunction = NULL;

  flv5H = new TLorentzVector();
  flv3He = new TLorentzVector();

  cout << "ERDecayEXP1803 constructed." << endl;
}

//-------------------------------------------------------------------------------------------------
ERDecayEXP1803::~ERDecayEXP1803() {
  if (fADInput) {
    delete fADInput;
    fADInput = NULL;
  }
  if (fADFunction) {
    delete fADFunction;
    fADFunction = NULL;
  }

  ////if (flv5H) { delete flv5H; flv5H = NULL; }
  ////if (flv3He) { delete flv3He; flv3He = NULL; }

}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::SetH5Exitation(Double_t excMean, Double_t fwhm, Double_t distibWeight) {
  f5HExcitationMean.push_back(excMean);
  f5HExcitationSigma.push_back(fwhm / 2.355);
  if (!fIs5HExcitationSet) {
    f5HExcitationWeight.push_back(distibWeight);    
    fIs5HExcitationSet = true;
    return ;
  }
  f5HExcitationWeight.push_back(f5HExcitationWeight.back() + distibWeight);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1803::Init() {

  cout << "Decayer Init." << endl;

  f6He = TDatabasePDG::Instance()->GetParticle("6He");
  if ( ! f6He ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 6He not found in database!" << endl;
    return kFALSE;
  }

  f2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! f2H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion Deuteron not found in database!" << endl;
    return kFALSE;
  }

  f5H = TDatabasePDG::Instance()->GetParticle("5H");
  if ( ! f5H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 5H not found in database!" << endl;
    return kFALSE;
  }

  f3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName());
  if ( ! f3He ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion 3He not found in database!" << endl;
    return kFALSE;
  }

  f3H = TDatabasePDG::Instance()->GetParticle("Triton");
  if ( ! f3H ) {
    std::cerr  << "-W- ERDecayEXP1803: Ion Triton not found in database!" << endl;
    return kFALSE;
  }

  fn = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fn ) {
    std::cerr  << "-W- ERDecayEXP1803: Particle neutron not found in database!" << endl;
    return kFALSE;
  }
  if (fIs5HUserMassSet) {
    fUnstableIon5H->SetMass(f5HMass / .931494028);
  } else {
    f5HMass = f5H->Mass(); // if user mass is not defined in ERDecayEXP1803::SetH5Mass() than get a GEANT mass
  }
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1803::Stepping() {
  if(!fDecayFinish && gMC->TrackPid() == 1000020060 && TString(gMC->CurrentVolName()).Contains(fVolumeName)){
    gMC->SetMaxStep(fMinStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    if (curPos.Z() > fTargetReactZ){
      // std::cout << "Start reation in target. Defined pos: " << fTargetReactZ << ", current pos: " << curPos.Z() << endl;
      // 6He + 2H → 3He + 5H
      TLorentzVector lv6He;
      gMC->TrackMomentum(lv6He);

      if (lv6He.P() == 0) { // temporary fix of bug with zero linetic energy
        return kTRUE;
      }

      TLorentzVector lv2H(0., 0., 0., f2H->Mass());
      TLorentzVector lvReaction;
      lvReaction = lv6He + lv2H;

      const TVector3 boost = lvReaction.BoostVector(); //Get Pcm 3 vector
      Double_t ECM = 0;
      TLorentzVector lv6HeCM, lv2HCM;
      lv6HeCM = lv6He;
      lv2HCM = lv2H;
      lv6HeCM.Boost(-boost);
      lv2HCM.Boost(-boost);
      ECM = lv6HeCM(3) + lv2HCM(3);

      Int_t decayHappen = kFALSE;
      // while decay condition is not fullfilled  
      Double_t decay5HMass;
      while (decayHappen==kFALSE) { // сделать условие, что если не получается разыграть такой распад, то приступать к следующему событию (энергия пучка слигшком мала.)
        decay5HMass = f5HMass;
        Double_t excitation = 0;  // excitation energy
        if (fIs5HExcitationSet) {
          Double_t randWeight = gRandom->Uniform(0., f5HExcitationWeight.back());
          Int_t distribNum = 0;
          // choose distribution by weight
          for (; distribNum < f5HExcitationWeight.size(); distribNum++) {
            if (randWeight < f5HExcitationWeight[distribNum]) {
              break;
            }
          }
          excitation = gRandom->Gaus(f5HExcitationMean[distribNum], f5HExcitationSigma[distribNum]);
          fUnstableIon5H->SetExcEnergy(excitation);
        }
        decay5HMass += excitation;
        if((ECM - f3He->Mass() - decay5HMass) > 0) { // выход из цикла while для PhaseGen2
          decayHappen = kTRUE;
        }

        // Double_t reactMasses[2];
         //reactMasses[0] = f3He->Mass();
        // reactMasses[1] = decay5HMass;
       
         //decayHappen = fReactionPhaseSpace->SetDecay(lvReaction, 2, reactMasses);

        //if(decayHappen == kFALSE) cout << " forbidden " << ECM - f3He->Mass() - decay5HMass << endl;  
        // cout << lv6HeCM(3) << " " << lv2HCM(3) << " " << f3He->Mass() << " " <<  decay5HMass << endl; 
        // if (lv6He(3) == 0) {
          // cout << lv6He(0) << " " << lv6He(1) << " " << lv6He(2)<< " " <<lv6He(3) << endl;  
        // }
      }
      // cout << " allowed " << ECM - f3He->Mass() - decay5HMass << endl; 
      // cout << " MASS 5H " <<  decay5HMass << endl; 
      //// TLorentzVector *flv3He;
      //// TLorentzVector *lv5H;

      PhaseGen2(ECM, decay5HMass);
      flv5H->Boost(boost);
      flv3He->Boost(boost);

      // fReactionPhaseSpace->Generate();
    //   flv3He = fReactionPhaseSpace->GetDecay(0); // ?
      // flv5H = fReactionPhaseSpace->GetDecay(1); // ?

      //5H → f3H + n +n.
      Double_t decayMasses[3];
      decayMasses[0] = f3H->Mass();
      decayMasses[1] = fn->Mass(); 
      decayMasses[2] = fn->Mass();

      //cout << flv5H.E() << " its fine! " << decay5HMass << endl;

      fDecayPhaseSpace->SetDecay(*flv5H, 3, decayMasses);
      fDecayPhaseSpace->Generate();

      TLorentzVector *lv3H = fDecayPhaseSpace->GetDecay(0);
      TLorentzVector *lvn1 = fDecayPhaseSpace->GetDecay(1);
      TLorentzVector *lvn2 = fDecayPhaseSpace->GetDecay(2);


      Int_t He6TrackNb, H5TrackNb, He3TrackNb, H3TrackNb, n1TrackNb, n2TrackNb;

      He6TrackNb = gMC->GetStack()->GetCurrentTrackNumber();

      gMC->GetStack()->PushTrack(1, He6TrackNb, f5H->PdgCode(),
                                 flv5H->Px(), flv5H->Py(), flv5H->Pz(),
                                 flv5H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H5TrackNb, decay5HMass, 0);
      gMC->GetStack()->PushTrack(1, He6TrackNb, f3He->PdgCode(),
                                 flv3He->Px(), flv3He->Py(), flv3He->Pz(),
                                 flv3He->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, He3TrackNb, f3He->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He6TrackNb, f3H->PdgCode(),
                                 lv3H->Px(), lv3H->Py(), lv3H->Pz(),
                                 lv3H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H3TrackNb, f3H->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He6TrackNb, fn->PdgCode(),
                                 lvn1->Px(),lvn1->Py(),lvn1->Pz(),
                                 lvn1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n1TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He6TrackNb, fn->PdgCode(),
                                 lvn2->Px(),lvn2->Py(),lvn2->Pz(),
                                 lvn2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n2TrackNb, fn->Mass(), 0);
      LOG(INFO) << He6TrackNb << " " << H5TrackNb << " " << He3TrackNb << FairLogger::endl;
      gMC->StopTrack();
      fDecayFinish = kTRUE;
      gMC->SetMaxStep(100.);

      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetDecayPos(curPos.Vect());
        header->SetInputIon(He6TrackNb);
        header->AddOutputParticle(H5TrackNb);
        header->AddOutputParticle(He3TrackNb);
        header->AddOutputParticle(H3TrackNb);
        header->AddOutputParticle(n1TrackNb);
        header->AddOutputParticle(n2TrackNb);
      }
    }
  }
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::BeginEvent() { 
  fDecayFinish = kFALSE;
  fTargetReactZ = fRnd->Uniform(-fTargetThickness / 2, fTargetThickness / 2);
  FairRunSim* run = FairRunSim::Instance();
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::FinishEvent() {
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::PhaseGen2(Double_t Ecm, Double_t h5Mass) {
  //generate 2 - body decay in phase space approach.
  //Ecm -Total energy in CM
  //No security checks, cause it should be fast!

  //todo !!Vratislav: set excited masses should be function
  //it is the same algorithm for PhaseGen2 and phasegen3


  Double_t m1 = h5Mass;
  Double_t m2 = f3He->Mass();

  // Energy of 1-st particle in cm:
  //TODO Vratislav: check this statement
  //is it true also for binary reaction?
  Double_t E1 = 0.5 * (Ecm * Ecm + m1 * m1 - m2 * m2) / Ecm;

  //Impulse in CM
  Double_t Pcm = TMath::Sqrt(E1 * E1 - m1 * m1);

  //Generate angles of particles in CM

  Double_t thetaCM = TMath::ACos(fRnd2->Uniform(-1., 1.));
   //Double_t thetaCM = fADFunction->GetRandom(1.,150.)*TMath::DegToRad();
  Double_t phi = gRandom->Uniform(0., 2. * TMath::Pi());

  TVector3 Pcmv;
  Pcmv.SetMagThetaPhi(Pcm, thetaCM, phi);

  flv5H->SetXYZM(0., 0., 0., 0.);
  flv3He->SetXYZM(0., 0., 0., 0.);
  flv5H->SetXYZM(Pcmv(0), Pcmv(1), Pcmv(2), m1);
  flv3He->SetXYZM(-Pcmv(0), -Pcmv(1), -Pcmv(2), m2);
}

//-------------------------------------------------------------------------------------------------
Double_t ERDecayEXP1803::ADEvaluate(Double_t *x, Double_t *p) {
  //this function is necessary for TF1 constructor
  if (fADInput->IsZombie()) {
    Error("ERDecayEXP1803::ADEvaluate", "AD input was not loaded");
    return -1;
  }
  return fADInput->Eval(x[0]);
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1803::ReadADInput(TString ADfile) {
  fADFile = ADfile;

  if (fADInput) {
    delete fADInput;
    fADInput = NULL;
  }
  fADInput = new TGraph(fADFile, "%lg %*lg %lg");

  //create function from external input

  if (fADInput->IsZombie()) {
    Error("ERDecayEXP1803::CreateADFunction", "AD input cannot be read and AD function won't be initialized");
    return;
  }
  if (fADInput->GetN() <= 0) {
    Info("ERDecayEXP1803::CreateADFunction","Too few inputs for creation of AD function!");
    return;
  }
  Double_t* angle = fADInput->GetX();

  if (fADFunction) {
    delete fADFunction;
    fADFunction = NULL;
  }
  fADFunction = new TF1("angDistr", this, &ERDecayEXP1803::ADEvaluate, angle[0], angle[fADInput->GetN()-1], 0, "ERDecayEXP1803", "ADEvaluate");

  /*cout << "++++++++++++++++" << endl;
  for(Int_t i=0; i<fADInput->GetN(); i++) {
    cout << angle[i] << " " << fADFunction->Eval(angle[i]) << endl;
  }
  cout << "++++++++++++++++" << endl;*/

}

//-------------------------------------------------------------------------------------------------
ClassImp(ERDecayEXP1803)