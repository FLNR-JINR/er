/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTelescopeGeoComponentPassive.h"

//--------------------------------------------------------------------------------
ERTelescopeGeoComponentPassive::ERTelescopeGeoComponentPassive(
    const TString& fileName, const TString& topVolumeName, 
    const TVector3& position, const TVector3& rotation)
    : ERGeoComponent("None", fileName, position, rotation), fFileName(fileName), 
      fTopVolumeName(topVolumeName) {
  fGeoFile = new TFile(fileName, "READ");
  if (!fGeoFile->IsOpen())
    LOG(FATAL) << "Can not open file: " << fileName << FairLogger::endl;
}
//--------------------------------------------------------------------------------
void ERTelescopeGeoComponentPassive::ConstructGeometryVolume() {
  fVolume = static_cast<TGeoVolume*>(fGeoFile->Get(fTopVolumeName));
  if (!fVolume)
    LOG(FATAL) << "Volume " << fTopVolumeName << " not found in file " << fFileName 
               << FairLogger::endl;
}
//--------------------------------------------------------------------------------
ClassImp(ERTelescopeGeoComponentPassive)