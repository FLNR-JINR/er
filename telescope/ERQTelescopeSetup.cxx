/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeSetup.h"

#include <ostream>

#include "TGeoTube.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TROOT.h"
#include <Riostream.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"

using namespace std;

ERQTelescopeSetup* ERQTelescopeSetup::fInstance = NULL;
map<TString, vector<ERQTelescopeStrip*>> ERQTelescopeSetup::fStrips;  // map<subassembly,map<component, vector<strip>>>            
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup::ERQTelescopeSetup(): ERSetup() {
  LOG(DEBUG) << "ERQTelescopeSetup initialized! "<< FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup::~ERQTelescopeSetup() {
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup* ERQTelescopeSetup::Instance(){
  if (fInstance == NULL) {
    fInstance = new ERQTelescopeSetup();
    return fInstance;
  }
  else
    return fInstance;
}

//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripX(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fX;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripY(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fY;
}
//--------------------------------------------------------------------------------------------------
Double_t ERQTelescopeSetup::GetStripZ(TString componentBranchName, Int_t stripNb){
  return fStrips[componentBranchName][stripNb]->fZ;
}
// // --------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::ReadGeoParamsFromParContainer() {
  if ( ! gGeoManager ) {
    std::cerr << "ERQTelescopeSetup: cannot initialise without TGeoManager!"<< std::endl;
  }
  gGeoManager->CdTop();

  TGeoNode* cave = gGeoManager->GetCurrentNode();
  TGeoNode* qtelescope  = NULL;
  TGeoNode* qtelescopeSubassembly = NULL;
  TGeoNode* qtelescopeComponent = NULL;
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) { // cycle by volumes in TOP
    TString moduleName = cave->GetDaughter(iNode)->GetName();
    if ( moduleName.Contains("QTelescope", TString::kIgnoreCase) ) {
      qtelescope = cave->GetDaughter(iNode);    
      for (Int_t iSubassembly = 0; iSubassembly < qtelescope->GetNdaughters(); iSubassembly++) { // cycle by subassemblies in QTelescope
        qtelescopeSubassembly = qtelescope->GetDaughter(iSubassembly);
        TString subassemblyName = qtelescopeSubassembly->GetName();
        for (Int_t iComponent = 0; iComponent < qtelescopeSubassembly->GetNdaughters(); iComponent++) { // cycle by components in subassembly
          qtelescopeComponent = qtelescopeSubassembly->GetDaughter(iComponent);
          TString qtelescopeComponentName = qtelescopeComponent->GetName();
          Double_t stripInComponentTrans[3];
          Double_t stripInSubAssemblyTrans[3];
          Double_t stripGlobTrans[3];
          if (qtelescopeComponentName.Contains("DoubleSi", TString::kIgnoreCase) ) {
            TGeoNode* doubleSiStrip;
            TString   firstStripArrayName = (qtelescopeComponentName.Contains("XY")) ? qtelescopeComponentName + "_X"
                                                                                     : qtelescopeComponentName + "_Y";
            TString   secondStripArrayName = (firstStripArrayName.EndsWith("X"))   ? qtelescopeComponentName + "_Y"
                                                                                   : qtelescopeComponentName + "_X";
            Bool_t    flagFirstStripReaded = kFALSE;
            Int_t     iDoubleSiStrip = 0;
            for (; iDoubleSiStrip < qtelescopeComponent->GetNdaughters(); iDoubleSiStrip++) {
              doubleSiStrip = qtelescopeComponent->GetDaughter(iDoubleSiStrip);
              stripInComponentTrans[0] = doubleSiStrip->GetMatrix()->GetTranslation()[0];
              stripInComponentTrans[1] = doubleSiStrip->GetMatrix()->GetTranslation()[1];
              stripInComponentTrans[2] = doubleSiStrip->GetMatrix()->GetTranslation()[2];
              qtelescopeComponent->LocalToMaster(stripInComponentTrans, stripInSubAssemblyTrans);
              qtelescopeSubassembly->LocalToMaster(stripInSubAssemblyTrans, stripGlobTrans);

              // fStrips[subassemblyName][firstStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans));
              fStrips[firstStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans));
              TGeoNode* doubleSiBox;
              Int_t iDoubleSiBox = 0;
              if (!flagFirstStripReaded) {
                for (; iDoubleSiBox < doubleSiStrip->GetNdaughters(); iDoubleSiBox++) {
                  Double_t siBoxLocalTrans[3];
                  doubleSiBox = doubleSiStrip->GetDaughter(iDoubleSiBox);
                  TString siBoxName = doubleSiBox->GetName();
                  siBoxLocalTrans[0] = doubleSiBox->GetMatrix()->GetTranslation()[0];
                  siBoxLocalTrans[1] = doubleSiBox->GetMatrix()->GetTranslation()[1];
                  siBoxLocalTrans[2] = doubleSiBox->GetMatrix()->GetTranslation()[2];

                  doubleSiStrip->LocalToMaster(siBoxLocalTrans, stripInComponentTrans);
                  (qtelescopeComponentName.Contains("XY")) ? stripInComponentTrans[0] = 0
                                                           : stripInComponentTrans[1] = 0;
                  qtelescopeComponent->LocalToMaster(stripInComponentTrans, stripInSubAssemblyTrans);
                  qtelescopeSubassembly->LocalToMaster(stripInSubAssemblyTrans, stripGlobTrans);
                  // fStrips[subassemblyName][secondStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans));
                  fStrips[secondStripArrayName].push_back(new ERQTelescopeStrip(stripGlobTrans));
                }
                flagFirstStripReaded = kTRUE;
              }
            }
          }
          if (qtelescopeComponentName.Contains("SingleSi", TString::kIgnoreCase) ) {
            TGeoNode* singleSiStrip;
            Int_t     iSingleSiStrip = 0;
            for (; iSingleSiStrip < qtelescopeComponent->GetNdaughters(); iSingleSiStrip++) {
              singleSiStrip = qtelescopeComponent->GetDaughter(iSingleSiStrip);

              stripInComponentTrans[0] = singleSiStrip->GetMatrix()->GetTranslation()[0];
              stripInComponentTrans[1] = singleSiStrip->GetMatrix()->GetTranslation()[1];
              stripInComponentTrans[2] = singleSiStrip->GetMatrix()->GetTranslation()[2];

              singleSiStrip->LocalToMaster(stripInComponentTrans, stripGlobTrans);
              // fStrips[subassemblyName][qtelescopeComponentName].push_back(new ERQTelescopeStrip(stripGlobTrans));            
              fStrips[qtelescopeComponentName].push_back(new ERQTelescopeStrip(stripGlobTrans));            
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeSetup)
