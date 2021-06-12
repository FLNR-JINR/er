/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTelescopePID.h"

#include <numeric>

#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmCalculator.hh"
#include "G4NistManager.hh"

#include "TVector3.h"
#include "TMath.h"
#include "TGeoNode.h"
#include "TGeoManager.h"

#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "FairEventHeader.h"

#include "ERBeamDetTrack.h"
#include "ERRunAna.h"
#include "ERSupport.h"

//--------------------------------------------------------------------------------------------------
ERTelescopePID::ERTelescopePID()
  : ERTask("ER qtelescope particle identification scheme") {
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
ERTelescopePID::ERTelescopePID(Int_t verbose)
  : ERTask("ER qtelescope particle identification scheme", verbose) {
  fAvailibleRunManagers.push_back("ERRunAna");
}
//--------------------------------------------------------------------------------------------------
InitStatus ERTelescopePID::Init() {
  if (ERTask::Init() != kSUCCESS)
    return kFATAL;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) 
    Fatal("Init", "No FairRootManager");
  TList* allbrNames = ioman->GetBranchNameList();
  TIter nextBranch(allbrNames);
  TObjString* bName;
  while (bName = (TObjString*)nextBranch()) {
    TString bFullName = bName->GetString();
    if (bFullName.Contains("Digi") && bFullName.Contains("Telescope")) {
      fQTelescopeDigi[bFullName] = (TClonesArray*) ioman->GetObject(bFullName);
      LOG(DEBUG) << "Digi branch " << bFullName << FairLogger::endl; 
    }
    if (bFullName.Contains("Track") && bFullName.Contains("Telescope")) {
      Int_t bPrefixNameLength = bFullName.First('_'); 
      TString brName(bFullName(bPrefixNameLength + 1, bFullName.Length()));
      if (fParticleDescriptions.find(brName) == fParticleDescriptions.end())
        continue;
      fQTelescopeTrack[brName] = (TClonesArray*) ioman->GetObject(bFullName);
      //Creating particles collections
      for (auto particleDescription : fParticleDescriptions[brName]){
        TString brParticleName;
        const auto pdg = particleDescription.fPDG;
        brParticleName.Form("%s_%d",brName.Data(), pdg);
        fQTelescopeParticle[brName][pdg] = new TClonesArray("ERTelescopeParticle",
                                                            consts::approx_telescope_particle_number);
        ioman->Register("TelescopeParticle_" + brParticleName, "Telescope", 
                        fQTelescopeParticle[brName][pdg], kTRUE);
      }
    }
  }
  fQTelescopeSetup = ERTelescopeSetup::Instance();
  fQTelescopeSetup->ReadGeoParamsFromParContainer();
  return kSUCCESS;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopePID::SetParticle(
    const TString& trackBranchName, const PDG pdg, 
    const TString& deStation /*= ""*/, const TString& eStation /*= ""*/,
    const Double_t deNormalizedThickness /*= 0.002*/,
    const std::vector<TString>& stations_to_use_em_calculator_for_de_e /* = {}*/,
    const std::vector<TString>& stations_to_use_em_calculator_for_kinetic_energy/*= {}*/) {
  if ((deStation == "" && eStation != "") || (deStation != "" && eStation == ""))
    LOG(FATAL) << "If one of dE or E station is defined, another should be defined." 
               << FairLogger::endl;
  std::list<TString> eStations;
  if (eStation != "" )
    eStations.push_back(eStation);
  fParticleDescriptions[trackBranchName].emplace_back(pdg, deStation, eStations,
                                                      deNormalizedThickness,
                                                      stations_to_use_em_calculator_for_de_e,
                                                      stations_to_use_em_calculator_for_kinetic_energy);
}
//--------------------------------------------------------------------------------------------------
void ERTelescopePID::SetParticle(
    const TString& trackBranchName, const PDG pdg, 
    const TString& deStation /*= ""*/, const std::list<TString>& eStations /*= {}*/,
    const Double_t deNormalizedThickness /*= 0.002*/,
    const std::vector<TString>& stations_to_use_em_calculator_for_de_e /* = {}*/,
    const std::vector<TString>& stations_to_use_em_calculator_for_kinetic_energy/*= {}*/) {
  fParticleDescriptions[trackBranchName].emplace_back(pdg, deStation, eStations,
                                                      deNormalizedThickness,
                                                      stations_to_use_em_calculator_for_de_e,
                                                      stations_to_use_em_calculator_for_kinetic_energy);
}
//--------------------------------------------------------------------------------------------------
void ERTelescopePID::Exec(Option_t* opt) {
  LOG(DEBUG) << "[ERTelescopePID]------------Started-----------------------------------------------"
             << FairLogger::endl;
  Reset();
  for (const auto& itTrackBranches : fQTelescopeTrack) {
    const auto& trackBranch = itTrackBranches.first;
    const auto& tracks = itTrackBranches.second;
    if (tracks->GetEntriesFast() == 0)
      continue;
    LOG(DEBUG) << "[ERTelescopePID] Work with traсks in " << trackBranch << FairLogger::endl;
    for (Int_t iTrack(0); iTrack < tracks->GetEntriesFast(); iTrack++) { 
      const auto* track = static_cast<ERTelescopeTrack*>(tracks->At(iTrack));
      for (const auto particleDescription : fParticleDescriptions[trackBranch]) {
        // Get particle mass by PDG from G4IonTable
        const auto pdg = particleDescription.fPDG;
        const auto* particle = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
        if (!particle)
          LOG(FATAL) << "Particle with code " << pdg << " not found in Geant database "<< FairLogger::endl;
        const auto mass = particle->GetPDGMass(); // MeV
        // Find geometry point to start track back propagation.
        const auto back_propagation_start_point = FindBackPropagationStartPoint(*track);
        // Calc particle energy deposites in setup using back propagation from start point 
        // to middle of target volume. We assume that kinetic energy 
        // is fully detected by the setup, so calculate it as sum of 
        // energy deposites in digi and energy deposites in passive volumes (using G4EmCalculator).
        std::list<DigiOnTrack> digisOnTrack;
        const auto energyDeposites = CalcEnergyDeposites(
            *track, back_propagation_start_point, *particle, digisOnTrack, 
            particleDescription.stations_to_use_em_calculator_for_de_e_,
            particleDescription.stations_to_use_em_calculator_for_kinetic_energy_);
        Double_t edepInThickStation = -1., edepInThinStation = -1., 
                 edepInThickStationCorrected = -1., edepInThinStationCorrected = -1.;
        ERChannel channelOfThinStation = consts::undefined_channel;
        ERChannel channelOfThickStation = consts::undefined_channel;
        FindEnergiesForDeEAnalysis(trackBranch, digisOnTrack, 
                                   particleDescription.fEStations, particleDescription.fDeStation,
                                   particleDescription.fNormalizedThickness, edepInThickStation, edepInThinStation,
                                   edepInThickStationCorrected, edepInThinStationCorrected,
                                   channelOfThinStation, channelOfThickStation);
        const auto digisDeposite = energyDeposites.first;
        const auto deadDeposite = energyDeposites.second;
        const auto kinetic_energy = digisDeposite + deadDeposite;
        const Double_t fullEnergy = mass + kinetic_energy;
        // Calculate Lorentz vector of particle at the exit of the reaction in lab system.
        const Double_t momentumMag = sqrt(pow(fullEnergy, 2) - pow(mass, 2) );
        TVector3 direction = track->GetDirection();
        const auto momentum = momentumMag * direction;
        const TLorentzVector lvTarget (momentum, fullEnergy);
        // Add particle to collection.
        auto& particleCollection = *fQTelescopeParticle[trackBranch][pdg];
        AddParticle(lvTarget, kinetic_energy, deadDeposite, edepInThickStation, edepInThinStation,
                    edepInThickStationCorrected, edepInThinStationCorrected,
                    channelOfThinStation, channelOfThickStation, particleCollection);
      }
    }
  }
  LOG(DEBUG) << "[ERTelescopePID]------------Finished----------------------------------------------"
             << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopePID::Reset() {
  for (const auto itTrackBranches : fQTelescopeParticle) {
    for (const auto itParticleBranches : itTrackBranches.second)
      if (itParticleBranches.second) {
        itParticleBranches.second->Delete();
      }
  }
}
//--------------------------------------------------------------------------------------------------
ERTelescopeParticle* ERTelescopePID::
AddParticle(const TLorentzVector& lvInteraction, const Double_t kinetic_energy, const Double_t deadEloss,
            Double_t edepInThickStation, Double_t edepInThinStation,
            Double_t edepInThickStationCorrected, Double_t edepInThinStationCorrected,
            const ERChannel channelOfThinStation, const ERChannel channelOfThickStation,
            TClonesArray& col) {
  return new(col[col.GetEntriesFast()]) ERTelescopeParticle(lvInteraction, kinetic_energy, deadEloss,
                                            edepInThickStation, edepInThinStation, 
                                            edepInThickStationCorrected, edepInThinStationCorrected,
                                            channelOfThinStation, channelOfThickStation);
}
//--------------------------------------------------------------------------------------------------
TVector3 ERTelescopePID::FindBackPropagationStartPoint(const ERTelescopeTrack& track) {
  // Return geometry point at which the track exit last sensetive volume
  // on its direct propagation. If sensetive volume was not found, return 
  // track telescope vertex.
  const TVector3 telescopeVertex = track.GetXStationVertex();
  TVector3 back_propagation_start_point = telescopeVertex;
  const auto direction = track.GetDirection();
  TGeoNode* currentNode = gGeoManager->InitTrack(
                              telescopeVertex.X(), telescopeVertex.Y(), telescopeVertex.Z(),
                              direction.X(), direction.Y(), direction.Z());
  TGeoNode* lastSensetiveNode = nullptr;
  TString lastSensetivePath;
  TVector3 lastSensetivePosition;
  while(!gGeoManager->IsOutside()) {
    bool inSensetiveVolume = false;
    const TString path = gGeoManager->GetPath();
    if (path.Contains("Sensitive")) { 
      // Enter sensetive volume. Next step will be in sensetive volume.
      inSensetiveVolume = true;
      lastSensetiveNode = currentNode;
      lastSensetivePath = path; 
    }
    currentNode = gGeoManager->FindNextBoundary();
    currentNode = gGeoManager->Step();
    if (inSensetiveVolume) {
      // position, when track exit sensetve volume
      lastSensetivePosition = TVector3(gGeoManager->GetCurrentPoint());
    }
  }
  if (lastSensetiveNode) {
    back_propagation_start_point = lastSensetivePosition;
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track "
               << lastSensetivePath << FairLogger::endl;
  } else {
    LOG(DEBUG) << "[FindBackPropagationStartPoint] Last sensetive volume for track not found. "
               << "Track telescope vertex will be used as start point for back propagation\n";
  }
  LOG(DEBUG) << "[FindBackPropagationStartPoint] Back propagation start point " 
             << "(" << back_propagation_start_point.X() << "," << back_propagation_start_point.Y()
             << "," << back_propagation_start_point.Z() << ")" << FairLogger::endl;
  return back_propagation_start_point;
}
//--------------------------------------------------------------------------------------------------
std::pair<Double_t, Double_t> ERTelescopePID::
CalcEnergyDeposites(const ERTelescopeTrack& track, const TVector3& start_point, 
                    const G4ParticleDefinition& particle,
                    std::list<DigiOnTrack>& digisOnTrack,
                    const std::vector<TString>& stations_to_use_em_calculator_for_de_e,
                    const std::vector<TString>& stations_to_use_em_calculator_for_kinetic_energy) {
  // Calc paritcle energy deposites in setup using back track propagation from start point.
  // Return pair: first - sum of energy deposites in digi(sensetive volumes); 
  //              second - sum of energy deposites in passive volumes (or dead energy deposite).
  Double_t digiDepositesSum = 0.;
  Double_t deadDepositesSum = 0.;
  // We start with kinetic energy equal zero, because we assume
  // that start point is an exit point of the last sensetive volume,
  // which track has passed in setup. If the track stopped earlier, 
  // it will be taken into account automatically, because particle 
  // with zero kinetic energy can not loss energy ;)
  Double_t kinetic_energy = 0.;
  // Init track in back direction.
  auto back_direction = track.GetBackDirection();
  const TString log_prefix = "[ERTelescopePID] [CalcEnergyDeposites] ";
  LOG(DEBUG) << log_prefix << "Energy deposites calculation for particle " 
             << particle.GetParticleName() << "; start point = (" << start_point.X() << ","
             << start_point.Y() << "," << start_point.Z() << "; and direction = " 
             << back_direction.X() << "," << back_direction.Y() << "," << back_direction.Z()
             << FairLogger::endl;
  const auto calc_edep_using_em_calulator = 
  [&log_prefix, &particle](TGeoNode* node, float step, float T) {
    const TString materialName = node->GetMedium()->GetMaterial()->GetName();
    const auto* material = G4NistManager::Instance()->FindOrBuildMaterial(materialName.Data());
    LOG(DEBUG) << log_prefix <<" Calc energy deposite for range " << step << " in material "
                << materialName << " with kinetic energy " << T << FairLogger::endl;
    return CalcElossIntegralVolStep(T, particle, *material, step);
  };
  TGeoNode* node = gGeoManager->InitTrack(start_point.X(), start_point.Y(), start_point.Z(),
                                          back_direction.X(), back_direction.Y(), back_direction.Z());
  // While track not in interaction volume or outside the setup,
  // accumulate energy deposites and kinetic energy.
  while(!gGeoManager->IsOutside()) {
    gGeoManager->FindNextBoundary();
    const TVector3 current_position(gGeoManager->GetCurrentPoint());
    LOG(DEBUG) << log_prefix << "track position (" << current_position.X() << ", " 
               << current_position.Y() << ", " << current_position.Z() << ")" << FairLogger::endl;
    LOG(DEBUG) << log_prefix << "path  = " 
               << gGeoManager->GetPath() << FairLogger::endl;
    // If track in sensetive volume, try to find digi
    const TString nodePath = TString(gGeoManager->GetPath());
    bool is_last_step = false;
    if (nodePath.Contains("Sensitive")){
      const Bool_t use_em_calcaulator_for_de_e = 
        std::find_if(stations_to_use_em_calculator_for_de_e.begin(),
                     stations_to_use_em_calculator_for_de_e.end(), 
                     [&nodePath](const TString& station) {return nodePath.Contains(station);})
        != stations_to_use_em_calculator_for_de_e.end();
      const Bool_t use_em_calcaulator_for_kinetic_energy = 
        std::find_if(stations_to_use_em_calculator_for_kinetic_energy.begin(),
                     stations_to_use_em_calculator_for_kinetic_energy.end(), 
                     [&nodePath](const TString& station) {return nodePath.Contains(station);})
        != stations_to_use_em_calculator_for_kinetic_energy.end();
      LOG(DEBUG) << log_prefix <<" Finding digis..." << FairLogger::endl;
      auto branchAndDigis = FindDigisByNode(*node, gGeoManager->GetPath());
      Bool_t digis_for_node_are_found = !branchAndDigis.empty();
      float edep_using_em_calculator = -1.;
      if (!digis_for_node_are_found 
          || use_em_calcaulator_for_de_e 
          || use_em_calcaulator_for_kinetic_energy) {
        edep_using_em_calculator = calc_edep_using_em_calulator(node, gGeoManager->GetStep(),
                                                                kinetic_energy);
      }
      for (auto& branchAndDigi : branchAndDigis) {
        const auto digiBranch = branchAndDigi.first;
        auto* digi = branchAndDigi.second;
        if (use_em_calcaulator_for_de_e)
          digi->SetEdep(edep_using_em_calculator);
        digisOnTrack.emplace_back(digiBranch, digi, gGeoManager->GetStep());
      }
      const Double_t edepInStation = digis_for_node_are_found && !use_em_calcaulator_for_kinetic_energy
                                     ? ApplyEdepAccountingStrategy(branchAndDigis)
                                     : edep_using_em_calculator;
      kinetic_energy += edepInStation;
      digiDepositesSum += edepInStation;
    } else {
      // track in passive volume
      auto step = gGeoManager->GetStep();
      const auto target_vertex = track.GetTargetVertex();
      const double step_to_target_vertex = (target_vertex - current_position).Mag();
      is_last_step = step_to_target_vertex <= step;
      step = is_last_step ? step_to_target_vertex : step;
      const auto deadDeposite = calc_edep_using_em_calulator(node, step, kinetic_energy);
      kinetic_energy += deadDeposite;
      deadDepositesSum += deadDeposite;
    }
    LOG(DEBUG) << log_prefix << "Current kinetic Energy  = " << kinetic_energy << FairLogger::endl;
    if (is_last_step)
      break;
    node = gGeoManager->Step();
  }
  LOG(DEBUG) << log_prefix << "Finish deposite calculation with Kinetic energy = " 
             << kinetic_energy << " = digis sum deposite  : " << digiDepositesSum
             << " + dead deposites sum : " << deadDepositesSum << FairLogger::endl;
  return {digiDepositesSum, deadDepositesSum};
}
//--------------------------------------------------------------------------------------------------
std::map<TString, ERDigi*> ERTelescopePID::FindDigisByNode(const TGeoNode& node, const TString& nodePath) {
  // example SSD: /cave_1/QTelescopeTmp_0/Telescope_4_5/Telescope_4_SingleSi_SSD_V_4_X_11/SensitiveSingleSiStrip_X_2
  // example DSD: /cave_1/QTelescopeTmp_0/CT_3/CT_DoubleSi_DSD_XY_0/doubleSiStrip_XY_0/SensitiveDoubleSiBox_XY_16
  // example CsI: /cave_1/QTelescopeTmp_0/Central_telescope_3/Central_telescope_CsI_0/CsIBoxShell_8/SensitiveCsIBox_1
  // example NonUniform SSD: /cave_1/QTelescopeTmp_0/Telescope_1_1/Telescope_1_SingleSi_SSD20_1_X_0/pseudoSiStrip_4_4/SensitivePixelSiBox_X4_Y1_0
  // example R SSD: /cave_1/QTelescopeTmp_0/Telescope_1_1/Telescope_1_SingleSi_Phi1_X_0/r_station_0/Sensitivestrip_12
  // example R DSD: /cave_1/QTelescopeTmp_0/Telescope_1_1/Telescope_1_DoubleSi_R_XY_0/r_station_0/Segment_3/SensitiveSegment_1
  // example R CsI: /cave_1/QTelescopeTmp_0/Telescope_1_1/Telescope_1_CsI_Radial_0/CrystalShell_7/SensitiveCrystal_0
  std::map<TString, ERDigi*> resultDigis;
  const bool nodeOfDoubleSiStation = nodePath.Contains("DoubleSi");
  //@TODO here Setup->GetComponent(path) interface should be used in future.
  const auto getDigiBranchSubString = [&node, &nodePath, nodeOfDoubleSiStation]() -> TString {
    TString digiBranchSubString = nodePath;
    digiBranchSubString.Remove(digiBranchSubString.Last('/'), digiBranchSubString.Length());
    if (nodeOfDoubleSiStation || nodePath.Contains("CsI") || nodePath.Contains("pseudo"))
      digiBranchSubString.Remove(digiBranchSubString.Last('/'), digiBranchSubString.Length());
    if (digiBranchSubString.EndsWith("r_station_0"))
      digiBranchSubString.Remove(digiBranchSubString.Last('/'), digiBranchSubString.Length());
    digiBranchSubString.Remove(0, digiBranchSubString.Last('/') + 1);
    // remove node id
    digiBranchSubString.Remove(digiBranchSubString.Last('_'), digiBranchSubString.Length());
    return digiBranchSubString; // for ex: Telescope_4_SingleSi_SSD_V_4_X
  };
  const auto digiBranchSubstring = getDigiBranchSubString();
  LOG(DEBUG) <<"   [ERTelescopePID][CalcEnergyDeposites] Digi branch substring " 
             << digiBranchSubstring << FairLogger::endl;
  // Multiple digi collections can correspond to one geometry node.
  // DoubleSi station is example.
  //@TODO here component->GetBranchNames() interface should be used in future.
  std::list<TString> digiBranchNames;
  for (const auto& digiCollectionPair : fQTelescopeDigi) {
    if (digiCollectionPair.first.Contains(digiBranchSubstring))
      digiBranchNames.push_back(digiCollectionPair.first);
  }
  if (digiBranchNames.empty()) {  
    LOG(ERROR) << "   [ERTelescopePID][CalcEnergyDeposites]  Branch with substring " << digiBranchSubstring
               << " not found." << FairLogger::endl;
    return resultDigis;
  }
  //@TODO here component->GetChannelFromSensetiveNode() interface should be used in future.
  const auto getChannels = [&node, &nodePath, nodeOfDoubleSiStation]() -> std::vector<Int_t> {
    TString pathWithChannelPostfix = nodePath;
    if (nodePath.Contains("CsI") || nodePath.Contains("pseudo"))
      pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
    const TString channelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                              pathWithChannelPostfix.Length()));
    if (!nodeOfDoubleSiStation)
      return {channelStr.Atoi()};
    pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
    const TString secondChannelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                                          pathWithChannelPostfix.Length()));
    return {channelStr.Atoi(), secondChannelStr.Atoi()};
  };
  const auto channels = getChannels();
  for (const auto& digiBranchName : digiBranchNames) {
    auto* digis = fQTelescopeDigi[digiBranchName];
    // @BUG Code only for XY doubleSi stations.
    Int_t channel = (nodeOfDoubleSiStation && digiBranchName.EndsWith("_X")) ? channels[1] : channels[0];
    LOG(DEBUG) << "   [ERTelescopePID][CalcEnergyDeposites] Finding digi with channel number " 
               << channel << " in branch " << digiBranchName << FairLogger::endl;
    Bool_t digiFound = false;
    for (Int_t iDigi = 0; iDigi < digis->GetEntriesFast(); iDigi++){
      auto* digi = static_cast<ERDigi*>(digis->At(iDigi));
       LOG(DEBUG) << "   [ERTelescopePID][CalcEnergyDeposites]  Collection has digi with channel " 
                  << digi->Channel() << " and edep " << digi->Edep() << FairLogger::endl;
      if (digi->Channel() != channel)
        continue;
      digiFound = true;
      LOG(DEBUG) << "   [ERTelescopePID][CalcEnergyDeposites] Found digi with edep " 
                << digi->Edep() << FairLogger::endl;
      resultDigis[digiBranchName] = digi;
      break;
    }
    if (!digiFound)
      LOG(ERROR) << "   [ERTelescopePID][CalcEnergyDeposites] Digi with channel number " 
              << channel << " not found in collection " << digiBranchName << FairLogger::endl;
  }
  return resultDigis;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopePID::FindEnergiesForDeEAnalysis(const TString& trackBranch, 
    const std::list<DigiOnTrack>& digisOnTrack, const std::list<TString>& eStations,
    const TString& deStation, const Double_t normalizedThickness, Double_t& edepInThickStation,
    Double_t& edepInThinStation, Double_t& edepInThickStationCorrected,
    Double_t& edepInThinStationCorrected, ERChannel& channelOfThinStation, 
    ERChannel& channelOfThickStation) {
  const TString log_prefix = "[ERTelescopePID][FindEnergiesForDeEAnalysis] ";
  if (eStations.empty() || deStation == "")
    return;
  const auto getDigisOnTrack = [&digisOnTrack](const TString& stationName) -> std::list<DigiOnTrack> {
    std::list<DigiOnTrack> digisOnTrackInStation;
    for (const auto digiOnTrack : digisOnTrack) {
      if (digiOnTrack.fBranch.Contains(stationName)) {
        digisOnTrackInStation.push_back(digiOnTrack);
      }
    }
    return digisOnTrackInStation;
  };
  const auto deDigisOnTrack = getDigisOnTrack(deStation);
  if (deDigisOnTrack.empty()) {
    LOG(WARNING) << log_prefix << "Digi for station " << deStation 
                 << " not found on track from " << trackBranch << " path." << FairLogger::endl;
    return;
  }
  channelOfThinStation = deDigisOnTrack.front().fDigi->Channel();
  TString eStations_string;
  for (const auto& eStation : eStations) {
    const auto eDigisOnTrack = getDigisOnTrack(eStation);
    if (eDigisOnTrack.empty()) {
      LOG(WARNING) << log_prefix << "Digi for station " << eStation 
                  << " not found on track from " << trackBranch << " path" << FairLogger::endl;
      continue;
    }
    if (edepInThickStation == -1.) {
      edepInThickStation = ApplyEdepAccountingStrategy(eDigisOnTrack);
      eStations_string += eStation;
    } else {
      edepInThickStation += ApplyEdepAccountingStrategy(eDigisOnTrack);
      eStations_string += TString(", ") + eStation;
    }
    //TODO: Works only for single e station
    channelOfThickStation = eDigisOnTrack.front().fDigi->Channel();
  }
  if (edepInThickStation == -1.) {
    LOG(WARNING) << log_prefix << "No digis found for E stations in de-E analysis." << FairLogger::endl;
    return;
  }
  edepInThinStation = ApplyEdepAccountingStrategy(deDigisOnTrack);
  const auto deSensetiveThickness = deDigisOnTrack.front().fSensetiveThickness;
  // de and E correction
  edepInThinStationCorrected = edepInThinStation * normalizedThickness / deSensetiveThickness;
  LOG(DEBUG) << log_prefix <<  "Digi from station " << deStation 
            << " with edep = " << edepInThinStation << " [MeV] corrected to edep = " 
            << edepInThinStationCorrected << ". Normalized thickness = " 
            << normalizedThickness << ", step in sensetive volume = " << deSensetiveThickness
            << FairLogger::endl;
  edepInThickStationCorrected = edepInThickStation + edepInThinStation - edepInThinStationCorrected;
  LOG(DEBUG) << log_prefix << "Digis from stations " << eStations_string 
             << " with edep = " << edepInThickStation 
             << " corrected to " << edepInThickStationCorrected << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
Double_t ERTelescopePID::ApplyEdepAccountingStrategy(const std::map<TString, ERDigi*>& digisByBranchName) {
  if (digisByBranchName.empty())
    return 0;
  // @TODO setup->GetComponent(branchName) and component->ComponentName() should be used here
  const auto getStrategyForStation = [this](const TString& branchName) -> EdepAccountingStrategy {
    const auto itStationStrategy = std::find_if(
        fEdepAccountingStrategies.begin(), fEdepAccountingStrategies.end(),
        [&branchName](const std::pair<TString, EdepAccountingStrategy> stationAndStrategy) {
          return branchName.Contains(stationAndStrategy.first);
    });
    if (itStationStrategy != fEdepAccountingStrategies.end())
      return itStationStrategy->second;
    if (branchName.Contains("_X"))
      return EdepFromXChannel;
    else if (branchName.Contains("_Y"))
      return EdepFromYChannel;
    if (branchName.Contains("CsI"))
      return SummarizedEdep;
    LOG(FATAL) << "Unable to select edep accaunting strategy by branch " << branchName << FairLogger::endl;
  };
  const auto strategy = getStrategyForStation(digisByBranchName.begin()->first);
  Double_t summarizedEdep = 0.;
  for (const auto& branchAndDigi : digisByBranchName) {
    const auto branchName = branchAndDigi.first;
    const auto* digi = branchAndDigi.second;
    // @TODO component->GetBranchName(...) should be used here
    if ((strategy == EdepFromXChannel && branchName.Contains("_X"))
        || (strategy == EdepFromYChannel && branchName.Contains("_Y"))) {
      return digi->Edep();
    }
    summarizedEdep += digi->Edep();
  }
  if (strategy == AverageEdep)
    return summarizedEdep / digisByBranchName.size();
  else if (strategy == SummarizedEdep)
    return summarizedEdep;
  LOG(FATAL) << "[ERTelescopePID][ApplyEdepAccountingStrategy] Unable to apply edep accounting strategy." << FairLogger::endl;
  return -1;
}
//--------------------------------------------------------------------------------------------------
Double_t ERTelescopePID::ApplyEdepAccountingStrategy(const std::list<DigiOnTrack>& digisOnTrack) {
  std::map<TString, ERDigi*> digisByBranchName;
  for (auto& digiOnTrack : digisOnTrack) {
    digisByBranchName[digiOnTrack.fBranch] = digiOnTrack.fDigi;
  }
  return ApplyEdepAccountingStrategy(digisByBranchName);
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopePID)
