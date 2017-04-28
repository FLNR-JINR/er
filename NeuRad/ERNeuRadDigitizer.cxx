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

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "EREventHeader.h"

const Double_t ERNeuRadDigitizer::cSciFiLightYield= 8000.; // [photons/MeV]
const Double_t ERNeuRadDigitizer::cSpeedOfLight = 0.299792458e2;  //[cm/ns]
const Double_t ERNeuRadDigitizer::cMaterialSpeedOfLight = ERNeuRadDigitizer::cSpeedOfLight/1.58;//[cm/ns]
const Double_t ERNeuRadDigitizer::cLightFractionInTotalIntReflection = 0.04;
//доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
const Double_t ERNeuRadDigitizer::cPMTDelay=6.;//[ns] (H8500)
const Double_t ERNeuRadDigitizer::cPMTJitter = 0.4/2.36; //[ns] (H8500)
const Double_t ERNeuRadDigitizer::cScincilationTau = 3.2; //[ns]

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer()
  : FairTask("ER NeuRad Digitization scheme"),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0),
  fPMTSignalCreatingTime(0),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPMTSignal(NULL),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),
  fPECountF(0),
  fPECountB(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERNeuRadDigitizer::ERNeuRadDigitizer(Int_t verbose)
  : FairTask("ER NeuRad Digitization scheme ", verbose),
  fPMTJitter(cPMTJitter),
  fPMTDelay(cPMTDelay),
  fScincilationTau(cScincilationTau),
  fPhotoElectronsCreatingTime(0),
  fPMTSignalCreatingTime(0),
  fNeuRadSetup(NULL),
  fNeuRadPoints(NULL),
  fNeuRadPhotoElectron(NULL),
  fNeuRadPMTSignal(NULL),
  fSumAmplitudeF(0),
  fSumAmplitudeB(0),
  fPECountF(0),
  fPECountB(0)
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
  // Get input array
  cout << "Init" << endl;
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNeuRadPoints = (TClonesArray*) ioman->GetObject("NeuRadPoint");
  if ( ! fNeuRadPoints) Fatal("ERNeuRadDigitizer::Init","No NeuRadPoint Collection");
  
  // Register output array NeuRadPhotoElectron and NeuRadDigi
  fNeuRadPhotoElectron = new TClonesArray("ERNeuRadPhotoElectron",1000);									
  ioman->Register("NeuRadPhotoElectron", "NeuRad Fiber points", fNeuRadPhotoElectron, kTRUE);
  fNeuRadPMTSignal = new TClonesArray("ERNeuRadPMTSignal", 1000);
  ioman->Register("NeuRadPMTSignal", "Signal PMT", fNeuRadPMTSignal, kTRUE);

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
  Int_t nofFibers = fNeuRadSetup->NofFibers();
  Int_t nofModules = fNeuRadSetup->NofModules();
  //выделяем память под хранение фотоэлектронов по файберам
  vector<ERNeuRadPhotoElectron* >** peInFibersF = new vector<ERNeuRadPhotoElectron*>* [nofModules];
  vector<ERNeuRadPhotoElectron* >** peInFibersB = new vector<ERNeuRadPhotoElectron*>*  [nofModules];
  for (Int_t i = 0; i<nofModules; i++){
    peInFibersF[i] = new vector<ERNeuRadPhotoElectron*>  [nofFibers];
    peInFibersB[i] = new vector<ERNeuRadPhotoElectron*>  [nofFibers];
  }
  
  Int_t points_count = fNeuRadPoints->GetEntries();
  //Формируем промежуточные сущности PhotoElectrons
  
  fPhotoElectronsCreatingTimer.Start();
  for (Int_t iPoint=0; iPoint < points_count; iPoint++) { // цикл
    ERNeuRadPoint *point = (ERNeuRadPoint*) fNeuRadPoints->At(iPoint);
    PhotoElectronsCreating(iPoint,point,peInFibersF,0,fPECountF,fSumAmplitudeF);
    PhotoElectronsCreating(iPoint,point,peInFibersB,1,fPECountB,fSumAmplitudeB);
  }
  fPhotoElectronsCreatingTimer.Stop();
  
  fPhotoElectronsCreatingTime += fPhotoElectronsCreatingTimer.RealTime();
  //Формируем сигналы на ФЭУ и digi
  fPMTSignalCreatingTimer.Start();
  
  for (Int_t iModule = 0; iModule < nofModules; iModule++){
    //Генерируем сигналы на пикселях
    for (Int_t iFiber = 0; iFiber < nofFibers; iFiber++) {
      PMTSignalsCreating(iModule, iFiber, peInFibersF,0);
      PMTSignalsCreating(iModule, iFiber, peInFibersB,1);
    }
  }
  
  fPMTSignalCreatingTimer.Stop();
  fPMTSignalCreatingTime += fPMTSignalCreatingTimer.RealTime();
  
  //освобождаем память
  for (Int_t i = 0; i<nofModules; i++){
    delete [] peInFibersF[i];
    delete [] peInFibersB[i];
  }
  delete [] peInFibersF;
  delete [] peInFibersB;
  
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
                        std::vector<ERNeuRadPhotoElectron* >** peInFibers,Int_t side, Int_t& sumPECount,Float_t& sumAmplitude)
{
    //Генерация фотоэлектронов для поинта iPoint

    //Получение информации о поинте
    Double_t fiberLength = fNeuRadSetup->FiberLength();
    Int_t    pointModule = point->GetModuleNb();
    Int_t    pointFiber  = point->GetFiberInModuleNb();
    Double_t pointELoss      =  point->GetEnergyLoss(); //[GeV]
    Double_t pointLYield = point->GetLightYield();  //[GeV]
    Double_t pointZ          = point->GetZInLocal();
    Double_t pointTime       = point->GetTimeIn();
    //Double_t pointZInFiber = pointZ + fiberLength/2. - fNeuRadSetup->Z();
    Double_t pointZInFiber = pointZ + fiberLength/2.;

    //Значение квантовой эффективности для конкретного пикселе
    Double_t PMTQuantumEfficiency = fNeuRadSetup->PMTQuantumEfficiency(pointModule,pointFiber);
    
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
    Int_t peCount = (Int_t)gRandom->Poisson(pePhotonCount*PMTQuantumEfficiency);
    sumPECount+=peCount;

    for(Int_t iPE=0;iPE<peCount;iPE++){
      //Экпоненциальный закон высвечивания. Обратное распределение
      Double_t peLYTime = pointTime + (-1)*fScincilationTau*TMath::Log(1-gRandom->Uniform());
      //Скорость света в материале.
      Double_t peCathodeTime = peLYTime + flightLength/cMaterialSpeedOfLight;
      //Учёт кросстолков
      Int_t peFiber = pointFiber, peModule = pointModule;
      //Crosstalks(pointModule,pointFiber, peModule, peFiber);
      //Амплиту одноэлектронного сигнала
      Double_t pmtGain = fNeuRadSetup->PMTGain(peModule,peFiber);
      Double_t pmtSigma = fNeuRadSetup->PMTSigma(peModule,peFiber);
      Double_t peAmplitude = TMath::Abs(gRandom->Gaus(pmtGain, pmtSigma));
      sumAmplitude+=peAmplitude;
      //Задержка динодной системы и джиттер
      Double_t peAnodeTime = peCathodeTime + (Double_t)gRandom->Gaus(fPMTDelay, fPMTJitter);
      ERNeuRadPhotoElectron* pe = AddPhotoElectron(iPoint, side, peLYTime - pointTime, peCathodeTime, peAnodeTime, pePhotonCount, peAmplitude);
      peInFibers[peModule][peFiber].push_back(pe);
    }
}

//----------------------------------------------------------------------------
Int_t ERNeuRadDigitizer::Crosstalks(Int_t pointModule, Int_t pointFiber, Int_t& peModule, Int_t& peFiber){
  //Возвращает номер файбера в котором окажется фотоэлектрон после применения кростолков
  //Пока для простоты ввода данных масштабирует, значение кростолков для одного модуля на все остальные
  peFiber = pointFiber;
  peModule = pointModule;
  TArrayF crosstalks;
  fNeuRadSetup->PMTCrosstalks(peFiber, crosstalks);
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
    peFiber-= fNeuRadSetup->RowNofFibers();
  }
  if (csI == 2){
    peFiber+=fNeuRadSetup->RowNofFibers();
  }
  // Переход между столбцами волокон в модуле
  if (csJ == 0){
    peFiber-=1;
  }
  if (csJ == 2){
    peFiber+=1;
  }
}

//----------------------------------------------------------------------------
void ERNeuRadDigitizer::Reset()
{
  if (fNeuRadPhotoElectron){
    fNeuRadPhotoElectron->Delete();
  }
  if (fNeuRadPMTSignal){
    fNeuRadPMTSignal->Delete();
  }
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void ERNeuRadDigitizer::PMTSignalsCreating(Int_t iModule, Int_t iFiber,
                                std::vector<ERNeuRadPhotoElectron* >** peInFibers,Int_t side)
{
  if(peInFibers[iModule][iFiber].size() > 0){
    ERNeuRadPMTSignal* pmtSignal = AddPMTSignal(iModule, iFiber,peInFibers[iModule][iFiber].size(),side);
    for(Int_t iPE = 0; iPE < peInFibers[iModule][iFiber].size(); iPE++){
      ERNeuRadPhotoElectron* pe = peInFibers[iModule][iFiber][iPE];
      pmtSignal->AddPhotoElectron(pe);
    }
    pmtSignal->Generate();
  }
}
// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::Finish()
{   
  std::cout << "========== Finish of ERNeuRadDigitizer =================="<< std::endl;
  std::cout << "=====  Time on PhotoElectrons creating : " <<  fPhotoElectronsCreatingTime << " s" << std::endl;
  std::cout << "=====  Time on PMT signal creating : " <<  fPMTSignalCreatingTime << " s" << std::endl;
  std::cout << "=========================================================" << std::endl;
}
// ----------------------------------------------------------------------------
// ---------------------------------------------------------------------------- 
ERNeuRadPMTSignal* ERNeuRadDigitizer::AddPMTSignal(Int_t iModule, Int_t iFiber, Int_t fpoints_count, Int_t side){ 
  ERNeuRadPMTSignal *pmtSignal = new((*fNeuRadPMTSignal)[PMTSignalCount()]) 
                ERNeuRadPMTSignal(iModule, iFiber,fpoints_count, side);   return  pmtSignal; 
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
Int_t ERNeuRadDigitizer::PMTSignalCount()   const {
  return fNeuRadPMTSignal->GetEntriesFast();
}
//----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERNeuRadDigitizer)
