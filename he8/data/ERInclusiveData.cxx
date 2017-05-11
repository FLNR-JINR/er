#include "ERInclusiveData.h"

  ERInclusiveData::ERInclusiveData(){
  	Reset();
  }

  ERInclusiveData::~ERInclusiveData(){

  }

void ERInclusiveData::Reset(){
  tmis11 = -1000.;tmis12 = -1000.;tmis13 = -1000.;
  tmis21 = -1000.;tmis22 = -1000.;tmis23 = -1000.;
  tmis31 = -1000.;tmis32 = -1000.;tmis33 = -1000.;
  
  thmis11 = -1000.;thmis12 = -1000.;thmis13 = -1000.;
  thmis21 = -1000.;thmis22 = -1000.;thmis23 = -1000.;
  thmis31 = -1000.;thmis32 = -1000.;thmis33 = -1000.;
  
  phimis11 = -1000.;phimis12 = -1000.;phimis13 = -1000.;
  phimis21 = -1000.;phimis22 = -1000.;phimis23 = -1000.;
  phimis31 = -1000.;phimis32 = -1000.;phimis33 = -1000.;

  exmis11 = -1000.;exmis12 = -1000.;exmis13 = -1000.;
  exmis21 = -1000.;exmis22 = -1000.;exmis23 = -1000.;
  exmis31 = -1000.;exmis32 = -1000.;exmis33 = -1000.;

  tmis11cm0 = -1000.;tmis12cm0 = -1000.;tmis13cm0 = -1000.;
  tmis21cm0 = -1000.;tmis22cm0 = -1000.;tmis23cm0 = -1000.;
  tmis31cm0 = -1000.;tmis32cm0 = -1000.;tmis33cm0 = -1000.;
  
  thmis11cm0 = -1000.;thmis12cm0 = -1000.;thmis13cm0 = -1000.;
  thmis21cm0 = -1000.;thmis22cm0 = -1000.;thmis23cm0 = -1000.;
  thmis31cm0 = -1000.;thmis32cm0 = -1000.;thmis33cm0 = -1000.;
  
  tmis11cmp = -1000.;tmis12cmp = -1000.;tmis13cmp = -1000.;
  tmis21cmp = -1000.;tmis22cmp = -1000.;tmis23cmp = -1000.;
  tmis31cmp = -1000.;tmis32cmp = -1000.;tmis33cmp = -1000.;
  
  thmis11cmp = -1000.;thmis12cmp = -1000.;thmis13cmp = -1000.;
  thmis21cmp = -1000.;thmis22cmp = -1000.;thmis23cmp = -1000.;
  thmis31cmp = -1000.;thmis32cmp = -1000.;thmis33cmp = -1000.;
}

ClassImp(ERInclusiveData)