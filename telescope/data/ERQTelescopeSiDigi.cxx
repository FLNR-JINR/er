// -------------------------------------------------------------------------
// -----                       ERQTelescopeSiDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERQTelescopeSiDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERQTelescopeSiDigi::ERQTelescopeSiDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERQTelescopeSiDigi::ERQTelescopeSiDigi(Int_t id, Float_t Edep, Double_t time, Int_t stationNb, Int_t stripNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fStationNb(stationNb),
  fStripNb(stripNb)
{
}

ERQTelescopeSiDigi::ERQTelescopeSiDigi(const ERQTelescopeSiDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fStationNb(right.fStationNb),
  fStripNb(right.fStripNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERQTelescopeSiDigi::~ERQTelescopeSiDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERQTelescopeSiDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERQTelescopeSiDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERQTelescopeSiDigi)
