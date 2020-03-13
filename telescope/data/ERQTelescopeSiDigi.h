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

#include "FairMultiLinkedData.h"

class ERQTelescopeSiDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERQTelescopeSiDigi();
  
  /** Constructor with arguments
   **/
  ERQTelescopeSiDigi(Int_t id, Double_t Edep, Double_t time, Int_t stationNb, Int_t stripNb);
                   
  /** Copy constructor **/
  ERQTelescopeSiDigi(const ERQTelescopeSiDigi&);

  /** Destructor **/
  virtual ~ERQTelescopeSiDigi();

  ERQTelescopeSiDigi& operator=(const ERQTelescopeSiDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Double_t GetEdep() const {return fEdep;}
  Double_t GetStripNb() const {return fStripNb;}
  Double_t GetTime() const {return fTime;}
  /* Modifiers */
  void SetTime(const Double_t time) { fTime = time; }
  void SetEdep (const Double_t edep) { fEdep = edep; }
 protected:
  Int_t     fID;
  Int_t     fStationNb;
  Int_t     fStripNb;
  Double_t  fTime;
  Double_t  fEdep;
  
  ClassDef(ERQTelescopeSiDigi,1)
};

#endif
