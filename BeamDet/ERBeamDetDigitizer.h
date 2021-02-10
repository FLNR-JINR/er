/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERBeamDetDigitizer_H
#define ERBeamDetDigitizer_H

#include "TClonesArray.h"

#include "ERDigitizer.h"
#include "ERBeamDetMWPCPoint.h"
#include "ERBeamDetTOFPoint.h"
#include "ERDigi.h"
#include "ERSupport.h"

/** @class ERBeamDetDigitizer
 ** @brief Class for convertion simulation data to format like real detector data.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** ERBeamDetDigitizer is intended for converting VMC simulation data (Point classes)
 ** to format which recieved from the real detector (Digi classes). 
 ** Parameters of simulation are summarized for each sensor with considering 
 ** different effects which cause inaccuracies in output signals. If output signal
 ** less than predetermined thersholds by modifiers methods then it is not written 
 ** to output branches.
 ** Time in ToFs is determined by time of first point in event. **/
class ERBeamDetDigitizer : public ERDigitizer {
public:
  /** @brief Default constructor. **/
  ERBeamDetDigitizer();
  /** @brief Constructor. 
   ** @param verbose: 1 - only standard log print, 2 - print digi information. **/
  ERBeamDetDigitizer(Int_t verbose);
  /** @brief Destructor **/
  virtual ~ERBeamDetDigitizer() = default;
  /** Modifiers **/
  /** @brief Sets energy threshold for ToF plastics. 
   ** @param treshold   energy thershold [MeV] **/
  void SetToFElossThreshold(Float_t treshold){fToFElossThreshold = treshold;}
  /** @brief Sets energy threshold for MWPC wires. 
   ** @param treshold   energy thershold [MeV] **/
  void SetMWPCElossThreshold(Float_t treshold){fMWPCElossThreshold = treshold;}
  /** @brief Sets standart deviation for energy losses in ToF plastics.
   ** @param sigma   standart deviation [MeV] **/
  void SetToFElossSigma(Float_t sigma) {fElossSigmaToF = sigma;}
  /** @brief Sets standart deviation for energy losses in MWPC wires. 
   ** @param sigma   standart deviation [MeV] **/
  void SetMWPCElossSigma(Float_t sigma) {fElossSigmaMWPC = sigma;}
  /** @brief Sets standart deviation for time values in ToF plastics. 
   ** @param sigma   standart deviation [ns] **/
  void SetToFTimeSigma(Float_t sigma) {fTimeSigmaToF = sigma;}
  /** @brief Sets standart deviation for time values in MWPC wires. 
   ** @param sigma   standart deviation [ns] **/
  void SetMWPCTimeSigma(Float_t sigma) {fTimeSigmaMWPC = sigma;}
  /** @brief Sets standart deviation for time values in MWPC wires.
   ** Sigma for each summary energy losses is defined according to equation 
   ** \f[ \frac{\sigma_{E}}{E}= \frac{a}{\sqrt{E/MeV}}\f]
   ** , where \f$E\f$ - summary energy losses in plastic, \f$a\f$ -parameter.
   ** @param a   parameter of spreading. **/
  void SetToFElossSigmaOverEloss(Float_t a) {
    fElossSigmaOverElossToF = a; 
    fSigmaEOverEToFIsSet = kTRUE;
  }
public:
  /** @brief Defines all input and output object colletions participates
   ** in digitization.
  **/
  virtual InitStatus Init();
  /** @brief Defines the transformation actions for all input data (Point) to 
   ** output data (Digi) for each event. 
  **/
  virtual void Exec(Option_t* opt);
  /** @brief Resets all output data. **/
  virtual void Reset();
protected:
  /** @brief Adds a digi to the output Collections **/
  ERDigi* AddMWPCDigi(float edep, float time, int mwpcNb, int planeNb, ERChannel wireNb);
  /** @brief Adds a digi to the output Collections **/
  ERDigi*  AddToFDigi(float edep, float time, int tofNb);
protected:
  //Input arrays
  TClonesArray* fBeamDetToFPoints = nullptr;  ///< input collection of ToF points
  TClonesArray* fBeamDetMWPCPoints = nullptr; ///< input collection of MWPC points
  //Output arrays
  TClonesArray* fBeamDetToFDigi1 = nullptr; ///< output collection of Digi in first plastic
  TClonesArray* fBeamDetToFDigi2 = nullptr; ///< output collection of Digi in second plastic
  /// output collection of Digi in first gas strip array in first MWPC station
  TClonesArray* fBeamDetMWPCDigiX1 = nullptr;
  /// output collection of Digi in first gas strip array in second MWPC station
  TClonesArray* fBeamDetMWPCDigiX2 = nullptr;
  /// output collection of Digi in second gas strip array in first MWPC station
  TClonesArray* fBeamDetMWPCDigiY1 = nullptr;
  /// output collection of Digi in second gas strip array in second MWPC station
  TClonesArray* fBeamDetMWPCDigiY2 = nullptr; 
  Float_t fElossSigmaOverElossToF = 0.; ///< parameter of spreading energy losses
  Float_t fElossSigmaToF = 0.; ///< standart deviation of energy losses in ToF      
  Float_t fTimeSigmaToF = 0.; ///< standart deviation of time in ToF
  Float_t fElossSigmaMWPC = 0.; ///< standart deviation of energy losses in MWPC
  Float_t fTimeSigmaMWPC = 0.; ///< standart deviation of time in MWPC
  Float_t fToFElossThreshold = 0.; ///< energy losses thershold in ToF
  Float_t fMWPCElossThreshold = 0.; ///< energy losses thershold in MWPC
  Bool_t  fSigmaEOverEToFIsSet = kFALSE; ///< true if SetToFElossSigmaOverEloss is set
private:
  ClassDef(ERBeamDetDigitizer,1)
};

#endif
