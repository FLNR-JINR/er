/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERBeamDetTrackFinder_H
#define ERBeamDetTrackFinder_H

#include "TClonesArray.h"
#include "TRandom3.h"

#include "ERTask.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetSetup.h"

/** @class ERBeamDetTrackFinder
 ** @brief Class for reconsruction ion's coordinate and momentum direction on target.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERBeamDetTrackFinder reconsructs the ion coordinate and momentum direction 
 ** on the target by means of the digitization output data and ERBeamDetSetup class.
 ** It is assumed that we have events with a multiplicity more than one after the
 ** digitization stage in each array of wires. In this way, we can calculate an
 ** arithmetic average for each cordinate in MWPC stations where all activated wires 
 ** are neigbours between each other. Through number of wires we get coordinates 
 ** (\f$X_1, Y_1, X_2, Y_2\f$) for each wire by means of global geo manager in ERBeamDetSetup.
 ** After that calculate arithmetic averages (\f$<X_1>, <Y_1>, <X_2>, <Y_2>\f$) .
 ** Ion track approximated by straight line passing through (\f$<X_1>,<Y_1>\f$) and
 ** (\f$<X_2>, <Y_2>\f$) points. **/
class ERBeamDetTrackFinder : public ERTask {
public:
  /** @brief Default constructor **/
  ERBeamDetTrackFinder();
  /** @brief Constructor 
   ** @param verbosity level **/
  ERBeamDetTrackFinder(Int_t verbose);
  /** @brief Destructor **/
  ~ERBeamDetTrackFinder() = default;
  /* Modifiers */
  /** @brief Defines position of interaction - final position of beamdet track.
   ** @param volume_name - name of volume to propagate track for. 
   ** @param depth_ration - ratio of track range in interaction volume to max track range 
   **        in this volume with track direction.**/
  void SetInteractionPosition(const TString& volume_name, float depth_ratio = 0.5) {
    interaction_volume_name_ = volume_name; depth_ratio_ = depth_ratio;
  }
  void SetTargetVolume(const TString& volume_name) { interaction_volume_name_ = volume_name; }
public:
  /** @brief Defines all input and output object colletions participates
   ** in track finding. **/
  virtual InitStatus Init();
  /** @brief Defines the transformation actions for all input data (MWPCDigi) to 
   ** output data (Track) for each event. **/
  virtual void Exec(Option_t* opt);
  /** @brief Resets all output data. **/
  virtual void Reset();
protected:
  //Paramaters
  ERBeamDetSetup* fBeamDetSetup = nullptr; ///< access to ERBeamDetSetup class instance
  //Input arrays
  TClonesArray* fBeamDetMWPCDigiX1 = nullptr; 
  TClonesArray* fBeamDetMWPCDigiX2 = nullptr;
  TClonesArray* fBeamDetMWPCDigiY1 = nullptr;
  TClonesArray* fBeamDetMWPCDigiY2 = nullptr; ///< input collection of MWPC Digi
  //Output arrays
  TClonesArray* fBeamDetTrack = nullptr;     ///< output collection of tracks
  Int_t fMultipicityMWPCX1 = -1;
  Int_t fMultipicityMWPCY1 = -1;
  Int_t fMultipicityMWPCX2 = -1;
  Int_t fMultipicityMWPCY2 = -1;
  TRandom3* fRand = nullptr;
  TString interaction_volume_name_ = "target";
  float depth_ratio_ = 0.5;
private:
  /** @brief Adds a ERBeamDetTrack to the output Collection **/
  ERBeamDetTrack* AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v);
  /** @brief Checks if the collection of digies contatains only neigbour wires.
   ** @param digiArray   collection of digies. **/
  Bool_t   IsCluster(TClonesArray* digiArray);
  /** @brief Calculates an arithmetic average value in array of consequent wires.
   ** @param digiArray   collection of digies
   ** @param coordType   type of coordinate (can take values 'X', 'Y' or 'Z') **/
  Double_t CalcCoordinateAvg(const TString& digi_branch_name, const TClonesArray* digiArray,
                             char coordType);
  ClassDef(ERBeamDetTrackFinder,1)
};
#endif
