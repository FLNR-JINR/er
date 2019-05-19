/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoNonUniformSingleSi_H
#define ERQTelescopeGeoNonUniformSingleSi_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"
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
class ERQTelescopeGeoNonUniformSingleSi : public ERGeoComponent {
public:
  ERQTelescopeGeoNonUniformSingleSi();

  /** @brief Constructor.
   ** @param xmlTypeSingleSi  single silicon station type from equipment database
   ** @param id1  single silicon station identificator from equipment database
   ** @param xmlTypeSecondSt  second station type from the equipment database
   ** @param id2  second station identificator from equipment database
   ** @param plane2  X or Y strip plane for.
   ** @param pathFullMap  path to the full thin detector thicknes map
   ** @param pathFrontDeadMap  path to the thin detector front dead layer thicknes map
   ** @param pathBackDeadMap  path to the thin detector back dead layer thicknes map
  **/
  ERQTelescopeGeoNonUniformSingleSi(TString xmlTypeSingleSi, TString id1,
                                    TString xmlTypeSecondSt, TString id2, TString plane2,
                                    TString pathFullMap,
                                    TString pathFrontDeadMap,
                                    TString pathBackDeadMap); 

  /** @brief Default destructor.
  **/
  ~ERQTelescopeGeoNonUniformSingleSi();
  /* Modifiers */
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);

private:
  TString  fOrientAroundZ;
  TString  fComponentId1;
  TString  fComponentId2;
  // Double_t fSizeX; // equals to first (thin) detector size
  // Double_t fSizeY; // equals to second (thick) detector size
  // Double_t fSizeZ; // equals to max pixel thickness in strip 
  Double_t fSensX; // value of the strip X-length equals to first (thin) detector size 
  Double_t fSensY; // value of the strip Y-length equals to second (thick) detector size
  Double_t fSensZ; // value of the strip Z-length equals to max pixel thickness in strip
  Int_t    fStripPlaneCnt_1; // strips count in the first plane of the pseudo-intersection 
  Int_t    fStripPlaneCnt_2; // strips count in the second plane of the pseudo-intersection
  TString  fPlane2; // strips plane from the second detector used in the non-uniformity map building
  TString  fMedia; // name of the material from media.geo file
  TString  fPathFullMap; // full thickness of the pseudo-pixel including thickness of the dead layer in thick detector
  TString  fPathFrontDeadMap; // estimation of the dead layer in pixel front side
  TString  fPathBackDeadMap; // estimation of the dead layer in pixel back side
  void ParseXmlParameters();

  ClassDef(ERQTelescopeGeoNonUniformSingleSi,1)
};
#endif
