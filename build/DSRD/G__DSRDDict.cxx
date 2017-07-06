// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIDSRDdIG__DSRDDict

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
#include "ERDSRD.h"
#include "ERDSRDHitFinder.h"
#include "ERDSRDSetup.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERDSRD(void *p = 0);
   static void *newArray_ERDSRD(Long_t size, void *p);
   static void delete_ERDSRD(void *p);
   static void deleteArray_ERDSRD(void *p);
   static void destruct_ERDSRD(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDSRD*)
   {
      ::ERDSRD *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDSRD >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDSRD", ::ERDSRD::Class_Version(), "invalid", 27,
                  typeid(::ERDSRD), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDSRD::Dictionary, isa_proxy, 4,
                  sizeof(::ERDSRD) );
      instance.SetNew(&new_ERDSRD);
      instance.SetNewArray(&newArray_ERDSRD);
      instance.SetDelete(&delete_ERDSRD);
      instance.SetDeleteArray(&deleteArray_ERDSRD);
      instance.SetDestructor(&destruct_ERDSRD);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDSRD*)
   {
      return GenerateInitInstanceLocal((::ERDSRD*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDSRD*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDSRDHitFinder(void *p = 0);
   static void *newArray_ERDSRDHitFinder(Long_t size, void *p);
   static void delete_ERDSRDHitFinder(void *p);
   static void deleteArray_ERDSRDHitFinder(void *p);
   static void destruct_ERDSRDHitFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDSRDHitFinder*)
   {
      ::ERDSRDHitFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDSRDHitFinder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDSRDHitFinder", ::ERDSRDHitFinder::Class_Version(), "invalid", 174,
                  typeid(::ERDSRDHitFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDSRDHitFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ERDSRDHitFinder) );
      instance.SetNew(&new_ERDSRDHitFinder);
      instance.SetNewArray(&newArray_ERDSRDHitFinder);
      instance.SetDelete(&delete_ERDSRDHitFinder);
      instance.SetDeleteArray(&deleteArray_ERDSRDHitFinder);
      instance.SetDestructor(&destruct_ERDSRDHitFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDSRDHitFinder*)
   {
      return GenerateInitInstanceLocal((::ERDSRDHitFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDSRDHitFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_ERDSRDSetup(void *p);
   static void deleteArray_ERDSRDSetup(void *p);
   static void destruct_ERDSRDSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDSRDSetup*)
   {
      ::ERDSRDSetup *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDSRDSetup >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDSRDSetup", ::ERDSRDSetup::Class_Version(), "ERDSRDSetup.h", 16,
                  typeid(::ERDSRDSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDSRDSetup::Dictionary, isa_proxy, 4,
                  sizeof(::ERDSRDSetup) );
      instance.SetDelete(&delete_ERDSRDSetup);
      instance.SetDeleteArray(&deleteArray_ERDSRDSetup);
      instance.SetDestructor(&destruct_ERDSRDSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDSRDSetup*)
   {
      return GenerateInitInstanceLocal((::ERDSRDSetup*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDSRDSetup*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERDSRD::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDSRD::Class_Name()
{
   return "ERDSRD";
}

//______________________________________________________________________________
const char *ERDSRD::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRD*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDSRD::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRD*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDSRD::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRD*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDSRD::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRD*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDSRDHitFinder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDSRDHitFinder::Class_Name()
{
   return "ERDSRDHitFinder";
}

//______________________________________________________________________________
const char *ERDSRDHitFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHitFinder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDSRDHitFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHitFinder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDSRDHitFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHitFinder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDSRDHitFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHitFinder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDSRDSetup::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDSRDSetup::Class_Name()
{
   return "ERDSRDSetup";
}

//______________________________________________________________________________
const char *ERDSRDSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDSetup*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDSRDSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDSetup*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDSRDSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDSetup*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDSRDSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDSetup*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERDSRD::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDSRD.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDSRD::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDSRD::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDSRD(void *p) {
      return  p ? new(p) ::ERDSRD : new ::ERDSRD;
   }
   static void *newArray_ERDSRD(Long_t nElements, void *p) {
      return p ? new(p) ::ERDSRD[nElements] : new ::ERDSRD[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDSRD(void *p) {
      delete ((::ERDSRD*)p);
   }
   static void deleteArray_ERDSRD(void *p) {
      delete [] ((::ERDSRD*)p);
   }
   static void destruct_ERDSRD(void *p) {
      typedef ::ERDSRD current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDSRD

//______________________________________________________________________________
void ERDSRDHitFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDSRDHitFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDSRDHitFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDSRDHitFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDSRDHitFinder(void *p) {
      return  p ? new(p) ::ERDSRDHitFinder : new ::ERDSRDHitFinder;
   }
   static void *newArray_ERDSRDHitFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ERDSRDHitFinder[nElements] : new ::ERDSRDHitFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDSRDHitFinder(void *p) {
      delete ((::ERDSRDHitFinder*)p);
   }
   static void deleteArray_ERDSRDHitFinder(void *p) {
      delete [] ((::ERDSRDHitFinder*)p);
   }
   static void destruct_ERDSRDHitFinder(void *p) {
      typedef ::ERDSRDHitFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDSRDHitFinder

//______________________________________________________________________________
void ERDSRDSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDSRDSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDSRDSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDSRDSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERDSRDSetup(void *p) {
      delete ((::ERDSRDSetup*)p);
   }
   static void deleteArray_ERDSRDSetup(void *p) {
      delete [] ((::ERDSRDSetup*)p);
   }
   static void destruct_ERDSRDSetup(void *p) {
      typedef ::ERDSRDSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDSRDSetup

namespace {
  void TriggerDictionaryInitialization_G__DSRDDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/home/muzalevsky/check/data/DSRDData/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/base/",
"/home/muzalevsky/check/data",
"/home/muzalevsky/check/DSRD/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/DSRD/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__DSRDDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERDSRD;
class ERDSRDHitFinder;
class __attribute__((annotate("$clingAutoload$ERDSRDSetup.h")))  ERDSRDSetup;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__DSRDDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERDSRD header file                   -----
// -----                  Created data  by developer name              -----
// -------------------------------------------------------------------------

/**  ERDSRD.h
 **/


#ifndef ERDSRD_H
#define ERDSRD_H

#include "ERDetector.h"
#include "ERDSRDPoint.h"

#include "TLorentzVector.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERDSRD : public FairDetector
{
  
public:
  
  /** Default constructor **/
  ERDSRD();
  
  
  /** Standard constructor.
   *@param name    ERDSRD ERDSRD name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 3 - GEANT Step information
   **/
  ERDSRD(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERDSRD();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERDSRDPoint and adds it to the
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
   ** Constructs the ERDSRD geometry
   **/
  virtual void ConstructGeometry();
  
   /** Virtaul method Initialize
   **
   ** Initialize ERDSRD data
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
  TClonesArray*  fDSRDPoints;     //!  The point collection
  Int_t fVersion;                    //! geometry version
private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
  
  ERDSRDPoint* AddPoint(Int_t eventID, Int_t trackID,
			  Int_t mot0trackID,
			  Double_t mass,
			  TVector3 posIn,
			  TVector3 pos_out, TVector3 momIn,
			  TVector3 momOut, Double_t time,
			  Double_t length, Double_t eLoss,
        Int_t sector,Int_t sensor);
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  ClassDef(ERDSRD,1);
};

#endif 
// -------------------------------------------------------------------------
// -----                        ERDSRDHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERDSRDHitFinder_H
#define ERDSRDHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERDSRDHit.h"
#include "ERDSRDSetup.h"

class ERDSRDHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERDSRDHitFinder();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERDSRDHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERDSRDHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetElossDispersion(Float_t sigma) {fElossDispersion = sigma;} 
  void SetTimeDispersionPar(Float_t sigma) {fTimeDispersionPar = sigma;}
  void SetElossThreshold(Float_t th){fElossThreshold = th;}
  /** Accessors **/ 
  Float_t ElossDispersion() const {return fElossDispersion;}
  Float_t TimeDispersionPar() const {return fTimeDispersionPar;}
protected:
  //Input arrays
  TClonesArray *fDSRDPoints;
  //Output arrays
  TClonesArray *fDSRDHits;

  static Int_t fEvent;
  Float_t fElossDispersion;
  Float_t fTimeDispersionPar;
  Float_t fElossThreshold;

  ERDSRDSetup* fDSRDSetup;
protected:
  ERDSRDHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERDSRDHitFinder,1)
};

#endif
// -------------------------------------------------------------------------

// -----                        ERDSRDSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNDSETUP_H
#define ERNDSETUP_H

#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoNode.h"

class ERDSRDSetup {
    static ERDSRDSetup* fInstance;
    static TGeoNode* fDSRDnode;
    static Float_t fZ;
    static Float_t fRmin, fRmax;
    static Int_t fSecNb, fSenNb;
    ERDSRDSetup();
public:
    static ERDSRDSetup* Instance();
    
    Float_t SensorNb() {return fSenNb;}
    Float_t SectorNb() {return fSecNb;}
    Float_t Rmin() {return fRmin;}
    Float_t Rmax() {return fRmax;}
    Float_t Z() {return fZ;}

    void Print();

    ClassDef(ERDSRDSetup,1)
};

#endif


#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERDSRD", payloadCode, "@",
"ERDSRDHitFinder", payloadCode, "@",
"ERDSRDSetup", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__DSRDDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__DSRDDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__DSRDDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__DSRDDict() {
  TriggerDictionaryInitialization_G__DSRDDict_Impl();
}
