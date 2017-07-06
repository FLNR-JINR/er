// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddINeuRaddIG__NeuRadDict

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
#include "ERNeuRad.h"
#include "ERNeuRadDigitizer.h"
#include "ERNeuRadContFact.h"
#include "ERNeuRadDigiPar.h"
#include "ERNeuRadGeoPar.h"
#include "ERNeuRadSetup.h"
#include "ERNeuRadHitFinder.h"
#include "ERNeuRadHitFinderMF.h"
#include "ERNeuRadHitFinderWBT.h"
#include "ERNeuRadMatcher.h"
#include "ERNeuRadViewer.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERNeuRad(void *p = 0);
   static void *newArray_ERNeuRad(Long_t size, void *p);
   static void delete_ERNeuRad(void *p);
   static void deleteArray_ERNeuRad(void *p);
   static void destruct_ERNeuRad(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRad*)
   {
      ::ERNeuRad *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRad >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRad", ::ERNeuRad::Class_Version(), "invalid", 28,
                  typeid(::ERNeuRad), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRad::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRad) );
      instance.SetNew(&new_ERNeuRad);
      instance.SetNewArray(&newArray_ERNeuRad);
      instance.SetDelete(&delete_ERNeuRad);
      instance.SetDeleteArray(&deleteArray_ERNeuRad);
      instance.SetDestructor(&destruct_ERNeuRad);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRad*)
   {
      return GenerateInitInstanceLocal((::ERNeuRad*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRad*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadDigitizer(void *p = 0);
   static void *newArray_ERNeuRadDigitizer(Long_t size, void *p);
   static void delete_ERNeuRadDigitizer(void *p);
   static void deleteArray_ERNeuRadDigitizer(void *p);
   static void destruct_ERNeuRadDigitizer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadDigitizer*)
   {
      ::ERNeuRadDigitizer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadDigitizer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadDigitizer", ::ERNeuRadDigitizer::Class_Version(), "invalid", 224,
                  typeid(::ERNeuRadDigitizer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadDigitizer::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadDigitizer) );
      instance.SetNew(&new_ERNeuRadDigitizer);
      instance.SetNewArray(&newArray_ERNeuRadDigitizer);
      instance.SetDelete(&delete_ERNeuRadDigitizer);
      instance.SetDeleteArray(&deleteArray_ERNeuRadDigitizer);
      instance.SetDestructor(&destruct_ERNeuRadDigitizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadDigitizer*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadDigitizer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadDigitizer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadContFact(void *p = 0);
   static void *newArray_ERNeuRadContFact(Long_t size, void *p);
   static void delete_ERNeuRadContFact(void *p);
   static void deleteArray_ERNeuRadContFact(void *p);
   static void destruct_ERNeuRadContFact(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadContFact*)
   {
      ::ERNeuRadContFact *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadContFact >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadContFact", ::ERNeuRadContFact::Class_Version(), "invalid", 329,
                  typeid(::ERNeuRadContFact), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadContFact::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadContFact) );
      instance.SetNew(&new_ERNeuRadContFact);
      instance.SetNewArray(&newArray_ERNeuRadContFact);
      instance.SetDelete(&delete_ERNeuRadContFact);
      instance.SetDeleteArray(&deleteArray_ERNeuRadContFact);
      instance.SetDestructor(&destruct_ERNeuRadContFact);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadContFact*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadContFact*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadContFact*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadDigiPar(void *p = 0);
   static void *newArray_ERNeuRadDigiPar(Long_t size, void *p);
   static void delete_ERNeuRadDigiPar(void *p);
   static void deleteArray_ERNeuRadDigiPar(void *p);
   static void destruct_ERNeuRadDigiPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadDigiPar*)
   {
      ::ERNeuRadDigiPar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadDigiPar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadDigiPar", ::ERNeuRadDigiPar::Class_Version(), "ERNeuRadDigiPar.h", 23,
                  typeid(::ERNeuRadDigiPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadDigiPar::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadDigiPar) );
      instance.SetNew(&new_ERNeuRadDigiPar);
      instance.SetNewArray(&newArray_ERNeuRadDigiPar);
      instance.SetDelete(&delete_ERNeuRadDigiPar);
      instance.SetDeleteArray(&deleteArray_ERNeuRadDigiPar);
      instance.SetDestructor(&destruct_ERNeuRadDigiPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadDigiPar*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadDigiPar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadDigiPar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadGeoPar(void *p = 0);
   static void *newArray_ERNeuRadGeoPar(Long_t size, void *p);
   static void delete_ERNeuRadGeoPar(void *p);
   static void deleteArray_ERNeuRadGeoPar(void *p);
   static void destruct_ERNeuRadGeoPar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadGeoPar*)
   {
      ::ERNeuRadGeoPar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadGeoPar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadGeoPar", ::ERNeuRadGeoPar::Class_Version(), "invalid", 439,
                  typeid(::ERNeuRadGeoPar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadGeoPar::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadGeoPar) );
      instance.SetNew(&new_ERNeuRadGeoPar);
      instance.SetNewArray(&newArray_ERNeuRadGeoPar);
      instance.SetDelete(&delete_ERNeuRadGeoPar);
      instance.SetDeleteArray(&deleteArray_ERNeuRadGeoPar);
      instance.SetDestructor(&destruct_ERNeuRadGeoPar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadGeoPar*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadGeoPar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadGeoPar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_ERNeuRadSetup(void *p);
   static void deleteArray_ERNeuRadSetup(void *p);
   static void destruct_ERNeuRadSetup(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadSetup*)
   {
      ::ERNeuRadSetup *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadSetup >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadSetup", ::ERNeuRadSetup::Class_Version(), "ERNeuRadSetup.h", 30,
                  typeid(::ERNeuRadSetup), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadSetup::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadSetup) );
      instance.SetDelete(&delete_ERNeuRadSetup);
      instance.SetDeleteArray(&deleteArray_ERNeuRadSetup);
      instance.SetDestructor(&destruct_ERNeuRadSetup);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadSetup*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadSetup*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadSetup*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadHitFinder(void *p = 0);
   static void *newArray_ERNeuRadHitFinder(Long_t size, void *p);
   static void delete_ERNeuRadHitFinder(void *p);
   static void deleteArray_ERNeuRadHitFinder(void *p);
   static void destruct_ERNeuRadHitFinder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadHitFinder*)
   {
      ::ERNeuRadHitFinder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadHitFinder >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadHitFinder", ::ERNeuRadHitFinder::Class_Version(), "invalid", 553,
                  typeid(::ERNeuRadHitFinder), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadHitFinder::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadHitFinder) );
      instance.SetNew(&new_ERNeuRadHitFinder);
      instance.SetNewArray(&newArray_ERNeuRadHitFinder);
      instance.SetDelete(&delete_ERNeuRadHitFinder);
      instance.SetDeleteArray(&deleteArray_ERNeuRadHitFinder);
      instance.SetDestructor(&destruct_ERNeuRadHitFinder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadHitFinder*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadHitFinder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadHitFinder*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadMatcher(void *p = 0);
   static void *newArray_ERNeuRadMatcher(Long_t size, void *p);
   static void delete_ERNeuRadMatcher(void *p);
   static void deleteArray_ERNeuRadMatcher(void *p);
   static void destruct_ERNeuRadMatcher(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadMatcher*)
   {
      ::ERNeuRadMatcher *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadMatcher >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadMatcher", ::ERNeuRadMatcher::Class_Version(), "invalid", 740,
                  typeid(::ERNeuRadMatcher), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadMatcher::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadMatcher) );
      instance.SetNew(&new_ERNeuRadMatcher);
      instance.SetNewArray(&newArray_ERNeuRadMatcher);
      instance.SetDelete(&delete_ERNeuRadMatcher);
      instance.SetDeleteArray(&deleteArray_ERNeuRadMatcher);
      instance.SetDestructor(&destruct_ERNeuRadMatcher);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadMatcher*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadMatcher*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadMatcher*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadHitFinderMF(void *p = 0);
   static void *newArray_ERNeuRadHitFinderMF(Long_t size, void *p);
   static void delete_ERNeuRadHitFinderMF(void *p);
   static void deleteArray_ERNeuRadHitFinderMF(void *p);
   static void destruct_ERNeuRadHitFinderMF(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadHitFinderMF*)
   {
      ::ERNeuRadHitFinderMF *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadHitFinderMF >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadHitFinderMF", ::ERNeuRadHitFinderMF::Class_Version(), "invalid", 616,
                  typeid(::ERNeuRadHitFinderMF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadHitFinderMF::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadHitFinderMF) );
      instance.SetNew(&new_ERNeuRadHitFinderMF);
      instance.SetNewArray(&newArray_ERNeuRadHitFinderMF);
      instance.SetDelete(&delete_ERNeuRadHitFinderMF);
      instance.SetDeleteArray(&deleteArray_ERNeuRadHitFinderMF);
      instance.SetDestructor(&destruct_ERNeuRadHitFinderMF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadHitFinderMF*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadHitFinderMF*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadHitFinderMF*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadHitFinderWBT(void *p = 0);
   static void *newArray_ERNeuRadHitFinderWBT(Long_t size, void *p);
   static void delete_ERNeuRadHitFinderWBT(void *p);
   static void deleteArray_ERNeuRadHitFinderWBT(void *p);
   static void destruct_ERNeuRadHitFinderWBT(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadHitFinderWBT*)
   {
      ::ERNeuRadHitFinderWBT *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadHitFinderWBT >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadHitFinderWBT", ::ERNeuRadHitFinderWBT::Class_Version(), "invalid", 681,
                  typeid(::ERNeuRadHitFinderWBT), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadHitFinderWBT::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadHitFinderWBT) );
      instance.SetNew(&new_ERNeuRadHitFinderWBT);
      instance.SetNewArray(&newArray_ERNeuRadHitFinderWBT);
      instance.SetDelete(&delete_ERNeuRadHitFinderWBT);
      instance.SetDeleteArray(&deleteArray_ERNeuRadHitFinderWBT);
      instance.SetDestructor(&destruct_ERNeuRadHitFinderWBT);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadHitFinderWBT*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadHitFinderWBT*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadHitFinderWBT*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadViewer(void *p = 0);
   static void *newArray_ERNeuRadViewer(Long_t size, void *p);
   static void delete_ERNeuRadViewer(void *p);
   static void deleteArray_ERNeuRadViewer(void *p);
   static void destruct_ERNeuRadViewer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadViewer*)
   {
      ::ERNeuRadViewer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadViewer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadViewer", ::ERNeuRadViewer::Class_Version(), "invalid", 801,
                  typeid(::ERNeuRadViewer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadViewer::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadViewer) );
      instance.SetNew(&new_ERNeuRadViewer);
      instance.SetNewArray(&newArray_ERNeuRadViewer);
      instance.SetDelete(&delete_ERNeuRadViewer);
      instance.SetDeleteArray(&deleteArray_ERNeuRadViewer);
      instance.SetDestructor(&destruct_ERNeuRadViewer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadViewer*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadViewer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadViewer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRad::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRad::Class_Name()
{
   return "ERNeuRad";
}

//______________________________________________________________________________
const char *ERNeuRad::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRad*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRad::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRad*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRad::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRad*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRad::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRad*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadDigitizer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadDigitizer::Class_Name()
{
   return "ERNeuRadDigitizer";
}

//______________________________________________________________________________
const char *ERNeuRadDigitizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigitizer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadDigitizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigitizer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadDigitizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigitizer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadDigitizer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigitizer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadContFact::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadContFact::Class_Name()
{
   return "ERNeuRadContFact";
}

//______________________________________________________________________________
const char *ERNeuRadContFact::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadContFact*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadContFact::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadContFact*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadContFact::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadContFact*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadContFact::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadContFact*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadDigiPar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadDigiPar::Class_Name()
{
   return "ERNeuRadDigiPar";
}

//______________________________________________________________________________
const char *ERNeuRadDigiPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigiPar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadDigiPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigiPar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadDigiPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigiPar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadDigiPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigiPar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadGeoPar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadGeoPar::Class_Name()
{
   return "ERNeuRadGeoPar";
}

//______________________________________________________________________________
const char *ERNeuRadGeoPar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadGeoPar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadGeoPar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadGeoPar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadGeoPar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadGeoPar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadGeoPar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadGeoPar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadSetup::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadSetup::Class_Name()
{
   return "ERNeuRadSetup";
}

//______________________________________________________________________________
const char *ERNeuRadSetup::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadSetup*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadSetup::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadSetup*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadSetup::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadSetup*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadSetup::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadSetup*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadHitFinder::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadHitFinder::Class_Name()
{
   return "ERNeuRadHitFinder";
}

//______________________________________________________________________________
const char *ERNeuRadHitFinder::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinder*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadHitFinder::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinder*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinder::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinder*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinder::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinder*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadMatcher::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadMatcher::Class_Name()
{
   return "ERNeuRadMatcher";
}

//______________________________________________________________________________
const char *ERNeuRadMatcher::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadMatcher*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadMatcher::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadMatcher*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadMatcher::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadMatcher*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadMatcher::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadMatcher*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadHitFinderMF::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadHitFinderMF::Class_Name()
{
   return "ERNeuRadHitFinderMF";
}

//______________________________________________________________________________
const char *ERNeuRadHitFinderMF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderMF*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadHitFinderMF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderMF*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinderMF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderMF*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinderMF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderMF*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadHitFinderWBT::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadHitFinderWBT::Class_Name()
{
   return "ERNeuRadHitFinderWBT";
}

//______________________________________________________________________________
const char *ERNeuRadHitFinderWBT::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderWBT*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadHitFinderWBT::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderWBT*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinderWBT::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderWBT*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadHitFinderWBT::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitFinderWBT*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadViewer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadViewer::Class_Name()
{
   return "ERNeuRadViewer";
}

//______________________________________________________________________________
const char *ERNeuRadViewer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadViewer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadViewer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadViewer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadViewer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadViewer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadViewer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadViewer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERNeuRad::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRad.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRad::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRad::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRad(void *p) {
      return  p ? new(p) ::ERNeuRad : new ::ERNeuRad;
   }
   static void *newArray_ERNeuRad(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRad[nElements] : new ::ERNeuRad[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRad(void *p) {
      delete ((::ERNeuRad*)p);
   }
   static void deleteArray_ERNeuRad(void *p) {
      delete [] ((::ERNeuRad*)p);
   }
   static void destruct_ERNeuRad(void *p) {
      typedef ::ERNeuRad current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRad

//______________________________________________________________________________
void ERNeuRadDigitizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadDigitizer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadDigitizer::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadDigitizer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadDigitizer(void *p) {
      return  p ? new(p) ::ERNeuRadDigitizer : new ::ERNeuRadDigitizer;
   }
   static void *newArray_ERNeuRadDigitizer(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadDigitizer[nElements] : new ::ERNeuRadDigitizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadDigitizer(void *p) {
      delete ((::ERNeuRadDigitizer*)p);
   }
   static void deleteArray_ERNeuRadDigitizer(void *p) {
      delete [] ((::ERNeuRadDigitizer*)p);
   }
   static void destruct_ERNeuRadDigitizer(void *p) {
      typedef ::ERNeuRadDigitizer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadDigitizer

//______________________________________________________________________________
void ERNeuRadContFact::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadContFact.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadContFact::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadContFact::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadContFact(void *p) {
      return  p ? new(p) ::ERNeuRadContFact : new ::ERNeuRadContFact;
   }
   static void *newArray_ERNeuRadContFact(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadContFact[nElements] : new ::ERNeuRadContFact[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadContFact(void *p) {
      delete ((::ERNeuRadContFact*)p);
   }
   static void deleteArray_ERNeuRadContFact(void *p) {
      delete [] ((::ERNeuRadContFact*)p);
   }
   static void destruct_ERNeuRadContFact(void *p) {
      typedef ::ERNeuRadContFact current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadContFact

//______________________________________________________________________________
void ERNeuRadDigiPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadDigiPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadDigiPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadDigiPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadDigiPar(void *p) {
      return  p ? new(p) ::ERNeuRadDigiPar : new ::ERNeuRadDigiPar;
   }
   static void *newArray_ERNeuRadDigiPar(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadDigiPar[nElements] : new ::ERNeuRadDigiPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadDigiPar(void *p) {
      delete ((::ERNeuRadDigiPar*)p);
   }
   static void deleteArray_ERNeuRadDigiPar(void *p) {
      delete [] ((::ERNeuRadDigiPar*)p);
   }
   static void destruct_ERNeuRadDigiPar(void *p) {
      typedef ::ERNeuRadDigiPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadDigiPar

//______________________________________________________________________________
void ERNeuRadGeoPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadGeoPar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadGeoPar::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadGeoPar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadGeoPar(void *p) {
      return  p ? new(p) ::ERNeuRadGeoPar : new ::ERNeuRadGeoPar;
   }
   static void *newArray_ERNeuRadGeoPar(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadGeoPar[nElements] : new ::ERNeuRadGeoPar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadGeoPar(void *p) {
      delete ((::ERNeuRadGeoPar*)p);
   }
   static void deleteArray_ERNeuRadGeoPar(void *p) {
      delete [] ((::ERNeuRadGeoPar*)p);
   }
   static void destruct_ERNeuRadGeoPar(void *p) {
      typedef ::ERNeuRadGeoPar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadGeoPar

//______________________________________________________________________________
void ERNeuRadSetup::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadSetup.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadSetup::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadSetup::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERNeuRadSetup(void *p) {
      delete ((::ERNeuRadSetup*)p);
   }
   static void deleteArray_ERNeuRadSetup(void *p) {
      delete [] ((::ERNeuRadSetup*)p);
   }
   static void destruct_ERNeuRadSetup(void *p) {
      typedef ::ERNeuRadSetup current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadSetup

//______________________________________________________________________________
void ERNeuRadHitFinder::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadHitFinder.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadHitFinder::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadHitFinder::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadHitFinder(void *p) {
      return  p ? new(p) ::ERNeuRadHitFinder : new ::ERNeuRadHitFinder;
   }
   static void *newArray_ERNeuRadHitFinder(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadHitFinder[nElements] : new ::ERNeuRadHitFinder[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadHitFinder(void *p) {
      delete ((::ERNeuRadHitFinder*)p);
   }
   static void deleteArray_ERNeuRadHitFinder(void *p) {
      delete [] ((::ERNeuRadHitFinder*)p);
   }
   static void destruct_ERNeuRadHitFinder(void *p) {
      typedef ::ERNeuRadHitFinder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadHitFinder

//______________________________________________________________________________
void ERNeuRadMatcher::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadMatcher.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadMatcher::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadMatcher::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadMatcher(void *p) {
      return  p ? new(p) ::ERNeuRadMatcher : new ::ERNeuRadMatcher;
   }
   static void *newArray_ERNeuRadMatcher(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadMatcher[nElements] : new ::ERNeuRadMatcher[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadMatcher(void *p) {
      delete ((::ERNeuRadMatcher*)p);
   }
   static void deleteArray_ERNeuRadMatcher(void *p) {
      delete [] ((::ERNeuRadMatcher*)p);
   }
   static void destruct_ERNeuRadMatcher(void *p) {
      typedef ::ERNeuRadMatcher current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadMatcher

//______________________________________________________________________________
void ERNeuRadHitFinderMF::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadHitFinderMF.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadHitFinderMF::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadHitFinderMF::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadHitFinderMF(void *p) {
      return  p ? new(p) ::ERNeuRadHitFinderMF : new ::ERNeuRadHitFinderMF;
   }
   static void *newArray_ERNeuRadHitFinderMF(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadHitFinderMF[nElements] : new ::ERNeuRadHitFinderMF[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadHitFinderMF(void *p) {
      delete ((::ERNeuRadHitFinderMF*)p);
   }
   static void deleteArray_ERNeuRadHitFinderMF(void *p) {
      delete [] ((::ERNeuRadHitFinderMF*)p);
   }
   static void destruct_ERNeuRadHitFinderMF(void *p) {
      typedef ::ERNeuRadHitFinderMF current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadHitFinderMF

//______________________________________________________________________________
void ERNeuRadHitFinderWBT::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadHitFinderWBT.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadHitFinderWBT::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadHitFinderWBT::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadHitFinderWBT(void *p) {
      return  p ? new(p) ::ERNeuRadHitFinderWBT : new ::ERNeuRadHitFinderWBT;
   }
   static void *newArray_ERNeuRadHitFinderWBT(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadHitFinderWBT[nElements] : new ::ERNeuRadHitFinderWBT[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadHitFinderWBT(void *p) {
      delete ((::ERNeuRadHitFinderWBT*)p);
   }
   static void deleteArray_ERNeuRadHitFinderWBT(void *p) {
      delete [] ((::ERNeuRadHitFinderWBT*)p);
   }
   static void destruct_ERNeuRadHitFinderWBT(void *p) {
      typedef ::ERNeuRadHitFinderWBT current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadHitFinderWBT

//______________________________________________________________________________
void ERNeuRadViewer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadViewer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadViewer::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadViewer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadViewer(void *p) {
      return  p ? new(p) ::ERNeuRadViewer : new ::ERNeuRadViewer;
   }
   static void *newArray_ERNeuRadViewer(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadViewer[nElements] : new ::ERNeuRadViewer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadViewer(void *p) {
      delete ((::ERNeuRadViewer*)p);
   }
   static void deleteArray_ERNeuRadViewer(void *p) {
      delete [] ((::ERNeuRadViewer*)p);
   }
   static void destruct_ERNeuRadViewer(void *p) {
      typedef ::ERNeuRadViewer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadViewer

namespace {
  void TriggerDictionaryInitialization_G__NeuRadDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/data/NeuRadData/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/NeuRad/",
"/home/muzalevsky/check/base/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/NeuRad/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__NeuRadDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERNeuRad;
class ERNeuRadDigitizer;
class ERNeuRadContFact;
class __attribute__((annotate("$clingAutoload$ERNeuRadSetup.h")))  ERNeuRadDigiPar;
class ERNeuRadGeoPar;
class __attribute__((annotate("$clingAutoload$ERNeuRadSetup.h")))  ERNeuRadSetup;
class ERNeuRadHitFinder;
class ERNeuRadMatcher;
class ERNeuRadHitFinderMF;
class ERNeuRadHitFinderWBT;
class ERNeuRadViewer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__NeuRadDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                        ERNeuRad header file               -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRad_H
#define ERNeuRad_H

#include "TH1F.h"
#include "TLorentzVector.h"

using namespace std;


#include "ERDetector.h"
#include "ERNeuRadStep.h"
#include "ERNeuRadPoint.h"

class TClonesArray;
class FairVolume;
class TF1;

class ERNeuRad : public ERDetector
{
  
public:
  
  /** Default constructor **/
  ERNeuRad();
  
  
  /** Standard constructor.
   *@param name    ERNeuRad detector name
   *@param active  sensitivity flag
   *@param verbose Verbosity level. 1 - only standart logs, 2 - Print points after each event, 
                                    3 - GEANT Step information
   **/
  ERNeuRad(const char* name, Bool_t active, Int_t verbose);
  
  
  /** Destructor **/
  virtual ~ERNeuRad();
  
  
  /** Virtual method ProcessHits
   **   
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a ERNeuRadPoint and adds it to the
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
  
   /** Virtaul method Initialize
   **
   ** Initialize detector data
   **/
  virtual void Initialize();

  /** Virtaul method CheckIfSensitive 
	**Check whether a volume is sensitive.
  ** @param(name)  Volume name
  ** @value        kTRUE if volume is sensitive, else kFALSE
  **
  ** The decision is based on the volume name (has to contain "fiber").
  **/
  virtual Bool_t CheckIfSensitive(std::string name);
  
  /** Virtaul method SetGeomVersion
  **/
  void SetGeomVersion(Int_t vers ) { fVersion = vers; }
  
  void SetStorePrimarySteps() {fStorePrimarySteps = kTRUE;}
  void SetStoreAllSteps() {fStoreAllSteps = kTRUE;}

  void WriteHistos();
private:
  TClonesArray*  fNeuRadPoints;     //!  The point collection
  TClonesArray*  fNeuRadFirstStep;  //!  The first step collection
  TClonesArray*  fNeuRadSteps;      //!  The steps collection
  Int_t fVersion;                    //! geometry version
    
  Bool_t fStorePrimarySteps;
  Bool_t fStoreAllSteps;
  Int_t   fVerbose;

  Float_t fFullEnergy;
  Float_t fFullLY;

  /* current step information*/
  Int_t fEventID;                   //!  event index
  Int_t fTrackID;                   //!  track index
  Int_t fMot0TrackID;               //!  0th mother track index
  Double_t fMass;                   //!  mass
  TLorentzVector fPosIn;            //!  point start  position
  TLorentzVector fPosOut;            //!  point finish  position
  TLorentzVector fCurPosIn;         //!  current step position
  TVector3 fPosInLocal;             //!  position
  TLorentzVector fMomIn;            //!  point start momentum
  TLorentzVector fMomOut;            //!  point start momentum
  TLorentzVector fCurMomIn;         //!  current step momentum
  Double32_t     fTimeIn;           //!  point start time
  Double32_t     fTimeOut;           //!  point finish time
  Double32_t     fTrackLength;      //!  track length from his origin
  Double32_t     fELoss;            //!  energy loss
  Double_t       fLightYield;       //!  light yield
  Int_t          fFiberNb;          //!  number of fiber in pixel
  Int_t          fPixelNb;          //!  number of pixel in module
  Int_t          fModuleNb;         //!  number of module in NeuRad
  Int_t          fStepNb;           //!  current step numb in this active volumes
  ExpertTrackingStatus fTrackStatus; //!  
  TArrayI  fProcessesID;              //!  VMC prcess IDs in step


private:
  /** Private method AddPoint
   **
   ** Adds a NeuRadPoint to the Point Collection
   **/
  ERNeuRadPoint* AddPoint();
  
  /** Private method AddFirstStep
  **
  ** Adds a NeuRadStep to the FirstStep Collection
  **/
  ERNeuRadStep* AddFirstStep();
        
  /** Private method AddStep
  **
  ** Adds a NeuRadStep to the Steps Collection
  **/      
  ERNeuRadStep* AddStep();
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();
  
  void StartNewPoint();
  void FinishNewPoint();
                            
  Double_t CurPointLen(TLorentzVector& posIn);
  
  ClassDef(ERNeuRad,1);
};

#endif 
// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizer header file          -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZER_H
#define ERNEURADDIGITIZER_H

#include <vector>

#include "TStopwatch.h"

#include "FairTask.h"

#include "ERNeuRadPoint.h"
#include "ERNeuRadPhotoElectron.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

class TObjectArray;

class ERNeuRadDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNeuRadDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  inline void SetPixelJitter(const Double_t PixelJitter)    {fPixelJitter = PixelJitter;}
  inline void SetPixelDelay(const Double_t PixelDelay)      {fPixelDelay = PixelDelay;}
  inline void SetScincilationTau(const Double_t tau)    {fScincilationTau = tau;}
  inline void SetUseCrosstalks(const Bool_t use)        {fUseCrosstalks = use;}

  /** Accessors **/ 
  Int_t PhotoElectronCount()  const;
  Int_t PixelSignalCount()   const;
protected:
  //Digitization parameters
  ERNeuRadSetup* fNeuRadSetup;
  //Input arrays
  TClonesArray *fNeuRadPoints;

  //Output arrays
  TClonesArray *fNeuRadPhotoElectron;
  TClonesArray *fNeuRadPixelSignal;

  //Event header variable
  Int_t fPECountF;
  Int_t fPECountB;
  Float_t fSumAmplitudeF;
  Float_t fSumAmplitudeB;
  
  //constants
  static const Double_t cSciFiLightYield; // [photons/MeV]
  static const Double_t cSpeedOfLight; //[cm/ns]
  static const Double_t cMaterialSpeedOfLight;//[cm/ns]
  static const Double_t cLightFractionInTotalIntReflection;
  //доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
  static const Double_t cPixelDelay; //[ns]
  static const Double_t cPixelJitter; //[ns]
  static const Double_t cScincilationTau; //[ns]

  //Allow for user params
  Double_t fPixelJitter; //[ns]
  Double_t fPixelDelay; //[ns]
  Double_t fExcessNoiseFactor;
  Double_t fScincilationTau; //[ns]

  TStopwatch fPhotoElectronsCreatingTimer;
  Double_t fPhotoElectronsCreatingTime;
  TStopwatch fPixelSignalCreatingTimer;
  Double_t fPixelSignalCreatingTime;
  Bool_t fUseCrosstalks;
protected:
  ERNeuRadPhotoElectron* AddPhotoElectron(Int_t i_point, Int_t side, Double_t lytime, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count,Double_t amplitudes);

  virtual ERNeuRadPixelSignal* AddPixelSignal(Int_t iModule, Int_t iPixel, Int_t fpoints_count, Int_t side);
  
  virtual void PhotoElectronsCreating(Int_t i_point, ERNeuRadPoint *point,
                          std::vector<ERNeuRadPhotoElectron* >** pePerPixels, Int_t side, Int_t& sumPECount,Float_t& sumAmplitude);
                        
  virtual void PixelSignalsCreating(Int_t iModule, Int_t iPixel,
                                std::vector<ERNeuRadPhotoElectron* >** pePerPixels, Int_t side);

  Int_t Crosstalks(Int_t pointModule, Int_t pointPixel, Int_t& peModule, Int_t& pePixel);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadDigitizer,1)
};

#endif
// -------------------------------------------------------------------------
// -----                  ERNeuRadContFact header file             	   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadContFact_H
#define ERNeuRadContFact_H

#include "FairContFact.h"

class FairContainer;

class ERNeuRadContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    ERNeuRadContFact();
    ~ERNeuRadContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( ERNeuRadContFact,0)
};

#endif  /* !ERNeuRadContFact_H */
// -------------------------------------------------------------------------
// -----                  ERNeuRadDigiPar header file                 -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGIPAR_H
#define ERNEURADDIGIPAR_H


#include "FairParGenericSet.h"

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayF.h"
#include "TArrayI.h"

#include "ERNeuRadSetup.h"

class FairParIo;
class FairParamList;
class ERNeuRadSetup;

class ERNeuRadDigiPar : public FairParGenericSet
{

  public:

    /** Standard constructor **/
    ERNeuRadDigiPar(const char* name    = "ERNeuRadDigiPar",
                            const char* title   = "ERNeuRad Digitization Parameters",
                            const char* context = "Default");


    /** Destructor **/
    virtual ~ERNeuRadDigiPar();


    /** Initialisation from input device**/
    virtual Bool_t init(FairParIo* input);


    /** Output to file **/
    //virtual Int_t write(FairParIo* output);

    virtual void print();
    
    /** Reset all parameters **/
    virtual void clear();

    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);

    /** Modifiers**/
    
    /** Accessories  **/
    Int_t   NofFibers()     const {return fNofFibers;}
    Int_t   NofModules()    const {return fNofModules;}
    //@todo �������� ��������� ������ ��� �������
    Double_t PixelQuantumEfficiency (Int_t iFiber) const {return (*fPixelQuantumEfficiency)[iFiber];}
    Double_t PixelGain (Int_t iFiber) const {return (*fPixelGain)[iFiber];}
    Double_t PixelSigma (Int_t iFiber) const {return (*fPixelSigma)[iFiber];}
    Bool_t UseCrosstalks() const {return fUseCrosstalks;}
    void Crosstalks(Int_t iFiber, TArrayF& crosstalks) const;
    Int_t RowNofFibers() const {return fRowNofFibers;}
  private:
    TArrayF* fPixelQuantumEfficiency;
    TArrayF* fPixelGain;
    TArrayF* fPixelSigma;
    TArrayF* fPixelCrosstalks;
    TArrayF* fFiberCrosstalks;
    Int_t fNofFibers;
    Int_t fNofModules;
    Bool_t fUseCrosstalks;
    Int_t fRowNofFibers;

    ERNeuRadDigiPar(const ERNeuRadDigiPar&);
    ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

    ClassDef(ERNeuRadDigiPar,1);
};


#endif /**  ERNEURADDIGIPAR_H   **/
// -------------------------------------------------------------------------
// -----                  ERNeuRadGeoPar header file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadGeoPar_H
#define ERNeuRadGeoPar_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class ERNeuRadGeoPar : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    ERNeuRadGeoPar(const char* name="ERNeuRadGeoPar",
                           const char* title="NeuRad Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~ERNeuRadGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    ERNeuRadGeoPar(const ERNeuRadGeoPar&);
    ERNeuRadGeoPar& operator=(const ERNeuRadGeoPar&);

    ClassDef(ERNeuRadGeoPar,1)
};

#endif /* ERNeuRadGeoPar_H */
// -------------------------------------------------------------------------

// -----                        ERNeuRadSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNEURADSETUP_H
#define ERNEURADSETUP_H

#include <vector>
#include "Rtypes.h"
#include "ERNeuRadDigiPar.h"

class ERNeuRadDigiPar;

struct ERNeuRadModule{
        Float_t fX;
        Float_t fY;
        ERNeuRadModule(Float_t x, Float_t y){fX = x; fY = y;}
};

struct ERNeuRadFiber{
        Float_t fX;
        Float_t fY;
        ERNeuRadFiber(Float_t x, Float_t y){fX = x; fY = y;}
};

class ERNeuRadSetup {
    static ERNeuRadSetup* fInstance;
    static ERNeuRadDigiPar* fDigiPar;
    static std::vector<ERNeuRadModule*> fModules;
    static std::vector<std::vector<ERNeuRadFiber*> > fFibers;
    static Float_t fZ;
    static Float_t fLength;
    static Float_t fFiberWidth;
    static Int_t fRowNofFibers;
    static Int_t fRowNofModules;
    static Int_t fRowNofPixels;
    ERNeuRadSetup();
public:
    static ERNeuRadSetup* Instance();
    static Int_t   NofFibers();
    static Int_t   NofPixels();
    static Int_t   NofModules();
    static Float_t FiberLength();
    static Float_t FiberWidth();
    static Float_t ModuleX(Int_t iModule);
    static Float_t ModuleY(Int_t iModule);
    static Float_t FiberX(Int_t iModule, Int_t iFiber);
    static Float_t FiberY(Int_t iModule, Int_t iFiber);
    static Float_t PixelQuantumEfficiency(Int_t iModule, Int_t iFiber);
    static Float_t PixelGain(Int_t iModule, Int_t iFiber);
    static Float_t PixelSigma(Int_t iModule, Int_t iFiber);
    static Float_t Z(){return fZ;}
    static void    Print();
    static void Crosstalks(Int_t iFiber, TArrayF& crosstalks);
    static Int_t RowNofFibers();
    static Int_t RowNofPixels();
    static Int_t RowNofModules();
    static Bool_t UseCrosstalks();
    static Int_t SetParContainers();
    ClassDef(ERNeuRadSetup,1)

};

#endif

// -------------------------------------------------------------------------
// -----                        ERNeuRadHitFinder header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinder_H
#define ERNeuRadHitFinder_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitFinder : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitFinder();

  ERNeuRadHitFinder(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinder();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  virtual void SetPixelThreshold(Float_t pixelThreshold){fPixelThreshold=pixelThreshold;}
  /** Accessors **/
protected:
  //Input arrays
  TClonesArray *fNeuRadPMTSignals;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

  Float_t fPixelThreshold;

protected:
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  ModuleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinder,1)
};

#endif
// -------------------------------------------------------------------------
// -----                        ERNeuRadHitFinderMF header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinderMF_H
#define ERNeuRadHitFinderMF_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHit.h"

class ERNeuRadHitFinderMF : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitFinderMF();

  ERNeuRadHitFinderMF(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinderMF();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  inline void SetPixelThreshold(Float_t pixelThreshold){fPixelThreshold = pixelThreshold;}
  inline void SetModuleThreshold(Float_t moduleThreshold){fModuleThreshold = moduleThreshold;}
  /** Accessors **/
protected:
  //Input arrays
  TClonesArray *fNeuRadDigis;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

  Float_t fPixelThreshold;
  Float_t fModuleThreshold;

protected:
  ERNeuRadHit* AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t  ModuleIndex, Int_t FiberIndex, Float_t time);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinderMF,1)
};

#endif
// -------------------------------------------------------------------------
// -----                        ERNeuRadHitFinderWBT header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitFinderWBT_H
#define ERNeuRadHitFinderWBT_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNeuRadDigiPar.h"
#include "ERNeuRadHitWBT.h"

class ERNeuRadHitFinderWBT : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadHitFinderWBT();

  ERNeuRadHitFinderWBT(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadHitFinderWBT();

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
  TClonesArray *fNeuRadPMTSignals;
  //Output arrays
  TClonesArray *fNeuRadHits;

  ERNeuRadDigiPar* fDigiPar;

  static Int_t fEvent;

protected:
  ERNeuRadHitWBT* AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                         Int_t  ModuleIndex, Int_t FiberIndex, Float_t time, Float_t qInteg);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadHitFinderWBT,1)
};

#endif
// -------------------------------------------------------------------------
// -----                        ERNeuRadMatcher header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadMatcher_H
#define ERNeuRadMatcher_H

#include "TClonesArray.h"
#include "TH1F.h"

#include "FairTask.h"

class ERNeuRadMatcher : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadMatcher();

  ERNeuRadMatcher(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadMatcher();

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
  TClonesArray *fNeuRadHits;
  TClonesArray* fNeuRadFirstSteps;
  //Output arrays

  //Output histos
  TH1F* fHdxy;
  TH1F* fHdxyLess6;
  TH1F* fHdxyOF;
  TH1F* fHdxyLess6OF;
  TH1F* fHdxyTF;
  TH1F* fHdxyLess6TF;

private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadMatcher,1)
};

#endif
// -------------------------------------------------------------------------
// -----                        ERNeuRadViewer header file             -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadViewer_H
#define ERNeuRadViewer_H

#include "TClonesArray.h"
#include "TFile.h"

#include "FairTask.h"

class ERNeuRadViewer : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadViewer();

  ERNeuRadViewer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadViewer();

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
  TClonesArray *fNeuRadPixelSignals;
  
  //Outputs
  TFile* fFile;

private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadViewer,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERNeuRad", payloadCode, "@",
"ERNeuRadContFact", payloadCode, "@",
"ERNeuRadDigiPar", payloadCode, "@",
"ERNeuRadDigitizer", payloadCode, "@",
"ERNeuRadGeoPar", payloadCode, "@",
"ERNeuRadHitFinder", payloadCode, "@",
"ERNeuRadHitFinderMF", payloadCode, "@",
"ERNeuRadHitFinderWBT", payloadCode, "@",
"ERNeuRadMatcher", payloadCode, "@",
"ERNeuRadSetup", payloadCode, "@",
"ERNeuRadViewer", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__NeuRadDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__NeuRadDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__NeuRadDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__NeuRadDict() {
  TriggerDictionaryInitialization_G__NeuRadDict_Impl();
}
