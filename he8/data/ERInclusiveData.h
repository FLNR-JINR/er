// -------------------------------------------------------------------------
// -----                      ERRawEvent header file                   -----
// -----                  Created 05/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERInclusiveData_H
#define ERInclusiveData_H

#include "TNamed.h"

class ERInclusiveData : public TNamed
{
public:
  ERInclusiveData();
  ~ERInclusiveData();
  void Reset();
  double tmis11,tmis12,tmis13,tmis21,tmis22,tmis23,tmis31,tmis32,tmis33;
  double thmis11,thmis12,thmis13,thmis21,thmis22,thmis23,thmis31,thmis32,thmis33;
  double phimis11,phimis12,phimis13,phimis21,phimis22,phimis23,phimis31,phimis32,phimis33;
  double exmis11,exmis12,exmis13,exmis21,exmis22,exmis23,exmis31,exmis32,exmis33;
  double tmis11cm0,tmis12cm0,tmis13cm0,tmis21cm0,tmis22cm0,tmis23cm0,tmis31cm0,tmis32cm0,tmis33cm0;
  double thmis11cm0,thmis12cm0,thmis13cm0,thmis21cm0,thmis22cm0,thmis23cm0,thmis31cm0,thmis32cm0,thmis33cm0;
  double tmis11cmp,tmis12cmp,tmis13cmp,tmis21cmp,tmis22cmp,tmis23cmp,tmis31cmp,tmis32cmp,tmis33cmp;
  double thmis11cmp,thmis12cmp,thmis13cmp,thmis21cmp,thmis22cmp,thmis23cmp,thmis31cmp,thmis32cmp,thmis33cmp;
  ClassDef(ERInclusiveData, 1)
};

#endif
