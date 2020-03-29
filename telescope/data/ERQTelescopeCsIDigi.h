/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERQTelescopeCsIDigi_H
#define ERQTelescopeCsIDigi_H

#include "ERDigi.h"

class ERQTelescopeCsIDigi : public ERDigi {
 public:
  ERQTelescopeCsIDigi() = default;
  ERQTelescopeCsIDigi(Double_t edep, Double_t time, Int_t wallNb, Int_t channel);
  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;
  /* Accessors */
  Int_t WallNb() const {return fWallNb;}
 protected:
  Int_t     fWallNb = -1;
 
  ClassDef(ERQTelescopeCsIDigi,1)
};

#endif
