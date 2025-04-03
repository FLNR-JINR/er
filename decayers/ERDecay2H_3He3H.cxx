/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecay2H_3He3H.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "TVirtualMC.h"
#include "TLorentzVector.h"
#include "TMCProcess.h"
#include "TRandom.h"
#include "TVector.h"

#include "FairRunSim.h"
#include "FairLogger.h"

#include "ERDecayMCEventHeader.h"
#include "ER2H_3He3HEventHeader.h"
#include "ERMCEventHeader.h"

#include "G4IonTable.hh"

ERDecay2H_3He3H::ERDecay2H_3He3H():
  ERDecay("2H_3He3H"),
  fDecayFinish(kFALSE),
  fDecay6LiFinish(kFALSE),
  fTargetReactZ(0.),
  fMinStep(0.01),
  f8He(NULL),
  f2H (NULL),
  f6Li(NULL),
  f3H (NULL),
  f3He (NULL),
  fn  (NULL),
  fUnstable6Li(NULL),
  f4nMass(0.),
  fIs4nUserMassSet(false),
  fIs4nExcitationSet(false),
  fIs6LiExcitationSet(false),
  fADInput(NULL),
  fADFunction(NULL),
  fDecayFilePath(""),
  fDecayFileFinished(kFALSE),
  fDecayFileCurrentEvent(0),
  fDecay6LiFilePath(""),
  fDecay6LiFileFinished(kFALSE),
  fDecay6LiFileCurrentEvent(0)  
{
  fRnd = new TRandom3();
  // fRnd->SetSeed();
  fRnd2 = new TRandom3();
  fRnd2->SetSeed();
  fReactionPhaseSpace = new TGenPhaseSpace();
  fDecayPhaseSpace = new TGenPhaseSpace();
  fDecay6LiPhaseSpace = new TGenPhaseSpace();
  FairRunSim* run = FairRunSim::Instance();
  // fUnstable4n = new FairIon("4n",  0, 4, 0);
  fUnstable6Li     = new FairIon("6Li", 3, 6, 3);
  fIon3He     = new FairIon("3He", 2, 3, 2);
  fIon3H     = new FairIon("3H", 1, 3, 1);
  run->AddNewIon(fIon3H);
  run->AddNewIon(fIon3He);
  run->AddNewIon(fUnstable6Li);

  fLv4n = new TLorentzVector();
  fLv6Li = new TLorentzVector();
  LOG(DEBUG) << "LVS created" << endl;
}

//-------------------------------------------------------------------------------------------------
ERDecay2H_3He3H::~ERDecay2H_3He3H() {
  if (fDecayFile.is_open())
    fDecayFile.close();
  if (fDecayFilePath == ""){ // LV from TGenPhaseSpace will be deleted in TGenPhaseSpace
      delete fLvn1;
      delete fLvn2;
      delete fLvn3;
      delete fLvn4;
  }
  if (fDecay6LiFile.is_open())
    fDecay6LiFile.close();
  if (fDecay6LiFilePath == ""){ // LV from TGenPhaseSpace will be deleted in TGenPhaseSpace
      delete fLv3H;
      delete fLv3He;
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::Set4nExitation(Double_t excMean, Double_t fwhm, Double_t distibWeight) {
  f4nExcitationMean.push_back(excMean);
  f4nExcitationSigma.push_back(fwhm / 2.355);
  if (!fIs4nExcitationSet) {
    f4nExcitationWeight.push_back(distibWeight);    
    fIs4nExcitationSet = true;
    return ;
  }
  f4nExcitationWeight.push_back(f4nExcitationWeight.back() + distibWeight);
}
//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::Set6LiExitation(Double_t excMean, Double_t fwhm, Double_t distibWeight) {
  f6LiExcitationMean.push_back(excMean);
  f6LiExcitationSigma.push_back(fwhm / 2.355);
  if (!fIs6LiExcitationSet) {
    f6LiExcitationWeight.push_back(distibWeight);    
    fIs6LiExcitationSet = true;
    return;
  }
  f6LiExcitationWeight.push_back(f6LiExcitationWeight.back() + distibWeight);
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecay2H_3He3H::Init() {

  cout << "Decayer Init." << endl;

  f8He = TDatabasePDG::Instance()->GetParticle("8He");
  if ( ! f8He ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Ion 8He not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "8He " << f8He->Mass() << " " << G4IonTable::GetIonTable()->GetIon(2,8)->GetPDGMass() * 1e-3 << std::endl;

  f2H = TDatabasePDG::Instance()->GetParticle("Deuteron");
  if ( ! f2H ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Ion Deuteron not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "2H " << f2H->Mass() << " " << G4IonTable::GetIonTable()->GetIon(1,2)->GetPDGMass() * 1e-3 << std::endl;

  f3H = TDatabasePDG::Instance()->GetParticle(fIon3H->GetName());
  if ( ! f3H ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Ion 3H not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "3H " << f3H->Mass() << " " << G4IonTable::GetIonTable()->GetIon(1,3)->GetPDGMass() * 1e-3 << std::endl;

  f3He = TDatabasePDG::Instance()->GetParticle(fIon3He->GetName());
  if ( ! f3He ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Ion 3He not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "3He " << f3He->Mass() << " " << G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3 << std::endl;

  f6Li = TDatabasePDG::Instance()->GetParticle(fUnstable6Li->GetName());
  if ( ! f6Li ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Ion 6Li not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "6Li " << f6Li->Mass() << " " << G4IonTable::GetIonTable()->GetIon(3,6)->GetPDGMass() * 1e-3 << std::endl;

  fn = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fn ) {
    std::cerr  << "-W- ERDecay2H_3He3H: Particle neutron not found in database!" << endl;
    return kFALSE;
  }
  std::cout << "neutron " << fn->Mass() << std::endl;
  // if (fIs4nUserMassSet) {
  //   fUnstable4n->SetMass(f4nMass / .931494028);
  // } else {
  //   f4nMass = f4n->Mass(); // if user mass is not defined in ERDecay2H_3He3H::SetH7Mass() than get a GEANT mass
  // }
  CalculateTargetParameters();


  if (fDecayFilePath != ""){
    LOG(INFO) << "Use decay kinematics from external text file" << FairLogger::endl;
    fDecayFile.open(fDecayFilePath.Data());
    if (!fDecayFile.is_open())
      LOG(FATAL) << "Can`t open decay file " << fDecayFilePath << FairLogger::endl;
    //Пропускаем шапку файла
    std::string header;
    std::getline(fDecayFile,header);

    fLvn1 = new TLorentzVector();
    fLvn2 = new TLorentzVector();
    fLvn3 = new TLorentzVector();
    fLvn4 = new TLorentzVector();
  }

  if (fDecay6LiFilePath != ""){
    LOG(INFO) << "Use decay kinematics from external text file" << FairLogger::endl;
    fDecay6LiFile.open(fDecay6LiFilePath.Data());
    if (!fDecay6LiFile.is_open())
      LOG(FATAL) << "Can`t open decay file " << fDecay6LiFilePath << FairLogger::endl;
    //Пропускаем шапку файла
    std::string header;
    std::getline(fDecay6LiFile,header);
    fLv3H = new TLorentzVector();
    fLv3He = new TLorentzVector();
  }

  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecay2H_3He3H::Stepping() {
  if(!fDecayFinish && !fDecay6LiFinish && gMC->TrackPid() == 1000020080
     && TString(gMC->CurrentVolName()).Contains(GetInteractionVolumeName()))
  {
    if (!fIsInterationPointFound) {
      if (!FindInteractionPoint()) {
        fDecayFinish = kTRUE;
        fDecay6LiFinish = kTRUE;
        return kTRUE;
      } else {
        fDistanceFromEntrance = 0;
      }
    }
    gMC->SetMaxStep(fMinStep);
    TLorentzVector curPos;
    gMC->TrackPosition(curPos);
    Double_t trackStep = gMC->TrackStep();
    fDistanceFromEntrance += trackStep;
  
    if (fDistanceFromEntrance > fDistanceToInteractPoint) {
      // std::cout << "Start reation in target. Defined pos: " << fDistanceToInteractPoint << ", current pos: " << curPos.Z() << endl;
      
      // 8He + 2H → 6Li + 4n
      TLorentzVector lv8He;
      gMC->TrackMomentum(lv8He);
      
      if (lv8He.P() == 0) { // temporary fix of bug with zero kinetic energy
        return kTRUE;
      }

      TLorentzVector lv2H(0., 0., 0.,  1.875612);
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

      Int_t reactionHappen = kFALSE;
      
      Double_t decay4nMass;
      Double_t decay6LiMass;
      Int_t reactionAttempsCounter = 0;
      Double_t excitation4n = 0;  // excitation energy
      Double_t excitation6Li = 0;  // excitation energy
      while (reactionHappen==kFALSE) { // while reaction condition is not fullfilled   
        decay4nMass = f4nMass;
        if (fIs4nExcitationSet) {
          Double_t randWeight = gRandom->Uniform(0., f4nExcitationWeight.back());
          Int_t distribNum = 0;
          // choose distribution by weight
          for (; distribNum < f4nExcitationWeight.size(); distribNum++) {
            if (randWeight < f4nExcitationWeight[distribNum]) {
              break;
            }
          }
          excitation4n = gRandom->Gaus(f4nExcitationMean[distribNum], f4nExcitationSigma[distribNum]);
          // fUnstable4n->SetExcEnergy(excitation);
        }
        decay4nMass += excitation4n;

        if (fIs6LiExcitationSet) {
          Double_t randWeight = gRandom->Uniform(0., f6LiExcitationWeight.back());
          Int_t distribNum = 0;
          // choose distribution by weight
          for (; distribNum < f6LiExcitationWeight.size(); distribNum++) {
            if (randWeight < f6LiExcitationWeight[distribNum]) {
              break;
            }
          }
          excitation6Li = gRandom->Gaus(f6LiExcitationMean[distribNum], f6LiExcitationSigma[distribNum]);
          // fUnstable4n->SetExcEnergy(excitation);
        }
        decay6LiMass = G4IonTable::GetIonTable()->GetIon(3,6)->GetPDGMass() * 1e-3;
        decay6LiMass += excitation6Li;

        if((ECM - decay6LiMass - decay4nMass) > 0) { // выход из цикла while для PhaseGenerator
          reactionHappen = kTRUE;
          LOG(DEBUG) << "[ERDecay2H_3He3H] Reaction is happen" << endl;
        }
        reactionAttempsCounter++;
        if (reactionAttempsCounter > 1000){
          LOG(DEBUG) << "[ERDecay2H_3He3H] Reaction is forbidden for this CM energy" << endl;
          fDecayFinish = kTRUE;
          fDecay6LiFinish = kTRUE;
          return kTRUE;
        }
      }
      ReactionPhaseGenerator(ECM, decay4nMass, excitation6Li); 
      fLv4n->Boost(boost);
      fLv6Li->Boost(boost);
      //4n → n +n +n +n. 6Li->3He + 3H
      if (!fDecayFinish) {
        DecayPhaseGenerator(excitation4n);
      }
      if (!fDecay6LiFinish) {
        Decay6LiPhaseGenerator(excitation6Li);
      }      
      // if (!DecayPhaseGenerator(excitation4n)){
      //   fDecayFinish = kTRUE;
      //   return kTRUE;
      // }
      // if (!Decay6LiPhaseGenerator(excitation6Li)){
      //   fDecay6LiFinish = kTRUE;
      //   return kTRUE;
      // }      
      Int_t He8TrackNb, tetraNTrackNb, Li6TrackNb, He3TrackNb, H3TrackNb, n1TrackNb, n2TrackNb, n3TrackNb, n4TrackNb;

      He8TrackNb = gMC->GetStack()->GetCurrentTrackNumber();
      // std::cout << "He8TrackNb " << He8TrackNb << std::endl;
      /*
      gMC->GetStack()->PushTrack(1, He8TrackNb, f7H->PdgCode(),
                                 fLv7H->Px(), fLv7H->Py(), fLv7H->Pz(),
                                 fLv7H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H7TrackNb, decay7HMass, 0);*/
      gMC->GetStack()->PushTrack(1, He8TrackNb, f6Li->PdgCode(),
                                 fLv6Li->Px(), fLv6Li->Py(), fLv6Li->Pz(),
                                 fLv6Li->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, Li6TrackNb, decay6LiMass, 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, f3H->PdgCode(),
                                 fLv3H->Px(), fLv3H->Py(), fLv3H->Pz(),
                                 fLv3H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H3TrackNb, G4IonTable::GetIonTable()->GetIon(1,3)->GetPDGMass() * 1e-3, 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, f3He->PdgCode(),
                                 fLv3He->Px(), fLv3He->Py(), fLv3He->Pz(),
                                 fLv3He->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, He3TrackNb, G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3, 0);                                        
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 fLvn1->Px(),fLvn1->Py(),fLvn1->Pz(),
                                 fLvn1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n1TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 fLvn2->Px(),fLvn2->Py(),fLvn2->Pz(),
                                 fLvn2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n2TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 fLvn3->Px(),fLvn3->Py(),fLvn3->Pz(),
                                 fLvn3->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n3TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8TrackNb, fn->PdgCode(),
                                 fLvn4->Px(),fLvn4->Py(),fLvn4->Pz(),
                                 fLvn4->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n4TrackNb, fn->Mass(), 0);
      gMC->StopTrack();
      fDecayFinish = kTRUE;
      fDecay6LiFinish = kTRUE;
      gMC->SetMaxStep(100.);
      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetReactionPos(curPos.Vect());
        header->SetInputIon(He8TrackNb);
        header->AddOutputParticle(tetraNTrackNb);
        header->AddOutputParticle(Li6TrackNb);
        header->AddOutputParticle(H3TrackNb);
        header->AddOutputParticle(He3TrackNb);        
        header->AddOutputParticle(n1TrackNb);
        header->AddOutputParticle(n2TrackNb);
        header->AddOutputParticle(n3TrackNb);
        header->AddOutputParticle(n4TrackNb);
      }   
      // std::cout << fLv3H->Mag() << " " << fLv3He->Mag() << std::endl;
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ER2H_3He3HEventHeader")){   
        ER2H_3He3HEventHeader* header = (ER2H_3He3HEventHeader*)run->GetMCEventHeader();
        header->SetData(curPos.Vect(), lv8He,  lv2H, *fLv6Li, *fLv4n, *fLvn1, *fLvn2, *fLvn3, *fLvn4, *fLv3H, *fLv3He, fTheta);
        header->SetTrigger(1);
      }
    }
  }
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::BeginEvent() { 
  fDecayFinish = kFALSE;
  fDecay6LiFinish = kFALSE;
  fIsInterationPointFound = kFALSE;
  fTargetReactZ = fRnd->Uniform(-fTargetThickness / 2, fTargetThickness / 2);
  FairRunSim* run = FairRunSim::Instance();
}

//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::FinishEvent() {
  FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ER2H_3He3HEventHeader")){   
    ER2H_3He3HEventHeader* header = (ER2H_3He3HEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::ReactionPhaseGenerator(Double_t Ecm, Double_t n4Mass, Double_t exc) {
  Double_t m1 = n4Mass;
  Double_t m2 = G4IonTable::GetIonTable()->GetIon(3,6)->GetPDGMass() * 1e-3 + exc;

  // Energy of 1-st particle in cm.
  // total energy of the first particle is calculated as
  Double_t E1 = 0.5 * (Ecm * Ecm + m1 * m1 - m2 * m2) / Ecm;

  //Impulse in CM
  Double_t Pcm = TMath::Sqrt(E1 * E1 - m1 * m1);
  //Generate angles of particles in CM
  Double_t thetaCM;
  if(!fADInput) { // if file with angular distribution isn't setted than isotropic distribution is generated
    thetaCM = TMath::ACos(gRandom->Uniform(-1, 1));
  } else { 
    thetaCM = fADFunction->GetRandom(fThetaMin, fThetaMax)*TMath::DegToRad();
  }
  fTheta = thetaCM;
  Double_t phi = gRandom->Uniform(0., 2. * TMath::Pi());
  TVector3 Pcmv;
  Pcmv.SetMagThetaPhi(Pcm, thetaCM, phi);

  fLv4n->SetXYZM(0., 0., 0., 0.);
  fLv6Li->SetXYZM(0., 0., 0., 0.);
  fLv4n->SetXYZM(Pcmv(0), Pcmv(1), Pcmv(2), m1);
  fLv6Li->SetXYZM(-Pcmv(0), -Pcmv(1), -Pcmv(2), m2);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecay2H_3He3H::DecayPhaseGenerator(const Double_t excitation) {
  if (fDecayFilePath == ""){ // if decay file not defined, per morm decay using phase space
    Double_t decayMasses[4];
    decayMasses[0] = fn->Mass(); 
    decayMasses[1] = fn->Mass(); 
    decayMasses[2] = fn->Mass();
    decayMasses[3] = fn->Mass(); 
    fDecayPhaseSpace->SetDecay(*fLv4n, 4, decayMasses);
    fDecayPhaseSpace->Generate();
    fLvn1 = fDecayPhaseSpace->GetDecay(0);
    fLvn2 = fDecayPhaseSpace->GetDecay(1);
    fLvn3 = fDecayPhaseSpace->GetDecay(2);
    fLvn4 = fDecayPhaseSpace->GetDecay(3);
    return kTRUE;
  }
  if (fDecayFile.eof()){
    LOG(ERROR) << "Decay file finished! There are no more events in file " << fDecayFilePath
               << " to be processed." << FairLogger::endl;
    return kFALSE;
  }
  std::string event_line;
  std::getline(fDecayFile,event_line);
  std::istringstream iss(event_line);
  std::vector<std::string> outputs_components((std::istream_iterator<std::string>(iss)),
                                               std::istream_iterator<std::string>());
  if (outputs_components.size() < 4*3){
    LOG(ERROR) << "Wrong components number in raw in decay file!" << FairLogger::endl;
    return kFALSE;
  }
  // Fill momentum vectors in CM.
  TVector3 pn1(std::stod(outputs_components[0]),std::stod(outputs_components[1]),
               std::stod(outputs_components[2]));
  TVector3 pn2(std::stod(outputs_components[3]),std::stod(outputs_components[4]),
               std::stod(outputs_components[5]));
  TVector3 pn3(std::stod(outputs_components[6]),std::stod(outputs_components[7]),
               std::stod(outputs_components[8]));
  TVector3 pn4(std::stod(outputs_components[9]),std::stod(outputs_components[10]),
               std::stod(outputs_components[11]));
  // Apply scale factor
  const auto excitationScale = excitation > 0. ? sqrt(excitation / fDecayFileExcitation) : 1.;
  const auto MeV2GeV = 1./1000.;
  const auto scale = excitationScale * MeV2GeV;
  pn1 *= scale;
  pn2 *= scale;
  pn3 *= scale;
  pn4 *= scale;
  const auto fill_output_lorentz_vectors_in_lab = 
      [this](TLorentzVector* lv, const TVector3& p, const Double_t mass) {
        lv->SetXYZM(p.X(), p.Y(), p.Z(), mass);
        lv->Boost(fLv4n->BoostVector());
      };
  fill_output_lorentz_vectors_in_lab(fLvn1, pn1, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn2, pn2, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn3, pn3, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn4, pn4, fn->Mass());

  return kTRUE;
}
//-------------------------------------------------------------------------------------------------
Bool_t ERDecay2H_3He3H::Decay6LiPhaseGenerator(const Double_t excitation) {
  if (fDecay6LiFilePath == ""){ // if decay file not defined, per morm decay using phase space
    Double_t decayMasses[2];
    decayMasses[0] = G4IonTable::GetIonTable()->GetIon(1,3)->GetPDGMass() * 1e-3; 
    decayMasses[1] = G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3; 
    fDecay6LiPhaseSpace->SetDecay(*fLv6Li, 2, decayMasses);
    fDecay6LiPhaseSpace->Generate();
    fLv3H = fDecay6LiPhaseSpace->GetDecay(0);
    fLv3He = fDecay6LiPhaseSpace->GetDecay(1);
    return kTRUE;
  }
  if (fDecay6LiFile.eof()){
    LOG(ERROR) << "Decay file finished! There are no more events in file " << fDecay6LiFilePath
               << " to be processed." << FairLogger::endl;
    return kFALSE;
  }
  std::string event_line;
  std::getline(fDecayFile,event_line);
  std::istringstream iss(event_line);
  std::vector<std::string> outputs_components((std::istream_iterator<std::string>(iss)),
                                               std::istream_iterator<std::string>());
  if (outputs_components.size() < 2*3){
    LOG(ERROR) << "Wrong components number in raw in decay file!" << FairLogger::endl;
    return kFALSE;
  }
  // Fill momentum vectors in CM.
  TVector3 p3h(std::stod(outputs_components[0]),std::stod(outputs_components[1]),
               std::stod(outputs_components[2]));
  TVector3 p3he(std::stod(outputs_components[3]),std::stod(outputs_components[4]),
               std::stod(outputs_components[5]));
  // Apply scale factor
  const auto excitationScale = excitation > 0. ? sqrt(excitation / fDecay6LiFileExcitation) : 1.;
  const auto MeV2GeV = 1./1000.;
  const auto scale = excitationScale * MeV2GeV;
  p3h *= scale;
  p3he *= scale;
  const auto fill_output_lorentz_vectors_in_lab = 
      [this](TLorentzVector* lv, const TVector3& p, const Double_t mass) {
        lv->SetXYZM(p.X(), p.Y(), p.Z(), mass);
        lv->Boost(fLv6Li->BoostVector());
      };
  fill_output_lorentz_vectors_in_lab(fLv3H, p3h, G4IonTable::GetIonTable()->GetIon(1,3)->GetPDGMass() * 1e-3);
  fill_output_lorentz_vectors_in_lab(fLv3He, p3he, G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3);
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Double_t ERDecay2H_3He3H::ADEvaluate(Double_t *x, Double_t *p) {
  if (fADInput->IsZombie()) {
    Error("ERDecay2H_3He3H::ADEvaluate", "AD input was not loaded");
    return -1;
  }
  // on each step of creating distribution function returns interpolated value of input data
  return fADInput->Eval(x[0]);
}

//-------------------------------------------------------------------------------------------------
void ERDecay2H_3He3H::SetAngularDistribution(TString ADFile) {
  TString ADFilePath = gSystem->Getenv("VMCWORKDIR");
  ADFilePath =  ADFile;
  std::ifstream f;
  f.open(ADFilePath.Data());
  if (!f.is_open()) {
    LOG(FATAL) << "Can't open file " << ADFilePath << FairLogger::endl;
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
  fADInput = new TGraph(tet, sigma);
  if (fADInput->GetN() <= 0) { //if there are no points in input file
    LOG(INFO) << "ERDecay2H_3He3H::SetAngularDistribution: "
              << "Too few inputs for creation of AD function!" << FairLogger::endl;
    return;
  }
  Double_t* angle = fADInput->GetX();  // get first column variables that contains number of point 

  // Creation of angular distribution function using class member function.
  // Constructor divides interval (0; fADInput->GetN()-1) into grid.
  // On each step of grid it calls ADEvaluate() to get interpolated values of input data.
  fThetaMin = angle[0];
  fThetaMax = angle[fADInput->GetN()-1];
  fADFunction = new TF1("angDistr", this, &ERDecay2H_3He3H::ADEvaluate, 
                         fThetaMin, fThetaMax, 0, "ERDecay2H_3He3H", "ADEvaluate");
  // fADFunction->Eval(1.);
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERDecay2H_3He3H)
