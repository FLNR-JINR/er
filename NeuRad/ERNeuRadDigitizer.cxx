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
#include "TF1.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "EREventHeader.h"
#include "ERNeuRadPoint.h"
#include "ERNeuRadPhotoElectron.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

//TODO write description for each of these numbers! Would be very useful.

// Number of photons born for 1 MeV of energy deposit
// (of a charged particle in the scintillator)
const Double_t ERNeuRadDigitizer::cSciFiLightYield = 8000.; // [photons/MeV] //TODO value - ?!

// Speed of light in vacuum
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2; // [cm/ns]

// Speed of light in the scintillator
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58; // [cm/ns]

// доля света захватываемая файбером в полное внутреннее отражение в каждую сторону
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;

//
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; // [ns]

// Characteristics of the (MA) PMT
const Double_t ERNeuRadDigitizer::cPixelDelay = 6.; // [ns] (H8500) //TODO

const Double_t ERNeuRadDigitizer::cPixelJitter = 0.4/2.36; // [ns] (H8500) //TODO

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization"),
  fPixelJitter(cPixelJitter),
  fPixelDelay(cPixelDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0.),
  fPixelSignalCreatingTime(0.),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPixelSignal(NULL),
  /*fPECountF(0),
  fPECountB(0),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),*/
  fUseCrosstalks(kTRUE)
{
  fSPEfunc = new TF1("fSPEfunc", "ERNeuRadDigitizer::SPEfunc", 0., 2000., 7);
  fSPEfunc->SetParameters(85.8656, 30.6158, 447.112, 447.111, 52., 433., 141.);
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization", verbose),
  fPixelJitter(cPixelJitter),
  fPixelDelay(cPixelDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0.),
  fPixelSignalCreatingTime(0.),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPixelSignal(NULL),
  /*fPECountF(0),
  fPECountB(0),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),*/
  fUseCrosstalks(kTRUE)
{
  fSPEfunc = new TF1("fSPEfunc", "ERNeuRadDigitizer::SPEfunc", 0., 2000., 7);
  fSPEfunc->SetParameters(85.8656, 30.6158, 447.112, 447.111, 52., 433., 141.);
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::~ERNeuRadDigitizer()
{
  //if (fSPEfunc) {
    //delete fSPEfunc;
    //fSPEfunc = NULL;
  //}
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
  if ( ! fNeuRadPoints) Fatal("ERNeuRadDigitizer::Init", "No NeuRadPoint Collection");

  // Register output array NeuRadPhotoElectron and NeuRadDigi
  fNeuRadPhotoElectron = new TClonesArray("ERNeuRadPhotoElectron", 1000);
  ioman->Register("NeuRadPhotoElectron", "NeuRad photoelectrons", fNeuRadPhotoElectron, kTRUE);
  fNeuRadPixelSignal = new TClonesArray("ERNeuRadPixelSignal", 1000);
  ioman->Register("NeuRadPixelSignal", "Signal Pixel", fNeuRadPixelSignal, kTRUE);

  fNeuRadSetup->Print();
  return kSUCCESS;
}
// ----------------------------------------------------------------------------

// -----   Public method Exec   -----------------------------------------------
void ERNeuRadDigitizer::Exec(Option_t* opt)
{
  LOG(DEBUG) << "----------------------------------------------------------------------" << FairLogger::endl;

  Int_t fPECountF = 0;
  Int_t fPECountB = 0;
  Float_t fSumAmplitudeF = 0;
  Float_t fSumAmplitudeB = 0;

  // Reset entries in output arrays
  Reset();

  //TODO we do not really need to ask for these numbers for each event!
  // Let's just do it once at the beginning!
  Int_t nofPixels = fNeuRadSetup->GetNofPixelsPerPMT(); //TODO check!
  Int_t nofPMTs = fNeuRadSetup->GetNofModules();
  LOG(DEBUG2) << "SETUP: nofPixels=" << nofPixels << "\t" << "nofPMTs=" << nofPMTs << FairLogger::endl;

  // Выделяем память под хранение фотоэлектронов по файберам
  // Memory allocation
  //
  //
  //FIXME This should not be done for each event!
  // Allocate once at the beginning and use it!
  std::vector<ERNeuRadPhotoElectron* >** peInPixelsF = new std::vector<ERNeuRadPhotoElectron*>* [nofPMTs]; // front (smaller Z)
  std::vector<ERNeuRadPhotoElectron* >** peInPixelsB = new std::vector<ERNeuRadPhotoElectron*>* [nofPMTs]; // back (bigger Z)
  for (Int_t i = 0; i<nofPMTs; i++) {
    peInPixelsF[i] = new std::vector<ERNeuRadPhotoElectron*> [nofPixels]; // front (smaller Z)
    peInPixelsB[i] = new std::vector<ERNeuRadPhotoElectron*> [nofPixels]; // back (bigger Z)
  }

  //TODO is this the correct way to get the current event number?
  Int_t iEvent = FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();

  // Number of points in NeuRad in the current event
  Int_t points_count = fNeuRadPoints->GetEntries();
  LOG(INFO) << "Event " << iEvent << "\t" << "points_count=" << points_count << FairLogger::endl;

  // Timer for debugging
  fPhotoElectronsCreatingTimer.Start();

  // Формируем промежуточные сущности PhotoElectrons
  //TODO ну так себе коммент
  for (UInt_t iPoint=0; iPoint<points_count; iPoint++) {
    // Current point
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    // Generate photoelectrons on the front and the back sides of the detector for the current point
    PhotoElectronsCreating(iPoint, point, peInPixelsF, 0, fPECountF, fSumAmplitudeF); // front (smaller Z)
    PhotoElectronsCreating(iPoint, point, peInPixelsB, 1, fPECountB, fSumAmplitudeB); // back (bigger Z)
  }

  // Timer for debugging
  fPhotoElectronsCreatingTimer.Stop();
  fPhotoElectronsCreatingTime += fPhotoElectronsCreatingTimer.RealTime();

  // Timer for debugging
  fPixelSignalCreatingTimer.Start();

  // Формируем сигналы на ФЭУ и digi
  // Генерируем сигналы на пикселях
  //TODO ну так себе коммент
  for (UInt_t iPMT=0; iPMT<nofPMTs; iPMT++) {
    for (UInt_t iPixel=0; iPixel<nofPixels; iPixel++) {
      PixelSignalsCreating(iPMT, iPixel, peInPixelsF, 0); // front (smaller Z)
      PixelSignalsCreating(iPMT, iPixel, peInPixelsB, 1); // back (bigger Z)
    }
  }

  // Timer for debugging
  fPixelSignalCreatingTimer.Stop();
  fPixelSignalCreatingTime += fPixelSignalCreatingTimer.RealTime();

  // Free dynamically allocated memory
  for (UInt_t i=0; i<nofPMTs; i++) {
    delete [] peInPixelsF[i];
    delete [] peInPixelsB[i];
  }
  delete [] peInPixelsF;
  delete [] peInPixelsB;

  // Fill event header
  FairRunAna* run = FairRunAna::Instance();
  EREventHeader* header = (EREventHeader*)run->GetEventHeader();
  header->SetNeuRadPECountF(fPECountF);
  header->SetNeuRadPECountB(fPECountB);
  header->SetNeuRadSumAmplitudeF(fSumAmplitudeF);
  header->SetNeuRadSumAmplitudeB(fSumAmplitudeB);

  LOG(DEBUG) << "----------------------------------------------------------------------" << FairLogger::endl;

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::PhotoElectronsCreating(Int_t iPoint,
                                               ERNeuRadPoint *point,
                                               std::vector< ERNeuRadPhotoElectron* >** peInPixels,
                                               Int_t side,
                                               Int_t& sumNphotoElectrons,
                                               Float_t& sumAmplitude)
{
    LOG(DEBUG2) << "iPoint=" << iPoint << FairLogger::endl;

    //TODO !!!!!
    Int_t pointPMT = point->GetPmtId();
    Int_t pointCh  = point->GetChId();
    LOG(DEBUG) << "PMT ID of the current point: " << pointPMT << FairLogger::endl;
    LOG(DEBUG) << "channel ID of the current point: " << pointCh << FairLogger::endl;

    // Get fiber length along Z axis from the setup singleton class object
    Double_t fiberLength = fNeuRadSetup->GetFiberLength();
    // Get IN and OUT Z coordinates of the point from the simulation
    // Compute average Z coordinate and use it a the point coordinate
    Double_t pointZInLocal =  point->GetZInLocal();
    Double_t pointZOutLocal = point->GetZOutLocal();
    Double_t pointZMidLocal = (pointZInLocal + pointZOutLocal) / 2.;
    // Compute the length of the path for the photons born for this point
    Double_t flightLength = 0.;
    if (side == 0) { // to the front (smaller Z) PMT
      flightLength = fiberLength/2. + pointZMidLocal;
    } else { // to the back (bigger Z) PMT
      flightLength = fiberLength/2. - pointZMidLocal;
    }

    LOG(DEBUG) << "side=" << side << "\t"
               << "pointZInLocal=" << pointZInLocal << "\t"
               << "pointZOutLocal=" << pointZOutLocal << "\t"
               << "pointZMidLocal=" << pointZMidLocal << "\t"
               << "flightLength=" << flightLength
               << FairLogger::endl;

    // Get some other information about the current point from simulation results
    Double_t pointELoss  = point->GetEnergyLoss(); // [GeV] //TODO not used!
    Double_t pointLYield = point->GetLightYield(); // [GeV]
    Double_t pointTime   = point->GetTimeIn();

    // Значение квантовой эффективности для конкретного пикселя
    //TODO !!!!!
    Double_t PixelQuantumEfficiency = fNeuRadSetup->GetPixelQuantumEfficiency(pointPMT, pointCh);

    // Две причины затухания света
    Double_t k1 = 0.5 - cLightFractionInTotalIntReflection;
    Double_t k2 = cLightFractionInTotalIntReflection;

    // Number of photons born from this MC-point
    Double_t nPhotosBorn = pointLYield * 1000. * cSciFiLightYield;
    LOG(DEBUG2) << "nPhotosBorn=" << nPhotosBorn << FairLogger::endl;

    // Number of photons reaching the photocathode
    // This takes into account two attenuation reasons
    // Absorption in the fiber and solid angle near the end
    Double_t nPhotonsAtCathode = nPhotosBorn * (k1*exp(-flightLength/0.5) + k2*exp(-flightLength/200.));
    LOG(DEBUG2) << "nPhotonsAtCathode=" << nPhotonsAtCathode << FairLogger::endl;

    // Number of photoelectrons (born in the photocathode)
    // Quantuum efficiency accounted here
    Int_t nPhotoElectrons = gRandom->Poisson(nPhotonsAtCathode * PixelQuantumEfficiency);
    sumNphotoElectrons += nPhotoElectrons;

    LOG(DEBUG2) << "nPhotoElectrons=" << nPhotoElectrons << "\tsumNphotoElectrons=" << sumNphotoElectrons << FairLogger::endl;

    // For each photoelectron
    for(Int_t iPE=0; iPE<nPhotoElectrons; iPE++)
    {
      // Экпоненциальный закон высвечивания. Обратное распределение.
      // Рождение.
      Double_t peLYTime = pointTime + (-1.) * fScincilationTau * TMath::Log(1-gRandom->Uniform());

      // Cathode time is computed from the length of the photon path
      // in the scintillator and its speed there
      Double_t peCathodeTime = peLYTime + flightLength/cMaterialSpeedOfLight;

      // Учёт кросстолков
      // На вход подаётся идентификатор канала, в котором был поинт (ФЭУ+канал)
      // метод возвращает через 3-й и 4-й аргументы идентификатор канала, в котором
      // собственно развилась электронная лавина и сформировался сигнал
      Int_t pePMT = pointPMT;
      Int_t peCh = pointCh;
      if (fUseCrosstalks) {
        LOG(DEBUG) << "pointPMT=" << pointPMT << "\tpointCh=" << pointCh << FairLogger::endl;
        this->Crosstalks(pointPMT, pointCh, pePMT, peCh);
      }

      LOG(DEBUG2) << "pePMT=" << pePMT << "\t"
                  << "peCh=" << peCh
                  << FairLogger::endl;

      if (pePMT != pointPMT || peCh != pointCh) {
        // This means that crosstalk changed the channel of the current photoelectron
        LOG(DEBUG) << "Crosstalk worked." << FairLogger::endl;
      }

      // The amplitude of the signal produced by the PMT
      // is generated using the analytical approximation
      // of the laboratory measurements
      Double_t peAmplitude = fSPEfunc->GetRandom(); // divide by 386.6

      //
      sumAmplitude += peAmplitude;

      // Dynode system transit time and transit time jitter
      Double_t dynodeSystemDelay = (Double_t)gRandom->Gaus(fPixelDelay, fPixelJitter);
      // Anode time is cathode time + dynode system transit time
      Double_t peAnodeTime = peCathodeTime + dynodeSystemDelay;
      // Generate the photoelectron
      ERNeuRadPhotoElectron* pe = this->AddPhotoElectron(iPoint, side, peLYTime - pointTime, peCathodeTime, peAnodeTime, nPhotonsAtCathode, peAmplitude);
      peInPixels[pePMT][peCh].push_back(pe);
    }
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::Crosstalks(Int_t pointPmtId, Int_t pointChId, Int_t& pePmtId, Int_t& peChId) {
  // Возвращает номер файбера в котором окажется фотоэлектрон после применения кростолков
  // Пока для простоты ввода данных масштабирует, значение кростолков для одного модуля на все остальные
  // Иными словами, идентификатор модуля (т.е. ФЭУ) игнорируется и учитывается только номер пикселя = номер канала
  // Но происходит это не тут, а глубже - в digipar

  LOG(DEBUG3) << "pointPmtId=" << pointPmtId << "\t"
             << "pointChId=" << pointChId
             << FairLogger::endl;

  // Get number of pixels in one direction in one PMT (8) from the setup
  Int_t nPixelInRow = fNeuRadSetup->GetRowNofPixels();

  // Get crosstalks sub-matrix for the current pixel
  TArrayF crosstalks;
  fNeuRadSetup->Crosstalks(pointPmtId, pointChId, crosstalks);

  Int_t crosstalkSubMatrixSize = 5; //TODO 3 or 5

  // Just debug output
  for (Int_t i=0; i<crosstalkSubMatrixSize; i++) {
    LOG(DEBUG3) << crosstalks[i*crosstalkSubMatrixSize + 0] << "\t"
                << crosstalks[i*crosstalkSubMatrixSize + 1] << "\t"
                << crosstalks[i*crosstalkSubMatrixSize + 2] << "\t"
                << crosstalks[i*crosstalkSubMatrixSize + 3] << "\t"
                << crosstalks[i*crosstalkSubMatrixSize + 4]
                << FairLogger::endl;
  }

  // Generate random number between 0 and 1
  Float_t prob = gRandom->Uniform();
  Float_t curProb = 0.;

  // Разбиваем отрезок от 0 до 1 на отрезки соответствующие вероятностям кросс-толков.
  // В какой именно промежуток вероятности попадёт prob, в тот файбер и перетечет фотоэлектрон.
  // Последний отрезок соответствует тому что фотоэлектрон останется в своём волокне.
  Bool_t crosstalkWorked = kFALSE;
  // Set output pixel same as input.
  // Further operations may change the output pixel
  // if any crosstalk happens
  pePmtId = pointPmtId;
  peChId = pointChId;
  Int_t subMatrixI; // Define outside of the loop block cause they will be used right after
  Int_t subMatrixJ; // Define outside of the loop block cause they will be used right after
  for (subMatrixI = 0; subMatrixI<crosstalkSubMatrixSize; subMatrixI++) {
    for (subMatrixJ = 0; subMatrixJ<crosstalkSubMatrixSize; subMatrixJ++) {

      Float_t curSubmatrixElement = crosstalks[subMatrixI*crosstalkSubMatrixSize + subMatrixJ];

      if (curSubmatrixElement == 0. || (subMatrixI==(crosstalkSubMatrixSize/2) && subMatrixJ==(crosstalkSubMatrixSize/2))) {
        //  Skip elements with zero probability and skip the central element
        LOG(DEBUG3) << subMatrixI << "\t" << subMatrixJ << "\t" << curSubmatrixElement << "\tcontinue" << FairLogger::endl;
        continue;
      }

      curProb += curSubmatrixElement;

      LOG(DEBUG3) << subMatrixI << "\t" << subMatrixJ << "\t" << curSubmatrixElement << "\tprob=" << prob << "\tcurProb=" << curProb << FairLogger::endl;
      if (curProb > prob) {
        // Here is the one which worked
        LOG(DEBUG3) << "Crosstalk worked. Break the loop." << FairLogger::endl;
        crosstalkWorked = kTRUE;
        break;
      }
    }
    //LOG(DEBUG3) << "Woops" << FairLogger::endl;
    if (crosstalkWorked) break;
  }

  if (crosstalkWorked) {
    // pePMTId is not changed in the current version
    Int_t dI = subMatrixI - crosstalkSubMatrixSize/2;
    Int_t dJ = subMatrixJ - crosstalkSubMatrixSize/2;
    peChId += dI*nPixelInRow + dJ;
    LOG(DEBUG3) << "subMatrixI=" << subMatrixI << "\t"
                << "subMatrixJ=" << subMatrixJ << "\t"
                << "dI=" << dI << "\t"
                << "dJ=" << dJ << "\t"
                << "new peChId=" << peChId
                << FairLogger::endl;
  }

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Double_t ERNeuRadDigitizer::SPEfunc(Double_t *x, Double_t *par) {
  Double_t fitval;
  if (x[0]<63) {
    fitval = 0;
  }
  if (x[0]>=63 && x[0]<par[0]) {
    fitval = (x[0]-63) * (par[1]) / (par[0]-63) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[0]) {
    fitval = par[1]*(x[0]-par[2])*(x[0]+par[2]-2*par[3])/((par[0]-par[2])*(par[0]+par[2]-2*par[3])) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[2]) {
    fitval = par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  return fitval;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
void ERNeuRadDigitizer::PixelSignalsCreating(Int_t iPMT,
                                             Int_t iChannel,
                                             std::vector<ERNeuRadPhotoElectron* >** peInPixels,
                                             Int_t side)
{
  if (peInPixels[iPMT][iChannel].size() > 0) {
    LOG(DEBUG2) << "side=" << side << "\t"
                << "iPMT=" << iPMT << "\t"
                << "iChannel=" << iChannel << "\t"
                << "peCount=" << peInPixels[iPMT][iChannel].size()
                << FairLogger::endl;
    ERNeuRadPixelSignal* PixelSignal = AddPixelSignal(iPMT, iChannel, peInPixels[iPMT][iChannel].size(), side);
    for (Int_t iPE=0; iPE<peInPixels[iPMT][iChannel].size(); iPE++) {
      ERNeuRadPhotoElectron* pe = peInPixels[iPMT][iChannel][iPE];
      PixelSignal->AddPhotoElectron(pe);
    }
    PixelSignal->Generate();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish() {
  LOG(INFO) << "========== Finish of ERNeuRadDigitizer =================="<< FairLogger::endl;
  LOG(INFO) << "===== Time on PhotoElectrons creating : " << fPhotoElectronsCreatingTime << " s" << FairLogger::endl;
  LOG(INFO) << "===== Time on Pixel signal creating : " << fPixelSignalCreatingTime << " s" << FairLogger::endl;
  LOG(INFO) << "=========================================================" << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadPixelSignal* ERNeuRadDigitizer::AddPixelSignal(Int_t iPMT,
                                                       Int_t iChannel,
                                                       Int_t numberOfPoints,
                                                       Int_t side)
{
  LOG(DEBUG2) << "ERNeuRadDigitizer::AddPixelSignal" << FairLogger::endl;
  ERNeuRadPixelSignal *PixelSignal = new((*fNeuRadPixelSignal)[PixelSignalCount()])
                ERNeuRadPixelSignal(iPMT, iChannel, numberOfPoints, side);
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
  //TODO maybe execute PhotoElectronCount once and use returned value twice?
  LOG(DEBUG2) << "side=" << side << "\t"
              << "iPoint=" << iPoint << "\t"
              << "photonCount=" << photonCount << "\t"
              << "amplitude=" << amplitude
              << FairLogger::endl;
  ERNeuRadPhotoElectron *fp = new ((*fNeuRadPhotoElectron)[PhotoElectronCount()])
    ERNeuRadPhotoElectron(PhotoElectronCount(), side, lytime, cathode_time, anode_time, photonCount, amplitude);
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
