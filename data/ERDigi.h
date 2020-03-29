/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERDigi_H
#define ERDigi_H

#include "FairMultiLinkedData.h"

class ERDigi : public FairMultiLinkedData
{
 public:
  ERDigi() = default;
  ERDigi(Double_t edep, Double_t time, Int_t channel);
  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;
  /* Accessors */
  Double_t Edep() const {return fEdep;}
  Double_t Time() const {return fTime;}
  Int_t Channel() const {return fChannel;}
  /* Modifiers */
  void SetTime(const Double_t time) {fTime = time;}
  void SetEdep (const Double_t edep) {fEdep = edep;}
  void SetChannel (const Int_t channel) {fChannel = channel;} 
 protected:
  Int_t     fChannel = -1;
  Double_t  fTime = -1.;
  Double_t  fEdep = -1.;
  
  ClassDef(ERDigi,1)
};

#endif