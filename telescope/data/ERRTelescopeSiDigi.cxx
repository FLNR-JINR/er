// -------------------------------------------------------------------------
// -----                       ERRTelescopeSiDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERRTelescopeSiDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERRTelescopeSiDigi::ERRTelescopeSiDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERRTelescopeSiDigi::ERRTelescopeSiDigi(Int_t id, Float_t Edep, Double_t time, Int_t sectorNb, Int_t sensorNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fSensorNb(sensorNb),
  fSectorNb(sectorNb)
{
}

ERRTelescopeSiDigi::ERRTelescopeSiDigi(const ERRTelescopeSiDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fSensorNb(right.fSensorNb),
  fSectorNb(right.fSectorNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERRTelescopeSiDigi::~ERRTelescopeSiDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERRTelescopeSiDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERRTelescopeSiDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERRTelescopeSiDigi)
