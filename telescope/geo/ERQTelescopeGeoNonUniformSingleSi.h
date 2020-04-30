/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoNonUniformSingleSi_H
#define ERQTelescopeGeoNonUniformSingleSi_H

#include "ERQTelescopeGeoComponentSingleSi.h"

#include "TString.h"
#include "TVector3.h"
#include "TH2D.h"
/** @class ERQTelescopeGeoNonUniformSingleSi
 ** @brief Class for the non-uniform single silicon station entity.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The class defines non-uniform single silicon station as pseudo-intersection
 ** between perpendicular stips between different silicon stations.
 ** It is constructed by inhomogeneity maps retrieved from the calibration procedure
 ** by means of (TODO: link to calibration classes description). 
 ** Strips of the single thin station are divided 
 ** into pixels depending on choosen second station strip plane. For example, if second 
 ** station is doubled then division into pixels may be different for first and second 
 ** double-Si strip planes due to different strips count on theese sides.
 ** 
 ** In this implementation there is no non-sensitive frame around the station.
 ** 
 ** For now, until clarification of the thin detector calibration, three maps are needed:
 ** * full pseudo-pixel map (/TODO: clarify what values are included)
 ** * map of the front dead layer for the single silicone station
 ** * map of the back dead layer for the single silicone station
 **
 ** The information about stations from the DB is needed for further advanced methods
 ** of the geometry creation when beam not parallel and relative position of
 ** stations influences on the spatial trajectories spreading.
**/
class ERQTelescopeGeoNonUniformSingleSi : public ERQTelescopeGeoComponentSingleSi {
public:
  ERQTelescopeGeoNonUniformSingleSi() = default;
  /** @brief Constructor.
   ** @param xmlTypeSingleSi single silicon station type from equipment database
   ** @param id  single silicon station identifier from equipment database
   ** @param thicknessMapFileName path to the detector thickness map
  **/
  ERQTelescopeGeoNonUniformSingleSi(const TString& xmlTypeSingleSi, const TString& id,
                                    const TVector3& position, const TVector3& rotation,
                                    const TString& orientAroundZ, const TString& thicknessMapFileName);
  /* Modifiers */
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);
private:
  TH2D* fThicknessMap = nullptr; // root file with full thickness of the pseudo-pixel including thickness of the dead layer in thick detector
  Int_t fXPseudoStripCount = 0;
  Int_t fYPseudoStripCount = 0;
  ClassDef(ERQTelescopeGeoNonUniformSingleSi,1)
};
#endif
