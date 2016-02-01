#include "ERNeuRadDigiPar.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairGenericParAsciiFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairLogger.h"

#include "TString.h"
#include "TMath.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

// -----   Standard constructor   ------------------------------------------
ERNeuRadDigiPar::ERNeuRadDigiPar(const char* name,
    const char* title,
    const char* context)
  : FairParGenericSet(name, title, context),
    fPMTQuantumEfficiency(new TArrayF(64)),
    fPMTGain(new TArrayF(64)),
    fFiberLength(0.),
    fNofFibers(-1),
    fNofBundles(-1)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERNeuRadDigiPar::~ERNeuRadDigiPar()
{
  clear();

}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void ERNeuRadDigiPar::clear()
{
  status = kFALSE;
  resetInputVersions();
}
// -------------------------------------------------------------------------

// -----   Public method print ---------------------------------------
void ERNeuRadDigiPar::print()
{
  Int_t rowNofFibers = (Int_t)TMath::Sqrt(fNofFibers);
  
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "          ERNeuRadDigiPar                " << FairLogger::endl;
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "   ERNeuRadFiberLength: " <<  fFiberLength <<  FairLogger::endl;
  LOG(INFO) << "   ERNeuRadNofBundles: " <<  fNofBundles <<  FairLogger::endl;
  LOG(INFO) << "   ERNeuRadNofFibers: " <<  fNofFibers <<  FairLogger::endl;
  LOG(INFO) << "   ERNeuRadPMTQuantumEfficiency: " <<  FairLogger::endl;
  for (Int_t iFiber = 0; iFiber < rowNofFibers; iFiber++){
    LOG(INFO) << "     ";
    for (Int_t jFiber = 0; jFiber < rowNofFibers; jFiber++)
      LOG(INFO) <<(*fPMTQuantumEfficiency)[iFiber*rowNofFibers + jFiber] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  
  LOG(INFO) << "   ERNeuRadPMTGain: " <<  FairLogger::endl;
  for (Int_t iFiber = 0; iFiber < rowNofFibers; iFiber++){
    LOG(INFO) << "     ";
    for (Int_t jFiber = 0; jFiber < rowNofFibers; jFiber++)
      LOG(INFO) <<(*fPMTGain)[iFiber*rowNofFibers + jFiber] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  
}
//------------------------------------------------------
void ERNeuRadDigiPar::putParams(FairParamList* l)
{ 
/*
  //   print();
  cout << " I am in ERNeuRadDigiPar::putParams " << endl;
  if (!l) { return; }
  l->add("FairTutorialDet2DigiStations", ftutdetdigiparstation);
  l->add("FairTutorialDet2DigiSectorsPerStation", ftutdetdigiparsector);
  Int_t count_sectors = 0;
  for ( Int_t i=0; i< ftutdetdigiparstation; i++) {
    count_sectors+=ftutdetdigiparsector[i];
  }
  Int_t array_size = (count_sectors * 10 + ftutdetdigiparstation * 3);
  cout << "Array Size: " << array_size << endl;
  /*   Float_t zwischen[array_size];
     for ( Int_t i=0; i< array_size; i++) {
       zwischen[i] = ftutdetdigipar.GetAt(i);
     }
   */
   /*
  ftutdetdigipar->Set(array_size);
  l->add("FairTutorialDet2DigiPar",*ftutdetdigipar);
  */
}

//------------------------------------------------------

Bool_t ERNeuRadDigiPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  
  if ( ! l->fill("ERNeuRadFiberLength", &fFiberLength) ) { return kFALSE; }

  if ( ! l->fill("ERNeuRadNofBundles", &fNofBundles) ) { return kFALSE; }
  
  if ( ! l->fill("ERNeuRadNofFibers", &fNofFibers) ) { return kFALSE; }
  
  fPMTQuantumEfficiency->Set(fNofFibers);
  fPMTGain->Set(fNofFibers);
  if ( ! l->fill("ERNeuRadPMTQuantumEfficiency", fPMTQuantumEfficiency) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPMTGain", fPMTGain) ) { return kFALSE; }
  
  return kTRUE;
}
//------------------------------------------------------
Bool_t ERNeuRadDigiPar::init(FairParIo* input){
  if ( ! TString(input->getFilename()).Contains("NeuRad.digi")){
    return kFALSE;
  }
  FairGenericParAsciiFileIo* p=new FairGenericParAsciiFileIo(((FairParAsciiFileIo*)input)->getFile());
  return p->init(this);
}
//------------------------------------------------------

ClassImp(ERNeuRadDigiPar)
