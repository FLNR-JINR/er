
/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERElossCalculatorUtils_H
#define ERElossCalculatorUtils_H

#include "TH1.h"
#include "TCut.h"
#include "TString.h"
#include "TGeant4.h"

#include "G4EmCalculator.hh"
#include "G4NistManager.hh"
#include "G4Proton.hh"
#include "G4IonTable.hh"

#include "FairRunAna.h"

class ERElossCalculatorUtils : public FairRunAna
{

public:
  static ERElossCalculatorUtils* Instance();
  virtual ~ERElossCalculatorUtils();

  float BackAndForthPropagation(const unsigned pid, const float energy, 
                                const TString& material, const float thickness, 
                                const float number_of_steps);

  float IntegralEloss (const unsigned pid, const float kinetic_energy /*[MeV]*/, 
                       const TString& material, const float thickness /*[um]*/, 
                       const float number_of_steps, const bool if_step_back);

  void Init();
private:
  ERElossCalculatorUtils();
  
  inline bool AreSame(const float a, const float b, const float eps = 1e-6) {
    return fabs(a - b) < eps;
  }

  static ERElossCalculatorUtils* fInstance;
  std::shared_ptr<TGeant4> fGeant4 = nullptr;
  ClassDef(ERElossCalculatorUtils ,1)
};

#endif
