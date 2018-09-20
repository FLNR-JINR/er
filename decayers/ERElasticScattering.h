/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERElasticScattering_H
#define ERElasticScattering_H

#include <ERDecay.h> // mother class

// ROOT
#include <TString.h>

class TF1;
class TParticlePDG;

class ERElasticScattering : public ERDecay
{
public:
    /** @brief Constructor with reaction name
     ** @param name   reaction name
    **/
    ERElasticScattering(TString name);

    /** @brief Destructor **/
    ~ERElasticScattering();

    /** Modifiers **/

    /** @brief Defines target ion parameters.
     ** @param A
     ** @param Z
     ** @param Q
    **/
    void SetTargetIon(Int_t A, Int_t Z, Int_t Q);

    /** @brief Defines file name that contains the value of theta CDF.
     ** @param fileName File name contains CDF
    **/
    void SetThetaCDF(TString fileName) { fThetaFileName = fileName; }

    /** @brief Defines range of theta value.
     ** @param th1 theta minimum.
     ** @param th2 theta maximum.
    **/
    void SetThetaRange(Double_t th1, Double_t th2) { fTheta1 = th1; fTheta2 = th2; }

    /** @brief Defines range of phi value.
     ** @param phi1 phi minimum.
     ** @param phi2 phi maximum.
    **/
    void SetPhiRange(Double_t phi1, Double_t phi2) { fPhi1 = phi1; fPhi2 = phi2; }

    /** @brief Defines theta position for detector in Lab.
     ** @param angle theta position for detector in Lab.
    **/
    void SetDetAngle(Double_t angle)  { fDetPos = angle; }

    /** @brief Defines primary ion mass.
     ** @param mass Primary ion mass.
    **/
    void SetIonMass(Double_t mass)    { fIonMass = mass; }

    /** @brief Defines target ion mass.
     ** @param mass Target ion mass.
    **/
    void SetTargetIonMass(Double_t mass) { fTargetIonMass = mass; }

    /** @brief Returns primary ion mass. **/
    Double_t GetIonMass()          const { return fIonMass; }

    /** @brief Returns target ion mass. **/
    Double_t GetTargetIonMass()    const { return fTargetIonMass; }

    /** @brief Returns phi range value: fPh2-fPhi1. **/
    Double_t GetdPhi()             const { return (fPhi2 - fPhi1); }

    /** @brief Returns sum of CDF ranges values of primary and target ion. **/
    Double_t GetCDFRangesSum()     const { return fCDFRangesSum; }

    /** @brief Returns number of interactions on target. **/
    Int_t GetInteractNumInTarget() const { return fInteractNumInTarget; }

public:
    Bool_t Init();
    Bool_t Stepping();

private:
    TString         fThetaFileName;             ///< File name that contains theta CDF values
    TString         fTargetIonName;             ///< Target ion name

    TParticlePDG*   fTargetIonPDG;              ///< Target ion PDG

    TF1*            fThetaCDF;                  ///< Pointer to theta CDF function
    TF1*            fThetaInvCDF;               ///< Pointer to inversety theta CDF function

    Double_t        fTheta1;                    ///< Theta minimum for primary ion in CM [Deg]
    Double_t        fTheta2;                    ///< Theta maximum for primary ion in CM [Deg]
    Double_t        fCDFmin;                    ///< ThetaCDF(fTheta1) minimum
    Double_t        fCDFmax;                    ///< ThetaCDF(fTheta2) maximum
    Double_t        fPhi1;                      ///< Phi minimum for primary ion in CM [Deg]
    Double_t        fPhi2;                      ///< Phi maximum for primary ion in CM [Deg]
    Double_t        fThetaTargetIon1;           ///< Theta minimum for target ion in CM [Deg]
    Double_t        fThetaTargetIon2;           ///< Theta maximum for target ion in CM [Deg]
    Double_t        fCDFminTargetIon;           ///< ThetaCDF(fThetaTargetIon1) minimum
    Double_t        fCDFmaxTargetIon;           ///< ThetaCDF(fThetaTargetIon2) maximum
    Double_t        fDetPos;                    ///< Detector position in Lab [Deg]
    Double_t        fIonMass;                   ///< Primary ion mass
    Double_t        fTargetIonMass;             ///< Target ion mass
    Double_t        fCDFRangesSum;              ///< The CDF ranges sum of the primary ion and target ion

    Int_t           fInteractNumInTarget;       ///< Interactions counter in target

    Bool_t          ionMassTrueOrFalseTester;   ///< Identify a difference between the PDG and Monte-Carlo ion mass

    /** @brief The private method is to generate theta value. **/
    Double_t ThetaGen();

    /** @brief The private method is to calculate range for It's generate of the ion scattering angle.
     ** @param iM primary ion mass.
     ** @param tM target ion mass.
    **/
    void RangesCalculate(Double_t iM, Double_t tM);

    ClassDef(ERElasticScattering, 1);
};

#endif // ERElasticScattering_H
