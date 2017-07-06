// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIglobaldIG__ERGlobalDict

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
#include "ERLi10Reconstructor.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERLi10Reconstructor(void *p = 0);
   static void *newArray_ERLi10Reconstructor(Long_t size, void *p);
   static void delete_ERLi10Reconstructor(void *p);
   static void deleteArray_ERLi10Reconstructor(void *p);
   static void destruct_ERLi10Reconstructor(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERLi10Reconstructor*)
   {
      ::ERLi10Reconstructor *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERLi10Reconstructor >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERLi10Reconstructor", ::ERLi10Reconstructor::Class_Version(), "invalid", 19,
                  typeid(::ERLi10Reconstructor), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERLi10Reconstructor::Dictionary, isa_proxy, 4,
                  sizeof(::ERLi10Reconstructor) );
      instance.SetNew(&new_ERLi10Reconstructor);
      instance.SetNewArray(&newArray_ERLi10Reconstructor);
      instance.SetDelete(&delete_ERLi10Reconstructor);
      instance.SetDeleteArray(&deleteArray_ERLi10Reconstructor);
      instance.SetDestructor(&destruct_ERLi10Reconstructor);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERLi10Reconstructor*)
   {
      return GenerateInitInstanceLocal((::ERLi10Reconstructor*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERLi10Reconstructor*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERLi10Reconstructor::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERLi10Reconstructor::Class_Name()
{
   return "ERLi10Reconstructor";
}

//______________________________________________________________________________
const char *ERLi10Reconstructor::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10Reconstructor*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERLi10Reconstructor::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10Reconstructor*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERLi10Reconstructor::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10Reconstructor*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERLi10Reconstructor::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10Reconstructor*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERLi10Reconstructor::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERLi10Reconstructor.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERLi10Reconstructor::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERLi10Reconstructor::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERLi10Reconstructor(void *p) {
      return  p ? new(p) ::ERLi10Reconstructor : new ::ERLi10Reconstructor;
   }
   static void *newArray_ERLi10Reconstructor(Long_t nElements, void *p) {
      return p ? new(p) ::ERLi10Reconstructor[nElements] : new ::ERLi10Reconstructor[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERLi10Reconstructor(void *p) {
      delete ((::ERLi10Reconstructor*)p);
   }
   static void deleteArray_ERLi10Reconstructor(void *p) {
      delete [] ((::ERLi10Reconstructor*)p);
   }
   static void destruct_ERLi10Reconstructor(void *p) {
      typedef ::ERLi10Reconstructor current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERLi10Reconstructor

namespace {
  void TriggerDictionaryInitialization_G__ERGlobalDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/home/muzalevsky/check/data/NDData/",
"/home/muzalevsky/check/data/DSRDData/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/base/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/global/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/global/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERGlobalDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERLi10Reconstructor;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERGlobalDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERLi10Reconstructor header file        -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERLi10Reconstructor_H
#define ERLi10Reconstructor_H

#include "TClonesArray.h"

#include "FairTask.h"
#include "ERLi10MCEventHeader.h"

class ERLi10Reconstructor : public FairTask {

public:
  /** Default constructor **/
  ERLi10Reconstructor();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERLi10Reconstructor(Int_t verbose);

  /** Destructor **/
  ~ERLi10Reconstructor();

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
  //Input arrays
  TClonesArray *fNDHits;
  TClonesArray *fDSRDHits;
  //Output arrays
  // /TClonesArray *fNDHits;

  ERLi10MCEventHeader* fMCHeader;
protected:
  /*ERNDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, 
          Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);*/
private:
  virtual void SetParContainers();
  
  ClassDef(ERLi10Reconstructor,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERLi10Reconstructor", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERGlobalDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERGlobalDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERGlobalDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERGlobalDict() {
  TriggerDictionaryInitialization_G__ERGlobalDict_Impl();
}
