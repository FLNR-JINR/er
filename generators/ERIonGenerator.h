/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                   ERIonGenerator header file                 -----
// -----          Created 09/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------

/**
 * 24.10.07  implement constructor with ion name. M. Al-Turany
*/

#ifndef ERIonGenerator_H
#define ERIonGenerator_H

#include "FairGenerator.h"              // for FairGenerator

#include "Rtypes.h"                     // for Double_t, Int_t, etc
#include "TMath.h"

class FairIon;
class FairPrimaryGenerator;

class ERIonGenerator : public FairGenerator
{

  public:

    /** Default constructor **/
    ERIonGenerator();


    /** Constructor with ion name
      ** For the generation of ions with pre-defined FairIon
     ** By default, the  excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param ionName  Ion name
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonGenerator(const Char_t* ionName, Int_t mult);


    /** Default constructor
     ** For the generation of ions with atomic number z and mass number a.
     ** By default, the mass equals a times the proton mass and the
     ** excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param z         Atomic number
     **@param a         Atomic mass
     **@param q         Electric charge [e]
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult);


    /** Destructor **/
    virtual ~ERIonGenerator();


    /** Modifiers **/
    void SetCharge(Int_t charge) {fQ=charge;}
    void SetExcitationEnergy(Double_t eExc);
    void SetMass(Double_t mass);
     /** Modifiers **/
    void SetPDGType      (Int_t pdg)  {fPDGType = pdg;  };

    void SetMultiplicity (Int_t mult) {fMult    = mult; };

    void SetPRange(Double32_t pmin=0   , Double32_t pmax=10)
    {fPMin=pmin; fPMax=pmax; fPRangeIsSet=kTRUE;}

    void SetPtRange      (Double32_t ptmin=0   , Double32_t ptmax=10)
    {fPtMin=ptmin; fPtMax=ptmax; fPtRangeIsSet=kTRUE;};

    void SetPhiRange     (Double32_t phimin=0  , Double32_t phimax=360)
    {fPhiMin=phimin; fPhiMax=phimax;};

    void SetEtaRange     (Double32_t etamin=-5 , Double32_t etamax=7)
    {fEtaMin=etamin; fEtaMax=etamax; fEtaRangeIsSet=kTRUE;};

    void SetYRange       (Double32_t ymin=-5   , Double32_t ymax=7)
    {fYMin=ymin; fYMax=ymax; fYRangeIsSet=kTRUE;};

    void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

    void SetCosTheta   ()
    {fCosThetaIsSet=kTRUE;};

    void SetXYZ   (Double32_t x=0, Double32_t y=0, Double32_t z=0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
    }

    void SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z=0) {
      fX1=x1;
      fY1=y1;
      fX2=x2;
      fY2=y2;
      fZ=z;
      fBoxVtxIsSet=kTRUE;
    }

    void SetSigmaXYZ(Double32_t x=0, Double32_t y=0, Double32_t z=0,
                    Double32_t sigmaX=1, Double32_t sigmaY=1)
    {
      fGausX = x;
      fGausY = y;
      fSigmaX = sigmaX;
      fSigmaY = sigmaY;
      fZ = z;
      fBoxSigmaIsSet = kTRUE;
    }

    void SetKinE(Double32_t kinE) 
    { fPz = TMath::Sqrt(kinE*kinE + 2.*kinE*fPDGMass);}

    void SetKinERange(Double32_t kinEMin, Double32_t kinEMax)
    { 
      fPMin = TMath::Sqrt(kinEMin*kinEMin + 2.*kinEMin*fPDGMass);
      fPMax = TMath::Sqrt(kinEMax*kinEMax + 2.*kinEMax*fPDGMass);
      fPRangeIsSet=kTRUE;
    }

    void SetKinSigma(Double32_t kinE, Double32_t sigmaKinE)
    { fGausKinE = kinE; fSigmaKinE = sigmaKinE; fSigmaKinEIsSet=kTRUE; } 

    void SetPSigma(Double32_t p=0 , Double32_t sigma = 1)
    {fGausP=p; fSigmaP=sigma; fSigmaPIsSet=kTRUE;}

    void SetDebug(Bool_t debug=0) {fDebug = debug;}


    /** Method ReadEvent
     ** Generates <mult> of the specified ions and hands hem to the
     ** FairPrimaryGenerator.
     **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

    FairIon* Ion() {return fIon;}

  protected:
    void spreadingParameters(void);

    Int_t      fMult;                // Multiplicity per event
    FairIon*   fIon;                 // Pointer to the FairIon to be generated
    Int_t      fQ;                   // Electric charge [e]

    TString    fName;
    Int_t      fPDGType;             // Particle type (PDG encoding)

    Double32_t fPDGMass;             // Particle mass [GeV]
    Double32_t fPtMin,  fPtMax;      // Transverse momentum range [GeV]
    Double32_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    Double32_t fEtaMin, fEtaMax;     // Pseudorapidity range in lab system
    Double32_t fYMin,   fYMax;       // Rapidity range in lab system
    Double32_t fPMin,   fPMax;       // Momentum range in lab system
    Double32_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
    Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double32_t fX1, fY1, fX2, fY2;   // Box vertex coords (x1,y1)->(x2,y2)
    Double32_t fPx, fPy, fPz;
    Double32_t fGausX, fGausY;
    Double32_t fSigmaX, fSigmaY;  
    Double32_t fGausKinE; 
    Double32_t fSigmaKinE;
    Double32_t fGausP;   
    Double32_t fSigmaP;  

    Bool_t     fEtaRangeIsSet;       // True if eta range is set
    Bool_t     fYRangeIsSet;         // True if rapidity range is set
    Bool_t     fThetaRangeIsSet;     // True if theta range is set
    Bool_t     fCosThetaIsSet;       // True if uniform distribution in
    //cos(theta) is set (default -> not set)
    Bool_t     fPtRangeIsSet;        // True if transverse momentum range is set
    Bool_t     fPRangeIsSet;         // True if abs.momentum range is set
    Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
    Bool_t     fBoxSigmaIsSet;
    Bool_t     fSigmaKinEIsSet;
    Bool_t     fSigmaPIsSet;
    Bool_t     fDebug;               // Debug switch

 private:
    ERIonGenerator(const ERIonGenerator&);
    ERIonGenerator& operator=(const ERIonGenerator&);

    ClassDef(ERIonGenerator,1)
};


#endif

