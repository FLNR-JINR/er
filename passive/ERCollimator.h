/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERCollimator_H
#define ERCollimator_H

#include "FairModule.h"

class TGeoCombiTrans;

class ERCollimator : public FairModule
{
public:

  /** Default constructor **/
  ERCollimator();

  /** Standard constructor **/
  ERCollimator(const char* name,
               const char* title="Collimator",
               Bool_t Active=kFALSE);

  /** Destructor **/
  virtual ~ERCollimator();

  void SetModulePosition(Double_t x, Double_t y, Double_t z);

  /** Build the geometry **/
  virtual void ConstructGeometry();

private:

  TGeoCombiTrans* fPositionRotation;

  ClassDef(ERCollimator, 1);
};

#endif // ERCollimator_H
