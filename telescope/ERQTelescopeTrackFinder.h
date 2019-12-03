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
#include <limits>
#include "TClonesArray.h"

#include "ERTask.h"
#include "ERQTelescopeTrack.h"
#include "ERQTelescopeSetup.h"
#include "ERQTelescopeSiDigi.h"

/** @class ERQTelescopeTrackFinder
 ** @brief 
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
**/

class ERQTelescopeTrackFinder : public ERTask {

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
  void SetHitStation(TString subassemblyName, TString componentIdX, TString componentIdY); 
  void SetHitStation(TString subassemblyName, TString componentId);
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
  ERQTelescopeSetup                   *fQTelSetup;      ///< access to ERQTelescopeSetup class instance
  //Input arrays
  std::map<TString, TClonesArray*>    fQTelDigi;
  TClonesArray                        *fBeamDetTrack;
  //Output arrays
  std::map<TString, TClonesArray*>    fQTelTrack;
  
  std::map<TString, map<TString, pair<TString, TString>>> fSiHitStationsPair; //// map<subassembly,map<component, pair<xBranch, yBranch>>> 


  Double_t fSiDigiEdepMin = std::numeric_limits<double>::min();
  Double_t fSiDigiEdepMax = std::numeric_limits<double>::max();
  Double_t fEdepDiffXY = std::numeric_limits<double>::max();
  Double_t fTargetX = 0.;
  Double_t fTargetY = 0.;
  Double_t fTargetZ = 0.;
  Bool_t   fUserTargetPointIsSet = false;
private:
  /** @brief Adds a ERQTelescopeTrack to the output Collection **/
  ERQTelescopeTrack* AddTrack(Double_t targetX, Double_t targetY, Double_t targetZ,  
                              Double_t globalX, Double_t globalY, Double_t globalZ,
                              Double_t localX,  Double_t localY,  Double_t localZ,
                              Double_t sumEdep,
                              TString digiBrName);

  ClassDef(ERQTelescopeTrackFinder,1)
};
#endif
