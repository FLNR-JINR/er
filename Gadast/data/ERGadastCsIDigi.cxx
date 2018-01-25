// -------------------------------------------------------------------------
// -----                       ERGadastCsIDigi source file                -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#include "ERGadastCsIDigi.h"
#include<iostream>

using namespace std;

// -----   Default constructor   -------------------------------------------
ERGadastCsIDigi::ERGadastCsIDigi()
  :fID(-1),
  fEdep(0.)
{
}

ERGadastCsIDigi::ERGadastCsIDigi(Int_t id, Float_t Edep,Int_t wall, Int_t block, Int_t cell)
  :fID(id),
  fEdep(Edep),
  fWall(wall),
  fBlock(block),
  fCell(cell)
{
}

ERGadastCsIDigi::ERGadastCsIDigi(const ERGadastCsIDigi& right)
  :fID(right.fID),
   fEdep(right.fEdep),
   fWall(right.fWall),
   fBlock(right.fBlock),
   fCell(right.fCell)
{
}



// -----   Destructor   ----------------------------------------------------
ERGadastCsIDigi::~ERGadastCsIDigi()
{
}

// -----   Public method Print   -------------------------------------------
void ERGadastCsIDigi::Print(const Option_t* opt /* = 0 */) const
{
  std::cout << "-I- ERGadastCsIDigi:  " << endl;
  std::cout << "    Edep : " << fEdep << endl;
  std::cout << "    Wall : " << fWall << endl;
  std::cout << "    Block : " << fBlock << endl;
  std::cout << "    Cell : " << fCell << endl;
}
// -------------------------------------------------------------------------

ClassImp(ERGadastCsIDigi)
