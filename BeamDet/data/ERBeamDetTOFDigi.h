/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERBeamDetTOFDigi_H
#define ERBeamDetTOFDigi_H

#include "ERDigi.h"

class ERBeamDetTOFDigi : public ERDigi {
 public:
  ERBeamDetTOFDigi() = default;
  ERBeamDetTOFDigi(Double_t edep, Double_t time, Int_t tofNb);
  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;
  /* Accessors */
  Int_t    ToFNb() const {return fToFNb;}
 protected:
  Int_t     fToFNb;
  
  ClassDef(ERBeamDetTOFDigi,1)

};

#endif
