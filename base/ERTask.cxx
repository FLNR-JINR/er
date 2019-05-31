/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTask.h"

#include "FairLogger.h"
#include "FairRuntimeDb.h"

// ----------------------------------------------------------------------------
ERTask::ERTask(TString name)
  : FairTask(name),
  fRun(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTask::ERTask(TString name, Int_t verbose)
  : FairTask(name, verbose),
  fRun(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTask::Init()
{
  fRun = FairRun::Instance();
  
  if (CheckRunManager() != kSUCCESS)
    return kFATAL;

  return kSUCCESS;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERTask::CheckRunManager()
{
  if (fAvailibleRunManagers.size() == 0){
    LOG(WARNING) << "Availible run manager not defined for task " << this->ClassName() << FairLogger::endl;
    LOG(WARNING) << "No check will be performed" << FairLogger::endl;
    return kSUCCESS;
  }

  for (auto availibleRun : fAvailibleRunManagers){
    if (fRun->ClassName() == availibleRun)
      return kSUCCESS;
  }

  return kFATAL;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTask::SetParContainers()
{
  fRun = FairRun::Instance();
  if (!fRun)
    LOG(FATAL) << "No run manager!" << FairLogger::endl;
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  if (!rtdb)
    LOG(FATAL) << "No runtime database!" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTask::Finish()
{

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERTask::Reset()
{

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERTask::~ERTask()
{
}
// ----------------------------------------------------------------------------
ClassImp(ERTask)