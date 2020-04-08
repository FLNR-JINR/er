/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** @class ERElasticScattering
** @brief Class for the elastic scattering simulate
** @author V.Schetinin <schetinin@jinr.ru>.
** @author I.Satyshev <satyshevi@gmail.com>.
** @version 1.0
**
** The ERElasticScattering defines the elastic scattering
** of the projectile in the target volume.
**/

#ifndef ERElasticScattering_H
#define ERElasticScattering_H

#include <ERDecay.h> // mother class

#include <TString.h>
#include <TLorentzVector.h>

enum ERInteractionParticipant{ kPROJECTILE, kTARGET, kEJECTILE}; // TODO It has to be in the header of the future class "Interaction.h"

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
  ** @param regIonSt defines which ion will be flying at the detector.
  **/
  void SetThetaRange(Double_t th1, Double_t th2, ERInteractionParticipant regIonSt);

  /** @brief Defines theta position for detector slot center in Lab.
  ** @param ThetaCenter is theta position for detector center in Lab.
  ** @param dTheta is a half of the theta range of the detector slit.
            User should choose the theta range as maximum as It is possible. 
            It means that the increase of chosen value dTheta does not lead to increase number of events registered by detector.
  ** @param relMod is mode for the relativistic case, by defulat is kFALSE
  **/
  void SetLabThetaRange(Double_t thetaCenter, Double_t dTheta, ERInteractionParticipant regIonSt, Bool_t relMod = kFALSE);

  /** @brief Defines range of phi value.
  ** @param phi1 phi minimum.
  ** @param phi2 phi maximum.
  **/
  void SetPhiRange(Double_t phi1, Double_t phi2) { fPhiMin = phi1; fPhiMax = phi2; }

  /** @brief Defines primary ion mass.
  ** @param mass Primary ion mass.
  **/
  void SetProjectileIonMass(Double_t mass) { fProjectileIonMass = mass; }

  /** @brief Defines target ion mass.
  ** @param mass Target ion mass.
  **/
  void SetTargetIonMass(Double_t mass) { fTargetIonMass = mass; }


  /** Accessors **/

  /** @brief Returns projectile ion mass. **/
  Double_t GetProjectileIonMass() const { return fProjectileIonMass; }

  /** @brief Returns target ion mass. **/
  Double_t GetTargetIonMass() const { return fTargetIonMass; }

  /** @brief Returns phi range value: fPh2-fPhiMin. **/
  Double_t GetdPhi() const { return (fPhiMax - fPhiMin); }

  /** @brief Returns ThetaCDF range value: fCDFmax-fCDFmin. **/
  Double_t GetdThetaCDF() const { return fCDFmax - fCDFmin; }

  /** @brief Returns number of interactions on target. **/
  Int_t GetInteractNumInTarget() const { return fInteractNumInTarget; }

  /** @brief Returns mean of thetaCM by all events in run [Deg]. **/
  Double_t GetThetaCMMean() const;

public:
  Bool_t Init();
  Bool_t Stepping();

private:
  /** @brief The private method is to convert Lab theta range to CM. **/
  void ThetaRangesLab2CM(Double_t pM, Double_t tM);

  /** @brief The private method is to convert Lab theta range to CM for relativistic case **/
  void ThetaRangesLab2CMRelativistic();

  /** @brief The private method is to read ThetaCDF cumulative function file. **/
  Bool_t ThetaCDFRead();

  /** @brief The private method is to generate theta value. **/
  Double_t ThetaGen();

  Bool_t DefineOfIonsMasses();

private:
  ERInteractionParticipant  fRegisterIonStatus;  ///< Ion status TODO

  TString         fThetaFileName;             ///< File name that contains theta CDF values
  TString         fTargetIonName;             ///< Target ion name

  TParticlePDG*   fTargetIonPDG;              ///< Target ion PDG

  TF1*            fThetaCDF;                  ///< Pointer to theta CDF function
  TF1*            fThetaInvCDF;               ///< Pointer to inversety theta CDF function

  Double_t        fThetaMin;                  ///< Theta minimum for primary ion in CM [Deg]
  Double_t        fThetaMax;                  ///< Theta maximum for primary ion in CM [Deg]
  Double_t        fThetaRangeCenter;          ///< Theta range's center
  Double_t        fThetaRangedTheta;          ///< The half-width of the range of theta
  Double_t        fPhiMin;                    ///< Phi minimum for primary ion in CM [Deg]
  Double_t        fPhiMax;                    ///< Phi maximum for primary ion in CM [Deg]
  Double_t        fCDFmin;                    ///< ThetaCDF(fThetaMin) minimum
  Double_t        fCDFmax;                    ///< ThetaCDF(fThetaMax) maximum
  Double_t        fProjectileIonMass;         ///< Projectile ion mass
  Double_t        fTargetIonMass;             ///< Target ion mass

  Bool_t          fThetaLabRangeIsSet;        ///< kTRUE if Lab theta range is set, kFALSE if It is not set
  Bool_t          fRelativisticMode;          ///< kTRUE of a relativistic case is simulated and kFALSE if It isn't

  /** Statistical attributes by all events. **/

  Double_t        fThetaCMSum;                ///< Sum of thetaCM by all events in run
  Int_t           fInteractNumInTarget;       ///< Interactions counter in target (by all events in run)

  ClassDef(ERElasticScattering, 1);
};

#endif // ERElasticScattering_H
