/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERDigi_H
#define ERDigi_H

#include "TObject.h"

#include "ERSupport.h"

class ERDigi : public TObject {
 public:
  ERDigi() = default;
  ERDigi(float edep, float time, ERChannel channel);
  /** Output to screen **/
  void Print() const;
  /* Accessors */
  float Edep() const {return fEdep;}
  float Time() const {return fTime;}
  ERChannel Channel() const {return fChannel;}
  /* Modifiers */
  void SetTime(const float time) {fTime = time;}
  void SetEdep (const float edep) {fEdep = edep;}
  void SetChannel (const int channel) {fChannel = channel;} 
 protected:
  ERChannel fChannel = consts::undefined_channel;
  float  fTime = -1.;
  float  fEdep = -1.;
  ClassDef(ERDigi,1)
};

#endif
