// -------------------------------------------------------------------------
// -----                      ERRawEvent header file                   -----
// -----                  Created 05/17  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERTelData_H
#define ERTelData_H

#include "TNamed.h"

class ERTelData : public TNamed
{
public:
  ERTelData();
  ~ERTelData();
  void Reset();
  double dep11,dep12,dep13,dep21,dep22,dep23,dep31,dep32,dep33;
  double x11,y11,z11,x21,y21,z21,x22,y22,z22,x23,y23,z23,x24,y24,z24,x25,y25,z25,x26,y26,z26;
  double al111,al112,al113,al121,al122,al123,al131,al132,al133,al211,al212,al213,al221,al222;
  double al223,al231,al232,al233,al242,al252,al262,al311,al312,al313,al321,al322,al323,al331,al332,al333;
  double tcheck11,tcheck12,tcheck13,tcheck21,tcheck22,tcheck23,tcheck31,tcheck32,tcheck33;
  double t11,t12,t13,t21,t22,t23,t31,t32,t33,th11,phi11,th12,phi12,th13,phi13;
  double th21,phi21,th22,phi22,th23,phi23,th31,phi31,th32,phi32,th33,phi33;
  double t11cm0,t12cm0,t13cm0,t21cm0,t22cm0,t23cm0,t31cm0,t32cm0,t33cm0;
  double th11cm0,th12cm0,th13cm0,th21cm0,th22cm0,th23cm0,th31cm0,th32cm0,th33cm0;
  double t11cmp,t12cmp,t13cmp,t21cmp,t22cmp,t23cmp,t31cmp,t32cmp,t33cmp;
  double th11cmp,th12cmp,th13cmp,th21cmp,th22cmp,th23cmp,th31cmp,th32cmp,th33cmp;
  double pz11cmp,pz12cmp,pz13cmp,pz21cmp,pz22cmp,pz23cmp,pz31cmp,pz32cmp,pz33cmp;
  ClassDef(ERTelData, 1)
};

#endif
