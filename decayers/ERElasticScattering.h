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
  ** @param DetIonType is a type of the registration ion. 
            It defines which ion will be flying at the detector. kPROJECTILE by default.
  **/
  void SetThetaRange(Double_t th1, Double_t th2, ERInteractionParticipant DetIonType = kPROJECTILE);

  /** @brief Defines theta position for detector slot center in Lab.
  ** @param ThetaCenter is theta position for detector center in Lab.
  ** @param dTheta is a half of the theta range of the detector slit.
            User should choose the theta range as maximum as It is possible. 
            It means that the increasing of the chosen value dTheta does not lead increasing of registered events number.
     @param DetIonType is a type of the registration ion. 
            It defines which ion will be flying at the detector. kPROJECTILE by default. 
  ** @param relMod is the relativistic mode. KTRUE for the relativistic case and kFALSE for the non relativistic one. 
            kTRUE by default. 
  ** @param BeamAvE the avarage energy of the beam. It is zero by default. ATTENTION! For relativistic case this energy must be set correct!
  **/
  void SetLabThetaRange(Double_t thetaCenter, Double_t dTheta, ERInteractionParticipant DetIonType = kPROJECTILE, 
  	                    Bool_t relMod = kTRUE, Double_t BeamAvE = 0.);

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
  void ThetaRangesLab2CM();

  /** @brief The private method is to convert Lab theta range to CM for relativistic case **/
  void ThetaRangesLab2CMRelativistic();

  /** @brief The private method is to read ThetaCDF cumulative function file. **/
  Bool_t ThetaCDFRead();

  /** @brief The private method is to generate theta value. **/
  Double_t ThetaGen();

  Bool_t DefineOfIonsMasses();

private:
  ERInteractionParticipant  fDetectionIonType=kPROJECTILE;  ///< Ion status TODO

  TString         fThetaFileName="";           ///< File name that contains theta CDF values
  TString         fTargetIonName="";           ///< Target ion name

  TParticlePDG*   fTargetIonPDG=NULL;          ///< Target ion PDG

  TF1*            fThetaCDF=NULL;              ///< Pointer to theta CDF function. 
  TF1*            fThetaInvCDF=NULL;           ///< Pointer to inversety theta CDF function

  Double_t        fThetaMinCM=0.;              ///< Theta minimum for primary ion in CM [Deg]
  Double_t        fThetaMaxCM=180.;            ///< Theta maximum for primary ion in CM [Deg]
  Double_t        fThetaRangeCenter=0.;        ///< Theta range's center
  Double_t        fThetaRangedTheta=0.;        ///< The half-width of the range of theta
  Double_t        fPhiMin=0.;                  ///< Phi minimum for primary ion in CM [Deg]
  Double_t        fPhiMax=360.;                ///< Phi maximum for primary ion in CM [Deg]
  Double_t        fCDFmin=0.;                  ///< ThetaCDF(fThetaMinCM) minimum
  Double_t        fCDFmax=1.;                  ///< ThetaCDF(fThetaMaxCM) maximum
  Double_t        fProjectileIonMass=-1.;      ///< Projectile ion mass
  Double_t        fTargetIonMass=0.;           ///< Target ion mass
  Double_t        fBeamAverageEnergy=0.;       ///< Avarage Energy of the beam
  Bool_t          fRelativisticMode=kTRUE;     ///< kTRUE of a relativistic case is simulated and kFALSE if It isn't. kTRUE by default

  /** Statistical attributes for all events. **/

  Double_t        fThetaCMSum=0.;              ///< Sum of thetaCM by all events in run
  Int_t           fInteractNumInTarget=0;      ///< Interactions counter in target (by all events in run)
 
  ClassDef(ERElasticScattering, 1);
};

#endif // ERElasticScattering_H
