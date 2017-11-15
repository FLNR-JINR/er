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

#include "FairTask.h"

#include "../NeuRad/data/ERNeuRadPhotoElectron.h"
#include "data/cls_RootEvent.h"

class TArrayF;
class TClonesArray;

class FairTask;

class ERNeuRadPhotoElectron;
class cls_RootEvent;

/** @task ERSimtoWPT
 ** @brief task for converting data from output digitizing files to files with structure of class RootEvent
 ** @author  I.Muzalevsky <ivanmuzalevsky@gmail.com>
**/ 

class ERSimtoWPT : public FairTask {

public:
  /** Default constructor **/
  ERSimtoWPT();

  /** Destructor **/
  ~ERSimtoWPT();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

protected:
  //Input objects
  //TClonesArray *fPhotoElectrons;

  //Output arrays
 // ERNeuRadRawEvent** fRawEvents;

  //Int_t fNChanels;
  //Int_t fNPoints;
  //Int_t fEvent;

  //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
  //Float_t* fResFunction;    //!
  //Хранит результирующий сигнал, как значения функции в узлах
  //TArrayF fResFunctionRoot; 

  ClassDef(ERSimtoWPT,1);
};

#endif
