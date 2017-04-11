#include "ERSimtoRaw.h"

#include "TClonesArray.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw(Int_t NEvents)
  : FairTask("Convert Sim event to Raw event", NEvents),
fNChanels(4),
fEvent(0),
fNPoints(1000),
fPMTSignals(NULL),
fRawEvents(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERSimtoRaw::ERSimtoRaw()
  : FairTask("Convert Sim event to Raw event"),
fNChanels(4),
fEvent(0),
fNPoints(1000),
fPMTSignals(NULL),
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
  // fPMTSignals = (ERNeuRadPMTSignal*) ioman->GetObject("NeuRadPMTSignal");
  fPMTSignals = (TClonesArray*) ioman->GetObject("NeuRadPMTSignal");
    if (!fPMTSignals)
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
  if ( !(fEvent%100) ) { std::cout << "####### EVENT " << fEvent << " #####" << std::endl; }
  Int_t simSize = 0;

  for (Int_t iSignal = 0; iSignal < fPMTSignals->GetEntriesFast(); iSignal++){
    ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)fPMTSignals->At(iSignal);
    // if (signal->Side() !=1 || signal->FiberIndex() != 25) continue;
    if (signal->FiberIndex() != 25) continue;
    TArrayF* result = signal->ResultSignal();
     simSize = result->GetSize();
    for(Int_t nPoint = 0; nPoint< fNPoints; nPoint++){

      if( nPoint<=(fNPoints - simSize)/2 || nPoint>=(fNPoints + simSize)/2 ) {
        fRawEvents[0]->SetAmp(0, nPoint);
      }
      else{
         fRawEvents[0]->SetAmp(-result->GetAt(nPoint - (fNPoints - simSize)/2), nPoint); //fill the RawEvent
        // fRawEvents[0]->SetAmp(0, nPoint); //fill the RawEvent
      }
      
      fRawEvents[0]->SetTime(nPoint*0.1, nPoint); 
    }
  }
  //----------------------------------------------------------------------------
    //cout << signal->Side() << "\t" << signal->FiberIndex() << endl;
    // Int_t count = (Int_t)(signal->FinishTime()-signal->StartTime())/signal->dT();
    // Float_t* times = new Float_t[count];
    // for (Int_t i = 0; i < count; i++){
    //   times[i] = signal->StartTime() + signal->dT()*i; 
    // }
    // TGraph* gr = new TGraph(count,times,result->GetArray());
    // TString title;
    // title.Form("PMT Signal, side %d,%d fiber, %d module, %d pe, %d", signal->Side(),
    //                     signal->FiberIndex(), signal->ModuleIndex(), signal->PECount(),
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
  //   //ERNeuRadPMTSignal* signal = (ERNeuRadPMTSignal*)fPMTSignals->At(iChanel);
  //   TArrayF* result = fPMTSignals->ResultSignal();
  //   for(Int_t nPoint = 0; nPoint< fNPoints; nPoint++){
  //     fRawEvents[iChanel]->SetAmp(result->GetAt(nPoint), nPoint); //fill the RawEvent
  //     fRawEvents[iChanel]->SetTime(nPoint*0.1, nPoint); 
  //   }
  // }  
  
}
//----------------------------------------------------------------------------