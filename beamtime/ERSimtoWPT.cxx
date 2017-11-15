/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERSimtoWPT.h"

#include "FairRun.h"

#include<iostream>

using namespace std;
// ----------------------------------------------------------------------------
ERSimtoWPT::ERSimtoWPT()
  : FairTask("Convert Sim event to WPT event")

{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoWPT::~ERSimtoWPT()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERSimtoWPT::Init()
{
/*  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  fPhotoElectrons = (TClonesArray*) ioman->GetObject("NeuRadPhotoElectron");
    if (!fPhotoElectrons)
      Fatal("Init", "Can`t find branch in input file!");

*/
  //Register output objects
/*
  fRawEvents = new ERNeuRadRawEvent*[fNChanels];
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fRawEvents[iChanel] = new ERNeuRadRawEvent(fNPoints);
    TString bName;
    bName.Form("ch%d.",iChanel+1);
    ioman->Register(bName,"Raw", fRawEvents[iChanel], kTRUE);
  }
*/

    //  return kSUCCESS;
}  
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERSimtoWPT::Exec(Option_t* opt)
{

}
//----------------------------------------------------------------------------

