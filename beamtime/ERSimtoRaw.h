// -------------------------------------------------------------------------
// -----                  ERRawToAnalyzeConverter header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERSimtoRaw_H
#define ERRSimtoRaw_H

#include "FairTask.h"

#include "ERNeuRadPixelSignal.h"
#include "RawEvent.h"
#include "AEvent.h"

class ERSimtoRaw : public FairTask {

public:
  /** Default constructor **/
  ERSimtoRaw();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERSimtoRaw(Int_t NEvents);

  /** Destructor **/
  ~ERSimtoRaw();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

protected:
  //Input objects
  TClonesArray *fPMTSignals;

  //Output arrays
  RawEvent** fRawEvents;


  Int_t fNChanels;
  Int_t fNPoints;
  Int_t fEvent;

  //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
  Float_t* fResFunction;    //!
  //Хранит результирующий сигнал, как значения функции в узлах
  TArrayF fResFunctionRoot; 


/*private:
  virtual void SetParContainers();
  
  ClassDef(ERRawToAnalyzeConverter,1)*/
};

#endif
