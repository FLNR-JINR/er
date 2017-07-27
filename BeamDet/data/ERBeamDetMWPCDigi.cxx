// -------------------------------------------------------------------------
// -----                       ERBeamDetMWPCDigi source file                -----
// -----           			   Created   by       			   -----
// -------------------------------------------------------------------------

#include "ERBeamDetMWPCDigi.h"
#include <iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERBeamDetMWPCDigi::ERBeamDetMWPCDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERBeamDetMWPCDigi::ERBeamDetMWPCDigi(Int_t id, Float_t Edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb)
  :fID(id),
  fEdep(Edep),
  fTime(time), 
  fMWPCNb(mwpcNb), 
  fPlaneNb(planeNb), 
  fWireNb(wireNb), 
  fWireX1(-1),
  fWireX2(-1),
  fWireY1(-1),
  fWireY2(-1)
{
  if(fMWPCNb == 1) {
    if(fPlaneNb == 1) {
      fWireX1 = fWireNb;
    } else {
      fWireY1 = fWireNb;
    }
  }
  if(fMWPCNb == 2) {
    if(fPlaneNb == 1) {
      fWireX2 = fWireNb;
    } else {
      fWireY2 = fWireNb;
    }
  }
}

ERBeamDetMWPCDigi::ERBeamDetMWPCDigi(const ERBeamDetMWPCDigi& right)
  :fID(right.fID),
   fEdep(right.fEdep),
   fTime(right.fTime), 
   fMWPCNb(right.fMWPCNb), 
   fPlaneNb(right.fPlaneNb), 
   fWireNb(right.fWireNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERBeamDetMWPCDigi::~ERBeamDetMWPCDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERBeamDetMWPCDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERBeamDetMWPCDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERBeamDetMWPCDigi)
