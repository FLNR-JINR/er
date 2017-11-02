// -------------------------------------------------------------------------
// -----                       ERRTelescopeCsIDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERRTelescopeCsIDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERRTelescopeCsIDigi::ERRTelescopeCsIDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERRTelescopeCsIDigi::ERRTelescopeCsIDigi(Int_t id, Int_t telescopeNb, Float_t Edep, Double_t time, Int_t crystall)
  :fID(id),
  fTelescopeNb(telescopeNb),
  fEdep(Edep),
  fTime(time),
  fCrystallNb(crystall)
{
}

ERRTelescopeCsIDigi::ERRTelescopeCsIDigi(const ERRTelescopeCsIDigi& right)
  :fID(right.fID),
  fTelescopeNb(right.fTelescopeNb),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fCrystallNb(right.fCrystallNb)
{
}



// -----   Destructor   ----------------------------------------------------
ERRTelescopeCsIDigi::~ERRTelescopeCsIDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERRTelescopeCsIDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERRTelescopeCsIDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERRTelescopeCsIDigi)
