/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecayEXP1811.h"

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
#include "EREXP1811EventHeader.h"
#include "ERMCEventHeader.h"

#include "G4IonTable.hh"

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
  fADFunction(NULL),
  fDecayFilePath(""),
  fDecayFileFinished(kFALSE),
  fDecayFileCurrentEvent(0)
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
}

//-------------------------------------------------------------------------------------------------
ERDecayEXP1811::~ERDecayEXP1811() {
  if (fDecayFile.is_open())
    fDecayFile.close();
  if (fDecayFilePath == ""){ // LV from TGenPhaseSpace will be deleted in TGenPhaseSpace
    if (fLv3H){
      delete fLv3H;
      delete fLvn1;
      delete fLvn2;
      delete fLvn3;
      delete fLvn4;
    }
  }
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


  if (fDecayFilePath != ""){
    LOG(INFO) << "Use decay kinematics from external text file" << FairLogger::endl;
    fDecayFile.open(fDecayFilePath.Data());
    if (!fDecayFile.is_open())
      LOG(FATAL) << "Can`t open decay file " << fDecayFilePath << FairLogger::endl;
    //Пропускаем шапку файла
    std::string header;
    std::getline(fDecayFile,header);

    fLv3H = new TLorentzVector();
    fLvn1 = new TLorentzVector();
    fLvn2 = new TLorentzVector();
    fLvn3 = new TLorentzVector();
    fLvn4 = new TLorentzVector();
  }

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
      
      Double_t decay7HMass;
      Int_t reactionAttempsCounter = 0;
      Double_t excitation = 0;  // excitation energy
      while (reactionHappen==kFALSE) { // while reaction condition is not fullfilled   
        decay7HMass = f7HMass;
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
        const float he3_mass = G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3;
        if((ECM - he3_mass - decay7HMass) > 0) { // выход из цикла while для PhaseGenerator
          reactionHappen = kTRUE;
          LOG(DEBUG) << "[ERDecayEXP1811] Reaction is happen" << endl;
        }
        reactionAttempsCounter++;
        if (reactionAttempsCounter > 1000){
          LOG(DEBUG) << "[ERDecayEXP1811] Reaction is forbidden for this CM energy" << endl;
          fDecayFinish = kTRUE;
          return kTRUE;
        }
      }

      ReactionPhaseGenerator(ECM, decay7HMass); 
      fLv7H->Boost(boost);
      fLv3He->Boost(boost);

      //7H → f3H + n +n +n +n
      if (!DecayPhaseGenerator(excitation)){
        fDecayFinish = kTRUE;
        return kTRUE;
      }

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
                                 fLv3H->Px(), fLv3H->Py(), fLv3H->Pz(),
                                 fLv3H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H3TrackNb, f3H->Mass(), 0);
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
      gMC->SetMaxStep(100.);

      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetReactionPos(curPos.Vect());
        header->SetInputIon(He8TrackNb);
        header->AddOutputParticle(H7TrackNb);
        header->AddOutputParticle(He3TrackNb);
        header->AddOutputParticle(H3TrackNb);
        header->AddOutputParticle(n1TrackNb);
        header->AddOutputParticle(n2TrackNb);
        header->AddOutputParticle(n3TrackNb);
        header->AddOutputParticle(n4TrackNb);
      }   
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("EREXP1811EventHeader")){   
        EREXP1811EventHeader* header = (EREXP1811EventHeader*)run->GetMCEventHeader();
        header->SetData(curPos.Vect(), lv8He,  lv2H, *fLv3He, *fLv3H,  *fLv7H, *fLvn1, *fLvn2, *fLvn3, *fLvn4);
        header->SetTrigger(1);
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
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("EREXP1811EventHeader")){   
    EREXP1811EventHeader* header = (EREXP1811EventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecayEXP1811::ReactionPhaseGenerator(Double_t Ecm, Double_t h7Mass) {
  Double_t m1 = h7Mass;
  Double_t m2 = G4IonTable::GetIonTable()->GetIon(2,3)->GetPDGMass() * 1e-3;

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
  Double_t phi = gRandom->Uniform(0., 2. * TMath::Pi());
  TVector3 Pcmv;
  Pcmv.SetMagThetaPhi(Pcm, thetaCM, phi);

  fLv7H->SetXYZM(0., 0., 0., 0.);
  fLv3He->SetXYZM(0., 0., 0., 0.);
  fLv7H->SetXYZM(Pcmv(0), Pcmv(1), Pcmv(2), m1);
  fLv3He->SetXYZM(-Pcmv(0), -Pcmv(1), -Pcmv(2), m2);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecayEXP1811::DecayPhaseGenerator(const Double_t excitation) {
  if (fDecayFilePath == ""){ // if decay file not defined, per morm decay using phase space
    Double_t decayMasses[5];
    decayMasses[0] = f3H->Mass();
    decayMasses[1] = fn->Mass(); 
    decayMasses[2] = fn->Mass();
    decayMasses[3] = fn->Mass(); 
    decayMasses[4] = fn->Mass();
    fDecayPhaseSpace->SetDecay(*fLv7H, 5, decayMasses);
    fDecayPhaseSpace->Generate();
    fLv3H = fDecayPhaseSpace->GetDecay(0);
    fLvn1 = fDecayPhaseSpace->GetDecay(1);
    fLvn2 = fDecayPhaseSpace->GetDecay(2);
    fLvn3 = fDecayPhaseSpace->GetDecay(3);
    fLvn4 = fDecayPhaseSpace->GetDecay(4);
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
  if (outputs_components.size() < 5*3){
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
  TVector3 p3H(std::stod(outputs_components[12]),std::stod(outputs_components[13]),
               std::stod(outputs_components[14]));
  // Apply scale factor
  const auto excitationScale = excitation > 0. ? sqrt(excitation / fDecayFileExcitation) : 1.;
  const auto MeV2GeV = 1./1000.;
  const auto scale = excitationScale * MeV2GeV;
  pn1 *= scale;
  pn2 *= scale;
  pn3 *= scale;
  pn4 *= scale;
  p3H *= scale;
  const auto fill_output_lorentz_vectors_in_lab = 
      [this](TLorentzVector* lv, const TVector3& p, const Double_t mass) {
        lv->SetXYZM(p.X(), p.Y(), p.Z(), mass);
        lv->Boost(fLv7H->BoostVector());
      };
  fill_output_lorentz_vectors_in_lab(fLvn1, pn1, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn2, pn2, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn3, pn3, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn4, pn4, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLv3H, p3H, f3H->Mass());
  return kTRUE;
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
  ADFilePath += "/input/" + ADFile;
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
    LOG(INFO) << "ERDecayEXP1811::SetAngularDistribution: "
              << "Too few inputs for creation of AD function!" << FairLogger::endl;
    return;
  }
  Double_t* angle = fADInput->GetX();  // get first column variables that contains number of point 

  // Creation of angular distribution function using class member function.
  // Constructor divides interval (0; fADInput->GetN()-1) into grid.
  // On each step of grid it calls ADEvaluate() to get interpolated values of input data.
  fThetaMin = angle[0];
  fThetaMax = angle[fADInput->GetN()-1];
  fADFunction = new TF1("angDistr", this, &ERDecayEXP1811::ADEvaluate, 
                         fThetaMin, fThetaMax, 0, "ERDecayEXP1811", "ADEvaluate");
  fADFunction->Eval(1.);
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERDecayEXP1811)
