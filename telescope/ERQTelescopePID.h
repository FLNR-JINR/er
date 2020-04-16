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
#include <list>

#include "TClonesArray.h"
#include "TString.h"
#include "TH1.h"
#include "TCut.h"
#include "TVector3.h"
#include "TCutG.h"

#include "ERTask.h"
#include "ERDigi.h"
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
      std::map<Int_t, TCutG>* deECut = nullptr);
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
      const G4ParticleDefinition& particle, std::list<DigiOnTrack>& digisOnTrack);
  std::pair<TString, const ERDigi*> FindDigiEdepByNode(const TGeoNode& node);
  bool IsInsideParticleCut(const TString& trackBranch, PDG pdg, 
                           const std::list<DigiOnTrack>& digisOnTrack);
private:
  /** @brief Adds a ERQTelescopeParticles to the output Collection **/
  ERQTelescopeParticle* AddParticle(const TLorentzVector& lvInteraction, Double_t kineticEnergy,
                                    Double_t deadEloss, TClonesArray& col);
  ClassDef(ERQTelescopePID,1)
};
#endif
