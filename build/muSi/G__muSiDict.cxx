// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddImuSidIG__muSiDict

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
#include "ERmuSi.h"
#include "ERmuSiHitProducer.h"
#include "ERmuSiTrackFinder.h"
#include "ERmuSiMatcher.h"
#include "ERmuSiVertexFinder.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERmuSi(void *p = 0);
   static void *newArray_ERmuSi(Long_t size, void *p);
   static void delete_ERmuSi(void *p);
   static void deleteArray_ERmuSi(void *p);
   static void destruct_ERmuSi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSi*)
   {
      ::ERmuSi *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSi >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSi", ::ERmuSi::Class_Version(), "invalid", 27,
                  typeid(::ERmuSi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSi::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSi) );
      instance.SetNew(&new_ERmuSi);
      instance.SetNewArray(&newArray_ERmuSi);
      instance.SetDelete(&delete_ERmuSi);
      instance.SetDeleteArray(&deleteArray_ERmuSi);
      instance.SetDestructor(&destruct_ERmuSi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSi*)
   {
      return GenerateInitInstanceLocal((::ERmuSi*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSi*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiHitProducer(void *p = 0);
   static void *newArray_ERmuSiHitProducer(Long_t size, void *p);
   static void delete_ERmuSiHitProducer(void *p);
   static void deleteArray_ERmuSiHitProducer(void *p);
   static void destruct_ERmuSiHitProducer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiHitProducer*)
   {
      ::ERmuSiHitProducer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiHitProducer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiHitProducer", ::ERmuSiHitProducer::Class_Version(), "invalid", 174,
                  typeid(::ERmuSiHitProducer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiHitProducer::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiHitProducer) );
      instance.SetNew(&new_ERmuSiHitProducer);
      instance.SetNewArray(&newArray_ERmuSiHitProducer);
      instance.SetDelete(&delete_ERmuSiHitProducer);
      instance.SetDeleteArray(&deleteArray_ERmuSiHitProducer);
      instance.SetDestructor(&destruct_ERmuSiHitProducer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiHitProducer*)
   {
      return GenerateInitInstanceLocal((::ERmuSiHitProducer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiHitProducer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiTrackFinder(void *p = 0);
   static void *newArray_ERmuSiTrackFinder(Long_t size, void *p);
   static void delete_ERmuSiTrackFinder(void *p);
   static void deleteArray_ERmuSiTrackFinder(void *p);
   static void destruct_ERmuSiTrackFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiTrackFinder*)
   {
      ::ERmuSiTrackFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiTrackFinder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiTrackFinder", ::ERmuSiTrackFinder::Class_Version(), "invalid", 234,
                  typeid(::ERmuSiTrackFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiTrackFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiTrackFinder) );
      instance.SetNew(&new_ERmuSiTrackFinder);
      instance.SetNewArray(&newArray_ERmuSiTrackFinder);
      instance.SetDelete(&delete_ERmuSiTrackFinder);
      instance.SetDeleteArray(&deleteArray_ERmuSiTrackFinder);
      instance.SetDestructor(&destruct_ERmuSiTrackFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiTrackFinder*)
   {
      return GenerateInitInstanceLocal((::ERmuSiTrackFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiTrackFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiMatcher(void *p = 0);
   static void *newArray_ERmuSiMatcher(Long_t size, void *p);
   static void delete_ERmuSiMatcher(void *p);
   static void deleteArray_ERmuSiMatcher(void *p);
   static void destruct_ERmuSiMatcher(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiMatcher*)
   {
      ::ERmuSiMatcher *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiMatcher >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiMatcher", ::ERmuSiMatcher::Class_Version(), "invalid", 299,
                  typeid(::ERmuSiMatcher), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiMatcher::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiMatcher) );
      instance.SetNew(&new_ERmuSiMatcher);
      instance.SetNewArray(&newArray_ERmuSiMatcher);
      instance.SetDelete(&delete_ERmuSiMatcher);
      instance.SetDeleteArray(&deleteArray_ERmuSiMatcher);
      instance.SetDestructor(&destruct_ERmuSiMatcher);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiMatcher*)
   {
      return GenerateInitInstanceLocal((::ERmuSiMatcher*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiMatcher*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiVertexFinder(void *p = 0);
   static void *newArray_ERmuSiVertexFinder(Long_t size, void *p);
   static void delete_ERmuSiVertexFinder(void *p);
   static void deleteArray_ERmuSiVertexFinder(void *p);
   static void destruct_ERmuSiVertexFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiVertexFinder*)
   {
      ::ERmuSiVertexFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiVertexFinder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiVertexFinder", ::ERmuSiVertexFinder::Class_Version(), "invalid", 375,
                  typeid(::ERmuSiVertexFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiVertexFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiVertexFinder) );
      instance.SetNew(&new_ERmuSiVertexFinder);
      instance.SetNewArray(&newArray_ERmuSiVertexFinder);
      instance.SetDelete(&delete_ERmuSiVertexFinder);
      instance.SetDeleteArray(&deleteArray_ERmuSiVertexFinder);
      instance.SetDestructor(&destruct_ERmuSiVertexFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiVertexFinder*)
   {
      return GenerateInitInstanceLocal((::ERmuSiVertexFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiVertexFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERmuSi::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSi::Class_Name()
{
   return "ERmuSi";
}

//______________________________________________________________________________
const char *ERmuSi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSi*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSi*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSi*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSi*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiHitProducer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiHitProducer::Class_Name()
{
   return "ERmuSiHitProducer";
}

//______________________________________________________________________________
const char *ERmuSiHitProducer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHitProducer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiHitProducer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHitProducer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiHitProducer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHitProducer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiHitProducer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHitProducer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiTrackFinder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiTrackFinder::Class_Name()
{
   return "ERmuSiTrackFinder";
}

//______________________________________________________________________________
const char *ERmuSiTrackFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrackFinder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiTrackFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrackFinder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiTrackFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrackFinder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiTrackFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrackFinder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiMatcher::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiMatcher::Class_Name()
{
   return "ERmuSiMatcher";
}

//______________________________________________________________________________
const char *ERmuSiMatcher::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiMatcher*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiMatcher::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiMatcher*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiMatcher::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiMatcher*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiMatcher::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiMatcher*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiVertexFinder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiVertexFinder::Class_Name()
{
   return "ERmuSiVertexFinder";
}

//______________________________________________________________________________
const char *ERmuSiVertexFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertexFinder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiVertexFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertexFinder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiVertexFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertexFinder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiVertexFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertexFinder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERmuSi::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSi::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSi(void *p) {
      return  p ? new(p) ::ERmuSi : new ::ERmuSi;
   }
   static void *newArray_ERmuSi(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSi[nElements] : new ::ERmuSi[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSi(void *p) {
      delete ((::ERmuSi*)p);
   }
   static void deleteArray_ERmuSi(void *p) {
      delete [] ((::ERmuSi*)p);
   }
   static void destruct_ERmuSi(void *p) {
      typedef ::ERmuSi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSi

//______________________________________________________________________________
void ERmuSiHitProducer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiHitProducer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiHitProducer::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiHitProducer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiHitProducer(void *p) {
      return  p ? new(p) ::ERmuSiHitProducer : new ::ERmuSiHitProducer;
   }
   static void *newArray_ERmuSiHitProducer(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiHitProducer[nElements] : new ::ERmuSiHitProducer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiHitProducer(void *p) {
      delete ((::ERmuSiHitProducer*)p);
   }
   static void deleteArray_ERmuSiHitProducer(void *p) {
      delete [] ((::ERmuSiHitProducer*)p);
   }
   static void destruct_ERmuSiHitProducer(void *p) {
      typedef ::ERmuSiHitProducer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiHitProducer

//______________________________________________________________________________
void ERmuSiTrackFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiTrackFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiTrackFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiTrackFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiTrackFinder(void *p) {
      return  p ? new(p) ::ERmuSiTrackFinder : new ::ERmuSiTrackFinder;
   }
   static void *newArray_ERmuSiTrackFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiTrackFinder[nElements] : new ::ERmuSiTrackFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiTrackFinder(void *p) {
      delete ((::ERmuSiTrackFinder*)p);
   }
   static void deleteArray_ERmuSiTrackFinder(void *p) {
      delete [] ((::ERmuSiTrackFinder*)p);
   }
   static void destruct_ERmuSiTrackFinder(void *p) {
      typedef ::ERmuSiTrackFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiTrackFinder

//______________________________________________________________________________
void ERmuSiMatcher::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiMatcher.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiMatcher::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiMatcher::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiMatcher(void *p) {
      return  p ? new(p) ::ERmuSiMatcher : new ::ERmuSiMatcher;
   }
   static void *newArray_ERmuSiMatcher(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiMatcher[nElements] : new ::ERmuSiMatcher[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiMatcher(void *p) {
      delete ((::ERmuSiMatcher*)p);
   }
   static void deleteArray_ERmuSiMatcher(void *p) {
      delete [] ((::ERmuSiMatcher*)p);
   }
   static void destruct_ERmuSiMatcher(void *p) {
      typedef ::ERmuSiMatcher current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiMatcher

//______________________________________________________________________________
void ERmuSiVertexFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiVertexFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiVertexFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiVertexFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiVertexFinder(void *p) {
      return  p ? new(p) ::ERmuSiVertexFinder : new ::ERmuSiVertexFinder;
   }
   static void *newArray_ERmuSiVertexFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiVertexFinder[nElements] : new ::ERmuSiVertexFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiVertexFinder(void *p) {
      delete ((::ERmuSiVertexFinder*)p);
   }
   static void deleteArray_ERmuSiVertexFinder(void *p) {
      delete [] ((::ERmuSiVertexFinder*)p);
   }
   static void destruct_ERmuSiVertexFinder(void *p) {
      typedef ::ERmuSiVertexFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiVertexFinder

namespace {
  void TriggerDictionaryInitialization_G__muSiDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/data/muSiData/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/muSi/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/muSi/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__muSiDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERmuSi;
class ERmuSiHitProducer;
class ERmuSiTrackFinder;
class ERmuSiMatcher;
class ERmuSiVertexFinder;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__muSiDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERmuSi header file                   -----
// -----                  Created data  by developer name              -----
// -------------------------------------------------------------------------

/**  ERmuSi.h
 **/


#ifndef ERmuSi_H
#define ERmuSi_H

#include "FairDetector.h"
#include "ERmuSiPoint.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERmuSi : public FairDetector
{
  
public:
  
  /** Default constructor **/
  ERmuSi();
  
  
  /** Standard constructor.
   *@param name    ERmuSi ERmuSi name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERmuSi(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERmuSi();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERmuSiPoint and adds it to the
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
   ** Constructs the ERmuSi geometry
   **/
  virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERmuSi data
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
  
private:
  TClonesArray*  fERmuSiPoints;     //!  The point collection
  Int_t fVersion;                    //! geometry version
private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
   
  ERmuSiPoint* AddPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Double_t mass,
			  TVector3 posIn,
			  TVector3 pos_out, TVector3 momIn,
			  TVector3 momOut, Double_t time,
			  Double_t length, Double_t eLoss, 
        Int_t station);
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  ClassDef(ERmuSi,1);
};

#endif 
// -------------------------------------------------------------------------
// -----                        ERmuSiHitProducer header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIHITPRODUCING_H
#define ERMUSIHITPRODUCING_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiHitProducer : public FairTask {

public:
  /** Default constructor **/
  ERmuSiHitProducer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERmuSiHitProducer(Int_t verbose);

  /** Destructor **/
  ~ERmuSiHitProducer();

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
  TClonesArray *fmuSiPoints;
  //Output arrays
  TClonesArray *fmuSiHits;

  static Int_t fEvent;
protected:
  ERmuSiHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station);
private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiHitProducer,1)
};

#endif
// -------------------------------------------------------------------------
// -----                        ERmuSiTrackFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSITRACKFINDER_H
#define ERMUSITRACKFINDER_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiHit.h"

class ERmuSiTrackFinder : public FairTask {

public:
  /** Default constructor **/
  ERmuSiTrackFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERmuSiTrackFinder(Int_t verbose);

  /** Destructor **/
  ~ERmuSiTrackFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetAngleCut(Double_t angle){fAngleCut = angle;}
  /** Accessors **/ 
protected:
  //Input arrays
  TClonesArray *fmuSiHits;

  //Output arrays
  TClonesArray *fmuSiTracks;
protected:

private:
  Double_t fAngleCut;

  virtual void SetParContainers();
  
  ClassDef(ERmuSiTrackFinder,1)
};

struct ERmuSiSinglet {
  ERmuSiHit* first;
  ERmuSiHit* second;
};

#endif
// -------------------------------------------------------------------------
// -----                        ERmuSiMatcher header file              -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIMATCHER_H
#define ERMUSIMATCHER_H

#include "TClonesArray.h"
#include "TH1F.h"

#include "FairTask.h"

class ERmuSiMatcher : public FairTask {

public:
  /** Default constructor **/
  ERmuSiMatcher();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERmuSiMatcher(Int_t verbose);

  /** Destructor **/
  ~ERmuSiMatcher();

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
  TClonesArray *fmuSiPoints;
  TClonesArray *fmuSiHits;
  TClonesArray *fmuSiTracks;
  TClonesArray *fmuSiVertecies;
  TClonesArray *fMCTracks;
  Int_t fTrueTracksNb;
  Int_t fWrongTracksNb;
  Int_t fMCTracksNb;
  Int_t fShortMCTracksNb;
  Int_t fNotFoundedVerteciesNb;

  //Histo
  TH1F* fHVertexDz;
  TH1F* fHVertexDxy;
protected:
  
private:
  virtual void SetParContainers();
  
  ClassDef(ERmuSiMatcher,1)
};

struct MCVertex{
  Float_t x;
  Float_t y;
  Float_t z;
  MCVertex(Float_t x_, Float_t y_, Float_t z_){x =x_;  y=y_; z=z_;}
};

#endif
// -------------------------------------------------------------------------
// -----                        ERmuSiVertexFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERMUSIVERTEXFINDER_H
#define ERMUSIVERTEXFINDER_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERmuSiVertex.h"

class ERmuSiVertexFinder : public FairTask {

public:
  /** Default constructor **/
  ERmuSiVertexFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERmuSiVertexFinder(Int_t verbose);

  /** Destructor **/
  ~ERmuSiVertexFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetTrackDistanceCut(Double_t dist){fDistanceCut = dist;}
  /** Accessors **/ 
protected:
  //Input arrays
  TClonesArray *fmuSiTracks;

  //Output arrays
  TClonesArray *fmuSiVertices;
  

  ERmuSiVertex* AddVertex(Float_t x, Float_t y, Float_t z);
protected:

private:
  Double_t fDistanceCut;

  virtual void SetParContainers();
  
  ClassDef(ERmuSiVertexFinder,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERmuSi", payloadCode, "@",
"ERmuSiHitProducer", payloadCode, "@",
"ERmuSiMatcher", payloadCode, "@",
"ERmuSiTrackFinder", payloadCode, "@",
"ERmuSiVertexFinder", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__muSiDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__muSiDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__muSiDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__muSiDict() {
  TriggerDictionaryInitialization_G__muSiDict_Impl();
}
