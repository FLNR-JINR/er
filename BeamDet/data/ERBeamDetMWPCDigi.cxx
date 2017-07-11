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
  fWireNb(wireNb)
{
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
