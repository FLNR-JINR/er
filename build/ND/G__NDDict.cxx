// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddINDdIG__NDDict

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
#include "ERND.h"
#include "ERNDHitFinder.h"
#include "ERNDSetup.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERND(void *p = 0);
   static void *newArray_ERND(Long_t size, void *p);
   static void delete_ERND(void *p);
   static void deleteArray_ERND(void *p);
   static void destruct_ERND(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERND*)
   {
      ::ERND *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERND >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERND", ::ERND::Class_Version(), "invalid", 27,
                  typeid(::ERND), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERND::Dictionary, isa_proxy, 4,
                  sizeof(::ERND) );
      instance.SetNew(&new_ERND);
      instance.SetNewArray(&newArray_ERND);
      instance.SetDelete(&delete_ERND);
      instance.SetDeleteArray(&deleteArray_ERND);
      instance.SetDestructor(&destruct_ERND);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERND*)
   {
      return GenerateInitInstanceLocal((::ERND*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERND*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNDHitFinder(void *p = 0);
   static void *newArray_ERNDHitFinder(Long_t size, void *p);
   static void delete_ERNDHitFinder(void *p);
   static void deleteArray_ERNDHitFinder(void *p);
   static void destruct_ERNDHitFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNDHitFinder*)
   {
      ::ERNDHitFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNDHitFinder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNDHitFinder", ::ERNDHitFinder::Class_Version(), "invalid", 173,
                  typeid(::ERNDHitFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNDHitFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ERNDHitFinder) );
      instance.SetNew(&new_ERNDHitFinder);
      instance.SetNewArray(&newArray_ERNDHitFinder);
      instance.SetDelete(&delete_ERNDHitFinder);
      instance.SetDeleteArray(&deleteArray_ERNDHitFinder);
      instance.SetDestructor(&destruct_ERNDHitFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNDHitFinder*)
   {
      return GenerateInitInstanceLocal((::ERNDHitFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNDHitFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_ERNDSetup(void *p);
   static void deleteArray_ERNDSetup(void *p);
   static void destruct_ERNDSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNDSetup*)
   {
      ::ERNDSetup *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNDSetup >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNDSetup", ::ERNDSetup::Class_Version(), "ERNDSetup.h", 16,
                  typeid(::ERNDSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNDSetup::Dictionary, isa_proxy, 4,
                  sizeof(::ERNDSetup) );
      instance.SetDelete(&delete_ERNDSetup);
      instance.SetDeleteArray(&deleteArray_ERNDSetup);
      instance.SetDestructor(&destruct_ERNDSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNDSetup*)
   {
      return GenerateInitInstanceLocal((::ERNDSetup*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNDSetup*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERND::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERND::Class_Name()
{
   return "ERND";
}

//______________________________________________________________________________
const char *ERND::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERND*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERND::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERND*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERND::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERND*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERND::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERND*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNDHitFinder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNDHitFinder::Class_Name()
{
   return "ERNDHitFinder";
}

//______________________________________________________________________________
const char *ERNDHitFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDHitFinder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNDHitFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDHitFinder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNDHitFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDHitFinder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNDHitFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDHitFinder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNDSetup::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNDSetup::Class_Name()
{
   return "ERNDSetup";
}

//______________________________________________________________________________
const char *ERNDSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDSetup*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNDSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDSetup*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNDSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDSetup*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNDSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDSetup*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERND::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERND.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERND::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERND::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERND(void *p) {
      return  p ? new(p) ::ERND : new ::ERND;
   }
   static void *newArray_ERND(Long_t nElements, void *p) {
      return p ? new(p) ::ERND[nElements] : new ::ERND[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERND(void *p) {
      delete ((::ERND*)p);
   }
   static void deleteArray_ERND(void *p) {
      delete [] ((::ERND*)p);
   }
   static void destruct_ERND(void *p) {
      typedef ::ERND current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERND

//______________________________________________________________________________
void ERNDHitFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNDHitFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNDHitFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNDHitFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNDHitFinder(void *p) {
      return  p ? new(p) ::ERNDHitFinder : new ::ERNDHitFinder;
   }
   static void *newArray_ERNDHitFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ERNDHitFinder[nElements] : new ::ERNDHitFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNDHitFinder(void *p) {
      delete ((::ERNDHitFinder*)p);
   }
   static void deleteArray_ERNDHitFinder(void *p) {
      delete [] ((::ERNDHitFinder*)p);
   }
   static void destruct_ERNDHitFinder(void *p) {
      typedef ::ERNDHitFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNDHitFinder

//______________________________________________________________________________
void ERNDSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNDSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNDSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNDSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERNDSetup(void *p) {
      delete ((::ERNDSetup*)p);
   }
   static void deleteArray_ERNDSetup(void *p) {
      delete [] ((::ERNDSetup*)p);
   }
   static void destruct_ERNDSetup(void *p) {
      typedef ::ERNDSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNDSetup

namespace {
  void TriggerDictionaryInitialization_G__NDDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/home/muzalevsky/check/data/NDData/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/base/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/ND/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/ND/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NDDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERND;
class ERNDHitFinder;
class __attribute__((annotate("$clingAutoload$ERNDSetup.h")))  ERNDSetup;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NDDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERND header file                   -----
// -----                  Created data  by developer name              -----
// -------------------------------------------------------------------------

/**  ERND.h
 **/


#ifndef ERND_H
#define ERND_H

#include "ERDetector.h"
#include "ERNDPoint.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERND : public FairDetector
{
  
public:
  
  /** Default constructor **/
  ERND();
  
  
  /** Standard constructor.
   *@param name    ERND ERND name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERND(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERND();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERNDPoint and adds it to the
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
   ** Constructs the ERND geometry
   **/
  virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERND data
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
  TClonesArray*  fNDPoints;     //!  The point collection
  Int_t fVersion;                    //! geometry version
private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
  
  ERNDPoint* AddPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Double_t mass,
			  TVector3 posIn,
			  TVector3 pos_out, TVector3 momIn,
			  TVector3 momOut, Double_t time,
			  Double_t length, Double_t eLoss, Int_t stilbenNr,Float_t lightYield);
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  ClassDef(ERND,1);
};

#endif 
// -------------------------------------------------------------------------
// -----                        ERNDHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNDHitFinder_H
#define ERNDHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNDHit.h"
#include "ERNDSetup.h"

class ERNDHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERNDHitFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNDHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERNDHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetLYDispersionA(Float_t sigma) {fLYDispersionA = sigma;} 
  void SetLYDispersionB(Float_t sigma) {fLYDispersionB = sigma;} 
  void SetTimeDispersionPar(Float_t sigma) {fTimeDispersionPar = sigma;}
  void SetQuenchThreshold(Float_t th) {fQuenchThreshold = th;}
  void SetLYThreshold(Float_t th) {fLYThreshold = th;}
  void SetProbabilityB(Float_t b) {fProbabilityB = b;}
  void SetProbabilityC(Float_t c) {fProbabilityC = c;}
  /** Accessors **/ 
  Float_t LYDispersionA() const {return fLYDispersionA;}
  Float_t LYDispersionB() const {return fLYDispersionB;}
  Float_t TimeDispersionPar() const {return fTimeDispersionPar;}
  Float_t ElossThreshold() const {return fQuenchThreshold;}
  Float_t LYThreshold() const {return fLYThreshold;}
  Float_t ProbabilityB() const {return fProbabilityB;}
  Float_t ProbabilityC() const {return fProbabilityC;}
protected:
  //Input arrays
  TClonesArray *fNDPoints;
  //Output arrays
  TClonesArray *fNDHits;

  static Int_t fEvent;
  Float_t fLYDispersionA;
  Float_t fLYDispersionB;
  Float_t fTimeDispersionPar;
  Float_t fQuenchThreshold;
  Float_t fLYThreshold;
  Float_t fProbabilityB;
  Float_t fProbabilityC;

  ERNDSetup* fSetup;
protected:
  ERNDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, 
          Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNDHitFinder,1)
};

#endif
// -------------------------------------------------------------------------

// -----                        ERNDSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNDSETUP_H
#define ERNDSETUP_H

#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoNode.h"

class ERNDSetup {
    static ERNDSetup* fInstance;
    static TGeoNode* fNDnode;
    ERNDSetup();
public:
    static ERNDSetup* Instance();
    ClassDef(ERNDSetup,1)

    void PMTPos(TVector3 pos, TVector3& pmtPos);
};

#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERND", payloadCode, "@",
"ERNDHitFinder", payloadCode, "@",
"ERNDSetup", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NDDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NDDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NDDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NDDict() {
  TriggerDictionaryInitialization_G__NDDict_Impl();
}
