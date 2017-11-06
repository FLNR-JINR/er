/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERIonGenerator_H
#define ERIonGenerator_H

#include "Rtypes.h"
#include "TMath.h"

#include "FairGenerator.h"

class FairIon;
class FairPrimaryGenerator;

/** @class ERIonGenerator
 ** @brief Class for the generation ion.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERIonGenerator defines the parameters of ion that participates in simulation.
 ** It provides interfaces for determining different types of dimentional, angular and 
 ** energy distributions of beam start position.
**/
class ERIonGenerator : public FairGenerator
{

  public:

    /** @brief Default constructor **/
    ERIonGenerator();

    /** @brief Constructor with ion name
     ** For the generation of ions with pre-defined FairIon
     ** By default, the  excitation energy is zero. This can be changed with the
     ** respective modifiers.
     ** @param ionName   Ion name
     ** @param mult      Number of ions per event
    **/
    ERIonGenerator(const Char_t* ionName, Int_t mult);

    /** @brief Default constructor
     ** For the generation of ions with atomic number z and mass number a.
     ** By default, the mass equals a times the proton mass and the
     ** excitation energy is zero. This can be changed with the
     ** respective modifiers.
     ** @param z         Atomic number
     ** @param a         Atomic mass
     ** @param q         Electric charge [e]
     ** @param mult      Number of ions per event
    **/
    ERIonGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult);

    /** @brief Destructor **/
    virtual ~ERIonGenerator();

    /** Modifiers **/

    /** @brief Defines ion charge.
     ** @param charge  Electric charge [e]
    **/
    void SetCharge(Int_t charge) {fQ=charge;}

    /** @brief Defines ion excitation energy of generated FairIon object.
     ** @param eExc  excitation energy [GeV] 
    **/
    void SetExcitationEnergy(Double_t eExc);

    /** @brief Defines mass of generated FairIon object.
     ** @param mass  mass [GeV].If mass is not given, it will be set to a times the proton mass.
    **/
    void SetMass(Double_t mass);

    void SetPDGType      (Int_t pdg)  {fPDGType = pdg;  };

    /** @brief Defines number of ions generated per event.
     ** @param mult      Number of ions per event
    **/
    void SetMultiplicity (Int_t mult) {fMult    = mult; };

    /** @brief Defines uniform distribution boundaries of ion momentum[GeV].
     ** @param pmin    minimun ion momentum [GeV]. By default is zero.
     ** @param pmin    maximum ion momentum [GeV]. By default is 10 [GeV].
    **/
    void SetPRange(Double32_t pmin = 0   , Double32_t pmax = 10)
    {fPMin=pmin; fPMax=pmax; fPRangeIsSet=kTRUE;}

    /** @brief Defines uniform distribution boundaries of ion transverse momentum[GeV].
     ** @param ptmin    minimun ion transverse momentum [GeV]. By default is zero.
     ** @param ptmax    maximum ion transverse momentum [GeV]. By default is 10 [GeV].
    **/
    void SetPtRange      (Double32_t ptmin = 0   , Double32_t ptmax = 10)
    {fPtMin=ptmin; fPtMax=ptmax; fPtRangeIsSet=kTRUE;};

    /** @brief Defines uniform distribution boundaries of ion azimuth angle[degree].
     ** @param phimin    minimun azimuth angle [degree]. By default is zero.
     ** @param phimax    maximum azimuth angle [degree]. By default is 360 degrees. 
    **/
    void SetPhiRange     (Double32_t phimin = 0  , Double32_t phimax = 360)
    {fPhiMin=phimin; fPhiMax=phimax;};

    /** @brief Defines uniform distribution boundaries of ion pseudorapidity in lab system.
     ** @param etamin    minimun pseudorapidity. By default is -5.
     ** @param etamax    maximum pseudorapidity. By default is 7. 
    **/
    void SetEtaRange     (Double32_t etamin = -5 , Double32_t etamax = 7)
    {fEtaMin=etamin; fEtaMax=etamax; fEtaRangeIsSet=kTRUE;};

    /** @brief Defines uniform distribution boundaries of ion rapidity in lab system.
     ** @param ymin    minimun rapidity. By default is -5.
     ** @param ymax    maximum rapidity. By default is 7. 
    **/
    void SetYRange       (Double32_t ymin = -5   , Double32_t ymax = 7)
    {fYMin=ymin; fYMax=ymax; fYRangeIsSet=kTRUE;};

    /** @brief Defines uniform distribution boundaries of ion polar angle in lab system [degree].
     ** @param thetamin    minimun polar angle. By default is 0 degrees.
     ** @param thetamax    maximum polar angle. By default is 90 degrees. 
    **/
    void SetThetaRange   (Double32_t thetamin = 0, Double32_t thetamax = 90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

    /** @brief Defines Gaussian distribution of ion polar angle in lab system [degree].
     ** @param theta       polar angle. By default is 0 degrees.
     ** @param sigmaTheta  standard deviation of polar angle. By default is 1. 
    **/
    void SetThetaSigma(Double32_t theta = 0, Double32_t sigmaTheta = 1) 
    {
      fGausTheta = theta;
      fSigmaTheta = sigmaTheta;
      fSigmaThetaIsSet = kTRUE;
    }

    /** @brief Defines kTRUE flag .
     ** @param theta       polar angle. By default is 0 degrees.
     ** @param sigmaTheta  standard deviation of polar angle. By default is 1. 
    **/
    void SetCosTheta   ()
    {fCosThetaIsSet=kTRUE;};

    /** @brief Defines fixed start position of ion beam in Cartesian coordinates 
     ** relative to the global cave сoordinate system .
     ** @param x  x-coordinate [cm]. By default is 0.
     ** @param y  y-coordinate [cm]. By default is 0.
     ** @param z  z-coordinate [cm]. By default is 0.
    **/
    void SetXYZ   (Double32_t x = 0, Double32_t y = 0, Double32_t z = 0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
    }

    /** @brief Defines uniform distribution boundaries of ion start position on X and Y axis 
     ** and fixed poition on Z axie in Cartesian coordinates relative to the
     ** global cave сoordinate system .
     ** @param x1  minimum x-coordinate [cm]. By default is 0.
     ** @param y1  maximum y-coordinate [cm]. By default is 0.
     ** @param x1  minimum x-coordinate [cm]. By default is 0.
     ** @param y1  maximum y-coordinate [cm]. By default is 0.
     ** @param z   z-coordinate [cm]. By default is 0.
    **/
    void SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z=0) {
      fX1=x1;
      fY1=y1;
      fX2=x2;
      fY2=y2;
      fZ=z;
      fBoxVtxIsSet=kTRUE;
    }

    /** @brief Defines Gaussian distribution of ion start position on X and Y axis 
     ** and fixed poition on Z axie in Cartesian coordinates relative to the
     ** global cave сoordinate system.
     ** @param x       x-coordinate [cm]. By default is 0.
     ** @param y       y-coordinate [cm]. By default is 0.
     ** @param z       z-coordinate [cm]. By default is 0.
     ** @param sigmaX  standard deviation of x-coordinate [cm]. By default is 1.
     ** @param sigmaY  standard deviation of y-coordinate [cm]. By default is 1.
    **/
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

    /** @brief Defines uniform distribution boundaries of ion kinetic energy [GeV].
     ** @param kinEMin  minimun ion kinetic energy [GeV].
     ** @param kinEMax  maximum ion kinetic energy [GeV].
    **/
    void SetKinERange(Double32_t kinEMin, Double32_t kinEMax);

    /** @brief Defines Gaussian distribution of ion kinetic energy [GeV].
     ** @param kinE       kinetic energy [GeV].
     ** @param sigmaKinE  standard deviation of kinetic energy. 
    **/
    void SetKinESigma(Double32_t kinE, Double32_t sigmaKinE); 

    /** @name Group of methods that are used collaborative.
     ** Method SetKinE() defines ion kinetic energy that is recalculated 
     ** to the momentum. Momentum is normally distributed according to 
     ** standart deviation that is defined by SetPSigmaOverP() method.
    **/
    ///@{
    /** @brief Defines fixed ion kinetic energy that is recalculated to the momentum
     ** according to equation \f[ P=\sqrt{T^2+2*T*m*c^2}\f], 
     ** where  \f$T\f$ - kinetic energy.
     ** @param kinE  kinetic energy [GeV].
    **/
    void SetKinE(Double32_t kinE);

    /** @brief Defines standard deviation of ion momentum as a percentage of
     ** full momentum.
     ** @param sigmaPOverP  percentage of ion momentum.
    **/
    void SetPSigmaOverP(Double32_t sigmaPOverP) { fSigmaP = sigmaPOverP*fGausP; fSigmaPIsSet = kTRUE;}
    ///@}

    /** @brief Defines Gaussian distribution of ion momentum [GeV].
     ** @param p       momentum [GeV].
     ** @param sigma   standard deviation of momentum. 
    **/
    void SetPSigma(Double32_t p = 0 , Double32_t sigma = 1)
    {fGausP=p; fSigmaP=sigma; fSigmaPIsSet=kTRUE;}

    void SetDebug(Bool_t debug=0) {fDebug = debug;}

    /** @brief Sets spreading of x and y coordinates on target (where z-position is zero)   
     ** and reconstruct them to the beam start position (to settes z-coordinate) 
     ** along momentum vector. 
     ** By default spreading of x and y coordinates implements on setted z-coordinate. 
    **/
    void SpreadingOnTarget(void) {fSpreadingOnTarget = kTRUE;}

    /** Accessors **/

    /** @brief Returns FairIon object
    **/
    FairIon* Ion() {return fIon;}

  public:
    /** Method ReadEvent
     ** Generates <mult> of the specified ions and hands hem to the
     ** FairPrimaryGenerator.
     **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);


  protected:
    /** @brief Spreads parameters recieved by accessor methods.
    **/
    void SpreadingParameters(void);

  protected:
    Int_t      fMult;                ///< Multiplicity per event
    FairIon*   fIon;                 ///< Pointer to the FairIon to be generated
    Int_t      fQ;                   ///< Electric charge [e]

    TString    fName;                ///< Ion name
    Int_t      fPDGType;             ///< Particle type (PDG encoding)

    Double32_t fPDGMass;             ///< Particle mass [GeV]
    Double32_t fPtMin,  fPtMax;      ///< Transverse momentum range [GeV]
    Double32_t fPhiMin, fPhiMax;     ///< Azimuth angle range [degree]
    Double32_t fEtaMin, fEtaMax;     ///< Pseudorapidity range in lab system
    Double32_t fYMin,   fYMax;       ///< Rapidity range in lab system
    Double32_t fPMin,   fPMax;       ///< Momentum range in lab system
    Double32_t fThetaMin, fThetaMax; ///< Polar angle range in lab system [degree]
    Double32_t fX, fY, fZ;           ///< Point vertex coordinates [cm]
    Double32_t fX1, fY1, fX2, fY2;   ///< Box vertex coords (x1,y1)->(x2,y2)
    Double32_t fPx, fPy, fPz;        ///< Momentum projection [GeV]
    Double32_t fGausX, fGausY;       ///< Amplitude values of coordinates in Gauss distibution [cm]
    Double32_t fSigmaX, fSigmaY;     ///< Coordinates normal deviation [cm]
    Double32_t fGausP;               ///< Amplitude value of momentum in Gauss distibution [GeV]
    Double32_t fSigmaP;              ///< Momentum normal deviation [GeV]
    Double32_t fGausTheta;           ///< Amplitude value of theta angle in Gauss distibution [degree]
    Double32_t fSigmaTheta;          ///< Theta angle normal deviation [degree]
    Double32_t fKinE;                ///< Kinetic energy [GeV]

    Bool_t     fEtaRangeIsSet;       ///< True if eta range is set
    Bool_t     fYRangeIsSet;         ///< True if rapidity range is set
    Bool_t     fThetaRangeIsSet;     ///< True if theta range is set
    Bool_t     fCosThetaIsSet;       ///< True if uniform distribution in cos(theta) is set (default -> not set)
    Bool_t     fPtRangeIsSet;        ///< True if transverse momentum range is set
    Bool_t     fPRangeIsSet;         ///< True if abs momentum range is set
    Bool_t     fPointVtxIsSet;       ///< True if point vertex is set
    Bool_t     fBoxVtxIsSet;         ///< True if box vertex is set
    Bool_t     fBoxSigmaIsSet;       ///< True if Gauss distribution for coordinates is set
    Bool_t     fSigmaPIsSet;         ///< True if Gauss distribution for momentum is set
    Bool_t     fDebug;               ///< Debug switch
    Bool_t     fSpreadingOnTarget;   ///< True if parameters are spreaded on target and reconstructed to beam start position.
    Bool_t     fSigmaThetaIsSet;     ///< True if Gauss distribution for theta angle is set

 private:
    ERIonGenerator(const ERIonGenerator&);
    ERIonGenerator& operator=(const ERIonGenerator&);

    ClassDef(ERIonGenerator,1)
};


#endif

