// -------------------------------------------------------------------------
// -----                       ERBeamDetToFDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERBeamDetToFDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERBeamDetToFDigi::ERBeamDetToFDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERBeamDetToFDigi::ERBeamDetToFDigi(Int_t id, Float_t Edep, Double_t time, Int_t tofNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fToFNb(tofNb)
{
}

ERBeamDetToFDigi::ERBeamDetToFDigi(const ERBeamDetToFDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fToFNb(right.fToFNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERBeamDetToFDigi::~ERBeamDetToFDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERBeamDetToFDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERBeamDetToFDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERBeamDetToFDigi)
