/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERBeamDetMWPCDigi_H
#define ERBeamDetMWPCDigi_H

#include "ERDigi.h"

class ERBeamDetMWPCDigi : public ERDigi {
 public:
  ERBeamDetMWPCDigi() = default;
  ERBeamDetMWPCDigi(Double_t Edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  virtual void Print(const Option_t* opt = 0) const;
  /* Accessors */
  Int_t   MWPCNb()  const { return fMWPCNb;}
  Int_t   PlaneNb() const { return fPlaneNb;}
 protected:
  Int_t		  fMWPCNb;
  Int_t     fPlaneNb;
  
  ClassDef(ERBeamDetMWPCDigi,1)
};

#endif
