// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIdecayersdIG__ERDecayersDict

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
#include "ERDecayer.h"
#include "ERDecay.h"
#include "ERDecay27Fto26O.h"
#include "ERDecay26Oto24O2n.h"
#include "ERDecay31Arto30Ar.h"
#include "ERDecay30Arto28S2p.h"
#include "ERDecayLi9DetoLi10_Li9n_p.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERDecayer(void *p = 0);
   static void *newArray_ERDecayer(Long_t size, void *p);
   static void delete_ERDecayer(void *p);
   static void deleteArray_ERDecayer(void *p);
   static void destruct_ERDecayer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecayer*)
   {
      ::ERDecayer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecayer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecayer", ::ERDecayer::Class_Version(), "invalid", 19,
                  typeid(::ERDecayer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecayer::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecayer) );
      instance.SetNew(&new_ERDecayer);
      instance.SetNewArray(&newArray_ERDecayer);
      instance.SetDelete(&delete_ERDecayer);
      instance.SetDeleteArray(&deleteArray_ERDecayer);
      instance.SetDestructor(&destruct_ERDecayer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecayer*)
   {
      return GenerateInitInstanceLocal((::ERDecayer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecayer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_ERDecay(void *p);
   static void deleteArray_ERDecay(void *p);
   static void destruct_ERDecay(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecay*)
   {
      ::ERDecay *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecay >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecay", ::ERDecay::Class_Version(), "ERDecay.h", 13,
                  typeid(::ERDecay), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecay::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecay) );
      instance.SetDelete(&delete_ERDecay);
      instance.SetDeleteArray(&deleteArray_ERDecay);
      instance.SetDestructor(&destruct_ERDecay);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecay*)
   {
      return GenerateInitInstanceLocal((::ERDecay*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecay*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDecay27Fto26O(void *p = 0);
   static void *newArray_ERDecay27Fto26O(Long_t size, void *p);
   static void delete_ERDecay27Fto26O(void *p);
   static void deleteArray_ERDecay27Fto26O(void *p);
   static void destruct_ERDecay27Fto26O(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecay27Fto26O*)
   {
      ::ERDecay27Fto26O *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecay27Fto26O >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecay27Fto26O", ::ERDecay27Fto26O::Class_Version(), "invalid", 78,
                  typeid(::ERDecay27Fto26O), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecay27Fto26O::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecay27Fto26O) );
      instance.SetNew(&new_ERDecay27Fto26O);
      instance.SetNewArray(&newArray_ERDecay27Fto26O);
      instance.SetDelete(&delete_ERDecay27Fto26O);
      instance.SetDeleteArray(&deleteArray_ERDecay27Fto26O);
      instance.SetDestructor(&destruct_ERDecay27Fto26O);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecay27Fto26O*)
   {
      return GenerateInitInstanceLocal((::ERDecay27Fto26O*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecay27Fto26O*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDecay26Oto24O2n(void *p = 0);
   static void *newArray_ERDecay26Oto24O2n(Long_t size, void *p);
   static void delete_ERDecay26Oto24O2n(void *p);
   static void deleteArray_ERDecay26Oto24O2n(void *p);
   static void destruct_ERDecay26Oto24O2n(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecay26Oto24O2n*)
   {
      ::ERDecay26Oto24O2n *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecay26Oto24O2n >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecay26Oto24O2n", ::ERDecay26Oto24O2n::Class_Version(), "invalid", 115,
                  typeid(::ERDecay26Oto24O2n), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecay26Oto24O2n::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecay26Oto24O2n) );
      instance.SetNew(&new_ERDecay26Oto24O2n);
      instance.SetNewArray(&newArray_ERDecay26Oto24O2n);
      instance.SetDelete(&delete_ERDecay26Oto24O2n);
      instance.SetDeleteArray(&deleteArray_ERDecay26Oto24O2n);
      instance.SetDestructor(&destruct_ERDecay26Oto24O2n);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecay26Oto24O2n*)
   {
      return GenerateInitInstanceLocal((::ERDecay26Oto24O2n*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecay26Oto24O2n*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDecay31Arto30Ar(void *p = 0);
   static void *newArray_ERDecay31Arto30Ar(Long_t size, void *p);
   static void delete_ERDecay31Arto30Ar(void *p);
   static void deleteArray_ERDecay31Arto30Ar(void *p);
   static void destruct_ERDecay31Arto30Ar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecay31Arto30Ar*)
   {
      ::ERDecay31Arto30Ar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecay31Arto30Ar >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecay31Arto30Ar", ::ERDecay31Arto30Ar::Class_Version(), "invalid", 157,
                  typeid(::ERDecay31Arto30Ar), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecay31Arto30Ar::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecay31Arto30Ar) );
      instance.SetNew(&new_ERDecay31Arto30Ar);
      instance.SetNewArray(&newArray_ERDecay31Arto30Ar);
      instance.SetDelete(&delete_ERDecay31Arto30Ar);
      instance.SetDeleteArray(&deleteArray_ERDecay31Arto30Ar);
      instance.SetDestructor(&destruct_ERDecay31Arto30Ar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecay31Arto30Ar*)
   {
      return GenerateInitInstanceLocal((::ERDecay31Arto30Ar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecay31Arto30Ar*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDecay30Arto28S2p(void *p = 0);
   static void *newArray_ERDecay30Arto28S2p(Long_t size, void *p);
   static void delete_ERDecay30Arto28S2p(void *p);
   static void deleteArray_ERDecay30Arto28S2p(void *p);
   static void destruct_ERDecay30Arto28S2p(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecay30Arto28S2p*)
   {
      ::ERDecay30Arto28S2p *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecay30Arto28S2p >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecay30Arto28S2p", ::ERDecay30Arto28S2p::Class_Version(), "invalid", 194,
                  typeid(::ERDecay30Arto28S2p), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecay30Arto28S2p::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecay30Arto28S2p) );
      instance.SetNew(&new_ERDecay30Arto28S2p);
      instance.SetNewArray(&newArray_ERDecay30Arto28S2p);
      instance.SetDelete(&delete_ERDecay30Arto28S2p);
      instance.SetDeleteArray(&deleteArray_ERDecay30Arto28S2p);
      instance.SetDestructor(&destruct_ERDecay30Arto28S2p);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecay30Arto28S2p*)
   {
      return GenerateInitInstanceLocal((::ERDecay30Arto28S2p*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecay30Arto28S2p*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDecayLi9DetoLi10_Li9n_p(void *p = 0);
   static void *newArray_ERDecayLi9DetoLi10_Li9n_p(Long_t size, void *p);
   static void delete_ERDecayLi9DetoLi10_Li9n_p(void *p);
   static void deleteArray_ERDecayLi9DetoLi10_Li9n_p(void *p);
   static void destruct_ERDecayLi9DetoLi10_Li9n_p(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDecayLi9DetoLi10_Li9n_p*)
   {
      ::ERDecayLi9DetoLi10_Li9n_p *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDecayLi9DetoLi10_Li9n_p >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDecayLi9DetoLi10_Li9n_p", ::ERDecayLi9DetoLi10_Li9n_p::Class_Version(), "invalid", 236,
                  typeid(::ERDecayLi9DetoLi10_Li9n_p), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDecayLi9DetoLi10_Li9n_p::Dictionary, isa_proxy, 4,
                  sizeof(::ERDecayLi9DetoLi10_Li9n_p) );
      instance.SetNew(&new_ERDecayLi9DetoLi10_Li9n_p);
      instance.SetNewArray(&newArray_ERDecayLi9DetoLi10_Li9n_p);
      instance.SetDelete(&delete_ERDecayLi9DetoLi10_Li9n_p);
      instance.SetDeleteArray(&deleteArray_ERDecayLi9DetoLi10_Li9n_p);
      instance.SetDestructor(&destruct_ERDecayLi9DetoLi10_Li9n_p);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDecayLi9DetoLi10_Li9n_p*)
   {
      return GenerateInitInstanceLocal((::ERDecayLi9DetoLi10_Li9n_p*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDecayLi9DetoLi10_Li9n_p*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERDecayer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecayer::Class_Name()
{
   return "ERDecayer";
}

//______________________________________________________________________________
const char *ERDecayer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecayer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecayer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecayer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecayer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecayer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecayer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecayer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecay::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecay::Class_Name()
{
   return "ERDecay";
}

//______________________________________________________________________________
const char *ERDecay::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecay::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecay::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecay::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecay27Fto26O::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecay27Fto26O::Class_Name()
{
   return "ERDecay27Fto26O";
}

//______________________________________________________________________________
const char *ERDecay27Fto26O::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay27Fto26O*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecay27Fto26O::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay27Fto26O*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecay27Fto26O::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay27Fto26O*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecay27Fto26O::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay27Fto26O*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecay26Oto24O2n::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecay26Oto24O2n::Class_Name()
{
   return "ERDecay26Oto24O2n";
}

//______________________________________________________________________________
const char *ERDecay26Oto24O2n::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay26Oto24O2n*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecay26Oto24O2n::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay26Oto24O2n*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecay26Oto24O2n::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay26Oto24O2n*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecay26Oto24O2n::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay26Oto24O2n*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecay31Arto30Ar::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecay31Arto30Ar::Class_Name()
{
   return "ERDecay31Arto30Ar";
}

//______________________________________________________________________________
const char *ERDecay31Arto30Ar::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay31Arto30Ar*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecay31Arto30Ar::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay31Arto30Ar*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecay31Arto30Ar::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay31Arto30Ar*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecay31Arto30Ar::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay31Arto30Ar*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecay30Arto28S2p::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecay30Arto28S2p::Class_Name()
{
   return "ERDecay30Arto28S2p";
}

//______________________________________________________________________________
const char *ERDecay30Arto28S2p::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay30Arto28S2p*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecay30Arto28S2p::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecay30Arto28S2p*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecay30Arto28S2p::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay30Arto28S2p*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecay30Arto28S2p::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecay30Arto28S2p*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDecayLi9DetoLi10_Li9n_p::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDecayLi9DetoLi10_Li9n_p::Class_Name()
{
   return "ERDecayLi9DetoLi10_Li9n_p";
}

//______________________________________________________________________________
const char *ERDecayLi9DetoLi10_Li9n_p::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecayLi9DetoLi10_Li9n_p*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDecayLi9DetoLi10_Li9n_p::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDecayLi9DetoLi10_Li9n_p*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDecayLi9DetoLi10_Li9n_p::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecayLi9DetoLi10_Li9n_p*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDecayLi9DetoLi10_Li9n_p::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDecayLi9DetoLi10_Li9n_p*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERDecayer::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecayer.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecayer::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecayer::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecayer(void *p) {
      return  p ? new(p) ::ERDecayer : new ::ERDecayer;
   }
   static void *newArray_ERDecayer(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecayer[nElements] : new ::ERDecayer[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecayer(void *p) {
      delete ((::ERDecayer*)p);
   }
   static void deleteArray_ERDecayer(void *p) {
      delete [] ((::ERDecayer*)p);
   }
   static void destruct_ERDecayer(void *p) {
      typedef ::ERDecayer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecayer

//______________________________________________________________________________
void ERDecay::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecay.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecay::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecay::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ERDecay(void *p) {
      delete ((::ERDecay*)p);
   }
   static void deleteArray_ERDecay(void *p) {
      delete [] ((::ERDecay*)p);
   }
   static void destruct_ERDecay(void *p) {
      typedef ::ERDecay current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecay

//______________________________________________________________________________
void ERDecay27Fto26O::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecay27Fto26O.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecay27Fto26O::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecay27Fto26O::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecay27Fto26O(void *p) {
      return  p ? new(p) ::ERDecay27Fto26O : new ::ERDecay27Fto26O;
   }
   static void *newArray_ERDecay27Fto26O(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecay27Fto26O[nElements] : new ::ERDecay27Fto26O[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecay27Fto26O(void *p) {
      delete ((::ERDecay27Fto26O*)p);
   }
   static void deleteArray_ERDecay27Fto26O(void *p) {
      delete [] ((::ERDecay27Fto26O*)p);
   }
   static void destruct_ERDecay27Fto26O(void *p) {
      typedef ::ERDecay27Fto26O current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecay27Fto26O

//______________________________________________________________________________
void ERDecay26Oto24O2n::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecay26Oto24O2n.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecay26Oto24O2n::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecay26Oto24O2n::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecay26Oto24O2n(void *p) {
      return  p ? new(p) ::ERDecay26Oto24O2n : new ::ERDecay26Oto24O2n;
   }
   static void *newArray_ERDecay26Oto24O2n(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecay26Oto24O2n[nElements] : new ::ERDecay26Oto24O2n[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecay26Oto24O2n(void *p) {
      delete ((::ERDecay26Oto24O2n*)p);
   }
   static void deleteArray_ERDecay26Oto24O2n(void *p) {
      delete [] ((::ERDecay26Oto24O2n*)p);
   }
   static void destruct_ERDecay26Oto24O2n(void *p) {
      typedef ::ERDecay26Oto24O2n current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecay26Oto24O2n

//______________________________________________________________________________
void ERDecay31Arto30Ar::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecay31Arto30Ar.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecay31Arto30Ar::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecay31Arto30Ar::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecay31Arto30Ar(void *p) {
      return  p ? new(p) ::ERDecay31Arto30Ar : new ::ERDecay31Arto30Ar;
   }
   static void *newArray_ERDecay31Arto30Ar(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecay31Arto30Ar[nElements] : new ::ERDecay31Arto30Ar[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecay31Arto30Ar(void *p) {
      delete ((::ERDecay31Arto30Ar*)p);
   }
   static void deleteArray_ERDecay31Arto30Ar(void *p) {
      delete [] ((::ERDecay31Arto30Ar*)p);
   }
   static void destruct_ERDecay31Arto30Ar(void *p) {
      typedef ::ERDecay31Arto30Ar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecay31Arto30Ar

//______________________________________________________________________________
void ERDecay30Arto28S2p::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecay30Arto28S2p.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecay30Arto28S2p::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecay30Arto28S2p::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecay30Arto28S2p(void *p) {
      return  p ? new(p) ::ERDecay30Arto28S2p : new ::ERDecay30Arto28S2p;
   }
   static void *newArray_ERDecay30Arto28S2p(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecay30Arto28S2p[nElements] : new ::ERDecay30Arto28S2p[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecay30Arto28S2p(void *p) {
      delete ((::ERDecay30Arto28S2p*)p);
   }
   static void deleteArray_ERDecay30Arto28S2p(void *p) {
      delete [] ((::ERDecay30Arto28S2p*)p);
   }
   static void destruct_ERDecay30Arto28S2p(void *p) {
      typedef ::ERDecay30Arto28S2p current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecay30Arto28S2p

//______________________________________________________________________________
void ERDecayLi9DetoLi10_Li9n_p::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDecayLi9DetoLi10_Li9n_p.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDecayLi9DetoLi10_Li9n_p::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDecayLi9DetoLi10_Li9n_p::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDecayLi9DetoLi10_Li9n_p(void *p) {
      return  p ? new(p) ::ERDecayLi9DetoLi10_Li9n_p : new ::ERDecayLi9DetoLi10_Li9n_p;
   }
   static void *newArray_ERDecayLi9DetoLi10_Li9n_p(Long_t nElements, void *p) {
      return p ? new(p) ::ERDecayLi9DetoLi10_Li9n_p[nElements] : new ::ERDecayLi9DetoLi10_Li9n_p[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDecayLi9DetoLi10_Li9n_p(void *p) {
      delete ((::ERDecayLi9DetoLi10_Li9n_p*)p);
   }
   static void deleteArray_ERDecayLi9DetoLi10_Li9n_p(void *p) {
      delete [] ((::ERDecayLi9DetoLi10_Li9n_p*)p);
   }
   static void destruct_ERDecayLi9DetoLi10_Li9n_p(void *p) {
      typedef ::ERDecayLi9DetoLi10_Li9n_p current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDecayLi9DetoLi10_Li9n_p

namespace ROOT {
   static TClass *vectorlEERDecaymUgR_Dictionary();
   static void vectorlEERDecaymUgR_TClassManip(TClass*);
   static void *new_vectorlEERDecaymUgR(void *p = 0);
   static void *newArray_vectorlEERDecaymUgR(Long_t size, void *p);
   static void delete_vectorlEERDecaymUgR(void *p);
   static void deleteArray_vectorlEERDecaymUgR(void *p);
   static void destruct_vectorlEERDecaymUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ERDecay*>*)
   {
      vector<ERDecay*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ERDecay*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ERDecay*>", -2, "vector", 210,
                  typeid(vector<ERDecay*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEERDecaymUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<ERDecay*>) );
      instance.SetNew(&new_vectorlEERDecaymUgR);
      instance.SetNewArray(&newArray_vectorlEERDecaymUgR);
      instance.SetDelete(&delete_vectorlEERDecaymUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEERDecaymUgR);
      instance.SetDestructor(&destruct_vectorlEERDecaymUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ERDecay*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<ERDecay*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEERDecaymUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ERDecay*>*)0x0)->GetClass();
      vectorlEERDecaymUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEERDecaymUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEERDecaymUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ERDecay*> : new vector<ERDecay*>;
   }
   static void *newArray_vectorlEERDecaymUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ERDecay*>[nElements] : new vector<ERDecay*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEERDecaymUgR(void *p) {
      delete ((vector<ERDecay*>*)p);
   }
   static void deleteArray_vectorlEERDecaymUgR(void *p) {
      delete [] ((vector<ERDecay*>*)p);
   }
   static void destruct_vectorlEERDecaymUgR(void *p) {
      typedef vector<ERDecay*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ERDecay*>

namespace {
  void TriggerDictionaryInitialization_G__ERDecayersDict_Impl() {
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
"/home/muzalevsky/check/build/decayers/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERDecayersDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERDecayer;
class __attribute__((annotate("$clingAutoload$ERDecay.h")))  ERDecay;
class ERDecay27Fto26O;
class ERDecay26Oto24O2n;
class ERDecay31Arto30Ar;
class ERDecay30Arto28S2p;
class ERDecayLi9DetoLi10_Li9n_p;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERDecayersDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                      ERDecayer header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY_H
#define ERDECAY_H

#include <vector>

#include "ERDecay.h"


class ERDecayer{
private:
	std::vector<ERDecay*> fDecays;
public:
	ERDecayer();
	~ERDecayer();

	Bool_t Stepping();

	void AddDecay(ERDecay* decay) {fDecays.push_back(decay);}

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecayer, 1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecay header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAYER_H
#define ERDECAYER_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TDatabasePDG.h"               //for TDatabasePDG

class ERDecay{
public:
	ERDecay();
	~ERDecay();

	virtual Bool_t Stepping() = 0;

	virtual void BeginEvent() = 0;
	virtual void FinishEvent() = 0;

	ClassDef(ERDecay,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecay27Fto26O header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY27FTO26O_H
#define ERDECAY27FTO26O_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay27Fto26O : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fSecondIon;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay27Fto26O();
	~ERDecay27Fto26O();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecay27Fto26O,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecay26Oto24O2n header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY26OTO24O2N_H
#define ERDECAY26OTO24O2N_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay26Oto24O2n : public ERDecay{
private:
	TRandom3 *fRnd;
	Float_t fTauCM; //ps
	
	TParticlePDG*   fSecondIon;
    TParticlePDG*   fThirdIon;

    TGenPhaseSpace* fPHSpace;

  	Bool_t fDirectReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay26Oto24O2n();
	~ERDecay26Oto24O2n();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	void SetDirectReactionTauCM(Float_t tau){fTauCM = tau;}

	ClassDef(ERDecay26Oto24O2n,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecay31Arto30Ar header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY31ARTO30AR_H
#define ERDECAY31ARTO30AR_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay31Arto30Ar : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fSecondIon;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay31Arto30Ar();
	~ERDecay31Arto30Ar();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecay31Arto30Ar,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecay30Arto28S2p header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY30ARTO28S2P_H
#define ERDECAY30ARTO28S2P_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecay30Arto28S2p : public ERDecay{
private:
	TRandom3 *fRnd;
	Float_t fTauCM; //ps
	
	TParticlePDG*   fSecondIon;
    TParticlePDG*   fThirdIon;

    TGenPhaseSpace* fPHSpace;

  	Bool_t fDirectReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecay30Arto28S2p();
	~ERDecay30Arto28S2p();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	void SetDirectReactionTauCM(Float_t tau){fTauCM = tau;}

	ClassDef(ERDecay30Arto28S2p,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERDecayLi9DetoLi10_Li9n_p header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDecayLi9DetoLi10_Li9n_p_H
#define ERDecayLi9DetoLi10_Li9n_p_H

#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include "ERDecay.h"

class ERDecayLi9DetoLi10_Li9n_p : public ERDecay{
private:
	Float_t fTargetReactZ;
	TRandom3 *fRnd;
	
	TParticlePDG*   fLi9;
	TParticlePDG*   fLi10;
	TParticlePDG*   fH2;
	TParticlePDG*   fn;
	TParticlePDG*   fp;

    Bool_t fTargetReactionFinish;

  	Int_t fSecondaryIonPDG;
public:
	ERDecayLi9DetoLi10_Li9n_p();
	~ERDecayLi9DetoLi10_Li9n_p();

	Bool_t Stepping();

	void BeginEvent();
	void FinishEvent();

	ClassDef(ERDecayLi9DetoLi10_Li9n_p,1)
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERDecay", payloadCode, "@",
"ERDecay26Oto24O2n", payloadCode, "@",
"ERDecay27Fto26O", payloadCode, "@",
"ERDecay30Arto28S2p", payloadCode, "@",
"ERDecay31Arto30Ar", payloadCode, "@",
"ERDecayLi9DetoLi10_Li9n_p", payloadCode, "@",
"ERDecayer", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERDecayersDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERDecayersDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERDecayersDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERDecayersDict() {
  TriggerDictionaryInitialization_G__ERDecayersDict_Impl();
}
