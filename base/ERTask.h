/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTask_H
#define ERTask_H

#include <vector>

#include "TString.h"

#include "FairTask.h"
#include "FairRun.h"

/** @class ERTask
 ** @brief Base abstract class for all tasks in er. 
 ** @author V.Schetinin <schetinin@jinr.ru> //base
 ** @version 1.0
 **
 ** Common functionality for all er tasks is realized here.
**/

class ERTask : public FairTask {

public:

  /** Default constructor 
    ** @param Task description
  **/
  ERTask(TString name);

  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERTask(TString name, Int_t verbose);

  /** Destructor **/
  ~ERTask();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

protected:

  /** @brief Check is instance of run manager availible for this task**/
  InitStatus CheckRunManager();

  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();

protected:

  FairRun* fRun;                              ///< Pointer to run manager object
  std::vector<TString> fAvailibleRunManagers; ///< Run managers that are availible for this task

private:
  
  ClassDef(ERTask,1)
};

#endif // ERTask_H
