
#ifndef ERRunAna_H
#define ERRunAna_H

#include "FairRunAna.h"

class ERRunAna : public FairRunAna
{

  public:
    virtual ~ERRunAna(){}
    ERRunAna();
    void Init();
    
    ClassDef(ERRunAna ,1)

};

#endif
