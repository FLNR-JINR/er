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

#include "FairTask.h"

#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetSetup.h"

/** @class ERBeamDetTrackFinder
 ** @brief Class for reconsruction ion's coordinate and momentum direction on target.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERBeamDetTrackFinder reconsructs ion coordinate and momentum direction 
 ** on target by means of the digitization output data and ERBeamDetSetup class.
 ** It is assumed that we have events with a multiplicity equal to four after the
 ** digitization stage. In this way, we have a signal from only one wire for each 
 ** cordinate in MWPC stations. Through number of wire we get MWPC coordinates 
 ** (\f$X_1, Y_1, X_2, Y_2\f$) by means of global geo manager in ERBeamDetSetup.
 ** Ion track approximated by straight line passing through (\f$X_1, Y_1\f$) and
 ** (\f$X_2, Y_2\f$) points.
**/

class ERBeamDetTrackFinder : public FairTask {

public:
  /** @brief Default constructor **/
  ERBeamDetTrackFinder();

  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERBeamDetTrackFinder(Int_t verbose);

  /** @brief Destructor **/
  ~ERBeamDetTrackFinder();

public:
  /** @brief Defines all input and output object colletions participates
   ** in track finding.
  **/
  virtual InitStatus Init();

  /** @brief Defines the transformation actions for all input data (MWPCDigi) to 
   ** output data (Track) for each event. 
  **/
  virtual void Exec(Option_t* opt);

  /** @brief Resets all output data. **/
  virtual void Reset();

  /** @brief Defines actions in the end of track finding. **/
  virtual void Finish();
  
protected:
  //Paramaters
  ERBeamDetSetup *fBeamDetSetup;      ///< access to ERBeamDetSetup class instance
  
  //Input arrays
  TClonesArray   *fBeamDetMWPCDigiX1; 
  TClonesArray   *fBeamDetMWPCDigiX2;
  TClonesArray   *fBeamDetMWPCDigiY1;
  TClonesArray   *fBeamDetMWPCDigiY2; ///< input collection of MWPC Digi

  //Output arrays
  TClonesArray   *fBeamDetTrack ;     ///< output collection of tracks

  Int_t   fMultipicityMWPCX1;
  Int_t   fMultipicityMWPCY1;
  Int_t   fMultipicityMWPCX2;
  Int_t   fMultipicityMWPCY2;

private:
  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();

  /** @brief Adds a ERBeamDetTrack to the output Collection **/
  ERBeamDetTrack* AddTrack(Double_t xt, Double_t yt, Double_t zt, TVector3 v);

  ClassDef(ERBeamDetTrackFinder,1)
};
#endif
