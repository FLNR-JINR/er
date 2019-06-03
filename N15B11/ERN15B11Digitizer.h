/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/** @class ERN15B11Digitizer
** @brief Class for the detectors response simulate.
** @author I.Satyshev <satyshevi@gmail.com>.
** @version 1.0
**/

#ifndef ERN15B11Digitizer_H
#define ERN15B11Digitizer_H

#include <map>

#include "FairTask.h" // mother class

class TClonesArray;

class ERN15B11Digitizer : public FairTask
{
public:
  /** @brief Default constructor **/
  ERN15B11Digitizer();

  /** @brief Destructor **/
  virtual ~ERN15B11Digitizer();

  virtual InitStatus Init();
  
  virtual void Exec(Option_t* option = "");

  virtual void FinishEvent();

protected:

  // Input
  std::map<TString,TClonesArray*> fPoints;

  // Output
  std::map<TString,TClonesArray*> fDigis;

private:
  /** @brief Write data to output file
  ** @param clref output branch adress for current detector
  ** @param Edep energy loss for current detector
  ** @param Time current time interval
  **/
  void AddOutputDigi(TClonesArray& clref, Double_t Edep = 0., Double_t Time = 0.);

  ClassDef(ERN15B11Digitizer, 1);
};

#endif // ERN15B11Digitizer_H
