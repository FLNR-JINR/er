/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeSiDigi_H
#define ERQTelescopeSiDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "ERDigi.h"

class ERQTelescopeSiDigi : public ERDigi {
 public:
  ERQTelescopeSiDigi() = default;
  ERQTelescopeSiDigi(Double_t edep, Double_t time, Int_t channel);
  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;
 protected:
  ClassDef(ERQTelescopeSiDigi,1)
};

#endif
