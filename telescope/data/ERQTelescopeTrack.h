/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERQTelescopeTrack_H
#define ERQTelescopeTrack_H

#include "TNamed.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

/** @class ERQTelescopeTrackFinder
 ** @brief 
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
**/

class ERQTelescopeTrack : public FairMultiLinkedData{

public:
  /** @brief Default constructor **/
  ERQTelescopeTrack();

  /** @brief Constructor 
   ** @param targetX - hit x-coordinate on target;
   ** @param targetY - hit y-coordinate on target;
   ** @param targetZ - hit z-coordinate on target;
   ** @param telescopeX - hit x-coordinate on QTelescope station;
   ** @param telescopeY - hit y-coordinate on QTelescope station;
   ** @param telescopeZ - hit z-coordinate on QTelescope station;
   ** @param sumEdep - summary enegy deposit in X and Y strips;
  **/
  ERQTelescopeTrack(Double_t targetX,    Double_t targetY,    Double_t targetZ, 
                    Double_t telescopeX, Double_t telescopeY, Double_t telescopeZ,
                    Double_t sumEdep);

  /* Accessors */
  TVector3 GetTargetVertex()    const {return TVector3(fTargetX,fTargetY,fTargetZ);}
  TVector3 GetTelescopeVertex() const {return TVector3(fTelescopeX,fTelescopeY,fTelescopeZ);}
  Double_t GetSumEdep()      const {return fSumEdep;}

private:
  Double_t  fTargetX;
  Double_t  fTargetY;
  Double_t  fTargetZ;                   // point coordinates on target
  Double_t  fTelescopeX;
  Double_t  fTelescopeY;
  Double_t  fTelescopeZ;

  Double_t  fSumEdep;


  ClassDef(ERQTelescopeTrack, 1)
};

#endif
