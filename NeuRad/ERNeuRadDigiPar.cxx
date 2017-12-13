/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadDigiPar.h"

#include "TString.h"
#include "TMath.h"

#include "FairParamList.h"
//#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairGenericParAsciiFileIo.h"
#include "FairGenericParRootFileIo.h"
#include "FairParAsciiFileIo.h"
#include "FairParRootFileIo.h"
#include "FairLogger.h"

#include "ERNeuRadSetup.h"

// -----   Standard constructor   -------------------------------------------------------
ERNeuRadDigiPar::ERNeuRadDigiPar(const char* name,
                                 const char* title,
                                 const char* context)
  : FairParGenericSet(name, title, context),
    fRowNofPixelsPerPMT(-1),
    fNofPixelsPerPMT(-1),
    fNofPMTs(-1),
    fUseCrosstalks(kFALSE),
    fPixelQuantumEfficiency(NULL),
    fPixelGain(NULL),
    fPixelSigma(NULL),
    fPixelCrosstalks(NULL) 
    //fFiberCrosstalks(NULL) //FIXME Keep it commented for time being
{
  LOG(INFO) << "ERNeuRadDigiPar constructor" << FairLogger::endl;
}

// -----   Destructor   -----------------------------------------------------------------
ERNeuRadDigiPar::~ERNeuRadDigiPar()
{
  clear();
}

// -----   Public method clear   --------------------------------------------------------
void ERNeuRadDigiPar::clear()
{
  status = kFALSE;
  resetInputVersions(); // What is this?
}

// -----   Public method print ----------------------------------------------------------
void ERNeuRadDigiPar::print() 
{
  //TODO Why should we compute it here?
  // This data member should be filled together with the
  // fNofPixelsPerPMT during initialization/import/whatever
  //fRowNofPixelsPerPMT = (Int_t)TMath::Sqrt(fNofPixelsPerPMT);

  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "          ERNeuRadDigiPar                " << FairLogger::endl;
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "   ERNeuRad RowNofPixelsPerPMT: " << fRowNofPixelsPerPMT << FairLogger::endl;
  LOG(INFO) << "   ERNeuRad NofPixelsPerPMT: " << fNofPixelsPerPMT << FairLogger::endl;
  LOG(INFO) << "   ERNeuRad NofPMTs: " << fNofPMTs << FairLogger::endl;
  LOG(INFO) << "   ERNeuRad PixelQuantumEfficiency: " << FairLogger::endl;

  for (Int_t iPixel = 0; iPixel < fRowNofPixelsPerPMT; iPixel++) {
    LOG(INFO) << "     ";
    for (Int_t jPixel = 0; jPixel < fRowNofPixelsPerPMT; jPixel++)
      LOG(INFO) << (*fPixelQuantumEfficiency)[iPixel*fRowNofPixelsPerPMT + jPixel] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;

  LOG(INFO) << "   ERNeuRadPixelGain: " << FairLogger::endl;
  for (Int_t iPixel = 0; iPixel < fRowNofPixelsPerPMT; iPixel++) {
    LOG(INFO) << "     ";
    for (Int_t jPixel = 0; jPixel < fRowNofPixelsPerPMT; jPixel++)
      LOG(INFO) << (*fPixelGain)[iPixel*fRowNofPixelsPerPMT + jPixel] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;

  LOG(INFO) << "   ERNeuRadPixelSigma: " << FairLogger::endl;
  for (Int_t iPixel = 0; iPixel < fRowNofPixelsPerPMT; iPixel++) {
    LOG(INFO) << "     ";
    for (Int_t jPixel = 0; jPixel < fRowNofPixelsPerPMT; jPixel++)
      LOG(INFO) << (*fPixelSigma)[iPixel*fRowNofPixelsPerPMT + jPixel] << "\t";
     LOG(INFO) << FairLogger::endl;
  }

  LOG(INFO) << "*****************************************" << FairLogger::endl;

  LOG(INFO) << "   ERNeuRadPixelCrosstalks: " << FairLogger::endl;
  for (Int_t i=0; i<3*8; i++) {
    for (Int_t j=0; j<3*8; j++) {
      LOG(INFO) << (*fPixelCrosstalks)[i*3*8+j] << "\t";
    }
    LOG(INFO) << FairLogger::endl;
  }

  LOG(INFO) << "*****************************************" << FairLogger::endl;
}

//---------------------------------------------------------------------------------------
void ERNeuRadDigiPar::putParams(FairParamList* l)
{
}

//---------------------------------------------------------------------------------------
//TODO this method has to be thoroughly checked
Bool_t ERNeuRadDigiPar::getParams(FairParamList* l)
{
  LOG(INFO) << "ERNeuRadDigiPar filling ..." << FairLogger::endl;

  // Here we acquire different setup parameters from the corresponding
  // singleton class
  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();

  fRowNofPixelsPerPMT = setup->GetRowNofPixels();
  fNofPixelsPerPMT = fRowNofPixelsPerPMT * fRowNofPixelsPerPMT;
  fNofPMTs = setup->GetRowNofModules() * setup->GetRowNofModules(); //TODO wat?

  //FIXME this probably will not work for multiple PMTs

  LOG(INFO) << "fNofPixelsPerPMT " << fNofPixelsPerPMT << FairLogger::endl;

  fPixelQuantumEfficiency = new TArrayF(fNofPixelsPerPMT);
  fPixelGain = new TArrayF(fNofPixelsPerPMT);
  fPixelSigma = new TArrayF(fNofPixelsPerPMT);
  fPixelCrosstalks = new TArrayF(fNofPixelsPerPMT*9);
  //fFiberCrosstalks = new TArrayF(fNofPixelsPerPMT*9); //FIXME Keep it commented for time being

  if ( ! l ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelQuantumEfficiency", fPixelQuantumEfficiency) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelGain", fPixelGain) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelSigma", fPixelSigma) ) { return kFALSE; }

  if ( ! l->fill("ERNeuRadPixelCrosstalks", fPixelCrosstalks) ) {
    LOG(WARNING) << "ERNeuRadDigiPar: can`t find ERNeuRadPixelCrosstalks" << FairLogger::endl;
    LOG(WARNING) << "Crosstalks will not be taken into account." << FairLogger::endl;
    fUseCrosstalks = kFALSE; // Just to be on the safe side
  } else {
    fUseCrosstalks = kTRUE;
  }

  //FIXME Keep it commented for time being
  /*
  if ( ! l->fill("ERNeuRadFiberCrosstalks", fFiberCrosstalks) ) {
    LOG(WARNING) << "ERNeuRadDigiPar: can`t find ERNeuRadFiberCrosstalks" << FairLogger::endl;
    LOG(WARNING) << "Crosstalks will not be taken into account." << FairLogger::endl;
    fUseCrosstalks = kFALSE; // Just to be on the safe side
  } else {
    fUseCrosstalks = kTRUE;
  }
  */

  // Set the sizes of the arrays
  //TODO why the size is set only AFTER filling the arrays with the data?
  fPixelQuantumEfficiency->Set(fNofPixelsPerPMT);
  fPixelGain->Set(fNofPixelsPerPMT);
  fPixelSigma->Set(fNofPixelsPerPMT);
  fPixelCrosstalks->Set(fNofPixelsPerPMT*9);
  // fFiberCrosstalks->Set(fNofPixelsPerPMT*9);  //FIXME Keep it commented for time being

  // This is probably needed only for debugging reasons?
  //LOG(INFO) << "fPixelQuantumEfficiency size: " << fPixelQuantumEfficiency->GetSize() << FairLogger::endl;

  return kTRUE;
}

//---------------------------------------------------------------------------------------
Bool_t ERNeuRadDigiPar::init(FairParIo* input) {
  LOG(INFO) << "ERNeuRadDigiPar::init from " << input->getFilename() << FairLogger::endl;
  if (TString(input->getFilename()).Contains(".digi")) { //TODO implement more clever filename checks!!!
    FairGenericParAsciiFileIo* p = new FairGenericParAsciiFileIo(((FairParAsciiFileIo*)input)->getFile()); //TODO is this correct?
    return p->init(this);
  }
  if (TString(input->getFilename()).Contains(".root")) { //TODO implement more clever filename checks!!! (here use EndsWith()?)
    FairGenericParRootFileIo* p = new FairGenericParRootFileIo(((FairParRootFileIo*)input)->getParRootFile()); //TODO is this correct? Why are they different?
    return p->init(this);
  }
  return kFALSE;
}

//---------------------------------------------------------------------------------------
void ERNeuRadDigiPar::Crosstalks(Int_t iPMT, Int_t iChannel, TArrayF& crosstalks) const {
  // Возвращает матрицу три на три. Каждый элемент матрицы - кросс-толк к соответствующему соседу.
  // Центральная ячейка - вероятность фотонов, которые останутся в волокне.
  // Вне зависимости от того, что написано в файле параметров,
  // потом пересчитывается, чтобы суммарная вероятность была равна 1.

  // Currently only channel ID is used.

  //TODO
  // 3 - one pixel crosstalk matrix size
  // 8 - is actually fRowNofPixelsPerPMT - number of pixels in one row of the PMT
  // 3*8 - width of the full crosstalk matrix
  // 3*3*8 - step in vertical direction

  Int_t x = iChannel % fRowNofPixelsPerPMT;
  Int_t y = iChannel / fRowNofPixelsPerPMT;

  Int_t startingIndex = x*3 + y*3*3*8;

  LOG(DEBUG) << "iChannel=" << iChannel << "\tx=" << x << "\ty=" << y << "\tstartingIndex=" << startingIndex << FairLogger::endl;

  crosstalks.Set(9);

  crosstalks[0] = (*fPixelCrosstalks)[startingIndex + 0 + 0*3*8];
  crosstalks[1] = (*fPixelCrosstalks)[startingIndex + 1 + 0*3*8];
  crosstalks[2] = (*fPixelCrosstalks)[startingIndex + 2 + 0*3*8];
  crosstalks[3] = (*fPixelCrosstalks)[startingIndex + 0 + 1*3*8];
  crosstalks[4] = (*fPixelCrosstalks)[startingIndex + 1 + 1*3*8];
  crosstalks[5] = (*fPixelCrosstalks)[startingIndex + 2 + 1*3*8];
  crosstalks[6] = (*fPixelCrosstalks)[startingIndex + 0 + 2*3*8]; 
  crosstalks[7] = (*fPixelCrosstalks)[startingIndex + 1 + 2*3*8];
  crosstalks[8] = (*fPixelCrosstalks)[startingIndex + 2 + 2*3*8];

  /*
  Int_t shift = iChannel*9;
  for (Int_t i=0; i<9; i++) {
    crosstalks[i] = (*fPixelCrosstalks)[shift + i]; // + (*fFiberCrosstalks)[shift + i]; //FIXME Keep it commented for time being
  }
  */
}

ClassImp(ERNeuRadDigiPar)
