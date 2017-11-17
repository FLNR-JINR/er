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
    fNofPixels(-1),
    fNofModules(-1),
    fUseCrosstalks(kFALSE),
    fRowNofFibers(-1),
    fPixelQuantumEfficiency(NULL),
    fPixelGain(NULL),
    fPixelSigma(NULL),
    fPixelCrosstalks(NULL),
    fFiberCrosstalks(NULL)
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
  resetInputVersions();
}

// -----   Public method print ----------------------------------------------------------
void ERNeuRadDigiPar::print()
{
  fRowNofFibers = (Int_t)TMath::Sqrt(fNofPixels);

  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "          ERNeuRadDigiPar                " << FairLogger::endl;
  LOG(INFO) << "*****************************************" << FairLogger::endl;
  LOG(INFO) << "   ERNeuRadNofModules: " << fNofModules << FairLogger::endl;
  LOG(INFO) << "   ERNeuRadNofPixels: " << fNofPixels << FairLogger::endl;
  LOG(INFO) << "   ERNeuRadPixelQuantumEfficiency: " << FairLogger::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++) {
    LOG(INFO) << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      LOG(INFO) <<(*fPixelQuantumEfficiency)[iFiber*fRowNofFibers + jFiber] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;

  LOG(INFO) << "   ERNeuRadPixelGain: " << FairLogger::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++) {
    LOG(INFO) << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      LOG(INFO) <<(*fPixelGain)[iFiber*fRowNofFibers + jFiber] << "\t";
     LOG(INFO) << FairLogger::endl;
  }
  LOG(INFO) << "*****************************************" << FairLogger::endl;

  LOG(INFO) << "   ERNeuRadPixelSigma: " << FairLogger::endl;
  for (Int_t iFiber = 0; iFiber < fRowNofFibers; iFiber++) {
    LOG(INFO) << "     ";
    for (Int_t jFiber = 0; jFiber < fRowNofFibers; jFiber++)
      LOG(INFO) <<(*fPixelSigma)[iFiber*fRowNofFibers + jFiber] << "\t";
     LOG(INFO) << FairLogger::endl;
  }

  LOG(INFO) << "*****************************************" << FairLogger::endl;
}

//---------------------------------------------------------------------------------------
void ERNeuRadDigiPar::putParams(FairParamList* l)
{
}

//---------------------------------------------------------------------------------------
Bool_t ERNeuRadDigiPar::getParams(FairParamList* l)
{
  LOG(INFO) << "ERNeuRadDigiPar Filling ..." << FairLogger::endl;

  ERNeuRadSetup* setup = ERNeuRadSetup::Instance();
  fNofPixels = setup->RowNofPixels()*setup->RowNofPixels();
  fNofModules = setup->RowNofModules()*setup->RowNofModules();

  LOG(INFO) << "fNofPixels " << fNofPixels << FairLogger::endl;

  fPixelQuantumEfficiency = new TArrayF(fNofPixels);
  fPixelGain = new TArrayF(fNofPixels);
  fPixelSigma = new TArrayF(fNofPixels);
  fPixelCrosstalks = new TArrayF(fNofPixels*9);
  fFiberCrosstalks = new TArrayF(fNofPixels*9);

  if (!l) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelQuantumEfficiency", fPixelQuantumEfficiency) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelGain", fPixelGain) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelSigma", fPixelSigma) ) { return kFALSE; }
  if ( ! l->fill("ERNeuRadPixelCrosstalks", fPixelCrosstalks) ) {
    LOG(WARNING) << "ERNeuRadDigiPar: can`t find ERNeuRadPixelCrosstalks" << FairLogger::endl;
  } else {
    fUseCrosstalks = kTRUE;
  }
  if ( ! l->fill("ERNeuRadFiberCrosstalks", fFiberCrosstalks) ) {
    LOG(WARNING) << "ERNeuRadDigiPar: can`t find ERNeuRadFiberCrosstalks" << FairLogger::endl;
  } else {
    fUseCrosstalks = kTRUE;
  }

  fPixelQuantumEfficiency->Set(fNofPixels);
  fPixelGain->Set(fNofPixels);
  fPixelSigma->Set(fNofPixels);
  fPixelCrosstalks->Set(fNofPixels*9);
  fFiberCrosstalks->Set(fNofPixels*9);
  LOG(INFO) << "fPixelQuantumEfficiency " << fPixelQuantumEfficiency->GetSize() << FairLogger::endl;
  return kTRUE;
}

//---------------------------------------------------------------------------------------
Bool_t ERNeuRadDigiPar::init(FairParIo* input) {
  LOG(INFO) << input->getFilename() << FairLogger::endl;
  if ( TString(input->getFilename()).Contains(".digi")) {
    FairGenericParAsciiFileIo* p = new FairGenericParAsciiFileIo(((FairParAsciiFileIo*)input)->getFile());
    return p->init(this);
  }
  if ( TString(input->getFilename()).Contains(".root")) {
    FairGenericParRootFileIo* p = new FairGenericParRootFileIo(((FairParRootFileIo*)input)->getParRootFile());
    return p->init(this);
  }
  return kFALSE;
}

//---------------------------------------------------------------------------------------
void ERNeuRadDigiPar::Crosstalks(Int_t iPixel, TArrayF& crosstalks) const {
  // Возвращает матрицу три на три. Каждый элемент матрицы - кросс-толк к соответствующему соседу.
  // Центральная ячейка - вероятность фотонов, которые останутся в волокне.
  // Вне зависимости от того, что написано в файле параметров,
  // потом пересчитывается, чтобы суммарная вероятнсть была равна 1.
  crosstalks.Set(9);
  Int_t shift = iPixel*9;
  for (Int_t i = 0; i < 9; i++) {
    crosstalks[i] = (*fPixelCrosstalks)[shift + i]+(*fFiberCrosstalks)[shift + i];
  }
}

ClassImp(ERNeuRadDigiPar)
