/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopePID_H
#define ERTelescopePID_H

#include <limits>
#include <map>
#include <list>

#include "TClonesArray.h"
#include "TString.h"
#include "TVector3.h"

#include "ERTask.h"
#include "ERDigi.h"
#include "ERTelescopeTrack.h"
#include "ERTelescopeParticle.h"
#include "ERTelescopeSetup.h"

class G4ParticleDefinition;

/** @class ERTelescopePID
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERTelescopePID : public ERTask {
  using PDG = Int_t;
public:
  enum EdepAccountingStrategy {EdepFromXChannel, EdepFromYChannel, AverageEdep, SummarizedEdep};
  struct ParticleDescription {
    const PDG fPDG = -1;
    const TString fDeStation;
    const TString fEStation;
    const Double_t fNormalizedThickness = 0.002;
    const std::vector<TString> fDoNotUseSignalFromStations;
    ParticleDescription() = default;
    ParticleDescription(PDG pdg, const TString& deStation, 
                        const TString& eStation, Double_t normalizedThickness,
                        const std::vector<TString>& doNotUseSignalFromStations)
        : fPDG(pdg), fDeStation(deStation), fEStation(eStation),
          fNormalizedThickness(normalizedThickness), 
          fDoNotUseSignalFromStations(doNotUseSignalFromStations) {}
  };
  struct DigiOnTrack {
    const TString fBranch;
    const ERDigi* fDigi = nullptr;
    const Double_t fSensetiveThickness = 0.;
    DigiOnTrack() = default;
    DigiOnTrack(const TString& branch, const ERDigi* digi, Double_t sensetiveThickness) :
        fBranch(branch), fDigi(digi), fSensetiveThickness(sensetiveThickness) {}
    bool IsFound() const { return fDigi != nullptr; }
  };
                                                     
  /** @brief Default constructor **/
  ERTelescopePID();
  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERTelescopePID(Int_t verbose);
  /** @brief Destructor **/
  ~ERTelescopePID() = default;
  /* Modifiers */
  void SetParticle(const TString& trackBranchName, const PDG pdg, 
                   const TString& deStation = "", const TString& eStation = "",
                   Double_t deNormalizedThickness = 0.002, const std::vector<TString>& doNotUseSignalFromStations = {});
  void SetEdepAccountingStrategy(
      const TString& station, EdepAccountingStrategy strategy) {
    fEdepAccountingStrategies[station] = strategy;
  }
public:
  /** @brief Defines all input and output object colletions participates
   ** in track finding.
  **/
  virtual InitStatus Init();
  /** @brief Defines the transformation actions for all input data (Digi) to 
   ** output data (Track) for each event. 
  **/
  virtual void Exec(Option_t* opt);
  /** @brief Resets all output data. **/
  virtual void Reset();
protected:
  //Paramaters
  ERTelescopeSetup* fQTelescopeSetup = nullptr; ///< access to ERTelescopeSetup class instance
  std::map<TString, std::list<ParticleDescription>> fParticleDescriptions;
  std::map<TString, EdepAccountingStrategy> fEdepAccountingStrategies;
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  std::map<TString, TClonesArray*> fQTelescopeTrack;
  //Output arrays
  std::map<TString, std::map<PDG, TClonesArray*> >  fQTelescopeParticle;
protected:
  TVector3 FindBackPropagationStartPoint(const ERTelescopeTrack& track);
  std::pair<Double_t, Double_t> CalcEnergyDeposites (
      const ERTelescopeTrack& track, const TVector3& startPoint,
      const G4ParticleDefinition& particle, std::list<DigiOnTrack>& digisOnTrack,
      const std::vector<TString>& doNotUseSignalFromStations);
  std::map<TString, const ERDigi*> FindDigisByNode(const TGeoNode& node, const TString& nodePath);
  void FindEnergiesForDeEAnalysis(const TString& trackBranch,
                                  const std::list<DigiOnTrack>& digisOnTrack, 
                                  const TString& eStation, const TString& deStation, 
                                  const Double_t normalizedThickness,
                                  Double_t& edepInThickStation, Double_t& edepInThinStation,
                                  Double_t& edepInThickStationCorrected, Double_t& edepInThinStationCorrected);
  Double_t ApplyEdepAccountingStrategy(const std::map<TString, const ERDigi*>& digisByBranchName);
  Double_t ApplyEdepAccountingStrategy(const std::list<DigiOnTrack>& digisOnTrack);
  /** @brief Adds a ERTelescopeParticles to the output Collection **/
  ERTelescopeParticle* AddParticle(const TLorentzVector& lvInteraction, Double_t kineticEnergy,
                                    Double_t deadEloss, Double_t edepInThickStation, Double_t edepInThinStation,
                                    Double_t edepInThickStationCorrected, Double_t edepInThinStationCorrected,
                                    TClonesArray& col);
  ClassDef(ERTelescopePID, 1)
};
#endif
