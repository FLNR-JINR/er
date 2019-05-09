/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecayEXP1811.h"

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

ERDecayEXP1811::ERDecayEXP1811():
  ERDecay("EXP1811"),
  fDecayFinish(kFALSE),
  fTargetReactZ(0.),
  fMinStep(0.01),
  f8He(NULL),
  f2H (NULL),
  f3He(NULL),
  f7H (NULL),
  f3H (NULL),
  fn  (NULL),
  fIon3He(NULL),
  f7HMass(0.),
  fIs7HUserMassSet(false),
  fIs7HExcitationSet(false),
  fADInput(NULL),
  fADFunction(NULL)
{
  fRnd = new TRandom3();
  // fRnd->SetSeed();
  fRnd2 = new TRandom3();
  fRnd2->SetSeed();
  fReactionPhaseSpace = new TGenPhaseSpace();
  fDecayPhaseSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  fUnstableIon7H = new FairIon("7H",  1, 7, 1);
  fIon3He        = new FairIon("3He", 2, 3, 2);
  run->AddNewIon(fUnstableIon7H);
  run->AddNewIon(fIon3He);

  fLv7H = new TLorentzVector();
  fLv3He = new TLorentzVector();

  cout << "ERDecayEXP1811 constructed." << endl;
}

//-------------------------------------------------------------------------------------------------
ERDecayEXP1811::~ERDecayEXP1811() {
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::SetH7Exitation(Double_t excMean, Double_t fwhm, Double_t distibWeight) {
  f7HExcitationMean.push_back(excMean);
  f7HExcitationSigma.push_back(fwhm / 2.355);
  if (!fIs7HExcitationSet) {
    f7HExcitationWeight.push_back(distibWeight);    
    fIs7HExcitationSet = true;
    return ;
  }
  f7HExcitationWeight.push_back(f7HExcitationWeight.back() + distibWeight);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1811::Init() {

  cout << "Decayer Init." << endl;

  f8He = TDatabasePDG::Instance()->GetParticle("8He");
  if ( ! f8He ) {
    std::cerr  << "-W- ERDecayEXP1811: Ion 8He not found in database!" << endl;
    return kFALSE;
  }

  f2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! f2H ) {
    std::cerr  << "-W- ERDecayEXP1811: Ion Deuteron not found in database!" << endl;
    return kFALSE;
  }

  f7H = TDatabasePDG::Instance()->GetParticle("7H");
  if ( ! f7H ) {
    std::cerr  << "-W- ERDecayEXP1811: Ion 7H not found in database!" << endl;
    return kFALSE;
  }

  f3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName());
  if ( ! f3He ) {
    std::cerr  << "-W- ERDecayEXP1811: Ion 3He not found in database!" << endl;
    return kFALSE;
  }

  f3H = TDatabasePDG::Instance()->GetParticle("Triton");
  if ( ! f3H ) {
    std::cerr  << "-W- ERDecayEXP1811: Ion Triton not found in database!" << endl;
    return kFALSE;
  }

  fn = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fn ) {
    std::cerr  << "-W- ERDecayEXP1811: Particle neutron not found in database!" << endl;
    return kFALSE;
  }
  if (fIs7HUserMassSet) {
    fUnstableIon7H->SetMass(f7HMass / .931494028);
  } else {
    f7HMass = f7H->Mass(); // if user mass is not defined in ERDecayEXP1811::SetH7Mass() than get a GEANT mass
  }
  CalculateTargetParameters();
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1811::Stepping() {
  if(!fDecayFinish && gMC->TrackPid() == 1000020080 
     && TString(gMC->CurrentVolName()).Contains(GetInteractionVolumeName()))
  {
    if (!fIsInterationPointFound) {
      if (!FindInteractionPoint()) {
        fDecayFinish = kTRUE;
        return kFALSE;
      } else {
        fDistanceFromEntrance = 0;
      }
    }
    gMC->SetMaxStep(fMinStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    Double_t trackStep = gMC->TrackStep();
    fDistanceFromEntrance += trackStep;
    // std::cout << "Track step: " << fDistanceFromEntrance << "; Diff " << (curPos.Vect() - fInputPoint).Mag() <<  endl;    
    // std::cout << "Track step: " << fDistanceFromEntrance <<  endl;    
    if (fDistanceFromEntrance > fDistanceToInteractPoint) {
      // std::cout << "Start reation in target. Defined pos: " << fDistanceToInteractPoint << ", current pos: " << curPos.Z() << endl;
      // 8He + 2H → 3He + 7H
      TLorentzVector lv8He;
      gMC->TrackMomentum(lv8He);
      
      if (lv8He.P() == 0) { // temporary fix of bug with zero kinetic energy
        return kTRUE;
      }

      TLorentzVector lv2H(0., 0., 0., f2H->Mass());
      TLorentzVector lvReaction;
      lvReaction = lv8He + lv2H;

      const TVector3 boost = lvReaction.BoostVector(); //Get Pcm 3 vector
      Double_t ECM = 0;
      TLorentzVector lv8HeCM, lv2HCM;
      lv8HeCM = lv8He;
      lv2HCM = lv2H;
      lv8HeCM.Boost(-boost);
      lv2HCM.Boost(-boost);
      ECM = lv8HeCM(3) + lv2HCM(3);

      Int_t decayHappen = kFALSE;
      
      Double_t decay7HMass;
      while (decayHappen==kFALSE) { // while decay condition is not fullfilled   
        decay7HMass = f7HMass;
        Double_t excitation = 0;  // excitation energy
        if (fIs7HExcitationSet) {
          Double_t randWeight = gRandom->Uniform(0., f7HExcitationWeight.back());
          Int_t distribNum = 0;
          // choose distribution by weight
          for (; distribNum < f7HExcitationWeight.size(); distribNum++) {
            if (randWeight < f7HExcitationWeight[distribNum]) {
              break;
            }
          }
          excitation = gRandom->Gaus(f7HExcitationMean[distribNum], f7HExcitationSigma[distribNum]);
          fUnstableIon7H->SetExcEnergy(excitation);
        }
        decay7HMass += excitation;
        if((ECM - f3He->Mass() - decay7HMass) > 0) { // выход из цикла while для PhaseGenerator
          decayHappen = kTRUE;
        }
        cout << "[ERDecayEXP1811::Stepping] mass" << endl;
      }

      PhaseGenerator(ECM, decay7HMass);
      fLv7H->Boost(boost);
      fLv3He->Boost(boost);

      //7H → f3H + n +n +n +n
      Double_t decayMasses[5];
      decayMasses[0] = f3H->Mass();
      decayMasses[1] = fn->Mass(); 
      decayMasses[2] = fn->Mass();
      decayMasses[3] = fn->Mass(); 
      decayMasses[4] = fn->Mass();

      fDecayPhaseSpace->SetDecay(*fLv7H, 5, decayMasses);
      fDecayPhaseSpace->Generate();

      TLorentzVector *lv3H = fDecayPhaseSpace->GetDecay(0);
      TLorentzVector *lvn1 = fDecayPhaseSpace->GetDecay(1);
      TLorentzVector *lvn2 = fDecayPhaseSpace->GetDecay(2);
      TLorentzVector *lvn3 = fDecayPhaseSpace->GetDecay(3);
      TLorentzVector *lvn4 = fDecayPhaseSpace->GetDecay(4);

      Int_t He8TrackNb, H7TrackNb, He3TrackNb, H3TrackNb, n1TrackNb, n2TrackNb, n3TrackNb, n4TrackNb;

      He8TrackNb = gMC->GetStack()->GetCurrentTrackNumber();
      // std::cout << "He8TrackNb " << He8TrackNb << std::endl;
      /*
      gMC->GetStack()->PushTrack(1, He8TrackNb, f7H->PdgCode(),
                                 fLv7H->Px(), fLv7H->Py(), fLv7H->Pz(),
                                 fLv7H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H7TrackNb, decay7HMass, 0);*/

      gMC->GetStack()->PushTrack(1, He8TrackNb, f3He->PdgCode(),
                                 fLv3He->Px(), fLv3He->Py(), fLv3He->Pz(),
                                 fLv3He->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, He3TrackNb, f3He->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, f3H->PdgCode(),
                                 lv3H->Px(), lv3H->Py(), lv3H->Pz(),
                                 lv3H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H3TrackNb, f3H->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 lvn1->Px(),lvn1->Py(),lvn1->Pz(),
                                 lvn1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n1TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 lvn2->Px(),lvn2->Py(),lvn2->Pz(),
                                 lvn2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n2TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 lvn3->Px(),lvn3->Py(),lvn3->Pz(),
                                 lvn3->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n3TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 lvn4->Px(),lvn4->Py(),lvn4->Pz(),
                                 lvn4->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n4TrackNb, fn->Mass(), 0);
      gMC->StopTrack();
      fDecayFinish = kTRUE;
      gMC->SetMaxStep(100.);

      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetDecayPos(curPos.Vect());
        header->SetInputIon(He8TrackNb);
        header->AddOutputParticle(H7TrackNb);
        header->AddOutputParticle(He3TrackNb);
        header->AddOutputParticle(H3TrackNb);
        header->AddOutputParticle(n1TrackNb);
        header->AddOutputParticle(n2TrackNb);
        header->AddOutputParticle(n3TrackNb);
        header->AddOutputParticle(n4TrackNb);
      }
    }
  }
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::BeginEvent() { 
  fDecayFinish = kFALSE;
  fIsInterationPointFound = kFALSE;
  fTargetReactZ = fRnd->Uniform(-fTargetThickness / 2, fTargetThickness / 2);
  FairRunSim* run = FairRunSim::Instance();
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::FinishEvent() {
  FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::PhaseGenerator(Double_t Ecm, Double_t h7Mass) {
  Double_t m1 = h7Mass;
  Double_t m2 = f3He->Mass();

  // Energy of 1-st particle in cm.
  // total energy of the first particle is calculated as
  Double_t E1 = 0.5 * (Ecm * Ecm + m1 * m1 - m2 * m2) / Ecm;

  //Impulse in CM
  Double_t Pcm = TMath::Sqrt(E1 * E1 - m1 * m1);

  //Generate angles of particles in CM
  Double_t thetaCM;
  if(fADInput == NULL) { // if file with angular distribution isn't setted than isotropic distribution is generated
    thetaCM = TMath::ACos(gRandom->Uniform(-1, 1));
  }
  else { 
    thetaCM = fADFunction->GetRandom(1., fADInput->GetN()-1)*TMath::DegToRad();
  }
  Double_t phi = gRandom->Uniform(0., 2. * TMath::Pi());

  TVector3 Pcmv;
  Pcmv.SetMagThetaPhi(Pcm, thetaCM, phi);

  fLv7H->SetXYZM(0., 0., 0., 0.);
  fLv3He->SetXYZM(0., 0., 0., 0.);
  fLv7H->SetXYZM(Pcmv(0), Pcmv(1), Pcmv(2), m1);
  fLv3He->SetXYZM(-Pcmv(0), -Pcmv(1), -Pcmv(2), m2);
}

//-------------------------------------------------------------------------------------------------
Double_t ERDecayEXP1811::ADEvaluate(Double_t *x, Double_t *p) {
  if (fADInput->IsZombie()) {
    Error("ERDecayEXP1811::ADEvaluate", "AD input was not loaded");
    return -1;
  }
  // on each step of creating distribution function returns interpolated value of input data
  return fADInput->Eval(x[0]);
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::SetAngularDistribution(TString ADFile) {
  TString ADFilePath = gSystem->Getenv("VMCWORKDIR");
  ADFilePath += "/input/generators/" + ADFile; 

  fADInput = new TGraph(ADFilePath, "%lg %*lg %lg");   // TGraph object is used for reading data from file with distribution
  
  if (fADInput->GetN() <= 0) { //if there are no points in input file
    LOG(INFO) << "ERDecayEXP1811::SetAngularDistribution: "
              << "Too few inputs for creation of AD function!" << FairLogger::endl;
    return;
  }
  Double_t* angle = fADInput->GetX();  // get first column variables that contains number of point 

  // Creation of angular distribution function using class member function.
  // Constructor divides interval (0; fADInput->GetN()-1) into grid.
  // On each step of grid it calls ADEvaluate() to get interpolated values of input data.
  fADFunction = new TF1("angDistr", this, &ERDecayEXP1811::ADEvaluate, 
                         angle[0], angle[fADInput->GetN()-1], 0, "ERDecayEXP1811", "ADEvaluate");
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERDecayEXP1811)
