/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERBeamDetSETUP_H
#define ERBeamDetSETUP_H

#include <map>
#include <vector>

#include "TString.h"
#include <TXMLNode.h>
#include "Rtypes.h"
#include <TGeoNode.h>

using namespace std;

#include "ERBeamDetTrack.h"

#include "ERSupport.h"

struct ERBeamDetWire{
  float fGlobX;
  float fGlobY;
  float fGlobZ;
  ERBeamDetWire(float xGlob, float yGlob, float zGlob) :
    fGlobX(xGlob), fGlobY(yGlob), fGlobZ(zGlob) {}
};

class ERBeamDetSetup {
public:
  ERBeamDetSetup();
  ~ERBeamDetSetup();
  static ERBeamDetSetup* Instance();
  /* Accessors */
  static Double_t GetWireGlobX(const TString& digi_branch_name, ERChannel channel);
  static Double_t GetWireGlobY(const TString& digi_branch_name, ERChannel channel);
  static Double_t GetWireGlobZ(const TString& digi_branch_name, ERChannel channel);
  static Bool_t   CheckIfTargetIsSet(void) {return fSensitiveTargetIsSet;}
  static Double_t DistanceBetweenMWPC() {return fDistanceBetweenMWPC;}
  static Double_t TargetR() {return fTargetR;}
  static void     PrintDetectorParameters(void);
  static void     PrintDetectorParametersToFile(TString fileName);
  static TString  GetToFType(Int_t number);
  /// Returns mwpc and plane indexies by branch name
  static std::pair<unsigned short, unsigned short> GetMwpcAndPlaneNumbers(const TString& digi_branch_name);
  /* Modifiers */
  static void SetXmlParametersFile(TString xmlFileName) {fParamsXmlFileName = xmlFileName;}
  static void SetSensitiveTarget(void) {fSensitiveTargetIsSet = true;}
  static void AddMWPC(TString type, Double_t position); 
  static void AddToF(TString type, Double_t position);
  /** @brief Sets the inverse order of wires numbering in an X plane.
   ** The inverse order of numbering it is the order when wire number 
   ** increase while coordinate of wires decrease. **/
  static void SetMWPCnumberingInvOrderX();
  /** @brief Sets the inverse order of wires numbering in an Y plane.
   ** The inverse order of numbering it is the order when wire number 
   ** increase while coordinate of wires decrease. **/
  static void SetMWPCnumberingInvOrderY();
  static Int_t    GetToFCount()  {return fToFCount;}
  static Double_t GetToFThickness(Int_t tofInd)  {return fPlasticZ[tofInd - 1];}
  static void     GetGeoParamsFromParContainer();
  static Double_t GetDistanceBetweenToF(Int_t tof1Ind, Int_t tof2Ind);
  static void     ConstructGeometry();
  static Int_t    SetParContainers();
protected:
  /** @brief Returns translation \f$(X,Y,Z)\f$ of the certain node in a mother node frame
   ** @param node  node for which translation is obtained
   ** @param trans[3]  translation **/
  static void GetTransInMotherNode(TGeoNode const* node, Double_t trans[3]);
private:
  static void ParseXmlParameters();
  static void GetToFParameters(TXMLNode *node);
  static void GetMWPCParameters(TXMLNode *node);
  static Double_t GetWireGlobCoord(const TString& digi_branch_name, ERChannel channel,
                                   float ERBeamDetWire::* coord);
  static ERBeamDetSetup* fInstance;
  static Bool_t fIsGettingGeoPar;
  static Int_t fMWPCCount;
  static Int_t fToFCount;
  static Double_t fTargetR;
  static Double_t fDistanceBetweenMWPC;
  static Double_t fDistanceBetweenToF;
  static map<Int_t, map<Int_t, map<Int_t, ERBeamDetWire*>>> fWires;
  // ----- ToF parameters -----------------------------------------------------
  static vector<Double_t> fPlasticX;
  static vector<Double_t> fPlasticY;
  static vector<Double_t> fPlasticZ;
  static vector<TString>  fPlasticMedia;
  // ----- MWPC parameters ----------------------------------------------------
  static vector<Double_t> fGasVolX; 
  static vector<Double_t> fGasVolY;
  static vector<Double_t> fGasVolZ;
  static vector<Double_t> fGasStripX;
  static vector<Double_t> fGasStripY;
  static vector<Double_t> fGasStripZ; //cm
  static vector<Double_t> fGasPlaneXOffset;
  static vector<Double_t> fGasPlaneYOffset;
  static vector<Double_t> fDistBetweenXandY;
  static vector<Double_t> fCathodeThickness;
  static vector<Double_t> fKaptonWindowThickness;
  static vector<Double_t> fAnodeWireDiameter;
  static vector<TString>  fKaptonWindowMedia;
  static vector<TString>  fCathodeMedia;
  static vector<TString>  fAnodeWireMedia;
  static vector<TString>  fGasMedia;
  static vector<Bool_t>  fMWPCInvNumberingOrderX;
  static vector<Bool_t>  fMWPCInvNumberingOrderY;
  // ------ fPosition of detector's parts relative to zero ---------------------
  static vector<Double_t> fPositionToF;
  static vector<Double_t> fPositionMWPC;
  // -------- Target parameters -----------------------------------------------
  static Double_t fTargetH2R;   //cm
  static Double_t fTargetH2Z;   //cm
  static Double_t fTargetShellThicknessSide;
  static Double_t fTargetShellThicknessZ;
  static Bool_t   fSensitiveTargetIsSet;
  static TString fParamsXmlFileName;
  static vector<TString> fToFType;
  static vector<TString> fMWPCType;
  static Bool_t fGeoFromContainerIsRead;
  ClassDef(ERBeamDetSetup,1)
};
#endif

