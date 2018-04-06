/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERGeoComponent_H
#define ERGeoComponent_H

#include "TString.h"
#include "TVector3.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "ERSetup.h"

class ERGeoComponent : public TNamed {
public:
  ERGeoComponent();
  ERGeoComponent(TString name);
  ERGeoComponent(TString name, TString typeFromXML, TVector3 position, TVector3 rotation);
  ~ERGeoComponent();
  /* Modifiers */
  
  /* Accessors */
  TGeoVolume*   GetVolume()   const {return fVolume;}
  TVector3*     GetPosition() const {return fPosition;}
  TGeoRotation* GetRotation() const {return fRotation;}
  TString       GetBranchName() {return (fType + this->GetName());}

public:
  virtual void ConstructGeometryVolume(void) = 0;

protected:
  virtual void ParseXmlParameters() = 0;
  
  TGeoVolume*   fVolume;
  TVector3*     fPosition;
  TGeoRotation* fRotation;
  TString       fType;


  ClassDef(ERGeoComponent,1)
};
#endif
