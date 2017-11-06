
#ifndef ERRunAna_H
#define ERRunAna_H

#include "FairRunAna.h"

class ERRunAna : public FairRunAna
{

  public:
    virtual ~ERRunAna(){}
    ERRunAna();
    void Init();
    void Calc();
    
    ClassDef(ERRunAna ,1)

};

#endif
