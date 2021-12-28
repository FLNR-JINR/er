/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDecay10Heto8He.h"

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
#include "ER10Heto8HeEventHeader.h"
#include "ERMCEventHeader.h"

ERDecay10Heto8He::ERDecay10Heto8He():
  ERDecay("10Heto8He"),
  fDecayFinish(kFALSE),
  fTargetReactZ(0.),
  fMinStep(0.01),
  f8He(NULL),
  f10He(NULL),
  f3H(NULL),
  fn(NULL),
  f1H(NULL),
  f10HeMass(0.),
  fIs10HeUserMassSet(false),
  fIs10HeExcitationSet(false),
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
  fUnstableIon10He = new FairIon("10He",  2, 10, 2);
  // fIon1H        = new FairIon("1H", 1, 1, 1);
  run->AddNewIon(fUnstableIon10He);
  // run->AddNewIon(fIon1H);

  fLv1H = new TLorentzVector();
  fLv10He = new TLorentzVector();
}

// // //-------------------------------------------------------------------------------------------------
ERDecay10Heto8He::~ERDecay10Heto8He() {
  if (fDecayFile.is_open())
    fDecayFile.close();
  if (fDecayFilePath == ""){ // LV from TGenPhaseSpace will be deleted in TGenPhaseSpace
    if (fLv8Hed){
      delete fLv8Hed;
      delete fLvn1;
      delete fLvn2;
      // delete fLvn3;
      // delete fLvn4;
    }
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecay10Heto8He::SetHe10Exitation(Double_t excMean, Double_t fwhm, Double_t distibWeight) {
  f10HeExcitationMean.push_back(excMean);
  f10HeExcitationSigma.push_back(fwhm / 2.355);
  if (!fIs10HeExcitationSet) {
    f10HeExcitationWeight.push_back(distibWeight);    
    fIs10HeExcitationSet = true;
    return ;
  }
  f10HeExcitationWeight.push_back(f10HeExcitationWeight.back() + distibWeight);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecay10Heto8He::Init() {

  cout << "Decayer Init." << endl;

  f8He = TDatabasePDG::Instance()->GetParticle("8He");
  if ( ! f8He ) {
    std::cerr  << "-W- ERDecay10Heto8He: Ion 8He not found in database!" << endl;
    return kFALSE;
  }

  f3H = TDatabasePDG::Instance()->GetParticle("Triton");
  if ( ! f3H ) {
    std::cerr  << "-W- ERDecay10Heto8He: Ion Triton not found in database!" << endl;
    return kFALSE;
  }
  

  fn = TDatabasePDG::Instance()->GetParticle("neutron");
  if ( ! fn ) {
    std::cerr  << "-W- ERDecay10Heto8He: Particle neutron not found in database!" << endl;
    return kFALSE;
  }
  
  f1H = TDatabasePDG::Instance()->GetParticle("proton");
  if ( ! f1H ) {
    std::cerr  << "-W- ERDecay10Heto8He: Particle proton not found in database!" << endl;
    return kFALSE;
  }
  
  if (fIs10HeUserMassSet) {
    fUnstableIon10He->SetMass(f10HeMass / .931494028);
  } else {
    f10HeMass = f10He->Mass(); // if user mass is not defined in ERDecay10Heto8He::SetHe10Mass() than get a GEANT mass
  }
  CalculateTargetParameters();


  printf("\n++++++++++++\nList of particles\n++++++++++++\n");
  f8He->Print();
  printf("mass 8He %f\n",f8He->Mass());
  printf("mass 10He %f\n",f10HeMass);
  printf("mass p %f\n",f1H->Mass());
  printf("mass n %f\n",fn->Mass());
  f3H->Print();
  f1H->Print();
  fn->Print();
 

  if (fDecayFilePath != ""){
    LOG(INFO) << "Use decay kinematics from external text file" << FairLogger::endl;
    fDecayFile.open(fDecayFilePath.Data());
    if (!fDecayFile.is_open())
      LOG(FATAL) << "Can`t open decay file " << fDecayFilePath << FairLogger::endl;
    //Пропускаем шапку файла
    std::string header;
    std::getline(fDecayFile,header);

    fLv8Hed = new TLorentzVector();
    fLvn1 = new TLorentzVector();
    fLvn2 = new TLorentzVector();
    // fLvn3 = new TLorentzVector();
    // fLvn4 = new TLorentzVector();
  }
  
  
    // f6He = TDatabasePDG::Instance()->GetParticle("6He");
  // if ( ! f6He ) {
    // std::cerr  << "-W- ERDecay10Heto8He: Ion 6He not found in database!" << endl;
    // return kFALSE;
  // }
 
  // printf("8He PDG = %i\n",f8He->PdgCode());
    // printf("6He PDG = %i\n",f6He->PdgCode());
  // printf("triton PDG = %i\n",f3H->PdgCode());
  // printf("neutron PDG = %i\n",fn->PdgCode());
  // printf("proton PDG = %i\n",f1H->PdgCode());
 //  printf("10He PDG = %i\n",fUnstableIon10He->PdgCode());
  
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecay10Heto8He::Stepping() {
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
	  // 8He + 3H -> 1H + 10He
      TLorentzVector lv8Heb;
      gMC->TrackMomentum(lv8Heb);
      
      if (lv8Heb.P() == 0) { // temporary fix of bug with zero kinetic energy
        return kTRUE;
      }

      TLorentzVector lv3H(0., 0., 0., f3H->Mass());
      TLorentzVector lvReaction;
      lvReaction = lv8Heb + lv3H;

      const TVector3 boost = lvReaction.BoostVector(); //Get Pcm 3 vector
      Double_t ECM = 0;
      TLorentzVector lv8HebCM, lv3HCM;
      lv8HebCM = lv8Heb;
      lv3HCM = lv3H;
      lv8HebCM.Boost(-boost);
      lv3HCM.Boost(-boost);
      ECM = lv8HebCM(3) + lv3HCM(3);

      Int_t reactionHappen = kFALSE;
      
      Double_t decay10HeMass;
      Int_t reactionAttempsCounter = 0;
      Double_t excitation = 0;  // excitation energy
      while (reactionHappen==kFALSE) { // while reaction condition is not fullfilled   
        decay10HeMass = f10HeMass;
        if (fIs10HeExcitationSet) {
          Double_t randWeight = gRandom->Uniform(0., f10HeExcitationWeight.back());
          Int_t distribNum = 0;
          // choose distribution by weight
          for (; distribNum < f10HeExcitationWeight.size(); distribNum++) {
            if (randWeight < f10HeExcitationWeight[distribNum]) {
              break;
            }
          }
          excitation = gRandom->Gaus(f10HeExcitationMean[distribNum], f10HeExcitationSigma[distribNum]);
          fUnstableIon10He->SetExcEnergy(excitation);
        }
        decay10HeMass += excitation;
        if((ECM - f1H->Mass() - decay10HeMass) > 0) { // выход из цикла while для PhaseGenerator
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

      ReactionPhaseGenerator(ECM, decay10HeMass); 
      fLv10He->Boost(boost);
      fLv1H->Boost(boost);

      //7H → f3H + n +n +n +n
	  //10He -> 8He + n + n
      if (!DecayPhaseGenerator(excitation)){
        fDecayFinish = kTRUE;
        return kTRUE;
      }

      Int_t He8bTrackNb, He10TrackNb,H1TrackNb, He8dTrackNb, n1TrackNb, n2TrackNb;

      He8bTrackNb = gMC->GetStack()->GetCurrentTrackNumber();
      // std::cout << "He8TrackNb " << He8TrackNb << std::endl;
      /*																					//???????????
      gMC->GetStack()->PushTrack(1, He8TrackNb, f7H->PdgCode(),						
                                 fLv7H->Px(), fLv7H->Py(), fLv7H->Pz(),
                                 fLv7H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H7TrackNb, decay7HMass, 0);*/

      gMC->GetStack()->PushTrack(1, He8bTrackNb, f1H->PdgCode(),
                                 fLv1H->Px(), fLv1H->Py(), fLv1H->Pz(),
                                 fLv1H->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, H1TrackNb, f1H->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8bTrackNb, f8He->PdgCode(),
                                 fLv8Hed->Px(), fLv8Hed->Py(), fLv8Hed->Pz(),
                                 fLv8Hed->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, He8dTrackNb, f8He->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8bTrackNb, fn->PdgCode(),
                                 fLvn1->Px(),fLvn1->Py(),fLvn1->Pz(),
                                 fLvn1->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n1TrackNb, fn->Mass(), 0);
      gMC->GetStack()->PushTrack(1, He8bTrackNb, fn->PdgCode(),
                                 fLvn2->Px(),fLvn2->Py(),fLvn2->Pz(),
                                 fLvn2->E(), curPos.X(), curPos.Y(), curPos.Z(),
                                 gMC->TrackTime(), 0., 0., 0.,
                                 kPDecay, n2TrackNb, fn->Mass(), 0);
      gMC->StopTrack();
      fDecayFinish = kTRUE;
      gMC->SetMaxStep(100.);

      FairRunSim* run = FairRunSim::Instance();
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
        ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
        header->SetDecayPos(curPos.Vect());
        header->SetInputIon(He8bTrackNb);
        header->AddOutputParticle(He10TrackNb);
        header->AddOutputParticle(H1TrackNb);
        header->AddOutputParticle(He8dTrackNb);
        header->AddOutputParticle(n1TrackNb);
        header->AddOutputParticle(n2TrackNb);
      }   
      if (TString(run->GetMCEventHeader()->ClassName()).Contains("ER10Heto8HeEventHeader")){   
        ER10Heto8HeEventHeader* header = (ER10Heto8HeEventHeader*)run->GetMCEventHeader();
        header->SetData(curPos.Vect(), lv8Heb,  lv3H, *fLv1H, *fLv8Hed,  *fLv10He, *fLvn1, *fLvn2, gMC->TrackTime() * 1e9);
        header->SetTrigger(1);
      }
    }
  }
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
void ERDecay10Heto8He::BeginEvent() { 
  fDecayFinish = kFALSE;
  fIsInterationPointFound = kFALSE;
  fTargetReactZ = fRnd->Uniform(-fTargetThickness / 2, fTargetThickness / 2);
  FairRunSim* run = FairRunSim::Instance();
}

//-------------------------------------------------------------------------------------------------
void ERDecay10Heto8He::FinishEvent() {
  FairRunSim* run = FairRunSim::Instance();
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ERDecayMCEventHeader")){   
    ERDecayMCEventHeader* header = (ERDecayMCEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
  if (TString(run->GetMCEventHeader()->ClassName()).Contains("ER10Heto8HeEventHeader")){   
    ER10Heto8HeEventHeader* header = (ER10Heto8HeEventHeader*)run->GetMCEventHeader();
    header->Clear();
  }
}

//-------------------------------------------------------------------------------------------------
void ERDecay10Heto8He::ReactionPhaseGenerator(Double_t Ecm, Double_t he10Mass) {
  Double_t m1 = he10Mass;
  Double_t m2 = f1H->Mass();

  // Energy of 1-st particle in cm.
  // total energy of the first particle is calculated as
  Double_t E1 = 0.5 * (Ecm * Ecm + m1 * m1 - m2 * m2) / Ecm;

  //Impulse in CM
  Double_t Pcm = TMath::Sqrt(E1 * E1 - m1 * m1);
  //Generate angles of particles in CM
  Double_t thetaCM;
  if(!fADInput) { // if file with angular distribution isn't setted than isotropic distribution is generated
//    thetaCM = TMath::ACos(gRandom->Uniform(-1, 1));
//    thetaCM = TMath::ACos(gRandom->Uniform(-1, -0.965925826));	

    thetaCM = TMath::ACos(gRandom->Uniform(0.984807753,1));		//вырезаю задние углы для попадания в протонный детектор
  } else { 
    thetaCM = fADFunction->GetRandom(fThetaMin, fThetaMax)*TMath::DegToRad();
  }
  Double_t phi = gRandom->Uniform(0., 2. * TMath::Pi());
  TVector3 Pcmv;
  Pcmv.SetMagThetaPhi(Pcm, thetaCM, phi);

  fLv10He->SetXYZM(0., 0., 0., 0.);
  fLv1H->SetXYZM(0., 0., 0., 0.);
  fLv10He->SetXYZM(Pcmv(0), Pcmv(1), Pcmv(2), m1);
  fLv1H->SetXYZM(-Pcmv(0), -Pcmv(1), -Pcmv(2), m2);
}

//-------------------------------------------------------------------------------------------------
Bool_t ERDecay10Heto8He::DecayPhaseGenerator(const Double_t excitation) {
  if (fDecayFilePath == ""){ // if decay file not defined, per morm decay using phase space
    // Double_t decayMasses[5];
    Double_t decayMasses[3];
    decayMasses[0] = f8He->Mass();
    decayMasses[1] = fn->Mass(); 
    decayMasses[2] = fn->Mass();

	// printf("8He = %.10f\t n = %.10f\n",decayMasses[0],decayMasses[1]);
	// printf("+++ fLv10He %f\t%f\n",fLv10He->E()-fLv10He->Mag(),fLv10He->Mag()-decayMasses[0]-decayMasses[1]-decayMasses[2]);	
    fDecayPhaseSpace->SetDecay(*fLv10He, 3, decayMasses);
    fDecayPhaseSpace->Generate();
    fLv8Hed = fDecayPhaseSpace->GetDecay(0);
    fLvn1 = fDecayPhaseSpace->GetDecay(1);
    fLvn2 = fDecayPhaseSpace->GetDecay(2);
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
  // if (outputs_components.size() < 5*3){
  if (outputs_components.size() < 3*3){
    LOG(ERROR) << "Wrong components number in raw in decay file!" << FairLogger::endl;
    return kFALSE;
  }
  // Fill momentum vectors in CM.
  TVector3 p8Hed(std::stod(outputs_components[0]),std::stod(outputs_components[1]),
               std::stod(outputs_components[2]));
  TVector3 pn1(std::stod(outputs_components[3]),std::stod(outputs_components[4]),
               std::stod(outputs_components[5]));
  TVector3 pn2(std::stod(outputs_components[6]),std::stod(outputs_components[7]),
               std::stod(outputs_components[8]));
			   
 // TVector3 pn1(std::stod(outputs_components[0]),std::stod(outputs_components[1]),
               // std::stod(outputs_components[2]));
  // TVector3 pn2(std::stod(outputs_components[3]),std::stod(outputs_components[4]),
               // std::stod(outputs_components[5]));
  // TVector3 p8Hed(std::stod(outputs_components[12]),std::stod(outputs_components[13]),
               // std::stod(outputs_components[14]));
 
  // Apply scale factor
  const auto excitationScale = excitation > 0. ? sqrt(excitation / fDecayFileExcitation) : 1.;
  // printf("\n\n++++++ %f ++++++\n",excitationScale);
  const auto MeV2GeV = 1./1000.;
  const auto scale = excitationScale * MeV2GeV;
  pn1 *= scale;
  pn2 *= scale;
  p8Hed *= scale;
  const auto fill_output_lorentz_vectors_in_lab = 
      [this](TLorentzVector* lv, const TVector3& p, const Double_t mass) {
        lv->SetXYZM(p.X(), p.Y(), p.Z(), mass);
        lv->Boost(fLv10He->BoostVector());
      };
  fill_output_lorentz_vectors_in_lab(fLvn1, pn1, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLvn2, pn2, fn->Mass());
  fill_output_lorentz_vectors_in_lab(fLv8Hed, p8Hed, f8He->Mass());
  return kTRUE;
}

//-------------------------------------------------------------------------------------------------
Double_t ERDecay10Heto8He::ADEvaluate(Double_t *x, Double_t *p) {
  if (fADInput->IsZombie()) {
    Error("ERDecay10Heto8He::ADEvaluate", "AD input was not loaded");
    return -1;
  }
  // on each step of creating distribution function returns interpolated value of input data
  return fADInput->Eval(x[0]);
}

//-------------------------------------------------------------------------------------------------
void ERDecay10Heto8He::SetAngularDistribution(TString ADFile) {
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
    LOG(INFO) << "ERDecay10Heto8He::SetAngularDistribution: "
              << "Too few inputs for creation of AD function!" << FairLogger::endl;
    return;
  }
  Double_t* angle = fADInput->GetX();  // get first column variables that contains number of point 

  // Creation of angular distribution function using class member function.
  // Constructor divides interval (0; fADInput->GetN()-1) into grid.
  // On each step of grid it calls ADEvaluate() to get interpolated values of input data.
  fThetaMin = angle[0];
  fThetaMax = angle[fADInput->GetN()-1];
  fADFunction = new TF1("angDistr", this, &ERDecay10Heto8He::ADEvaluate, 
                         fThetaMin, fThetaMax, 0, "ERDecay10Heto8He", "ADEvaluate");
  fADFunction->Eval(1.);
}
//-------------------------------------------------------------------------------------------------
// void ERDecay10Heto8He::PrintMass(){
	// printf("\n++++++++++++\nList of particles\n++++++++++++\n");
	// f8He->Print();
	// f10He->Print();
	// f3H->Print();
	// f1H->Print();
	// fn->Print();
	// // return;
// }
//-------------------------------------------------------------------------------------------------
ClassImp(ERDecay10Heto8He)
