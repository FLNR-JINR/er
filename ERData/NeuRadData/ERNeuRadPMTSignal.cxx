// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file            -----
// -------------------------------------------------------------------------

#include "ERNeuRadPMTSignal.h"

#include "FairLogger.h"
#include <iostream>
#include <algorithm>
using namespace std;

// -----   Default constructor   -------------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------
// -----   Constructor with arguments   ------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(Int_t iBundle, Int_t iFiber)
  :fFiberIndex(iFiber),
  fBundleIndex(iBundle)
{  
}
//-------------------------------------------------------------------------

//-----   Destructor   ----------------------------------------------------
ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){

}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

ClassImp(ERNeuRadPMTSignal);