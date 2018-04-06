/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeTrackFinder_H
#define ERQTelescopeTrackFinder_H

#include <utility>

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERQTelescopeTrack.h"
#include "ERQTelescopeSetup.h"
#include "ERQTelescopeSiDigi.h"

/** @class ERQTelescopeTrackFinder
 ** @brief 
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
**/

class ERQTelescopeTrackFinder : public FairTask {

public:

  /** @brief Default constructor **/
  ERQTelescopeTrackFinder();

  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERQTelescopeTrackFinder(Int_t verbose);

  /** @brief Destructor **/
  ~ERQTelescopeTrackFinder();

  /* Modifiers */
  void SetHitStation(TString stationID);
  void SetHitStation(TString xStationID, TString yStationID2);
  void SetHitSubAssembly(TString subassemblyName);
  void SetStripEdepRange(Double_t edepMin, Double_t edepMax);
  void SetEdepMaxDiffXY(Double_t edepDiff) {fEdepDiffXY = edepDiff;}
  void SetTargetPoint(Double_t x, Double_t y, Double_t z);

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

  /** @brief Defines actions in the end of track finding. **/
  virtual void Finish();
  
protected:
  //Paramaters
  ERQTelescopeSetup                   *fQTelescopeSetup;      ///< access to ERQTelescopeSetup class instance
  //Input arrays
  std::map<TString, TClonesArray*>    fQTelescopeDigi;
  TClonesArray                        *fBeamDetTrack;
  //Output arrays
  std::map<TString, TClonesArray*>          fQTelescopeTrack;
  std::map<TString, pair<TString, TString>> fSiHitStationsPair; 
  std::map<TString, map<TString, pair<TString, TString>>> fSiHitStationsPair1; //// map<subassembly,map<component, pair<xBranch, yBranch>>> 


  Double_t fSiDigiEdepMin;
  Double_t fSiDigiEdepMax;
  Double_t fEdepDiffXY;
  Double_t fTargetX;
  Double_t fTargetY;
  Double_t fTargetZ;
  Bool_t   fUserTargetPointIsSet;

private:
  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();

  /** @brief Adds a ERQTelescopeTrack to the output Collection **/
  ERQTelescopeTrack* AddTrack(Double_t targetX,    Double_t targetY,    Double_t targetZ, 
                              Double_t telescopeX, Double_t telescopeY, Double_t telescopeZ,
                              Double_t sumEdep,
                              TString digiBranchName);

  ClassDef(ERQTelescopeTrackFinder,1)
};
#endif
