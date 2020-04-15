/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopePID_H
#define ERQTelescopePID_H

#include <limits>
#include <map>
#include <set>

#include "TClonesArray.h"
#include "TString.h"
#include "TH1.h"
#include "TCut.h"
#include "TVector3.h"
#include "TCutG.h"

#include "ERTask.h"
#include "ERQTelescopeTrack.h"
#include "ERQTelescopeParticle.h"
#include "ERQTelescopeSetup.h"

class G4ParticleDefinition;

/** @class ERQTelescopePID
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERQTelescopePID : public ERTask {
  using PDG = Int_t;
public:
  struct ParticleCut {
    TString fDeStation;
    TString fEStation;
    Double_t fCutNormalizeThickness = -1.;
    std::map<Int_t, Double_t>* fDeMin = nullptr;
    std::map<Int_t, Double_t>* fDeMax = nullptr;
    std::map<Int_t, Double_t>* fEMin = nullptr;
    std::map<Int_t, Double_t>* fEMax = nullptr;
    std::map<Int_t, TCutG>* fDeECut = nullptr;
    ParticleCut() = default;
    ParticleCut(const TString& deStation, const TString& eStation, Double_t normalizeThickness, 
                std::map<Int_t, Double_t>* deMin, std::map<Int_t, Double_t>* deMax,
                    std::map<Int_t, Double_t>* eMin, std::map<Int_t, Double_t>* eMax,
                    std::map<Int_t, TCutG>* deECut) 
        : fCutNormalizeThickness(normalizeThickness), fDeMin(deMin), fDeMax(deMax),
          fEMin(eMin), fEMax(eMax), fDeECut(deECut) {}
    };
  /** @brief Default constructor **/
  ERQTelescopePID();
  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERQTelescopePID(Int_t verbose);
  /** @brief Destructor **/
  ~ERQTelescopePID() = default;
  /* Modifiers */
  void SetTrackForParticle(const TString& trackBranchName, const PDG pdg) {
    fParticleTracks[trackBranchName].insert(pdg);
  }
  void SetTrackAndCutForParticle(
      const TString& trackBranch,
      const PDG pdg,
      const TString& deStation,
      const TString& eStation,
      const Double_t normalizeThickness,
      std::map<Int_t, Double_t>* deMin = nullptr, 
      std::map<Int_t, Double_t>* deMax = nullptr,
      std::map<Int_t, Double_t>* eMin = nullptr, 
      std::map<Int_t, Double_t>* eMax = nullptr,
      std::map<Int_t, TCutG>* deECut = nullptr) {
    fParticleTracks[trackBranch].insert(pdg); 
    fParticleCuts[{trackBranch, pdg}] = ParticleCut(
        deStation, eStation, normalizeThickness, 
        deMin, deMax, eMin, eMax, deECut);
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
  ERQTelescopeSetup* fQTelescopeSetup = nullptr; ///< access to ERQTelescopeSetup class instance
  std::map<TString, std::set<PDG>> fParticleTracks;
  std::map<std::pair<TString, PDG>, ParticleCut> fParticleCuts;
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  std::map<TString, TClonesArray*> fQTelescopeTrack;
  //Output arrays
  std::map<TString, std::map<PDG, TClonesArray*> >  fQTelescopeParticle;
protected:
  TVector3 FindBackPropagationStartPoint(const ERQTelescopeTrack& track);
  std::pair<Double_t, Double_t> CalcEnergyDeposites (
      const ERQTelescopeTrack& track, const TVector3& startPoint,
      const G4ParticleDefinition& particle, std::map<TString, Double_t>& digiBrNameToEnergyDeposite);
  std::pair<TString, Double_t> FindDigiEdepByNode(const TGeoNode& node);
private:
  /** @brief Adds a ERQTelescopeParticles to the output Collection **/
  ERQTelescopeParticle* AddParticle(const TLorentzVector& lvInteraction, Double_t kineticEnergy,
                                    Double_t deadEloss, TClonesArray& col);
  ClassDef(ERQTelescopePID,1)
};
#endif
