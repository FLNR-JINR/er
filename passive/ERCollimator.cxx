/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERCollimator.h"

#include "TString.h"
#include "TGeoMatrix.h"

// -----   Default constructor   ------------------------------------------------
ERCollimator::ERCollimator()
  : FairModule("COLLIMATOR", "Collimator", kFALSE) // explicitly specify collimator to be passive (kFALSE and not Active here)
{
  fPositionRotation = new TGeoCombiTrans("mCollimator", 0., 0., 0.,
                      new TGeoRotation("rotNoRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Standard constructor   -----------------------------------------------
ERCollimator::ERCollimator(const char* name, const char* title, Bool_t Active)
  : FairModule(name, title, kFALSE) // explicitly specify collimator to be passive (kFALSE and not Active here)
{
  fPositionRotation = new TGeoCombiTrans("mCollimator", 0., 0., 0.,
                      new TGeoRotation("rotNoRot", 0., 0., 0.));
}
// ------------------------------------------------------------------------------

// -----   Destructor   ---------------------------------------------------------
ERCollimator::~ERCollimator()
{
//  if (fPositionRotation) {
//    delete fPositionRotation;
//    fPositionRotation = NULL;
//  }
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void ERCollimator::SetModulePosition(Double_t x, Double_t y, Double_t z)
{
  fPositionRotation->SetDx(x);
  fPositionRotation->SetDy(y);
  fPositionRotation->SetDz(z);
}
// ------------------------------------------------------------------------------

// -----   Public method ConstructGeometry   ------------------------------------
void ERCollimator::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if (fileName == "") {
    LOG(FATAL) << "Collimator geometry file name is not set." << FairLogger::endl;
  } else if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing collimator geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    //TODO Uncomment when the corresponding method is implemented in FairRoot
    ConstructRootGeometry(/*(TGeoMatrix*)(fPositionRotation)*/);
  } else if(fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing collimator geometry from GDML file " << fileName.Data() << FairLogger::endl;
    ConstructGDMLGeometry(fPositionRotation);
  } else {
    LOG(FATAL) << "Collimator geometry file name is not correct." << FairLogger::endl;
  }
}
// ------------------------------------------------------------------------------

ClassImp(ERCollimator)
