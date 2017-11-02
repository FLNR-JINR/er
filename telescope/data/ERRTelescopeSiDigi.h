// -------------------------------------------------------------------------
// -----                      ERRTelescopeSiDigi header file             -----
// -----                      Created   by               -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeSiDigi_H
#define ERRTelescopeSiDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"
#include "ERRTelescopeSiPoint.h"


class ERRTelescopeSiDigi : public FairMultiLinkedData
{
    public:
        /** Default constructor **/
        ERRTelescopeSiDigi();

        /** Constructor with arguments
        **/
        ERRTelescopeSiDigi(Int_t id, Int_t side, Int_t Nb, Int_t telescopeNb, Int_t detectorNb, Double_t time, Float_t edep);

        /** Copy constructor **/
        ERRTelescopeSiDigi(const ERRTelescopeSiDigi&);

        /** Destructor **/
        virtual ~ERRTelescopeSiDigi();

        ERRTelescopeSiDigi& operator=(const ERRTelescopeSiDigi&) { return *this; }

        /** Output to screen **/
        virtual void Print(const Option_t* opt = 0) const;
        Int_t AddEdep(ERRTelescopeSiPoint *Sipoint, Double_t time, Float_t edep);

        /* Accessors */
        Int_t ID () const {return fID;}
        Int_t Nb () const {return fNb;}
        Int_t TelescopeNb () const {return fTelescopeNb;}
        Int_t DetectorNb () const {return fDetectorNb;}
        Int_t Side () const {return fSide;}
        Float_t Edep() const {return fEdep;}


    protected:
        Int_t     fID;
        Int_t     fSide;      //  Side = 0 -- sensor ; Side = 1 -- sector ;
        Int_t     fNb;
        Int_t     fTelescopeNb;
        Int_t     fDetectorNb;
        Double_t  fTime;
        Float_t   fEdep;

        ClassDef(ERRTelescopeSiDigi,1)

};

#endif
