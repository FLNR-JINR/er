// -------------------------------------------------------------------------
// -----                       ERGadastDigi source file                -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERGadastDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERGadastDigi::ERGadastDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERGadastDigi::ERGadastDigi(Int_t id, Float_t Edep)
  :fID(id),
  fEdep(Edep)
{
}

ERGadastDigi::ERGadastDigi(const ERGadastDigi& right)
  :fID(right.fID),
   fEdep(right.fEdep)
{
}



// -----   Destructor   ----------------------------------------------------
ERGadastDigi::~ERGadastDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERGadastDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERGadastDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERGadastDigi)
