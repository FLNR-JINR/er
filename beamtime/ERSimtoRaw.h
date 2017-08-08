/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERSimtoRaw_H
#define ERSimtoRaw_H

#include "TArrayF.h"
#include "TClonesArray.h"

#include "ERNeuRadPixelSignal.h"
#include "ERRawEvent.h"

class TArrayF;
class TClonesArray;

class ERNeuRadPixelSignal;
class ERRawEvent;

/** @task ERSimtoRaw 
 ** task for converting data from output digitizing files to
 ** files with structure of class RawEvent
 ** @author  I.Muzalevsky <ivanmuzalevsky@gmail.com>
**/ 

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
  TClonesArray *fPixelSignals;

  //Output arrays
  ERRawEvent** fRawEvents;

  Int_t fNChanels;
  Int_t fNPoints;
  Int_t fEvent;

  //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
  Float_t* fResFunction;    //!
  //Хранит результирующий сигнал, как значения функции в узлах
  TArrayF fResFunctionRoot; 

  ClassDef(ERSimtoRaw,1);
};

#endif
