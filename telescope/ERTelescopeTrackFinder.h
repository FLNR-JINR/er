/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeTrackFinder_H
#define ERTelescopeTrackFinder_H

#include <utility>
#include <limits>
#include "TClonesArray.h"

#include "ERTask.h"
#include "ERTelescopeTrack.h"
#include "ERTelescopeSetup.h"

/** @class ERTelescopeTrackFinder
 ** @brief 
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
**/

class ERTelescopeTrackFinder : public ERTask {
public:
  /** @brief Default constructor **/
  ERTelescopeTrackFinder();
  /** @brief Constructor 
   ** @param verbosity level **/
  ERTelescopeTrackFinder(Int_t verbose);
  /** @brief Destructor **/
  ~ERTelescopeTrackFinder() = default;
  /* Modifiers */
  void SetHitStation(TString subassemblyName, TString componentIdX, TString componentIdY); 
  void SetHitStation(TString subassemblyName, TString componentId);
  void SetTrackPositionCorrection(const TString& station_name, ERChannel channel,
                                  float strip_fraction);
  void SetStripEdepRange(Double_t edepMin, Double_t edepMax);
  void SetEdepMaxDiffXY(Double_t edepDiff) {fEdepDiffXY = edepDiff;}
  void SetInteractionPosition(double x, double y, double z);
public:
  /** @brief Defines all input and output object colletions participates
   ** in track finding. **/
  virtual InitStatus Init();
  /** @brief Defines the transformation actions for all input data (Digi) to 
   ** output data (Track) for each event. **/
  virtual void Exec(Option_t* opt);
  /** @brief Resets all output data. **/
  virtual void Reset();
protected:
  //Paramaters
  ERTelescopeSetup* fQTelescopeSetup = nullptr;      ///< access to ERTelescopeSetup class instance
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  TClonesArray* fBeamDetTrack = nullptr;
  //Output arrays
  std::map<TString, TClonesArray*> fQTelescopeTrack;
  //// map<subassembly,map<component, pair<xBranch, yBranch>>> 
  std::map<TString, std::map<TString, std::pair<TString, TString>>> fSiHitStationsPair;
  std::map<TString, std::map<ERChannel, float>> track_position_corrections_; 
  Double_t fSiDigiEdepMin = std::numeric_limits<double>::min();
  Double_t fSiDigiEdepMax = std::numeric_limits<double>::max();
  Double_t fEdepDiffXY = std::numeric_limits<double>::max();
  double interaction_x_ = 0.;
  double interaction_y_ = 0.;
  double interaction_z_ = 0.;
  bool interaction_position_is_set_ = false;
private:
  /** @brief Adds a ERTelescopeTrack to the output Collection **/
  ERTelescopeTrack* AddTrack(const TVector3& targetVertex, const TVector3& xStationVertex, const TVector3& yStationVertex,
                              const TVector3& xStationLocalVertex, const TVector3& yStationLocalVertex, 
                              Int_t xChannel, Int_t yChannel, Double_t xEdep, Double_t yEdep,
                              const TString& digiBranchName);
  void CreateTrackInQTelescope(Int_t xChannelIndex, Int_t yChannelIndex, Int_t xChannel, Int_t yChannel, Double_t xEdep, Double_t yEdep,
                               const TString& xBranchName, const TString& yBranchName, const TString& trackBranchName);
  void CreateTrackInRTelescope(Int_t xChannelIndex, Int_t yChannelIndex, Int_t phiChannel, Int_t RChannel, Double_t phiEdep, Double_t rEdep,
                               const TString& phiBranchName, const TString& rBranchName, const TString& trackBranchName);
  TVector3 GetGlobalTrackPositionByStrip(const TString& branch_name, ERChannel channel) const;
  ClassDef(ERTelescopeTrackFinder, 1)
};
#endif
