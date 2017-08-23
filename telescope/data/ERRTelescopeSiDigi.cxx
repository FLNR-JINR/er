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

ERRTelescopeSiDigi::ERRTelescopeSiDigi(Int_t id, Int_t side, Int_t Nb, Int_t telescopeNb, Int_t detectorNb, Double_t time, Float_t edep)
    :fID(id),
     fSide(side),      //  Side = 0 -- sensor ; Side = 1 -- sector ;
     fNb(Nb),
     fTelescopeNb(telescopeNb),
     fDetectorNb(detectorNb),
     fTime(time),
     fEdep(edep)
{
}

ERRTelescopeSiDigi::ERRTelescopeSiDigi(const ERRTelescopeSiDigi& right)
    :fID(right.fID),
     fSide(right.fSide),      //  Side = 0 -- sensor ; Side = 1 -- sector ;
     fNb(right.fNb),
     fTelescopeNb(right.fTelescopeNb),
     fDetectorNb(right.fDetectorNb),
     fTime(right.fTime),
     fEdep(right.fEdep)
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
