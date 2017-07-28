#include "ERSimtoRaw.h"
#include "FairRun.h"

#include "TClonesArray.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw(Int_t NEvents)
  : FairTask("Convert Sim event to Raw event", NEvents),
fNChanels(2),
fEvent(0),
fNPoints(1000),
fPixelSignals(NULL),
fRawEvents(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw()
  : FairTask("Convert Sim event to Raw event"),
fNChanels(2),
fEvent(0),
fNPoints(1000),
fPixelSignals(NULL),
fRawEvents(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::~ERSimtoRaw()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERSimtoRaw::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  //Get input objects
  // fPixelSignals = (ERNeuRadPixelSignal*) ioman->GetObject("NeuRadPixelSignal");
  fPixelSignals = (TClonesArray*) ioman->GetObject("NeuRadPixelSignal");
    if (!fPixelSignals)
      Fatal("Init", "Can`t find branch in input file!");


  //Register output objects
  fRawEvents = new RawEvent*[fNChanels];
  for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
    fRawEvents[iChanel] = new RawEvent(fNPoints);
    TString bName;
    bName.Form("ch%d.",iChanel+1);
    ioman->Register(bName,"Raw", fRawEvents[iChanel], kTRUE);
  }

      return kSUCCESS;
}  
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERSimtoRaw::Exec(Option_t* opt)
{
  fEvent++;
  if ( !(fEvent%500) ) { std::cout << "####### EVENT " << fEvent << " #####" << std::endl; }
  Int_t simSize,side,counter,PEnum;
  Double_t maxAmp, StartTime, FinishTime;


  
  FairRun* run = FairRun::Instance();
  run->MarkFill(kFALSE);
  for (Int_t iSignal = 0; iSignal < fPixelSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fPixelSignals->At(iSignal);
    // if (signal->Side() !=1 || signal->PixelNb() != 25) continue;
//    if ( signal->PixelNb() != 136 ) continue;
    TArrayF* result = signal->ResultSignal();
    maxAmp = 0;
    simSize = 0;
    simSize = result->GetSize();

    if ( signal->Side() == 0 ){
      for(Int_t i = 0; i<simSize; i++){
        if( maxAmp < result->GetAt(i) ) {maxAmp = result->GetAt(i);}
      }  //searching maxAmp
//      cout << maxAmp << endl;
      if(maxAmp > 5) {
//        cout << " not NULL event found " << endl;
        run->MarkFill(kTRUE);
      }
    }
    // std::cout << "####### AMP " << maxAmp << " #####" << std::endl;
  // Int_t PixelNb() const {return fPixelNb;}
  // Int_t Side() const {return fSide;}

	StartTime = signal->GetStartTime();
	FinishTime = signal->GetFinishTime();
	//PEnum = signal->GetPECount();
    for(Int_t nPoint = 0; nPoint< fNPoints; nPoint++){

      if( nPoint<=StartTime*10 || nPoint>=(FinishTime*10-1) ) {
        if(signal->Side() == 0) {fRawEvents[0]->SetAmp(0, nPoint);}
        if(signal->Side() == 1) {fRawEvents[1]->SetAmp(0, nPoint);}
      }
      else{
         if(signal->Side() == 0) {fRawEvents[0]->SetAmp(-result->GetAt(nPoint - StartTime*10), nPoint);} //fill the RawEvent
         if(signal->Side() == 1) {fRawEvents[1]->SetAmp(-result->GetAt(nPoint - StartTime*10), nPoint);}
        // fRawEvents[0]->SetAmp(0, nPoint); //fill the RawEvent
      }
      
      if(signal->Side() == 0) {fRawEvents[0]->SetTime(nPoint*0.1, nPoint);}
      if(signal->Side() == 1) {fRawEvents[1]->SetTime(nPoint*0.1, nPoint);}
    }

 	if(signal->Side() == 0) fRawEvents[0]->SetStartTime(StartTime);
        if(signal->Side() == 1) fRawEvents[1]->SetStartTime(StartTime);
        if(signal->Side() == 0) fRawEvents[0]->SetFinishTime(FinishTime);
        if(signal->Side() == 1) fRawEvents[1]->SetFinishTime(FinishTime);
	
	if(signal->Side() == 0) {
		fRawEvents[0]->SetPETimes(signal->GetPETimes());
		fRawEvents[0]->SetPEAmps(signal->GetPEAmps());
	}
	if(signal->Side() == 1) {
		fRawEvents[1]->SetPETimes(signal->GetPETimes());
		fRawEvents[1]->SetPEAmps(signal->GetPEAmps());
	}

  }
  //----------------------------------------------------------------------------
    //cout << signal->Side() << "\t" << signal->PixelNb() << endl;
    // Int_t count = (Int_t)(signal->FinishTime()-signal->StartTime())/signal->dT();
    // Float_t* times = new Float_t[count];
    // for (Int_t i = 0; i < count; i++){
    //   times[i] = signal->StartTime() + signal->dT()*i; 
    // }
    // TGraph* gr = new TGraph(count,times,result->GetArray());
    // TString title;
    // title.Form("Pixel Signal, side %d,%d fiber, %d module, %d pe, %d", signal->Side(),
    //                     signal->PixelNb(), signal->ModuleNb(), signal->PECount(),
    //                     FairRunAna::Instance()->GetEventHeader()->GetMCEntryNumber());
    // gr->SetTitle(title);
    // if (signal->Side() == 0)
    //   front->cd();
    // else
    //   back->cd();
    // gr->Write();
    // delete gr;
    // delete [] times;
  


  // for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
  //   //ERNeuRadPixelSignal* signal = (ERNeuRadPixelSignal*)fPixelSignals->At(iChanel);
  //   TArrayF* result = fPixelSignals->ResultSignal();
  //   for(Int_t nPoint = 0; nPoint< fNPoints; nPoint++){
  //     fRawEvents[iChanel]->SetAmp(result->GetAt(nPoint), nPoint); //fill the RawEvent
  //     fRawEvents[iChanel]->SetTime(nPoint*0.1, nPoint); 
  //   }
  // }  
  
}
//----------------------------------------------------------------------------
