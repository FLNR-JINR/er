#include "ERNeuRadDigitizer.h"

#include <iostream>
#include <algorithm>

using std::sort;
using std::vector;
using std::cerr;
using std::endl;
using namespace std;

#include "TClonesArray.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TF1.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "EREventHeader.h"

const Double_t ERNeuRadDigitizer::cSciFiLightYield= 6000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2;  //[cm/ns]
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58;//[cm/ns]
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::cPixelDelay=6.;//[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPixelJitter = 0.4/2.36; //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; //[ns]

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
  fUseCrosstalks(kFALSE)
{
  fPEA = new TF1("fPEA", "ERNeuRadDigitizer::PeFunc", 0., 2000., 7);
  fPEA->SetParameters(85.8656,30.6158,447.112,447.111,52.,433.,141.);	
  //fPEA->SetParameters(3.2,30.6158,447.112,447.111,52.,433.,141.); 
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
  fUseCrosstalks(kFALSE)
{
  fPEA = new TF1("fPEA", "ERNeuRadDigitizer::PeFunc", 0., 2000., 7);
  fPEA->SetParameters(85.8656,30.6158,447.112,447.111,52.,433.,141.);	
  //fPEA->SetParameters(3.2,30.6158,447.112,447.111,52.,433.,141.); 
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::~ERNeuRadDigitizer()
{
  if (fPEA) {
    //delete fPEA;
    //fPEA = NULL;
  }
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
  // Get input array
  cout << "Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  if ( ! fNeuRadPoints) Fatal("ERNeuRadDigitizer::Init","No NeuRadPoint Collection");
  
  // Register output array NeuRadPhotoElectron and NeuRadDigi
  fNeuRadPhotoElectron = new TClonesArray("ERNeuRadPhotoElectron",1000);									
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
  fPECountB  = 0;
  fSumAmplitudeF = 0;
  fSumAmplitudeB = 0;

  Int_t iEvent =
			FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber();
  std::cout << "Event " << iEvent << std::endl;
  // Reset entries in output arrays
  Reset();
  Int_t nofPixels = fNeuRadSetup->NofPixels();
  Int_t nofModules = fNeuRadSetup->NofModules();
  //выделяем память под хранение фотоэлектронов по файберам
  vector<ERNeuRadPhotoElectron* >** peInPixelsF = new vector<ERNeuRadPhotoElectron*>* [nofModules];
  vector<ERNeuRadPhotoElectron* >** peInPixelsB = new vector<ERNeuRadPhotoElectron*>*  [nofModules];
  for (Int_t i = 0; i<nofModules; i++){
    peInPixelsF[i] = new vector<ERNeuRadPhotoElectron*>  [nofPixels];
    peInPixelsB[i] = new vector<ERNeuRadPhotoElectron*>  [nofPixels];
  }
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  //Формируем промежуточные сущности PhotoElectrons
  
  fPhotoElectronsCreatingTimer.Start();
  for (Int_t iPoint=0; iPoint < points_count; iPoint++) { // цикл
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    PhotoElectronsCreating(iPoint,point,peInPixelsF,0,fPECountF,fSumAmplitudeF);
    PhotoElectronsCreating(iPoint,point,peInPixelsB,1,fPECountB,fSumAmplitudeB);
  }
  fPhotoElectronsCreatingTimer.Stop();
  
  fPhotoElectronsCreatingTime += fPhotoElectronsCreatingTimer.RealTime();
  //Формируем сигналы на ФЭУ и digi
  fPixelSignalCreatingTimer.Start();
  
  for (Int_t iModule = 0; iModule < nofModules; iModule++){
    //Генерируем сигналы на пикселях
    for (Int_t iPixel = 0; iPixel < nofPixels; iPixel++) {
      PixelSignalsCreating(iModule, iPixel, peInPixelsF,0);
      PixelSignalsCreating(iModule, iPixel, peInPixelsB,1);
    }
  }
  
  fPixelSignalCreatingTimer.Stop();
  fPixelSignalCreatingTime += fPixelSignalCreatingTimer.RealTime();
  
  //освобождаем память
  for (Int_t i = 0; i<nofModules; i++){
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
void ERNeuRadDigitizer::PhotoElectronsCreating(Int_t iPoint, ERNeuRadPoint *point,
                        std::vector<ERNeuRadPhotoElectron* >** peInPixels,Int_t side, Int_t& sumPECount,Float_t& sumAmplitude)
{
    //Генерация фотоэлектронов для поинта iPoint

    //Получение информации о поинте
    Double_t fiberLength = fNeuRadSetup->FiberLength();
    Int_t    pointModule = point->GetModuleNb();
    Int_t    pointPixel  = point->GetPixelNb();
    Double_t pointELoss      =  point->GetEnergyLoss(); //[GeV]
    Double_t pointLYield = point->GetLightYield();  //[GeV]
    Double_t pointZ          = point->GetZInLocal();
    Double_t pointTime       = point->GetTimeIn();
    //Double_t pointZInFiber = pointZ + fiberLength/2. - fNeuRadSetup->Z();
    Double_t pointZInFiber = pointZ + fiberLength/2.;

    //Значение квантовой эффективности для конкретного пикселе
    Double_t PixelQuantumEfficiency = fNeuRadSetup->PixelQuantumEfficiency(pointModule,pointPixel);
    
    //Количество фотонов
    Double_t photonCount = pointLYield*1000.*cSciFiLightYield;

    //Две причины затухания света
    Double_t k1 = 0.5-cLightFractionInTotalIntReflection;
    Double_t k2 = cLightFractionInTotalIntReflection;
    Double_t flightLength = 0;
    if (side == 0)
      flightLength = pointZInFiber;
    else
      flightLength = fiberLength-pointZInFiber;
    Double_t pePhotonCount =  photonCount*(k1*exp(-flightLength/0.5) + k2*exp(-flightLength/200.));

    //Квантовая эффективность
    Int_t peCount = (Int_t)gRandom->Poisson(pePhotonCount*PixelQuantumEfficiency);
    sumPECount+=peCount;
    
    for(Int_t iPE=0;iPE<peCount;iPE++){
      //Экпоненциальный закон высвечивания. Обратное распределение
      Double_t peLYTime = pointTime + (-1)*fScincilationTau*TMath::Log(1-gRandom->Uniform());
      //Double_t peLYTime = (-1.)*fScincilationTau*TMath::Log(1.-gRandom->Uniform());
     // Double_t peLYTime = TMath::Abs(gRandom->Exp(3.2)/3.2);
      //Скорость света в материале.
      Double_t peCathodeTime = peLYTime + flightLength/cMaterialSpeedOfLight;
      //Учёт кросстолков
      Int_t pePixel = pointPixel, peModule = pointModule;
      if (fUseCrosstalks)
        Crosstalks(pointModule,pointPixel, peModule, pePixel);
      //Амплиту одноэлектронного сигнала
      Double_t PixelGain = fNeuRadSetup->PixelGain(peModule,pePixel);
      Double_t PixelSigma = fNeuRadSetup->PixelSigma(peModule,pePixel);
     // Double_t peAmplitude = TMath::Abs(gRandom->Gaus(PixelGain, PixelSigma));
      Double_t peAmplitude = TMath::Abs(fPEA->GetRandom());
      sumAmplitude+=peAmplitude;
      //Задержка динодной системы и джиттер
      Double_t peAnodeTime = peCathodeTime + (Double_t)gRandom->Gaus(fPixelDelay, fPixelJitter);
     // ERNeuRadPhotoElectron* pe = AddPhotoElectron(iPoint, side, peLYTime - pointTime, peCathodeTime, peAnodeTime, pePhotonCount, peAmplitude);
       ERNeuRadPhotoElectron* pe = AddPhotoElectron(iPoint, side, peLYTime, peCathodeTime, peAnodeTime, pePhotonCount, peAmplitude);
      peInPixels[peModule][pePixel].push_back(pe);
    }
}
//----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::Crosstalks(Int_t pointModule, Int_t pointPixel, Int_t& peModule, Int_t& pePixel){
  //Возвращает номер файбера в котором окажется фотоэлектрон после применения кростолков
  //Пока для простоты ввода данных масштабирует, значение кростолков для одного модуля на все остальные
  pePixel = pointPixel;
  peModule = pointModule;
  TArrayF crosstalks;
  fNeuRadSetup->Crosstalks(pePixel, crosstalks);
  Float_t prob = gRandom->Uniform();
  Float_t curProb = 0;
  Int_t csI = -1;
  Int_t csJ = -1;
  //Разбиваем отрезок от 0 до 1 на отрезки соответствующие веростностям кростолков. 
  //В какой имено промежуток вероятности попадёт prob, в тот файбер и перетечет фотоэлектрон
  //Последний отрезок соответствует, тому что фотоэлектрон останется в своём волокне
  for (Int_t i = 0; i < 3; i++){
    for (Int_t j = 0; j < 3; j++){
      if (crosstalks[i*3+j] == 0 || (i==1 && j==1))
        continue;
      
      curProb += crosstalks[i*3+j];

      if (curProb > prob){
        csI = i;
        csJ = j;
        break;
      }
    }
    if (csI != -1)
      break;
  }
  // Переход между строками волокон в модуле
  if (csI == 0){
    pePixel-= fNeuRadSetup->RowNofPixels();
  }
  if (csI == 2){
    pePixel+=fNeuRadSetup->RowNofPixels();
  }
  // Переход между столбцами волокон в модуле
  if (csJ == 0){
    pePixel-=1;
  }
  if (csJ == 2){
    pePixel+=1;
  }
}

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset()
{
  if (fNeuRadPhotoElectron){
    fNeuRadPhotoElectron->Delete();
  }
  if (fNeuRadPixelSignal){
    fNeuRadPixelSignal->Delete();
  }
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::PixelSignalsCreating(Int_t iModule, Int_t iPixel,
                                std::vector<ERNeuRadPhotoElectron* >** peInPixels,Int_t side)
{
  if(peInPixels[iModule][iPixel].size() > 0){
    ERNeuRadPixelSignal* PixelSignal = AddPixelSignal(iModule, iPixel,peInPixels[iModule][iPixel].size(),side);
    for(Int_t iPE = 0; iPE < peInPixels[iModule][iPixel].size(); iPE++){
      ERNeuRadPhotoElectron* pe = peInPixels[iModule][iPixel][iPE];
      PixelSignal->AddPhotoElectron(pe);
    }
    PixelSignal->Generate();
  }
}
// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish()
{   
  std::cout << "========== Finish of ERNeuRadDigitizer =================="<< std::endl;
  std::cout << "=====  Time on PhotoElectrons creating : " <<  fPhotoElectronsCreatingTime << " s" << std::endl;
  std::cout << "=====  Time on Pixel signal creating : " <<  fPixelSignalCreatingTime << " s" << std::endl;
  std::cout << "=========================================================" << std::endl;
}
// ----------------------------------------------------------------------------
// ---------------------------------------------------------------------------- 
ERNeuRadPixelSignal* ERNeuRadDigitizer::AddPixelSignal(Int_t iModule, Int_t iPixel, Int_t fpoints_count, Int_t side){ 
  ERNeuRadPixelSignal *PixelSignal = new((*fNeuRadPixelSignal)[PixelSignalCount()]) 
                ERNeuRadPixelSignal(iModule, iPixel,fpoints_count, side);   return  PixelSignal; 
}
// ----------------------------------------------------------------------------
ERNeuRadPhotoElectron* ERNeuRadDigitizer::AddPhotoElectron(Int_t iPoint, Int_t side, Double_t lytime, Double_t cathode_time, Double_t anode_time, 
									Int_t photonCount, Double_t amplitude){
  ERNeuRadPhotoElectron *fp = new ((*fNeuRadPhotoElectron)[PhotoElectronCount()])
								ERNeuRadPhotoElectron(PhotoElectronCount(),side, lytime, cathode_time, anode_time, photonCount, amplitude);
  fp->AddLink(FairLink("NeuRadPoint",iPoint));
  return fp;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::PhotoElectronCount()  const {
  return fNeuRadPhotoElectron->GetEntriesFast();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::PixelSignalCount()   const {
  return fNeuRadPixelSignal->GetEntriesFast();
}
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Double_t ERNeuRadDigitizer::PeFunc(Double_t *x, Double_t *par) {
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
ClassImp(ERNeuRadDigitizer)
