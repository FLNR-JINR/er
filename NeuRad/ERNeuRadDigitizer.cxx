/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNeuRadDigitizer.h"

#include <vector>
#include <algorithm>

#include "TClonesArray.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "EREventHeader.h"
#include "ERNeuRadPoint.h"
#include "ERNeuRadPhotoElectron.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

//TODO write description for each of these numbers! Woule be very useful.
const Double_t ERNeuRadDigitizer::cSciFiLightYield= 2000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2; // [cm/ns]
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58; // [cm/ns]
// доля света захватываемая файбером в полное внутреннее отражение в каждую сторону
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;
const Double_t ERNeuRadDigitizer::cPixelDelay=6.; //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPixelJitter = 0.4/2.36; // [ns] (H8500)
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; // [ns]

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fPixelJitter(cPixelJitter),
  fPixelDelay(cPixelDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0),
  fPixelSignalCreatingTime(0),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPixelSignal(NULL),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),
  fPECountF(0),
  fPECountB(0),
  fUseCrosstalks(kTRUE)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fPixelJitter(cPixelJitter),
  fPixelDelay(cPixelDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0),
  fPixelSignalCreatingTime(0),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPixelSignal(NULL),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),
  fPECountF(0),
  fPECountB(0),
  fUseCrosstalks(kTRUE)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::~ERNeuRadDigitizer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::SetParContainers()
{
  fNeuRadSetup = ERNeuRadSetup::Instance();
  fNeuRadSetup->SetParContainers();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERNeuRadDigitizer::Init()
{
  LOG(INFO) << "ERNeuRadDigitizer::Init()" << FairLogger::endl;

  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");

  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  if ( ! fNeuRadPoints) Fatal("ERNeuRadDigitizer::Init","No NeuRadPoint Collection");

  // Register output array NeuRadPhotoElectron and NeuRadDigi
  fNeuRadPhotoElectron = new TClonesArray("ERNeuRadPhotoElectron", 1000);
  ioman->Register("NeuRadPhotoElectron", "NeuRad photoelectrons", fNeuRadPhotoElectron, kTRUE);
  fNeuRadPixelSignal = new TClonesArray("ERNeuRadPixelSignal", 1000);
  ioman->Register("NeuRadPixelSignal", "Signal Pixel", fNeuRadPixelSignal, kTRUE);

  fNeuRadSetup->Print();
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  fPECountF = 0;
  fPECountB = 0;
  fSumAmplitudeF = 0;
  fSumAmplitudeB = 0;

  //TODO is this the correct way to get the current event number?
  Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
  LOG(INFO) << "ERNeuRadDigitizer::Exec: " << "Event " << iEvent << FairLogger::endl;

  // Reset entries in output arrays
  Reset();
  Int_t nofPixels = fNeuRadSetup->NofPixels();
  Int_t nofModules = fNeuRadSetup->NofModules();

  LOG(DEBUG) << "ERNeuRadDigitizer::Exec: "  << "nofPixels = " << nofPixels << FairLogger::endl;
  LOG(DEBUG) << "ERNeuRadDigitizer::Exec: "  << "nofModules = " << nofModules << FairLogger::endl;

  //выделяем память под хранение фотоэлектронов по файберам
  std::vector<ERNeuRadPhotoElectron* >** peInPixelsF = new std::vector<ERNeuRadPhotoElectron*>* [nofModules];
  std::vector<ERNeuRadPhotoElectron* >** peInPixelsB = new std::vector<ERNeuRadPhotoElectron*>* [nofModules];
  for (Int_t i = 0; i<nofModules; i++){
    peInPixelsF[i] = new std::vector<ERNeuRadPhotoElectron*> [nofPixels];
    peInPixelsB[i] = new std::vector<ERNeuRadPhotoElectron*> [nofPixels];
  }

  Int_t points_count = fNeuRadPoints->GetEntries();

  LOG(DEBUG) << "ERNeuRadDigitizer::Exec: "  << "points_count = " << points_count << FairLogger::endl;

  // Формируем промежуточные сущности PhotoElectrons
  fPhotoElectronsCreatingTimer.Start();
  for (UInt_t iPoint=0; iPoint<points_count; iPoint++) { // цикл (серьёзно?!)
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    PhotoElectronsCreating(iPoint, point, peInPixelsF, 0, fPECountF, fSumAmplitudeF);
    PhotoElectronsCreating(iPoint, point, peInPixelsB, 1, fPECountB, fSumAmplitudeB);
  }
  fPhotoElectronsCreatingTimer.Stop();

  fPhotoElectronsCreatingTime += fPhotoElectronsCreatingTimer.RealTime();

  LOG(DEBUG) << "-------------------------------------------------------------" << FairLogger::endl;

  // Формируем сигналы на ФЭУ и digi
  fPixelSignalCreatingTimer.Start();

  for (UInt_t iModule=0; iModule<nofModules; iModule++) {
    // Генерируем сигналы на пикселях
    for (UInt_t iPixel=0; iPixel<nofPixels; iPixel++) {
      PixelSignalsCreating(iModule, iPixel, peInPixelsF, 0);
      PixelSignalsCreating(iModule, iPixel, peInPixelsB, 1);
    }
  }

  fPixelSignalCreatingTimer.Stop();
  fPixelSignalCreatingTime += fPixelSignalCreatingTimer.RealTime();

  // Освобождаем память
  for (UInt_t i = 0; i<nofModules; i++) {
    delete [] peInPixelsF[i];
    delete [] peInPixelsB[i];
  }
  delete [] peInPixelsF;
  delete [] peInPixelsB;

  FairRunAna* run = FairRunAna::Instance();
  EREventHeader* header = (EREventHeader*)run->GetEventHeader();
  header->SetNeuRadPECountF(fPECountF);
  header->SetNeuRadPECountB(fPECountB);
  header->SetNeuRadSumAmplitudeF(fSumAmplitudeF);
  header->SetNeuRadSumAmplitudeB(fSumAmplitudeB);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::PhotoElectronsCreating(Int_t iPoint,
                                               ERNeuRadPoint *point,
                                               std::vector< ERNeuRadPhotoElectron* >** peInPixels,
                                               Int_t side,
                                               Int_t& sumPECount,
                                               Float_t& sumAmplitude)
{
  LOG(DEBUG) << "ERNeuRadDigitizer::PhotoElectronsCreating" << FairLogger::endl;

    // Генерация фотоэлектронов для поинта iPoint

    // Получение информации о поинте
    Double_t fiberLength = fNeuRadSetup->FiberLength();

    //TODO !!!!!
    Int_t    pointModule = point->GetPmtId();
    Int_t    pointPixel  = point->GetChId();

    Double_t pointELoss  = point->GetEnergyLoss(); // [GeV]
    Double_t pointLYield = point->GetLightYield(); // [GeV]
    Double_t pointZ      = point->GetZInLocal();
    Double_t pointTime   = point->GetTimeIn();
    //Double_t pointZInFiber = pointZ + fiberLength/2. - fNeuRadSetup->Z();
    Double_t pointZInFiber = pointZ + fiberLength/2.;

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;
    //TODO !!!!!
    LOG(DEBUG) << "pointModule = " << pointModule << FairLogger::endl;
    LOG(DEBUG) << "pointPixel = " << pointPixel << FairLogger::endl;

    // Значение квантовой эффективности для конкретного пикселе
    //TODO !!!!!
    Double_t PixelQuantumEfficiency = fNeuRadSetup->PixelQuantumEfficiency(pointModule, pointPixel);

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    // Количество фотонов
    Double_t photonCount = pointLYield*1000.*cSciFiLightYield;

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    // Две причины затухания света
    Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
    Double_t k2 = cLightFractionInTotalIntReflection;
    Double_t flightLength = 0.;

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    if (side == 0)
      flightLength = pointZInFiber;
    else
      flightLength = fiberLength-pointZInFiber;

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    Double_t pePhotonCount = photonCount*(k1*exp(-flightLength/0.5) + k2*exp(-flightLength/200.));

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    // Квантовая эффективность
    Int_t peCount = gRandom->Poisson(pePhotonCount*PixelQuantumEfficiency);
    sumPECount += peCount;

    LOG(DEBUG) << "+++++++++++" << FairLogger::endl;

    for(Int_t iPE=0; iPE<peCount; iPE++){
      // Экпоненциальный закон высвечивания. Обратное распределение
      Double_t peLYTime = pointTime + (-1.)*fScincilationTau*TMath::Log(1-gRandom->Uniform());
      // Скорость света в материале
      Double_t peCathodeTime = peLYTime + flightLength/cMaterialSpeedOfLight;
      // Учёт кросстолков
      //TODO !!!!!
      Int_t pePixel = pointPixel;
      Int_t peModule = pointModule;
      if (fUseCrosstalks) {
        //TODO !!!!!
        Crosstalks(pointModule, pointPixel, peModule, pePixel);
      }
      // Амплитуда одноэлектронного сигнала
      Double_t PixelGain = fNeuRadSetup->PixelGain(peModule, pePixel);
      Double_t PixelSigma = fNeuRadSetup->PixelSigma(peModule, pePixel);
      Double_t peAmplitude = TMath::Abs(gRandom->Gaus(PixelGain, PixelSigma));
      sumAmplitude += peAmplitude;
      // Задержка динодной системы и джиттер
      Double_t peAnodeTime = peCathodeTime + (Double_t)gRandom->Gaus(fPixelDelay, fPixelJitter);
      ERNeuRadPhotoElectron* pe = AddPhotoElectron(iPoint, side, peLYTime - pointTime, peCathodeTime, peAnodeTime, pePhotonCount, peAmplitude);
      peInPixels[peModule][pePixel].push_back(pe);
    }
}

//----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::Crosstalks(Int_t pointModule, Int_t pointPixel, Int_t& peModule, Int_t& pePixel) {
  // Возвращает номер файбера в котором окажется фотоэлектрон после применения кростолков
  // Пока для простоты ввода данных масштабирует, значение кростолков для одного модуля на все остальные

  LOG(DEBUG) << "ERNeuRadDigitizer::Crosstalks" << FairLogger::endl;

  pePixel = pointPixel;
  peModule = pointModule;
  TArrayF crosstalks;
  fNeuRadSetup->Crosstalks(pePixel, crosstalks);
  Float_t prob = gRandom->Uniform();
  Float_t curProb = 0;
  Int_t csI = -1;
  Int_t csJ = -1;

  // Разбиваем отрезок от 0 до 1 на отрезки соответствующие вероятностям кросс-толков.
  // В какой именно промежуток вероятности попадёт prob, в тот файбер и перетечет фотоэлектрон.
  // Последний отрезок соответствует тому что фотоэлектрон останется в своём волокне.
  for (Int_t i = 0; i < 3; i++){
    for (Int_t j = 0; j < 3; j++){
      if (crosstalks[i*3+j] == 0 || (i==1 && j==1))
        continue;

      curProb += crosstalks[i*3+j];

      if (curProb > prob) {
        csI = i;
        csJ = j;
        break;
      }
    }
    if (csI != -1)
      break;
  }

  // Переход между строками волокон в модуле
  if (csI == 0) {
    pePixel -= fNeuRadSetup->RowNofPixels();
  }
  if (csI == 2) {
    pePixel +=fNeuRadSetup->RowNofPixels();
  }

  // Переход между столбцами волокон в модуле
  if (csJ == 0) {
    pePixel -= 1;
  }
  if (csJ == 2) {
    pePixel += 1;
  }
}

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset() {
  if (fNeuRadPhotoElectron) {
    fNeuRadPhotoElectron->Delete(); //TODO точно?
  }
  if (fNeuRadPixelSignal) {
    fNeuRadPixelSignal->Delete(); //TODO точно?
  }
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::PixelSignalsCreating(Int_t iModule, Int_t iPixel,
                                std::vector<ERNeuRadPhotoElectron* >** peInPixels, Int_t side)
{
  LOG(DEBUG) << "ERNeuRadDigitizer::PixelSignalsCreating" << FairLogger::endl;

  if (peInPixels[iModule][iPixel].size() > 0) {
    ERNeuRadPixelSignal* PixelSignal = AddPixelSignal(iModule, iPixel,peInPixels[iModule][iPixel].size(),side);
    for (Int_t iPE = 0; iPE < peInPixels[iModule][iPixel].size(); iPE++) {
      ERNeuRadPhotoElectron* pe = peInPixels[iModule][iPixel][iPE];
      PixelSignal->AddPhotoElectron(pe);
    }
    PixelSignal->Generate();
  }
}
// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish() {
  LOG(INFO) << "========== Finish of ERNeuRadDigitizer =================="<< FairLogger::endl;
  LOG(INFO) << "===== Time on PhotoElectrons creating : " << fPhotoElectronsCreatingTime << " s" << FairLogger::endl;
  LOG(INFO) << "===== Time on Pixel signal creating : " << fPixelSignalCreatingTime << " s" << FairLogger::endl;
  LOG(INFO) << "=========================================================" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadPixelSignal* ERNeuRadDigitizer::AddPixelSignal(Int_t iModule,
                                                       Int_t iPixel,
                                                       Int_t fpoints_count,
                                                       Int_t side)
{
  LOG(DEBUG) << "ERNeuRadDigitizer::AddPixelSignal" << FairLogger::endl;
  ERNeuRadPixelSignal *PixelSignal = new((*fNeuRadPixelSignal)[PixelSignalCount()])
                ERNeuRadPixelSignal(iModule, iPixel,fpoints_count, side);
  return PixelSignal;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadPhotoElectron* ERNeuRadDigitizer::AddPhotoElectron(Int_t iPoint,
  Int_t side,
  Double_t lytime,
  Double_t cathode_time,
  Double_t anode_time,
  Int_t photonCount,
  Double_t amplitude)
{
  LOG(DEBUG) << "ERNeuRadDigitizer::AddPhotoElectron" << FairLogger::endl;
  ERNeuRadPhotoElectron *fp = new ((*fNeuRadPhotoElectron)[PhotoElectronCount()])
    ERNeuRadPhotoElectron(PhotoElectronCount(),side, lytime, cathode_time, anode_time, photonCount, amplitude);
  fp->AddLink(FairLink("NeuRadPoint", iPoint));
  return fp;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::PhotoElectronCount() const {
  return fNeuRadPhotoElectron->GetEntriesFast();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::PixelSignalCount() const {
  return fNeuRadPixelSignal->GetEntriesFast();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
