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

#include "FairTask.h"

#include "ERQTelescopeTrack.h"
#include "ERQTelescopeParticle.h"
#include "ERQTelescopeSetup.h"
#include "ERQTelescopeSiDigi.h"

/** @class ERQTelescopePID
 ** @brief 
 ** @author V.Schetinin <schetinin@jinr.ru>
 ** @version 1.0
**/

class ERQTelescopePID : public FairTask {

public:

  /** @brief Default constructor **/
  ERQTelescopePID();

  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERQTelescopePID(Int_t verbose);

  /** @brief Destructor **/
  ~ERQTelescopePID();

  /* Modifiers */
  void SetStationParticle(TString station, Int_t pdg) {fStationParticles[station].push_back(pdg);}
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
  std::map<TString, TClonesArray*>    fQTelescopeTrack;
  //Output arrays
  std::map<TString, std::map<Int_t,TClonesArray*> >    fQTelescopeParticle;

  std::map<TString, std::vector<Int_t>> fStationParticles;

protected:

  Double_t CalcEloss(TString station, ERQTelescopeTrack* track, Int_t pdg);

private:
  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();

  /** @brief Adds a ERQTelescopeParticles to the output Collection **/
  ERQTelescopeParticle* AddParticle(Double_t deadEloss, TClonesArray* col);

  ClassDef(ERQTelescopePID,1)
};
#endif
