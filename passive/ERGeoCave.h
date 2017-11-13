// -------------------------------------------------------------------------
// -----                       ERGeoCave header file                   -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------


#ifndef ERGEOCAVE_H
#define ERGEOCAVE_H

#include "FairGeoSet.h"
#include "TString.h"
#include "FairGeoMedia.h"
#include <fstream>
class  ERGeoCave : public FairGeoSet {
protected:
  TString name;
public:
  ERGeoCave();
  ~ERGeoCave() {}
  const char* getModuleName(Int_t) {return name.Data();}  
  Bool_t read(std::fstream&,FairGeoMedia*);
  void addRefNodes();
  void write(std::fstream&);
  void print();
  ClassDef(ERGeoCave,0) // Class for the geometry of CAVE
};

#endif  //ERGEOCAVE_H
