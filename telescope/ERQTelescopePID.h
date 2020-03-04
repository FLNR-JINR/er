/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopePID_H
#define ERQTelescopePID_H

#include "TClonesArray.h"
#include "TString.h"
#include "TH1.h"
#include "TCut.h"
#include "TVector3.h"

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
  /** @brief Default constructor **/
  ERQTelescopePID();
  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERQTelescopePID(Int_t verbose);
  /** @brief Destructor **/
  ~ERQTelescopePID() = default;
  /* Modifiers */
  void SetStationParticle(const TString& station,
                          const PDG pdg) {
    fStationParticles[station].push_back(pdg);
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
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  std::map<TString, TClonesArray*> fQTelescopeTrack;
  //Output arrays
  std::map<TString, std::map<PDG, TClonesArray*> >  fQTelescopeParticle;
  std::map<TString, std::vector<PDG>> fStationParticles;
protected:
  TVector3 FindBackPropagationStartPoint(const ERQTelescopeTrack& track);
  std::pair<Double_t, Double_t> CalcEnergyDeposites (
      const ERQTelescopeTrack& track, const TVector3& startPoint,
      const G4ParticleDefinition& particle);
  Double_t FindDigiEdepByNode(const TGeoNode& node);
  Double_t FindCsIEdepByTrack(ERQTelescopeTrack* track, PDG pdg);
private:
  /** @brief Adds a ERQTelescopeParticles to the output Collection **/
  ERQTelescopeParticle* AddParticle(const TLorentzVector& lvInteraction, Double_t kineticEnergy,
                                    Double_t deadEloss, TClonesArray& col);
  ClassDef(ERQTelescopePID,1)
};
#endif
