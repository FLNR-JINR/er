/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERBeamDetPID_H
#define ERBeamDetPID_H

#include "TClonesArray.h"
#include "TParticlePDG.h"

#include "FairTask.h"
#include "FairIon.h"

#include "ERBeamDetTOFDigi.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetParticle.h"
#include "ERBeamDetSetup.h"

/** @class ERBeamDetPID
 ** @brief Class for particle identification.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERBeamDetPID identifies particle via selection ions from the limited
 ** area assigned by the time of flight between ToF plastics \f$\Delta T\f$ and summary  
 ** energy deposit in ToF \f$\Delta E\f$. Then it calculates the Four-vector \f$(p_x, p_y, p_z, E)\f$ 
 ** and initilise the projectile object determined by class ERBeamDetParticle.\n
 ** In the first version of ERBeamDetPID class user should set mass of ion [MeV] for identification. 
**/

class ERBeamDetPID : public FairTask {

public:
  /** @brief Default constructor **/
  ERBeamDetPID();

  /** @brief Constructor 
   ** @param verbosity level
  **/
  ERBeamDetPID(Int_t verbose);

  /** @brief Destructor **/
  ~ERBeamDetPID();

  /** @brief Defines all input and output object colletions participate
   ** in particle identification.
  **/
  virtual InitStatus Init();

  /** @brief Defines selection and calculetion of parameters for each event:
   ** Four-vector \f$(p_x, p_y, p_z, E)\f$
   ** \nEnergy is determined by equations: \f$E = \gamma*M\f$, \f$M\f$ - ion mass [MeV].
   ** \nMomentum \f$p = \gamma*\beta*M\f$
   ** \n Gamma factor \f$\gamma = \frac{1}{\sqrt{1-\beta^2}}\f$
   ** \n Beta \f$\beta = \frac{L}{\Delta/C}\f$, \f$L\f$ - distance betweet plastics in ToF, 
   ** \f$\Delta T\f$ - time of flight between ToF plastics, \f$C\f$ - light speed.   
  **/
  virtual void Exec(Option_t* opt);

  /** @brief Resets all output data. **/
  virtual void Reset();

  /** @brief Defines actions in the end of identification. **/
  virtual void Finish();
  
  /** Modifiers **/

  /** @brief Sets ion PDG. 
   ** @param pdg   ion pdg.
  **/
  void SetPID(Int_t pdg) {fPID = pdg;}

  /** @brief Sets selection box area on the ToF/dE scatter plot. 
   ** @param tof1  time value low boundary [ns]
   ** @param tof2  time value high boundary [ns] .
   ** @param dE1   summary energy deposit low boundary [GeV]
   ** @param dE2   summary energy deposit high boundary [GeV]
  **/
  void SetBoxPID(Double_t tof1, Double_t tof2, Double_t dE1, Double_t dE2) {
    fToF1 = tof1;
    fToF2 = tof2;
    fdE1  = dE1;
    fdE2  = dE2;
  }

  /** @brief Sets calibration parameter for ToF selection. 
   ** @param offsetToF  calibration parameter [ns].
  **/
  void SetOffsetToF(Double_t offsetToF) {fOffsetToF = offsetToF;}
  
  /** @brief Sets ion mass.
   ** @param mass  ion mass [MeV].
  **/
  void SetIonMass(Double_t mass) {fIonMass = mass;}

  /** @brief Sets probability threshold.
   ** @param probabilityThreshold  probability threshold.
  **/
  void SetProbabilityThreshold(Double_t probabilityThreshold) {fProbabilityThreshold = probabilityThreshold;}

  /** @brief Sets ion mass number.
   ** @param a - nucleon number.
  **/
  void SetIonMassNumber(Int_t a);

  /** @brief Sets ion mass.
   ** @param mass - ion mass.
  **/
  void SetIonMass(Float_t mass){fIonMass = mass;}

protected:
  //Paramaters
  ERBeamDetSetup *fBeamDetSetup;        ///< access to ERBeamDetSetup class instance
  
  //Input arrays
  TClonesArray   *fBeamDetToFDigi1;     ///< input collection of ToF first plastic points
  TClonesArray   *fBeamDetToFDigi2;     ///< input collection of ToF second plastic points
  TClonesArray   *fBeamDetTrack;        ///< input collection of tracks

  Int_t          fPID;                  ///< ion PDG
  Double_t       fToF1, fToF2;          ///< ToF selection boundaries
  Double_t       fdE1, fdE2;            ///< ToF summary energy deposit boundaries
  Double_t       fOffsetToF;            ///< ToF calibration parameter
  Double_t       fIonMass;              ///< ion mass
  Double_t       fProbabilityThreshold; ///< probability threshold
  TParticlePDG   *fIon;
  Double_t       fA, fZ, fQ;
  TString        fIonName;
  //Output arrays
  TClonesArray   *fProjectile;           ///< output projectile collection

private:
  /** @brief Adds a ERBeamDetParticle to the output Collection **/
  ERBeamDetParticle* AddParticle(Int_t pid, TLorentzVector tofState, TLorentzVector targetState, Double_t probability);
  
  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();

  ClassDef(ERBeamDetPID,1)
};
#endif
