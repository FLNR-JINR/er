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

#include "FairTask.h"

#include "ERBeamDetMWPCPoint.h"
#include "ERBeamDetTOFPoint.h"
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTOFDigi.h"


class ERBeamDetDigitizer : public FairTask {

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
 ** Time in ToFs is determined by time of first point in event.
**/
public:
  /** @brief Default constructor. **/
  ERBeamDetDigitizer();

  /** @brief Constructor. 
   ** @param verbose: 1 - only standard log print, 2 - print digi information. 
  **/
  ERBeamDetDigitizer(Int_t verbose);

  /** @brief Destructor **/
  virtual ~ERBeamDetDigitizer();

  /** Modifiers **/

  /** @brief Sets energy threshold for ToF plastics. 
   ** @param treshold   energy thershold [GeV]
  **/
  void SetToFElossThreshold(Float_t treshold){fToFElossThreshold = treshold;}

  /** @brief Sets energy threshold for MWPC wires. 
   ** @param treshold   energy thershold [GeV]
  **/
  void SetMWPCElossThreshold(Float_t treshold){fMWPCElossThreshold = treshold;}

  /** @brief Sets standart deviation for energy losses in ToF plastics.
   ** @param sigma   standart deviation [GeV]
  **/
  void SetToFElossSigma(Float_t sigma) {fElossSigmaToF = sigma;}

  /** @brief Sets standart deviation for energy losses in MWPC wires. 
   ** @param sigma   standart deviation [GeV]
  **/
  void SetMWPCElossSigma(Float_t sigma) {fElossSigmaMWPC = sigma;}

  /** @brief Sets standart deviation for time values in ToF plastics. 
   ** @param sigma   standart deviation [ns]
  **/
  void SetToFTimeSigma(Float_t sigma) {fTimeSigmaToF = sigma;}

  /** @brief Sets standart deviation for time values in MWPC wires. 
   ** @param sigma   standart deviation [ns]
  **/
  void SetMWPCTimeSigma(Float_t sigma) {fTimeSigmaMWPC = sigma;}


  /** @brief Sets standart deviation for time values in MWPC wires.
   ** Sigma for each summary energy losses is defined according to equation 
   ** \f[ \frac{\sigma_{E}}{E}= \frac{a}{\sqrt{E/GeV}}\f]
   ** , where \f$E\f$ - summary energy losses in plastic, \f$a\f$ -parameter.
   ** @param a   parameter of spreading.
  **/
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

  /** @brief Defines actions in the end of digitization. **/
  virtual void Finish();

protected:
  /** @brief Adds a ERBeamDetMWPCDigi to the output Collections **/
  ERBeamDetMWPCDigi* AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  
  /** @brief Adds a ERBeamDetTOFDigi to the output Collections **/
  ERBeamDetTOFDigi*  AddToFDigi(Float_t edep, Double_t time, Int_t tofNb);

protected:
  //Input arrays
  TClonesArray *fBeamDetToFPoints;  ///< input collection of ToF points
  TClonesArray *fBeamDetMWPCPoints; ///< input collection of MWPC points
  //Output arrays
  TClonesArray *fBeamDetToFDigi1;   ///< output collection of Digi in first plastic
  TClonesArray *fBeamDetToFDigi2;   ///< output collection of Digi in second plastic

  TClonesArray *fBeamDetMWPCDigiX1; ///< output collection of Digi in first gas strip array in first MWPC station
  TClonesArray *fBeamDetMWPCDigiX2; ///< output collection of Digi in first gas strip array in second MWPC station
  TClonesArray *fBeamDetMWPCDigiY1; ///< output collection of Digi in second gas strip array in first MWPC station
  TClonesArray *fBeamDetMWPCDigiY2; ///< output collection of Digi in second gas strip array in second MWPC station
  
  Float_t fElossSigmaOverElossToF;  ///< parameter of spreading energy losses
  Float_t fElossSigmaToF;           ///< standart deviation of energy losses in ToF      
  Float_t fTimeSigmaToF;            ///< standart deviation of time in ToF
  Float_t fElossSigmaMWPC;          ///< standart deviation of energy losses in MWPC
  Float_t fTimeSigmaMWPC;           ///< standart deviation of time in MWPC
  Float_t fToFElossThreshold;       ///< energy losses thershold in ToF
  Float_t fMWPCElossThreshold;      ///< energy losses thershold in MWPC

  Bool_t  fSigmaEOverEToFIsSet;     ///< true if SetToFElossSigmaOverEloss is set

private:
  /** @brief Initializes runtime database for getting parameters from .par file**/
  virtual void SetParContainers();
  
  ClassDef(ERBeamDetDigitizer,1)
};

#endif
