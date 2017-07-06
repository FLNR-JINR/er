// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIpassivedIG__ERPassiveDict

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
#include "ERCave.h"
#include "ERGeoCave.h"
#include "ERGeoPassivePar.h"
#include "ERTarget.h"
#include "ERCollimator.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERCave(void *p = 0);
   static void *newArray_ERCave(Long_t size, void *p);
   static void delete_ERCave(void *p);
   static void deleteArray_ERCave(void *p);
   static void destruct_ERCave(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERCave*)
   {
      ::ERCave *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERCave >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERCave", ::ERCave::Class_Version(), "invalid", 18,
                  typeid(::ERCave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERCave::Dictionary, isa_proxy, 4,
                  sizeof(::ERCave) );
      instance.SetNew(&new_ERCave);
      instance.SetNewArray(&newArray_ERCave);
      instance.SetDelete(&delete_ERCave);
      instance.SetDeleteArray(&deleteArray_ERCave);
      instance.SetDestructor(&destruct_ERCave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERCave*)
   {
      return GenerateInitInstanceLocal((::ERCave*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERCave*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGeoCave(void *p = 0);
   static void *newArray_ERGeoCave(Long_t size, void *p);
   static void delete_ERGeoCave(void *p);
   static void deleteArray_ERGeoCave(void *p);
   static void destruct_ERGeoCave(void *p);
   static void streamer_ERGeoCave(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGeoCave*)
   {
      ::ERGeoCave *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGeoCave >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGeoCave", ::ERGeoCave::Class_Version(), "invalid", 45,
                  typeid(::ERGeoCave), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGeoCave::Dictionary, isa_proxy, 16,
                  sizeof(::ERGeoCave) );
      instance.SetNew(&new_ERGeoCave);
      instance.SetNewArray(&newArray_ERGeoCave);
      instance.SetDelete(&delete_ERGeoCave);
      instance.SetDeleteArray(&deleteArray_ERGeoCave);
      instance.SetDestructor(&destruct_ERGeoCave);
      instance.SetStreamerFunc(&streamer_ERGeoCave);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGeoCave*)
   {
      return GenerateInitInstanceLocal((::ERGeoCave*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGeoCave*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGeoPassivePar(void *p = 0);
   static void *newArray_ERGeoPassivePar(Long_t size, void *p);
   static void delete_ERGeoPassivePar(void *p);
   static void deleteArray_ERGeoPassivePar(void *p);
   static void destruct_ERGeoPassivePar(void *p);
   static void streamer_ERGeoPassivePar(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGeoPassivePar*)
   {
      ::ERGeoPassivePar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGeoPassivePar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGeoPassivePar", ::ERGeoPassivePar::Class_Version(), "invalid", 72,
                  typeid(::ERGeoPassivePar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGeoPassivePar::Dictionary, isa_proxy, 16,
                  sizeof(::ERGeoPassivePar) );
      instance.SetNew(&new_ERGeoPassivePar);
      instance.SetNewArray(&newArray_ERGeoPassivePar);
      instance.SetDelete(&delete_ERGeoPassivePar);
      instance.SetDeleteArray(&deleteArray_ERGeoPassivePar);
      instance.SetDestructor(&destruct_ERGeoPassivePar);
      instance.SetStreamerFunc(&streamer_ERGeoPassivePar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGeoPassivePar*)
   {
      return GenerateInitInstanceLocal((::ERGeoPassivePar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGeoPassivePar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERTarget(void *p = 0);
   static void *newArray_ERTarget(Long_t size, void *p);
   static void delete_ERTarget(void *p);
   static void deleteArray_ERTarget(void *p);
   static void destruct_ERTarget(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERTarget*)
   {
      ::ERTarget *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERTarget >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERTarget", ::ERTarget::Class_Version(), "invalid", 101,
                  typeid(::ERTarget), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERTarget::Dictionary, isa_proxy, 4,
                  sizeof(::ERTarget) );
      instance.SetNew(&new_ERTarget);
      instance.SetNewArray(&newArray_ERTarget);
      instance.SetDelete(&delete_ERTarget);
      instance.SetDeleteArray(&deleteArray_ERTarget);
      instance.SetDestructor(&destruct_ERTarget);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERTarget*)
   {
      return GenerateInitInstanceLocal((::ERTarget*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERTarget*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERCollimator(void *p = 0);
   static void *newArray_ERCollimator(Long_t size, void *p);
   static void delete_ERCollimator(void *p);
   static void deleteArray_ERCollimator(void *p);
   static void destruct_ERCollimator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERCollimator*)
   {
      ::ERCollimator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERCollimator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERCollimator", ::ERCollimator::Class_Version(), "invalid", 217,
                  typeid(::ERCollimator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERCollimator::Dictionary, isa_proxy, 4,
                  sizeof(::ERCollimator) );
      instance.SetNew(&new_ERCollimator);
      instance.SetNewArray(&newArray_ERCollimator);
      instance.SetDelete(&delete_ERCollimator);
      instance.SetDeleteArray(&deleteArray_ERCollimator);
      instance.SetDestructor(&destruct_ERCollimator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERCollimator*)
   {
      return GenerateInitInstanceLocal((::ERCollimator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERCollimator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERCave::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERCave::Class_Name()
{
   return "ERCave";
}

//______________________________________________________________________________
const char *ERCave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERCave*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERCave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERCave*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERCave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERCave*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERCave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERCave*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGeoCave::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGeoCave::Class_Name()
{
   return "ERGeoCave";
}

//______________________________________________________________________________
const char *ERGeoCave::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGeoCave*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGeoCave::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGeoCave*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGeoCave::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGeoCave*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGeoCave::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGeoCave*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGeoPassivePar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGeoPassivePar::Class_Name()
{
   return "ERGeoPassivePar";
}

//______________________________________________________________________________
const char *ERGeoPassivePar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGeoPassivePar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGeoPassivePar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGeoPassivePar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGeoPassivePar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGeoPassivePar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGeoPassivePar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGeoPassivePar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERTarget::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERTarget::Class_Name()
{
   return "ERTarget";
}

//______________________________________________________________________________
const char *ERTarget::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERTarget*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERTarget::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERTarget*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERTarget::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERTarget*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERTarget::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERTarget*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERCollimator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERCollimator::Class_Name()
{
   return "ERCollimator";
}

//______________________________________________________________________________
const char *ERCollimator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERCollimator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERCollimator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERCollimator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERCollimator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERCollimator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERCollimator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERCollimator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERCave::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERCave.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERCave::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERCave::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERCave(void *p) {
      return  p ? new(p) ::ERCave : new ::ERCave;
   }
   static void *newArray_ERCave(Long_t nElements, void *p) {
      return p ? new(p) ::ERCave[nElements] : new ::ERCave[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERCave(void *p) {
      delete ((::ERCave*)p);
   }
   static void deleteArray_ERCave(void *p) {
      delete [] ((::ERCave*)p);
   }
   static void destruct_ERCave(void *p) {
      typedef ::ERCave current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERCave

//______________________________________________________________________________
void ERGeoCave::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGeoCave.

   FairGeoSet::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGeoCave(void *p) {
      return  p ? new(p) ::ERGeoCave : new ::ERGeoCave;
   }
   static void *newArray_ERGeoCave(Long_t nElements, void *p) {
      return p ? new(p) ::ERGeoCave[nElements] : new ::ERGeoCave[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGeoCave(void *p) {
      delete ((::ERGeoCave*)p);
   }
   static void deleteArray_ERGeoCave(void *p) {
      delete [] ((::ERGeoCave*)p);
   }
   static void destruct_ERGeoCave(void *p) {
      typedef ::ERGeoCave current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ERGeoCave(TBuffer &buf, void *obj) {
      ((::ERGeoCave*)obj)->::ERGeoCave::Streamer(buf);
   }
} // end of namespace ROOT for class ::ERGeoCave

//______________________________________________________________________________
void ERGeoPassivePar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGeoPassivePar.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      FairParGenericSet::Streamer(R__b);
      R__b >> fGeoSensNodes;
      R__b >> fGeoPassNodes;
      R__b.CheckByteCount(R__s, R__c, ERGeoPassivePar::IsA());
   } else {
      R__c = R__b.WriteVersion(ERGeoPassivePar::IsA(), kTRUE);
      FairParGenericSet::Streamer(R__b);
      R__b << fGeoSensNodes;
      R__b << fGeoPassNodes;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGeoPassivePar(void *p) {
      return  p ? new(p) ::ERGeoPassivePar : new ::ERGeoPassivePar;
   }
   static void *newArray_ERGeoPassivePar(Long_t nElements, void *p) {
      return p ? new(p) ::ERGeoPassivePar[nElements] : new ::ERGeoPassivePar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGeoPassivePar(void *p) {
      delete ((::ERGeoPassivePar*)p);
   }
   static void deleteArray_ERGeoPassivePar(void *p) {
      delete [] ((::ERGeoPassivePar*)p);
   }
   static void destruct_ERGeoPassivePar(void *p) {
      typedef ::ERGeoPassivePar current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ERGeoPassivePar(TBuffer &buf, void *obj) {
      ((::ERGeoPassivePar*)obj)->::ERGeoPassivePar::Streamer(buf);
   }
} // end of namespace ROOT for class ::ERGeoPassivePar

//______________________________________________________________________________
void ERTarget::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERTarget.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERTarget::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERTarget::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERTarget(void *p) {
      return  p ? new(p) ::ERTarget : new ::ERTarget;
   }
   static void *newArray_ERTarget(Long_t nElements, void *p) {
      return p ? new(p) ::ERTarget[nElements] : new ::ERTarget[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERTarget(void *p) {
      delete ((::ERTarget*)p);
   }
   static void deleteArray_ERTarget(void *p) {
      delete [] ((::ERTarget*)p);
   }
   static void destruct_ERTarget(void *p) {
      typedef ::ERTarget current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERTarget

//______________________________________________________________________________
void ERCollimator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERCollimator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERCollimator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERCollimator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERCollimator(void *p) {
      return  p ? new(p) ::ERCollimator : new ::ERCollimator;
   }
   static void *newArray_ERCollimator(Long_t nElements, void *p) {
      return p ? new(p) ::ERCollimator[nElements] : new ::ERCollimator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERCollimator(void *p) {
      delete ((::ERCollimator*)p);
   }
   static void deleteArray_ERCollimator(void *p) {
      delete [] ((::ERCollimator*)p);
   }
   static void destruct_ERCollimator(void *p) {
      typedef ::ERCollimator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERCollimator

namespace {
  void TriggerDictionaryInitialization_G__ERPassiveDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/base",
"/home/muzalevsky/check/data",
"/home/muzalevsky/check/passive",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/passive/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERPassiveDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERCave;
class __attribute__((annotate(R"ATTRDUMP(Class for the geometry of CAVE)ATTRDUMP"))) ERGeoCave;
class ERGeoPassivePar;
class ERTarget;
class ERCollimator;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERPassiveDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                       ERCave header file                      -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#ifndef ERCAVE_H
#define ERCAVE_H

#include "FairDetector.h"
#include "FairModule.h"


class ERCave : public FairModule {
  public:
	ERCave(const char * name, const char *Title="Expert Cave");
	ERCave();
    virtual ~ERCave();
    virtual void ConstructGeometry();
private:
     Double_t world[3]; //!
     ClassDef(ERCave,1)
};

#endif //ERCAVE_H

// -------------------------------------------------------------------------
// -----                       ERGeoCave header file                   -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------


#ifndef ERGEOCAVE_H
#define ERGEOCAVE_H

#include "FairGeoSet.h"
#include "TString.h"
#include "FairGeoMedia.h"
#include <fstream>
class  ERGeoCave : public FairGeoSet {
protected:
  TString name;
public:
  ERGeoCave();
  ~ERGeoCave() {}
  const char* getModuleName(Int_t) {return name.Data();}  
  Bool_t read(std::fstream&,FairGeoMedia*);
  void addRefNodes();
  void write(std::fstream&);
  void print();
  ClassDef(ERGeoCave,0) // Class for the geometry of CAVE
};

#endif  //ERGEOCAVE_H
// -------------------------------------------------------------------------
// -----                       ERGeoPassivePar header file                   -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#ifndef ERGEOPASSIVEPAR_H
#define ERGEOPASSIVEPAR_H

#include "FairParGenericSet.h"
#include "TH1F.h"

class ERGeoPassivePar : public FairParGenericSet {
public:
  TObjArray            *fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray            *fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  ERGeoPassivePar(const char* name="ERGeoPassivePar",
             const char* title="Passive Geometry Parameters",
             const char* context="TestDefaultContext");
  ~ERGeoPassivePar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray             *GetGeoSensitiveNodes(){return fGeoSensNodes;}
  TObjArray             *GetGeoPassiveNodes(){return fGeoPassNodes;}

  ClassDef(ERGeoPassivePar,1)
};

#endif /* !ERGeoPassivePar_H */
#ifndef ERTarget_H
#define ERTarget_H

#include "ERDetector.h"

#include "TClonesArray.h"
#include <TString.h>


class ERTarget : public ERDetector {
	static	Double_t fThickness;
	Int_t fVersion;
	TClonesArray*  fTargetPoints;     //!  The point collection //fix
public:
 /** Default constructor **/
  ERTarget();
  
  
  /** Standard constructor.
   *@param name    ERTarget ERTarget name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERTarget(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERTarget();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERTargetPoint and adds it to the
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
   ** Constructs the ERTarget geometry
   **/
  virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERTarget data
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
  
  static Double_t Thickness(){return fThickness;}

  ClassDef(ERTarget,1);
};

#endif //ERTarget_H
#ifndef ERCollimator_H
#define ERCollimator_H

#include "FairModule.h"

class ERCollimator : public FairModule {

  public:

    /** Default constructor **/

    ERCollimator();

    /** Constructor with file name
     ** @param fileName  Name of geometry file
     **
     ** This constructor will force the target geometry to be
     ** constructed from a geometry file (ROOT format only).
     **/

    ERCollimator(const char* fileName);

    /** Destructor **/
    virtual ~ERCollimator();

    /** Built the geometry **/
    virtual void ConstructGeometry();

    ClassDef(ERCollimator,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERCave", payloadCode, "@",
"ERCollimator", payloadCode, "@",
"ERGeoCave", payloadCode, "@",
"ERGeoPassivePar", payloadCode, "@",
"ERTarget", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERPassiveDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERPassiveDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERPassiveDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERPassiveDict() {
  TriggerDictionaryInitialization_G__ERPassiveDict_Impl();
}
