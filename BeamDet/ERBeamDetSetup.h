// -------------------------------------------------------------------------

// -----                        ERBeamDetSetup header file              -----

// -----                              -----

// -------------------------------------------------------------------------
#ifndef ERBeamDetSETUP_H
#define ERBeamDetSETUP_H

using namespace std;

#include <map>
#include <vector>
#include "Rtypes.h"

struct ERBeamDetWire{
  Float_t fX;
  Float_t fY;
  Float_t fZ;
  ERBeamDetWire(Float_t x, Float_t y, Float_t z){fX = x; fY = y; fZ = z;}
};

class ERBeamDetSetup {
  static ERBeamDetSetup* fInstance;
  static Double_t        fTargetR;
  static Double_t        fDistanceBetweenMWPC;
  static Double_t        fDistanceBetweenTOF;
  static map<Int_t, map<Int_t, map<Int_t, ERBeamDetWire*>>> fWires;
  // ----- ToF parameters -----------------------------------------------------
  static Double_t fPlasticX;
  static Double_t fPlasticY;
  static Double_t fPlasticZ;
  // --------------------------------------------------------------------------
  // ----- MWPC parameters ----------------------------------------------------
  static Double_t fGasVolX;
  static Double_t fGasVolY;
  static Double_t fGasVolZ;

  static Double_t fGasX;
  static Double_t fGasY;
  static Double_t fGasZ; //cm

  static Double_t fDistBetweenXandY;

  static Double_t fAluminiumThickness;

  static Double_t fKaptonThickness;

  static Double_t fWireDiameter;
  // --------------------------------------------------------------------------
  // ------ fPosition of detector's parts relative to zero ---------------------
  static Double_t fPositionToF1;
  static Double_t fPositionToF2;

  static Double_t fPositionMWPC1;
  static Double_t fPositionMWPC2;
  // -------- Target parameters -----------------------------------------------
  static Double_t fTargetH2R;   //cm
  static Double_t fTargetH2Z;   //cm

  static Double_t fTargetShellThickness;
  ERBeamDetSetup();
public:
  static ERBeamDetSetup* Instance();
  static Double_t WireX(Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  static Double_t WireY(Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  static Double_t WireZ(Int_t mwpcNb, Int_t planeNb, Int_t wireNb);

  static Double_t DistanceBetweenMWPC() {return fDistanceBetweenMWPC;}
  static Double_t DistanceBetweenTOF() {return fDistanceBetweenTOF;}
  static Double_t TargetR() {return fTargetR;}

  static void ConstructGeometry();

  static Int_t    SetParContainers();
  ClassDef(ERBeamDetSetup,1)

};
#endif

