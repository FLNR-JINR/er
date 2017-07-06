// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIbasedIG__ERBaseDict

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
#include "ERDetector.h"
#include "ERMCApplication.h"
#include "ERRunSim.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *ERDetector_Dictionary();
   static void ERDetector_TClassManip(TClass*);
   static void delete_ERDetector(void *p);
   static void deleteArray_ERDetector(void *p);
   static void destruct_ERDetector(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDetector*)
   {
      ::ERDetector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ERDetector));
      static ::ROOT::TGenericClassInfo 
         instance("ERDetector", "invalid", 13,
                  typeid(::ERDetector), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ERDetector_Dictionary, isa_proxy, 4,
                  sizeof(::ERDetector) );
      instance.SetDelete(&delete_ERDetector);
      instance.SetDeleteArray(&deleteArray_ERDetector);
      instance.SetDestructor(&destruct_ERDetector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDetector*)
   {
      return GenerateInitInstanceLocal((::ERDetector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDetector*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ERDetector_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ERDetector*)0x0)->GetClass();
      ERDetector_TClassManip(theClass);
   return theClass;
   }

   static void ERDetector_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_ERMCApplication(void *p = 0);
   static void *newArray_ERMCApplication(Long_t size, void *p);
   static void delete_ERMCApplication(void *p);
   static void deleteArray_ERMCApplication(void *p);
   static void destruct_ERMCApplication(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERMCApplication*)
   {
      ::ERMCApplication *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERMCApplication >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERMCApplication", ::ERMCApplication::Class_Version(), "invalid", 50,
                  typeid(::ERMCApplication), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERMCApplication::Dictionary, isa_proxy, 4,
                  sizeof(::ERMCApplication) );
      instance.SetNew(&new_ERMCApplication);
      instance.SetNewArray(&newArray_ERMCApplication);
      instance.SetDelete(&delete_ERMCApplication);
      instance.SetDeleteArray(&deleteArray_ERMCApplication);
      instance.SetDestructor(&destruct_ERMCApplication);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERMCApplication*)
   {
      return GenerateInitInstanceLocal((::ERMCApplication*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERMCApplication*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERRunSim(void *p = 0);
   static void *newArray_ERRunSim(Long_t size, void *p);
   static void delete_ERRunSim(void *p);
   static void deleteArray_ERRunSim(void *p);
   static void destruct_ERRunSim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERRunSim*)
   {
      ::ERRunSim *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERRunSim >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERRunSim", ::ERRunSim::Class_Version(), "invalid", 101,
                  typeid(::ERRunSim), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERRunSim::Dictionary, isa_proxy, 4,
                  sizeof(::ERRunSim) );
      instance.SetNew(&new_ERRunSim);
      instance.SetNewArray(&newArray_ERRunSim);
      instance.SetDelete(&delete_ERRunSim);
      instance.SetDeleteArray(&deleteArray_ERRunSim);
      instance.SetDestructor(&destruct_ERRunSim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERRunSim*)
   {
      return GenerateInitInstanceLocal((::ERRunSim*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERRunSim*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERMCApplication::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERMCApplication::Class_Name()
{
   return "ERMCApplication";
}

//______________________________________________________________________________
const char *ERMCApplication::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCApplication*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERMCApplication::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCApplication*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERMCApplication::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCApplication*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERMCApplication::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCApplication*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERRunSim::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERRunSim::Class_Name()
{
   return "ERRunSim";
}

//______________________________________________________________________________
const char *ERRunSim::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRunSim*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERRunSim::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRunSim*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERRunSim::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRunSim*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERRunSim::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRunSim*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERDetector(void *p) {
      delete ((::ERDetector*)p);
   }
   static void deleteArray_ERDetector(void *p) {
      delete [] ((::ERDetector*)p);
   }
   static void destruct_ERDetector(void *p) {
      typedef ::ERDetector current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDetector

//______________________________________________________________________________
void ERMCApplication::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERMCApplication.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERMCApplication::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERMCApplication::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERMCApplication(void *p) {
      return  p ? new(p) ::ERMCApplication : new ::ERMCApplication;
   }
   static void *newArray_ERMCApplication(Long_t nElements, void *p) {
      return p ? new(p) ::ERMCApplication[nElements] : new ::ERMCApplication[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERMCApplication(void *p) {
      delete ((::ERMCApplication*)p);
   }
   static void deleteArray_ERMCApplication(void *p) {
      delete [] ((::ERMCApplication*)p);
   }
   static void destruct_ERMCApplication(void *p) {
      typedef ::ERMCApplication current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERMCApplication

//______________________________________________________________________________
void ERRunSim::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERRunSim.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERRunSim::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERRunSim::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERRunSim(void *p) {
      return  p ? new(p) ::ERRunSim : new ::ERRunSim;
   }
   static void *newArray_ERRunSim(Long_t nElements, void *p) {
      return p ? new(p) ::ERRunSim[nElements] : new ::ERRunSim[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERRunSim(void *p) {
      delete ((::ERRunSim*)p);
   }
   static void deleteArray_ERRunSim(void *p) {
      delete [] ((::ERRunSim*)p);
   }
   static void destruct_ERRunSim(void *p) {
      typedef ::ERRunSim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERRunSim

namespace {
  void TriggerDictionaryInitialization_G__ERBaseDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/base",
"/home/muzalevsky/check/data",
"/home/muzalevsky/check/passive",
"/home/muzalevsky/check/decayers",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/base/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERBaseDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERDetector;
class __attribute__((annotate(R"ATTRDUMP(Interface to MonteCarlo application)ATTRDUMP"))) ERMCApplication;
class ERRunSim;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERBaseDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H


#ifndef ERDETECTOR_CXX_
#define ERDETECTOR_CXX_

#include "FairDetector.h"

class ERDetector : public FairDetector
{

public:
    /**
      constructor
      Name :  Detector Name
      Active: kTRUE for active detectors  (ProcessHits() will be called)
              kFALSE for inactive detectors
    */
	ERDetector(const char* Name, Bool_t Active, Int_t DetId=0);

	/** Virtaul method Construct geometry
	 **
	 ** Constructs the ERNeuRad geometry
	**/
	virtual void ConstructGeometry();
};





#endif /* ERDETECTOR_CXX_ */

// -------------------------------------------------------------------------
// -----                      ERMCApplication header file              -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERMCAPPLICATION_H
#define ERMCAPPLICATION_H

#include "FairMCApplication.h"
#include "ERDecayer.h"

class ERMCApplication : public FairMCApplication{


public:
	ERMCApplication(const char* name, const char* title, TObjArray* ModList, const char* MatName);
    /** default constructor
    */
    ERMCApplication();
    /** default destructor
    */
    virtual ~ERMCApplication();
    /** Singelton instance
    */
    static ERMCApplication* Instance();
    
    /** Define action at each step, dispatch the action to the corresponding detectors */
    virtual void          Stepping();                                       // MC Application

    /** Define actions at the beginning of the event */
    virtual void          BeginEvent();                                     // MC Application

    /** Define actions at the end of event */
    virtual void          FinishEvent();                                    // MC Application

    void SetDecayer(ERDecayer* decayer){fDecayer = decayer;}
  private:
  	ERDecayer* fDecayer;
  	
    ClassDef(ERMCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline ERMCApplication* ERMCApplication::Instance()
{ return (ERMCApplication*)(TVirtualMCApplication::Instance());}

#endif
// -------------------------------------------------------------------------
// -----                      ERRunSim header file                     -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRUNSIM_H
#define ERRUNSIM_H

#include "FairRunSim.h"                    // for FairRunSim

#include "ERMCApplication.h"
#include "ERDecayer.h"

class ERRunSim : public FairRunSim
{
  public:
    /** default ctor*/
    ERRunSim(Bool_t isMaster = kTRUE);
    /** default dtor*/
    virtual ~ERRunSim();
    /**
    *       Initialize the Simulation
    */
    virtual void        Init();

    void SetDecayer(ERDecayer* deacyer){((ERMCApplication*)fApp)->SetDecayer(deacyer);}

  private:
    ERRunSim(const ERRunSim& M);
    ERRunSim& operator= (const  ERRunSim&) {return *this;}

    void SetMCConfig();

    ClassDef(ERRunSim ,1)

};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERDetector", payloadCode, "@",
"ERMCApplication", payloadCode, "@",
"ERRunSim", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERBaseDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERBaseDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERBaseDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERBaseDict() {
  TriggerDictionaryInitialization_G__ERBaseDict_Impl();
}
