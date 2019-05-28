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
  virtual ~ERElasticScattering();

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

  /** @brief Defines primary ion mass.
  ** @param mass Primary ion mass.
  **/
  void SetIonMass(Double_t mass) { fIonMass = mass; }

  /** @brief Defines target ion mass.
  ** @param mass Target ion mass.
  **/
  void SetTargetIonMass(Double_t mass) { fTargetIonMass = mass; }

  /** @brief Defines theta position for detector slot center in Lab.
  ** @param ThetaCenter theta position for detector center in Lab.
  ** @param dTheta half of the theta range of the detector slit.
  **/
  void SetDetectorsSlot(Double_t thetaCenter, Double_t dTheta);

  /*Accessors*/

  /** @brief Returns primary ion mass. **/
  Double_t GetIonMass() const { return fIonMass; }

  /** @brief Returns target ion mass. **/
  Double_t GetTargetIonMass() const { return fTargetIonMass; }

  /** @brief Returns phi range value: fPh2-fPhi1. **/
  Double_t GetdPhi() const { return (fPhi2 - fPhi1); }

  /** @brief Returns sum of CDF ranges values of primary and target ion. **/
  Double_t GetCDFRangesSum() const { return fCDFRangesSum; }

  /** @brief Returns ThetaCM mean for primary Ion. **/
  Double_t GetThetaCMMeanPri() const { return fThetaCMSumPri/fNumOfPriIons; }

  /** @brief Returns ThetaCM mean for target Ion. **/
  Double_t GetThetaCMMeanTar() const { return fThetaCMSumTar/fNumOfTarIons; }

  /** @brief Returns number of interactions on target. **/
  Int_t GetInteractNumInTarget() const { return fInteractNumInTarget; }

public:
  Bool_t Init();
  Bool_t Stepping();

private:
  /** @brief The private method is to generate theta value. **/
  Double_t ThetaGen();

  /** @brief The private method is to calculate range for It's generate of the ion scattering angle.
  ** @param iM primary ion mass.
  ** @param tM target ion mass.
  **/
  void RangesCalculate(Double_t iM, Double_t tM);

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
  Double_t        fDetThetaCenter;            ///< Detector position in Lab [Deg]
  Double_t        fDetdTheta;                 ///< Detector Theta Width in Lab [Deg]
  Double_t        fIonMass;                   ///< Primary ion mass
  Double_t        fTargetIonMass;             ///< Target ion mass
  Double_t        fCDFRangesSum;              ///< The CDF ranges sum of the primary ion and target ion
  Double_t        fThetaCMSumPri;             ///< Sum of Thetas CM for primary Ion hit to the target
  Double_t        fThetaCMSumTar;             ///< Sum of Thetas CM for target Ion hit to the target
  Int_t           fInteractNumInTarget;       ///< Interactions counter in target
  Int_t           fNumOfPriIons;              ///< Interactions counter of primary ions in target
  Int_t           fNumOfTarIons;              ///< Interactions counter of target ions in target
  Bool_t          fIonTester;                 ///< kTRUE if target ion kFLASE if primary ionfIonTester
  Bool_t          fDetSlotIsSet;              ///< kTRUE if ERElasticScattering::SetDetectorsSlot  is called kFALSE by default

  ClassDef(ERElasticScattering, 1);
};

#endif // ERElasticScattering_H
