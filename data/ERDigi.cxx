// -------------------------------------------------------------------------
// -----                       ERDigi source file                -----
// -----           			   Created   by        			   -----
// -------------------------------------------------------------------------

#include "ERDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERDigi::ERDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERDigi::ERDigi(Int_t id, Float_t Edep, Double_t time,Int_t volNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fVolNb(volNb)
{
}

ERDigi::ERDigi(const ERDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fVolNb(right.fVolNb)
{
}

// -----   Destructor   ----------------------------------------------------
ERDigi::~ERDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERDigi)