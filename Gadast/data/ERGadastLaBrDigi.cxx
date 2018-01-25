// -------------------------------------------------------------------------
// -----                       ERGadastLaBrDigi source file                -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERGadastLaBrDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERGadastLaBrDigi::ERGadastLaBrDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERGadastLaBrDigi::ERGadastLaBrDigi(Int_t id, Float_t Edep, Int_t cell)
  :fID(id),
  fEdep(Edep),
  fCell(cell)
{
}

ERGadastLaBrDigi::ERGadastLaBrDigi(const ERGadastLaBrDigi& right)
  :fID(right.fID),
   fEdep(right.fEdep),
   fCell(right.fCell)
{
}



// -----   Destructor   ----------------------------------------------------
ERGadastLaBrDigi::~ERGadastLaBrDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERGadastLaBrDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERGadastLaBrDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
  std::cout << "    Cell : " << fCell << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERGadastLaBrDigi)
