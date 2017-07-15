// -------------------------------------------------------------------------
// -----                       ERRTelescopeDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERRTelescopeDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERRTelescopeDigi::ERRTelescopeDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERRTelescopeDigi::ERRTelescopeDigi(Int_t id, Float_t Edep, Double_t time, Int_t sensorNb, Int_t sectorNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fSensorNb(sensorNb),
  fSectorNb(sectorNb)
{
}

ERRTelescopeDigi::ERRTelescopeDigi(const ERRTelescopeDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fSensorNb(right.fSensorNb),
  fSectorNb(right.fSectorNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERRTelescopeDigi::~ERRTelescopeDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERRTelescopeDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERRTelescopeDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERRTelescopeDigi)