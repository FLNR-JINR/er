#ifndef ERTarget_H
#define ERTarget_H

#include "FairModule.h"
#include <TString.h>


class ERTarget : public FairModule {
public:
  ERTarget(const char * name, const char *Title="ER Target");
  ERTarget();
  virtual ~ERTarget();

  virtual void ConstructGeometry();
  
  virtual Bool_t CheckIfSensitive(std::string name);
  
protected:
  TString *fTargetName; //!
  
public:
  ClassDef(ERTarget,1)
};

#endif //ERTarget_H