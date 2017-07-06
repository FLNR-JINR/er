// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIGadastdIG__ERGadastDict

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
#include "ERGadast.h"
#include "ERGadastDigiPar.h"
#include "ERGadastGeoPar.h"
#include "ERGadastContFact.h"
#include "ERGadastSetup.h"
#include "ERGadastDigitizer.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERGadast(void *p = 0);
   static void *newArray_ERGadast(Long_t size, void *p);
   static void delete_ERGadast(void *p);
   static void deleteArray_ERGadast(void *p);
   static void destruct_ERGadast(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadast*)
   {
      ::ERGadast *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadast >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadast", ::ERGadast::Class_Version(), "invalid", 30,
                  typeid(::ERGadast), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadast::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadast) );
      instance.SetNew(&new_ERGadast);
      instance.SetNewArray(&newArray_ERGadast);
      instance.SetDelete(&delete_ERGadast);
      instance.SetDeleteArray(&deleteArray_ERGadast);
      instance.SetDestructor(&destruct_ERGadast);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadast*)
   {
      return GenerateInitInstanceLocal((::ERGadast*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadast*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastContFact(void *p = 0);
   static void *newArray_ERGadastContFact(Long_t size, void *p);
   static void delete_ERGadastContFact(void *p);
   static void deleteArray_ERGadastContFact(void *p);
   static void destruct_ERGadastContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastContFact*)
   {
      ::ERGadastContFact *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastContFact >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastContFact", ::ERGadastContFact::Class_Version(), "invalid", 326,
                  typeid(::ERGadastContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastContFact::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastContFact) );
      instance.SetNew(&new_ERGadastContFact);
      instance.SetNewArray(&newArray_ERGadastContFact);
      instance.SetDelete(&delete_ERGadastContFact);
      instance.SetDeleteArray(&deleteArray_ERGadastContFact);
      instance.SetDestructor(&destruct_ERGadastContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastContFact*)
   {
      return GenerateInitInstanceLocal((::ERGadastContFact*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastContFact*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastGeoPar(void *p = 0);
   static void *newArray_ERGadastGeoPar(Long_t size, void *p);
   static void delete_ERGadastGeoPar(void *p);
   static void deleteArray_ERGadastGeoPar(void *p);
   static void destruct_ERGadastGeoPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastGeoPar*)
   {
      ::ERGadastGeoPar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastGeoPar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastGeoPar", ::ERGadastGeoPar::Class_Version(), "invalid", 286,
                  typeid(::ERGadastGeoPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastGeoPar::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastGeoPar) );
      instance.SetNew(&new_ERGadastGeoPar);
      instance.SetNewArray(&newArray_ERGadastGeoPar);
      instance.SetDelete(&delete_ERGadastGeoPar);
      instance.SetDeleteArray(&deleteArray_ERGadastGeoPar);
      instance.SetDestructor(&destruct_ERGadastGeoPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastGeoPar*)
   {
      return GenerateInitInstanceLocal((::ERGadastGeoPar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastGeoPar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastDigiPar(void *p = 0);
   static void *newArray_ERGadastDigiPar(Long_t size, void *p);
   static void delete_ERGadastDigiPar(void *p);
   static void deleteArray_ERGadastDigiPar(void *p);
   static void destruct_ERGadastDigiPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastDigiPar*)
   {
      ::ERGadastDigiPar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastDigiPar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastDigiPar", ::ERGadastDigiPar::Class_Version(), "invalid", 211,
                  typeid(::ERGadastDigiPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastDigiPar::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastDigiPar) );
      instance.SetNew(&new_ERGadastDigiPar);
      instance.SetNewArray(&newArray_ERGadastDigiPar);
      instance.SetDelete(&delete_ERGadastDigiPar);
      instance.SetDeleteArray(&deleteArray_ERGadastDigiPar);
      instance.SetDestructor(&destruct_ERGadastDigiPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastDigiPar*)
   {
      return GenerateInitInstanceLocal((::ERGadastDigiPar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastDigiPar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ERGadastSetup_Dictionary();
   static void ERGadastSetup_TClassManip(TClass*);
   static void delete_ERGadastSetup(void *p);
   static void deleteArray_ERGadastSetup(void *p);
   static void destruct_ERGadastSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastSetup*)
   {
      ::ERGadastSetup *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ERGadastSetup));
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastSetup", "invalid", 356,
                  typeid(::ERGadastSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ERGadastSetup_Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastSetup) );
      instance.SetDelete(&delete_ERGadastSetup);
      instance.SetDeleteArray(&deleteArray_ERGadastSetup);
      instance.SetDestructor(&destruct_ERGadastSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastSetup*)
   {
      return GenerateInitInstanceLocal((::ERGadastSetup*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastSetup*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ERGadastSetup_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ERGadastSetup*)0x0)->GetClass();
      ERGadastSetup_TClassManip(theClass);
   return theClass;
   }

   static void ERGadastSetup_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastDigitizer(void *p = 0);
   static void *newArray_ERGadastDigitizer(Long_t size, void *p);
   static void delete_ERGadastDigitizer(void *p);
   static void deleteArray_ERGadastDigitizer(void *p);
   static void destruct_ERGadastDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastDigitizer*)
   {
      ::ERGadastDigitizer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastDigitizer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastDigitizer", ::ERGadastDigitizer::Class_Version(), "invalid", 401,
                  typeid(::ERGadastDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastDigitizer) );
      instance.SetNew(&new_ERGadastDigitizer);
      instance.SetNewArray(&newArray_ERGadastDigitizer);
      instance.SetDelete(&delete_ERGadastDigitizer);
      instance.SetDeleteArray(&deleteArray_ERGadastDigitizer);
      instance.SetDestructor(&destruct_ERGadastDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastDigitizer*)
   {
      return GenerateInitInstanceLocal((::ERGadastDigitizer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastDigitizer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERGadast::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadast::Class_Name()
{
   return "ERGadast";
}

//______________________________________________________________________________
const char *ERGadast::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadast*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadast::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadast*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadast::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadast*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadast::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadast*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastContFact::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastContFact::Class_Name()
{
   return "ERGadastContFact";
}

//______________________________________________________________________________
const char *ERGadastContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastContFact*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastContFact*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastContFact*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastContFact*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastGeoPar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastGeoPar::Class_Name()
{
   return "ERGadastGeoPar";
}

//______________________________________________________________________________
const char *ERGadastGeoPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastGeoPar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastGeoPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastGeoPar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastGeoPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastGeoPar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastGeoPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastGeoPar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastDigiPar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastDigiPar::Class_Name()
{
   return "ERGadastDigiPar";
}

//______________________________________________________________________________
const char *ERGadastDigiPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigiPar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastDigiPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigiPar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastDigiPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigiPar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastDigiPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigiPar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastDigitizer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastDigitizer::Class_Name()
{
   return "ERGadastDigitizer";
}

//______________________________________________________________________________
const char *ERGadastDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigitizer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigitizer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigitizer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigitizer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERGadast::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadast.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadast::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadast::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadast(void *p) {
      return  p ? new(p) ::ERGadast : new ::ERGadast;
   }
   static void *newArray_ERGadast(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadast[nElements] : new ::ERGadast[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadast(void *p) {
      delete ((::ERGadast*)p);
   }
   static void deleteArray_ERGadast(void *p) {
      delete [] ((::ERGadast*)p);
   }
   static void destruct_ERGadast(void *p) {
      typedef ::ERGadast current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadast

//______________________________________________________________________________
void ERGadastContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastContFact(void *p) {
      return  p ? new(p) ::ERGadastContFact : new ::ERGadastContFact;
   }
   static void *newArray_ERGadastContFact(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastContFact[nElements] : new ::ERGadastContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastContFact(void *p) {
      delete ((::ERGadastContFact*)p);
   }
   static void deleteArray_ERGadastContFact(void *p) {
      delete [] ((::ERGadastContFact*)p);
   }
   static void destruct_ERGadastContFact(void *p) {
      typedef ::ERGadastContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastContFact

//______________________________________________________________________________
void ERGadastGeoPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastGeoPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastGeoPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastGeoPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastGeoPar(void *p) {
      return  p ? new(p) ::ERGadastGeoPar : new ::ERGadastGeoPar;
   }
   static void *newArray_ERGadastGeoPar(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastGeoPar[nElements] : new ::ERGadastGeoPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastGeoPar(void *p) {
      delete ((::ERGadastGeoPar*)p);
   }
   static void deleteArray_ERGadastGeoPar(void *p) {
      delete [] ((::ERGadastGeoPar*)p);
   }
   static void destruct_ERGadastGeoPar(void *p) {
      typedef ::ERGadastGeoPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastGeoPar

//______________________________________________________________________________
void ERGadastDigiPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastDigiPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastDigiPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastDigiPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastDigiPar(void *p) {
      return  p ? new(p) ::ERGadastDigiPar : new ::ERGadastDigiPar;
   }
   static void *newArray_ERGadastDigiPar(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastDigiPar[nElements] : new ::ERGadastDigiPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastDigiPar(void *p) {
      delete ((::ERGadastDigiPar*)p);
   }
   static void deleteArray_ERGadastDigiPar(void *p) {
      delete [] ((::ERGadastDigiPar*)p);
   }
   static void destruct_ERGadastDigiPar(void *p) {
      typedef ::ERGadastDigiPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastDigiPar

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERGadastSetup(void *p) {
      delete ((::ERGadastSetup*)p);
   }
   static void deleteArray_ERGadastSetup(void *p) {
      delete [] ((::ERGadastSetup*)p);
   }
   static void destruct_ERGadastSetup(void *p) {
      typedef ::ERGadastSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastSetup

//______________________________________________________________________________
void ERGadastDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastDigitizer(void *p) {
      return  p ? new(p) ::ERGadastDigitizer : new ::ERGadastDigitizer;
   }
   static void *newArray_ERGadastDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastDigitizer[nElements] : new ::ERGadastDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastDigitizer(void *p) {
      delete ((::ERGadastDigitizer*)p);
   }
   static void deleteArray_ERGadastDigitizer(void *p) {
      delete [] ((::ERGadastDigitizer*)p);
   }
   static void destruct_ERGadastDigitizer(void *p) {
      typedef ::ERGadastDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastDigitizer

namespace {
  void TriggerDictionaryInitialization_G__ERGadastDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/data/GadastData/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/base/",
"/home/muzalevsky/check/Gadast/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/Gadast/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERGadastDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERGadast;
class ERGadastContFact;
class ERGadastGeoPar;
class ERGadastDigiPar;
class ERGadastSetup;
class ERGadastDigitizer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERGadastDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERGadast header file                   -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------


#ifndef ERGadast_H
#define ERGadast_H


#include "TVector3.h"
#include "TRandom3.h"
#include "TH1F.h"

class TRandom3;

#include "ERDetector.h"
#include "ERGadastCsIPoint.h"
#include "ERGadastLaBrPoint.h"
#include "ERGadastStep.h"
#include "ERDetectorList.h"

class ERGadastMesh;

class ERGadast : public ERDetector
{
  
public:
  
  /** Default constructor **/
  ERGadast();
  
  
  /** Standard constructor.
   *@param name    ERGadast ERGadast name
   *@param active  sensitivity flag
   **/
  ERGadast(const char* name, Bool_t active);
  
  
  /** Destructor **/
  virtual ~ERGadast();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERGadastCsIPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  
  
  /** Virtual method BeginEvent
   **
   **/
  virtual void BeginEvent();
  
  /** Virtual method EndOfEvent
   **
   ** If verbosity level is set, print point collection at the
   ** end of the event.
   **/
  virtual void EndOfEvent();
  
  /** Virtual method Register
   **
   ** Registers the point collection in the ROOT manager.
   **/
  virtual void Register();
  
  
  /** Accessor to the point collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  
  
  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print(Option_t *option="") const;
  
  
  /** Virtual method Reset
   **
   ** Clears the point collection
   **/
  virtual void Reset();
  
  
  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);
  
  
  /** Virtaul method Construct geometry
   **
   ** Constructs the ERGadast geometry
   **/
  //virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERGadast data
   **/
  virtual void Initialize();

  /** Virtaul method CheckIfSensitive 
	**Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name.
  **/
  virtual Bool_t CheckIfSensitive(std::string name);
  
  /** Virtaul method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
  
  void SetStoreSteps() {fStoreSteps = kTRUE;}

  void WriteHistos();
private:
  TClonesArray*  fCsIPoints;     //!  The point collection
  TClonesArray*  fLaBrPoints;     //!  The point collection
  TClonesArray*  fGadastSteps;      //!  The steps collection
  Int_t fVersion;                    //! geometry version
  Bool_t fStoreSteps;
  //ERGadastMesh* fMesh;
  //TRandom3* fRnd;
  //current point state 
  Int_t fEventID;           //!  event index
  Int_t fTrackID;           //!  track index
  Int_t fMot0TrackID;       //!  0th mother track index
  Double_t fMass;              //!  mass
  TVector3 fPosIn, fPosOut;    //!  position
  TVector3 fMomIn, fMomOut;    //!  momentum
  Double32_t     fTime;              //!  time
  Double32_t     fLength;            //!  length
  Double32_t     fELoss;             //!  energy loss
  Int_t fPDG;
  Int_t  fStepNr;            //!  current step numb in this active volumes
  ERGadastDetectorType  fDetectorType;      //! CsI, LaBr
  Int_t fCsICell, fCsIBlock, fCsIWall;
  Int_t fMeshElement;

  TH1F* fHCsIElossInEvent;
  TH1F* fHLaBrElossInEvent;

  Float_t fCsIElossInEvent;
  Float_t fLaBrElossInEvent;
private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
   
  ERGadastCsIPoint* AddCsIPoint();

  ERGadastLaBrPoint* AddLaBrPoint();

  ERGadastStep* AddStep();

  void StartPoint();
  void FinishPoint();
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  ClassDef(ERGadast,1);
};

#endif 
// -------------------------------------------------------------------------
// -----                  ERGadastDigiPar header file                 -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigiPar_H
#define ERGadastDigiPar_H


#include "FairParGenericSet.h"

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayF.h"
#include "TArrayI.h"

class FairParIo;
class FairParamList;


class ERGadastDigiPar : public FairParGenericSet
{

  public:

    /** Standard constructor **/
    ERGadastDigiPar(const char* name    = "ERGadastDigiPar",
                            const char* title   = "ERGadast Digitization Parameters",
                            const char* context = "Default");


    /** Destructor **/
    virtual ~ERGadastDigiPar();


    /** Initialisation from input device**/
    virtual Bool_t init(FairParIo* input);


    /** Output to file **/
    //virtual Int_t write(FairParIo* output);

    virtual void print();
    
    /** Reset all parameters **/
    virtual void clear();

    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    
    /** Accessories  **/
    Int_t CsIMECout() const {return fCsIMECount;}
    Int_t LaBrMECout() const {return fLaBrMECount;}
    Float_t CsILC(Int_t me) const {return (*fCsILC)[me];}
    Float_t LaBrLC(Int_t me) const {return (*fLaBrLC)[me];}
    Float_t CsIDispA(Int_t me) const {return (*fCsIDispA)[me];}
    Float_t LaBrDispA(Int_t me) const {return (*fLaBrDispA)[me];}
    Float_t CsIDispB(Int_t me) const {return (*fCsIDispB)[me];}
    Float_t LaBrDispB(Int_t me) const {return (*fLaBrDispB)[me];}
  private:
    //количество элементов в сетке
    Int_t fCsIMECount; 
    Int_t fLaBrMECount;

    TArrayF* fCsILC;
    TArrayF* fLaBrLC;

    TArrayF* fCsIDispA;
    TArrayF* fLaBrDispA;

    TArrayF* fCsIDispB;
    TArrayF* fLaBrDispB;

    ERGadastDigiPar(const ERGadastDigiPar&);
    ERGadastDigiPar& operator=(const ERGadastDigiPar&);

    ClassDef(ERGadastDigiPar,1);
};


#endif /**  ERGadastDigiPar_H   **/
// -------------------------------------------------------------------------
// -----                  ERGadastGeoPar header file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastGeoPar_H
#define ERGadastGeoPar_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class ERGadastGeoPar : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    ERGadastGeoPar(const char* name="ERGadastGeoPar",
                           const char* title="Gadast Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~ERGadastGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    ERGadastGeoPar(const ERGadastGeoPar&);
    ERGadastGeoPar& operator=(const ERGadastGeoPar&);

    ClassDef(ERGadastGeoPar,1)
};

#endif /* ERGadastGeoPar_H */// -------------------------------------------------------------------------
// -----                  ERGadastContFact header file             	   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastContFact_H
#define ERGadastContFact_H

#include "FairContFact.h"

class FairContainer;

class ERGadastContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    ERGadastContFact();
    ~ERGadastContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( ERGadastContFact,0)
};

#endif  /* !ERNeuRadContFact_H */// -------------------------------------------------------------------------
// -----                        ERGadastSetup header file               -----
// -----                  Created data  by Schetinin Vitaliy           -----
// -------------------------------------------------------------------------


#ifndef ERGadastSetup_H
#define ERGadastSetup_H


#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TGeoPhysicalNode.h"


#include "ERGadastDigiPar.h"
#include "ERDetectorList.h"

class ERGadastSetup {
public:
	static ERGadastSetup* Instance();
	static Bool_t Init();
	static Float_t CsILC(TVector3* pos);
    static Float_t LaBrLC(TVector3* pos);
    static Float_t CsIDispA(TVector3* pos);
    static Float_t LaBrDispA(TVector3* pos);
    static Float_t CsIDispB(TVector3* pos);
    static Float_t LaBrDispB(TVector3* pos);

	int GetMeshElement(TVector3* pos,ERGadastDetectorType detType);
private:
	ERGadastSetup();
	static ERGadastSetup* fInstance;
	static ERGadastDigiPar* fDigiPar;
	static TGeoPhysicalNode* fGadastNode;
};

#endif// -------------------------------------------------------------------------
// -----                        ERGadastDigitizer header file          -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigitizer_H
#define ERGadastDigitizer_H

#include "TRandom3.h"
#include "TStopwatch.h"
#include "TH1F.h"
#include "TClonesArray.h"

#include "FairTask.h"

#include "ERGadastDigi.h"
#include "ERGadastDigiPar.h"
#include "ERGadastSetup.h"

class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom;

class ERGadastDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERGadastDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERGadastDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERGadastDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
 
  /** Accessors **/ 

protected:
  //Digitization parameters
  ERGadastDigiPar* fDigiPar;
  //Input arrays
  TClonesArray *fGadastCsIPoints;
  TClonesArray *fGadastLaBrPoints;

  ERGadastSetup* fSetup;

  //Output arrays
  TClonesArray *fGadastDigi;

  TH1F* fHCsIElossInEvent;
  TH1F* fHLaBrElossInEvent;

  Float_t fCsIElossInEvent;
  Float_t fLaBrElossInEvent;
protected:
  
  ERGadastDigi* AddDigi(Float_t Edep);
  
private:
  virtual void SetParContainers();
  
  ClassDef(ERGadastDigitizer,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERGadast", payloadCode, "@",
"ERGadastContFact", payloadCode, "@",
"ERGadastDigiPar", payloadCode, "@",
"ERGadastDigitizer", payloadCode, "@",
"ERGadastGeoPar", payloadCode, "@",
"ERGadastSetup", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERGadastDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERGadastDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERGadastDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERGadastDict() {
  TriggerDictionaryInitialization_G__ERGadastDict_Impl();
}
