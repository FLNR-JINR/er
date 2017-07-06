// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIgeneratorsdIG__generatorsDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "ERGenerator.h"
#include "ERGammaGenerator.h"
#include "ERIonGenerator.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERGammaGenerator(void *p = 0);
   static void *newArray_ERGammaGenerator(Long_t size, void *p);
   static void delete_ERGammaGenerator(void *p);
   static void deleteArray_ERGammaGenerator(void *p);
   static void destruct_ERGammaGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGammaGenerator*)
   {
      ::ERGammaGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGammaGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGammaGenerator", ::ERGammaGenerator::Class_Version(), "invalid", 103,
                  typeid(::ERGammaGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGammaGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::ERGammaGenerator) );
      instance.SetNew(&new_ERGammaGenerator);
      instance.SetNewArray(&newArray_ERGammaGenerator);
      instance.SetDelete(&delete_ERGammaGenerator);
      instance.SetDeleteArray(&deleteArray_ERGammaGenerator);
      instance.SetDestructor(&destruct_ERGammaGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGammaGenerator*)
   {
      return GenerateInitInstanceLocal((::ERGammaGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGammaGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGenerator(void *p = 0);
   static void *newArray_ERGenerator(Long_t size, void *p);
   static void delete_ERGenerator(void *p);
   static void deleteArray_ERGenerator(void *p);
   static void destruct_ERGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGenerator*)
   {
      ::ERGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGenerator", ::ERGenerator::Class_Version(), "invalid", 27,
                  typeid(::ERGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::ERGenerator) );
      instance.SetNew(&new_ERGenerator);
      instance.SetNewArray(&newArray_ERGenerator);
      instance.SetDelete(&delete_ERGenerator);
      instance.SetDeleteArray(&deleteArray_ERGenerator);
      instance.SetDestructor(&destruct_ERGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGenerator*)
   {
      return GenerateInitInstanceLocal((::ERGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERIonGenerator(void *p = 0);
   static void *newArray_ERIonGenerator(Long_t size, void *p);
   static void delete_ERIonGenerator(void *p);
   static void deleteArray_ERIonGenerator(void *p);
   static void destruct_ERIonGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERIonGenerator*)
   {
      ::ERIonGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERIonGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERIonGenerator", ::ERIonGenerator::Class_Version(), "invalid", 177,
                  typeid(::ERIonGenerator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERIonGenerator::Dictionary, isa_proxy, 4,
                  sizeof(::ERIonGenerator) );
      instance.SetNew(&new_ERIonGenerator);
      instance.SetNewArray(&newArray_ERIonGenerator);
      instance.SetDelete(&delete_ERIonGenerator);
      instance.SetDeleteArray(&deleteArray_ERIonGenerator);
      instance.SetDestructor(&destruct_ERIonGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERIonGenerator*)
   {
      return GenerateInitInstanceLocal((::ERIonGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERIonGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERGammaGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGammaGenerator::Class_Name()
{
   return "ERGammaGenerator";
}

//______________________________________________________________________________
const char *ERGammaGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGammaGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGammaGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGammaGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGammaGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGammaGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGammaGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGammaGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGenerator::Class_Name()
{
   return "ERGenerator";
}

//______________________________________________________________________________
const char *ERGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERIonGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERIonGenerator::Class_Name()
{
   return "ERIonGenerator";
}

//______________________________________________________________________________
const char *ERIonGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERIonGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERIonGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERIonGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERIonGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERIonGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERIonGenerator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERIonGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERGammaGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGammaGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGammaGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGammaGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGammaGenerator(void *p) {
      return  p ? new(p) ::ERGammaGenerator : new ::ERGammaGenerator;
   }
   static void *newArray_ERGammaGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::ERGammaGenerator[nElements] : new ::ERGammaGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGammaGenerator(void *p) {
      delete ((::ERGammaGenerator*)p);
   }
   static void deleteArray_ERGammaGenerator(void *p) {
      delete [] ((::ERGammaGenerator*)p);
   }
   static void destruct_ERGammaGenerator(void *p) {
      typedef ::ERGammaGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGammaGenerator

//______________________________________________________________________________
void ERGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGenerator(void *p) {
      return  p ? new(p) ::ERGenerator : new ::ERGenerator;
   }
   static void *newArray_ERGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::ERGenerator[nElements] : new ::ERGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGenerator(void *p) {
      delete ((::ERGenerator*)p);
   }
   static void deleteArray_ERGenerator(void *p) {
      delete [] ((::ERGenerator*)p);
   }
   static void destruct_ERGenerator(void *p) {
      typedef ::ERGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGenerator

//______________________________________________________________________________
void ERIonGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERIonGenerator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERIonGenerator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERIonGenerator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERIonGenerator(void *p) {
      return  p ? new(p) ::ERIonGenerator : new ::ERIonGenerator;
   }
   static void *newArray_ERIonGenerator(Long_t nElements, void *p) {
      return p ? new(p) ::ERIonGenerator[nElements] : new ::ERIonGenerator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERIonGenerator(void *p) {
      delete ((::ERIonGenerator*)p);
   }
   static void deleteArray_ERIonGenerator(void *p) {
      delete [] ((::ERIonGenerator*)p);
   }
   static void destruct_ERIonGenerator(void *p) {
      typedef ::ERIonGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERIonGenerator

namespace {
  void TriggerDictionaryInitialization_G__generatorsDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include/Geant4",
"/opt/fs_may16p1_inst/transport/geant4/source/interfaces/common/include",
"/opt/fs_may16p1_inst/transport/geant4/physics_lists/hadronic/Packaging/include",
"/opt/fs_may16p1_inst/transport/geant4/physics_lists/hadronic/QGSP/include",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/generators",
"/home/muzalevsky/check/data",
"/home/muzalevsky/check/base",
"/home/muzalevsky/check/passive",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/generators/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__generatorsDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERGammaGenerator;
class ERGenerator;
class ERIonGenerator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__generatorsDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                ERGenerator header file                   -----
// -----          Created 02/14  by V.Schetinin                        -----
// -------------------------------------------------------------------------

#ifndef ERGENERATOR_H
#define ERGENERATOR_H 1


#include "FairGenerator.h"              //for FairGenerator
#include "FairIon.h"                    //for FairIon
#include "FairPrimaryGenerator.h"       //for FairPrimaryGenerator

#include "TRandom3.h"
#include "TGenPhaseSpace.h"       //for TGenPhaseSpace 

#include <iostream>

//using namespace std;


class ERGenerator : public FairGenerator
{

 public:

  /** Default constructor without arguments should not be used. **/
  ERGenerator();

  ERGenerator(Int_t z, Int_t a, Int_t q,
              Double_t px, Double_t py, Double_t pz,
              Double_t vx, Double_t vy, Double_t vz);

  /** Destructor. **/
  virtual ~ERGenerator();

  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
  **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  /*
  void SetBeamEnergy(Double_t en){fEBeam = en;}
  void SetGammaCMEnergy(Double_t en){fEGammaCM = en;}
  void SetGammaCMPhiRange(Double_t phiMin, Double_t phiMax){fPhiMinCM = phiMin; fPhiMaxCM= phiMax;}
  void SetGammaCMThetaRange(Double_t thetaMin, Double_t thetaMax){fThetaMinCM = thetaMin; fThetaMaxCM= thetaMax; }
  void SetIon(Int_t z, Int_t a){fA = a; fZ = z;}
  */

  //Skip some events in file
  Bool_t SkipEvents(Int_t count);
   
 private:
  Double_t fEBeam;
  Double_t fEGammaCM;
  Double_t fPhiMinCM, fPhiMaxCM;
  Double_t fThetaMinCM, fThetaMaxCM;
  Double_t fPrimaryIonMass;
  Int_t fGammaPDG;

  static Int_t fgNIon;     //! Number of the instance of this class
  FairIon*   fPrimaryIon;          // Pointer to the FairIon to be generated
  FairIon*   fSecondIon;
  FairIon*   fThirdIon;
  Double_t fPx, fPy, fPz;  // Momentum components [GeV] per nucleon
  Double_t fVx, fVy, fVz;  // Vertex coordinates [cm]
  TGenPhaseSpace* fPHSpace;
  TRandom3 *fRnd;

  ERGenerator(const ERGenerator&);
  ERGenerator& operator=(const ERGenerator&);

  ClassDef(ERGenerator,1);
 
};

#endif
   
// -------------------------------------------------------------------------
// -----                ERGammaGenerator header file                   -----
// -----          Created 02/14  by V.Schetinin                        -----
// -------------------------------------------------------------------------

#ifndef ERGAMMAGENERATOR_H
#define ERGAMMAGENERATOR_H 1


#include "FairGenerator.h"
#include "FairPrimaryGenerator.h"

#include "TRandom3.h"

#include <iostream>

//using namespace std;


class ERGammaGenerator : public FairGenerator
{

 public:

  /** Default constructor without arguments should not be used. **/
  ERGammaGenerator();

  /** Destructor. **/
  virtual ~ERGammaGenerator();

  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param pStack    pointer to the stack
   ** @param ver       not used
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
  void SetBeamEnergy(Double_t en){fEBeam = en;}
  void SetGammaCMEnergy(Double_t en){fEGammaCM = en;}
  void SetGammaCMPhiRange(Double_t phiMin, Double_t phiMax){fPhiMinCM = phiMin; fPhiMaxCM= phiMax;}
  void SetGammaCMThetaRange(Double_t thetaMin, Double_t thetaMax);
  void SetIon(Int_t z, Int_t a){fA = a; fZ = z;}

  //Skip some events in file
  Bool_t SkipEvents(Int_t count);
   
 private:
  Int_t fA;
  Int_t fZ;
  Double_t fEBeam;
  Double_t fEGammaCM;
  Double_t fPhiMinCM, fPhiMaxCM;
  Double_t fThetaMinCM, fThetaMaxCM;
  Double_t fIonMass;
  Int_t fGammaPDG;

  TRandom3 *fRnd;

  ERGammaGenerator(const ERGammaGenerator&);
  ERGammaGenerator& operator=(const ERGammaGenerator&);

  ClassDef(ERGammaGenerator,1);
 
};

#endif
   
/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                   ERIonGenerator header file                 -----
// -----          Created 09/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------

/**
 * 24.10.07  implement constructor with ion name. M. Al-Turany
*/

#ifndef ERIonGenerator_H
#define ERIonGenerator_H

#include "FairGenerator.h"              // for FairGenerator

#include "Rtypes.h"                     // for Double_t, Int_t, etc

class FairIon;
class FairPrimaryGenerator;

class ERIonGenerator : public FairGenerator
{

  public:

    /** Default constructor **/
    ERIonGenerator();


    /** Constructor with ion name
      ** For the generation of ions with pre-defined FairIon
     ** By default, the  excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param ionName  Ion name
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonGenerator(const Char_t* ionName, Int_t mult);


    /** Default constructor
     ** For the generation of ions with atomic number z and mass number a.
     ** By default, the mass equals a times the proton mass and the
     ** excitation energy is zero. This can be changed with the
     ** respective modifiers.
     **@param z         Atomic number
     **@param a         Atomic mass
     **@param q         Electric charge [e]
     **@param mult      Number of ions per event
     **@param px,py,pz  Momentum components [GeV] per nucleon!
     **@param vx,vy,vz  Vertex coordinates [cm]
     **/
    ERIonGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult);


    /** Destructor **/
    virtual ~ERIonGenerator();


    /** Modifiers **/
    void SetCharge(Int_t charge) {fQ=charge;}
    void SetExcitationEnergy(Double_t eExc);
    void SetMass(Double_t mass);
     /** Modifiers **/
    void SetPDGType      (Int_t pdg)  {fPDGType = pdg;  };

    void SetMultiplicity (Int_t mult) {fMult    = mult; };

    void SetPRange(Double32_t pmin=0   , Double32_t pmax=10)
    {fPMin=pmin; fPMax=pmax; fPRangeIsSet=kTRUE;}

    void SetPtRange      (Double32_t ptmin=0   , Double32_t ptmax=10)
    {fPtMin=ptmin; fPtMax=ptmax; fPtRangeIsSet=kTRUE;};

    void SetPhiRange     (Double32_t phimin=0  , Double32_t phimax=360)
    {fPhiMin=phimin; fPhiMax=phimax;};

    void SetEtaRange     (Double32_t etamin=-5 , Double32_t etamax=7)
    {fEtaMin=etamin; fEtaMax=etamax; fEtaRangeIsSet=kTRUE;};

    void SetYRange       (Double32_t ymin=-5   , Double32_t ymax=7)
    {fYMin=ymin; fYMax=ymax; fYRangeIsSet=kTRUE;};

    void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

    void SetCosTheta   ()
    {fCosThetaIsSet=kTRUE;};

    void SetXYZ   (Double32_t x=0, Double32_t y=0, Double32_t z=0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
    }

    void SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z=0) {
      fX1=x1;
      fY1=y1;
      fX2=x2;
      fY2=y2;
      fZ=z;
      fBoxVtxIsSet=kTRUE;
    }

    void SetDebug(Bool_t debug=0) {fDebug = debug;}


    /** Method ReadEvent
     ** Generates <mult> of the specified ions and hands hem to the
     ** FairPrimaryGenerator.
     **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

    FairIon* Ion() {return fIon;}

  private:
    Int_t    fMult;          // Multiplicity per event
    FairIon*   fIon;          // Pointer to the FairIon to be generated
    Int_t    fQ;    // Electric charge [e]

    TString fName;
    Int_t      fPDGType;             // Particle type (PDG encoding)

    Double32_t fPDGMass;             // Particle mass [GeV]
    Double32_t fPtMin,  fPtMax;      // Transverse momentum range [GeV]
    Double32_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    Double32_t fEtaMin, fEtaMax;     // Pseudorapidity range in lab system
    Double32_t fYMin,   fYMax;       // Rapidity range in lab system
    Double32_t fPMin,   fPMax;       // Momentum range in lab system
    Double32_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
    Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double32_t fX1, fY1, fX2, fY2;   // Box vertex coords (x1,y1)->(x2,y2)

    Bool_t     fEtaRangeIsSet;       // True if eta range is set
    Bool_t     fYRangeIsSet;         // True if rapidity range is set
    Bool_t     fThetaRangeIsSet;     // True if theta range is set
    Bool_t     fCosThetaIsSet;       // True if uniform distribution in
    //cos(theta) is set (default -> not set)
    Bool_t     fPtRangeIsSet;        // True if transverse momentum range is set
    Bool_t     fPRangeIsSet;         // True if abs.momentum range is set
    Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
    Bool_t     fDebug;               // Debug switch

    ERIonGenerator(const ERIonGenerator&);
    ERIonGenerator& operator=(const ERIonGenerator&);

    ClassDef(ERIonGenerator,1)
};


#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERGammaGenerator", payloadCode, "@",
"ERGenerator", payloadCode, "@",
"ERIonGenerator", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__generatorsDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__generatorsDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__generatorsDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__generatorsDict() {
  TriggerDictionaryInitialization_G__generatorsDict_Impl();
}
