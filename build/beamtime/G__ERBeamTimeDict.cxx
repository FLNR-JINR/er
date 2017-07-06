// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIbeamtimedIG__ERBeamTimeDict

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
#include "ERDRS4Source.h"
#include "ERTektronixSource.h"
#include "ERRawToAnalyzeConverter.h"
#include "ERLiCalibrator.h"
#include "ERRootSource.h"
#include "ERNXyterTreeSource.h"
#include "ERNXyterCalibrator.h"
#include "ERpmtPixelMap.h"
#include "ERSimtoRaw.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERDRS4Source(void *p = 0);
   static void *newArray_ERDRS4Source(Long_t size, void *p);
   static void delete_ERDRS4Source(void *p);
   static void deleteArray_ERDRS4Source(void *p);
   static void destruct_ERDRS4Source(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDRS4Source*)
   {
      ::ERDRS4Source *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDRS4Source >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDRS4Source", ::ERDRS4Source::Class_Version(), "invalid", 55,
                  typeid(::ERDRS4Source), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDRS4Source::Dictionary, isa_proxy, 4,
                  sizeof(::ERDRS4Source) );
      instance.SetNew(&new_ERDRS4Source);
      instance.SetNewArray(&newArray_ERDRS4Source);
      instance.SetDelete(&delete_ERDRS4Source);
      instance.SetDeleteArray(&deleteArray_ERDRS4Source);
      instance.SetDestructor(&destruct_ERDRS4Source);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDRS4Source*)
   {
      return GenerateInitInstanceLocal((::ERDRS4Source*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDRS4Source*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERTektronixSource(void *p = 0);
   static void *newArray_ERTektronixSource(Long_t size, void *p);
   static void delete_ERTektronixSource(void *p);
   static void deleteArray_ERTektronixSource(void *p);
   static void destruct_ERTektronixSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERTektronixSource*)
   {
      ::ERTektronixSource *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERTektronixSource >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERTektronixSource", ::ERTektronixSource::Class_Version(), "invalid", 131,
                  typeid(::ERTektronixSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERTektronixSource::Dictionary, isa_proxy, 4,
                  sizeof(::ERTektronixSource) );
      instance.SetNew(&new_ERTektronixSource);
      instance.SetNewArray(&newArray_ERTektronixSource);
      instance.SetDelete(&delete_ERTektronixSource);
      instance.SetDeleteArray(&deleteArray_ERTektronixSource);
      instance.SetDestructor(&destruct_ERTektronixSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERTektronixSource*)
   {
      return GenerateInitInstanceLocal((::ERTektronixSource*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERTektronixSource*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERRawToAnalyzeConverter(void *p = 0);
   static void *newArray_ERRawToAnalyzeConverter(Long_t size, void *p);
   static void delete_ERRawToAnalyzeConverter(void *p);
   static void deleteArray_ERRawToAnalyzeConverter(void *p);
   static void destruct_ERRawToAnalyzeConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERRawToAnalyzeConverter*)
   {
      ::ERRawToAnalyzeConverter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERRawToAnalyzeConverter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERRawToAnalyzeConverter", ::ERRawToAnalyzeConverter::Class_Version(), "invalid", 188,
                  typeid(::ERRawToAnalyzeConverter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERRawToAnalyzeConverter::Dictionary, isa_proxy, 4,
                  sizeof(::ERRawToAnalyzeConverter) );
      instance.SetNew(&new_ERRawToAnalyzeConverter);
      instance.SetNewArray(&newArray_ERRawToAnalyzeConverter);
      instance.SetDelete(&delete_ERRawToAnalyzeConverter);
      instance.SetDeleteArray(&deleteArray_ERRawToAnalyzeConverter);
      instance.SetDestructor(&destruct_ERRawToAnalyzeConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERRawToAnalyzeConverter*)
   {
      return GenerateInitInstanceLocal((::ERRawToAnalyzeConverter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERRawToAnalyzeConverter*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ERSimtoRaw_Dictionary();
   static void ERSimtoRaw_TClassManip(TClass*);
   static void *new_ERSimtoRaw(void *p = 0);
   static void *newArray_ERSimtoRaw(Long_t size, void *p);
   static void delete_ERSimtoRaw(void *p);
   static void deleteArray_ERSimtoRaw(void *p);
   static void destruct_ERSimtoRaw(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERSimtoRaw*)
   {
      ::ERSimtoRaw *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ERSimtoRaw));
      static ::ROOT::TGenericClassInfo 
         instance("ERSimtoRaw", "invalid", 524,
                  typeid(::ERSimtoRaw), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ERSimtoRaw_Dictionary, isa_proxy, 4,
                  sizeof(::ERSimtoRaw) );
      instance.SetNew(&new_ERSimtoRaw);
      instance.SetNewArray(&newArray_ERSimtoRaw);
      instance.SetDelete(&delete_ERSimtoRaw);
      instance.SetDeleteArray(&deleteArray_ERSimtoRaw);
      instance.SetDestructor(&destruct_ERSimtoRaw);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERSimtoRaw*)
   {
      return GenerateInitInstanceLocal((::ERSimtoRaw*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERSimtoRaw*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ERSimtoRaw_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ERSimtoRaw*)0x0)->GetClass();
      ERSimtoRaw_TClassManip(theClass);
   return theClass;
   }

   static void ERSimtoRaw_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_ERLiCalibrator(void *p = 0);
   static void *newArray_ERLiCalibrator(Long_t size, void *p);
   static void delete_ERLiCalibrator(void *p);
   static void deleteArray_ERLiCalibrator(void *p);
   static void destruct_ERLiCalibrator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERLiCalibrator*)
   {
      ::ERLiCalibrator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERLiCalibrator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERLiCalibrator", ::ERLiCalibrator::Class_Version(), "invalid", 257,
                  typeid(::ERLiCalibrator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERLiCalibrator::Dictionary, isa_proxy, 4,
                  sizeof(::ERLiCalibrator) );
      instance.SetNew(&new_ERLiCalibrator);
      instance.SetNewArray(&newArray_ERLiCalibrator);
      instance.SetDelete(&delete_ERLiCalibrator);
      instance.SetDeleteArray(&deleteArray_ERLiCalibrator);
      instance.SetDestructor(&destruct_ERLiCalibrator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERLiCalibrator*)
   {
      return GenerateInitInstanceLocal((::ERLiCalibrator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERLiCalibrator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERRootSource(void *p = 0);
   static void *newArray_ERRootSource(Long_t size, void *p);
   static void delete_ERRootSource(void *p);
   static void deleteArray_ERRootSource(void *p);
   static void destruct_ERRootSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERRootSource*)
   {
      ::ERRootSource *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERRootSource >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERRootSource", ::ERRootSource::Class_Version(), "invalid", 314,
                  typeid(::ERRootSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERRootSource::Dictionary, isa_proxy, 4,
                  sizeof(::ERRootSource) );
      instance.SetNew(&new_ERRootSource);
      instance.SetNewArray(&newArray_ERRootSource);
      instance.SetDelete(&delete_ERRootSource);
      instance.SetDeleteArray(&deleteArray_ERRootSource);
      instance.SetDestructor(&destruct_ERRootSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERRootSource*)
   {
      return GenerateInitInstanceLocal((::ERRootSource*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERRootSource*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNXyterTreeSource(void *p = 0);
   static void *newArray_ERNXyterTreeSource(Long_t size, void *p);
   static void delete_ERNXyterTreeSource(void *p);
   static void deleteArray_ERNXyterTreeSource(void *p);
   static void destruct_ERNXyterTreeSource(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNXyterTreeSource*)
   {
      ::ERNXyterTreeSource *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNXyterTreeSource >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNXyterTreeSource", ::ERNXyterTreeSource::Class_Version(), "invalid", 378,
                  typeid(::ERNXyterTreeSource), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNXyterTreeSource::Dictionary, isa_proxy, 4,
                  sizeof(::ERNXyterTreeSource) );
      instance.SetNew(&new_ERNXyterTreeSource);
      instance.SetNewArray(&newArray_ERNXyterTreeSource);
      instance.SetDelete(&delete_ERNXyterTreeSource);
      instance.SetDeleteArray(&deleteArray_ERNXyterTreeSource);
      instance.SetDestructor(&destruct_ERNXyterTreeSource);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNXyterTreeSource*)
   {
      return GenerateInitInstanceLocal((::ERNXyterTreeSource*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNXyterTreeSource*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNXyterCalibrator*)
   {
      ::ERNXyterCalibrator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNXyterCalibrator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNXyterCalibrator", ::ERNXyterCalibrator::Class_Version(), "invalid", 452,
                  typeid(::ERNXyterCalibrator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNXyterCalibrator::Dictionary, isa_proxy, 4,
                  sizeof(::ERNXyterCalibrator) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNXyterCalibrator*)
   {
      return GenerateInitInstanceLocal((::ERNXyterCalibrator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNXyterCalibrator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ERpmtPixelMap_Dictionary();
   static void ERpmtPixelMap_TClassManip(TClass*);
   static void *new_ERpmtPixelMap(void *p = 0);
   static void *newArray_ERpmtPixelMap(Long_t size, void *p);
   static void delete_ERpmtPixelMap(void *p);
   static void deleteArray_ERpmtPixelMap(void *p);
   static void destruct_ERpmtPixelMap(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERpmtPixelMap*)
   {
      ::ERpmtPixelMap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ERpmtPixelMap));
      static ::ROOT::TGenericClassInfo 
         instance("ERpmtPixelMap", "invalid", 494,
                  typeid(::ERpmtPixelMap), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ERpmtPixelMap_Dictionary, isa_proxy, 4,
                  sizeof(::ERpmtPixelMap) );
      instance.SetNew(&new_ERpmtPixelMap);
      instance.SetNewArray(&newArray_ERpmtPixelMap);
      instance.SetDelete(&delete_ERpmtPixelMap);
      instance.SetDeleteArray(&deleteArray_ERpmtPixelMap);
      instance.SetDestructor(&destruct_ERpmtPixelMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERpmtPixelMap*)
   {
      return GenerateInitInstanceLocal((::ERpmtPixelMap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERpmtPixelMap*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ERpmtPixelMap_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ERpmtPixelMap*)0x0)->GetClass();
      ERpmtPixelMap_TClassManip(theClass);
   return theClass;
   }

   static void ERpmtPixelMap_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERDRS4Source::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDRS4Source::Class_Name()
{
   return "ERDRS4Source";
}

//______________________________________________________________________________
const char *ERDRS4Source::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDRS4Source*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDRS4Source::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDRS4Source*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDRS4Source::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDRS4Source*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDRS4Source::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDRS4Source*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERTektronixSource::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERTektronixSource::Class_Name()
{
   return "ERTektronixSource";
}

//______________________________________________________________________________
const char *ERTektronixSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERTektronixSource*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERTektronixSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERTektronixSource*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERTektronixSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERTektronixSource*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERTektronixSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERTektronixSource*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERRawToAnalyzeConverter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERRawToAnalyzeConverter::Class_Name()
{
   return "ERRawToAnalyzeConverter";
}

//______________________________________________________________________________
const char *ERRawToAnalyzeConverter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRawToAnalyzeConverter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERRawToAnalyzeConverter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRawToAnalyzeConverter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERRawToAnalyzeConverter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRawToAnalyzeConverter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERRawToAnalyzeConverter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRawToAnalyzeConverter*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERLiCalibrator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERLiCalibrator::Class_Name()
{
   return "ERLiCalibrator";
}

//______________________________________________________________________________
const char *ERLiCalibrator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLiCalibrator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERLiCalibrator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLiCalibrator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERLiCalibrator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLiCalibrator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERLiCalibrator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLiCalibrator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERRootSource::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERRootSource::Class_Name()
{
   return "ERRootSource";
}

//______________________________________________________________________________
const char *ERRootSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRootSource*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERRootSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERRootSource*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERRootSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRootSource*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERRootSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERRootSource*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNXyterTreeSource::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNXyterTreeSource::Class_Name()
{
   return "ERNXyterTreeSource";
}

//______________________________________________________________________________
const char *ERNXyterTreeSource::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterTreeSource*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNXyterTreeSource::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterTreeSource*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNXyterTreeSource::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterTreeSource*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNXyterTreeSource::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterTreeSource*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNXyterCalibrator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNXyterCalibrator::Class_Name()
{
   return "ERNXyterCalibrator";
}

//______________________________________________________________________________
const char *ERNXyterCalibrator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterCalibrator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNXyterCalibrator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterCalibrator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNXyterCalibrator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterCalibrator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNXyterCalibrator::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNXyterCalibrator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERDRS4Source::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDRS4Source.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDRS4Source::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDRS4Source::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDRS4Source(void *p) {
      return  p ? new(p) ::ERDRS4Source : new ::ERDRS4Source;
   }
   static void *newArray_ERDRS4Source(Long_t nElements, void *p) {
      return p ? new(p) ::ERDRS4Source[nElements] : new ::ERDRS4Source[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDRS4Source(void *p) {
      delete ((::ERDRS4Source*)p);
   }
   static void deleteArray_ERDRS4Source(void *p) {
      delete [] ((::ERDRS4Source*)p);
   }
   static void destruct_ERDRS4Source(void *p) {
      typedef ::ERDRS4Source current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDRS4Source

//______________________________________________________________________________
void ERTektronixSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERTektronixSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERTektronixSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERTektronixSource::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERTektronixSource(void *p) {
      return  p ? new(p) ::ERTektronixSource : new ::ERTektronixSource;
   }
   static void *newArray_ERTektronixSource(Long_t nElements, void *p) {
      return p ? new(p) ::ERTektronixSource[nElements] : new ::ERTektronixSource[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERTektronixSource(void *p) {
      delete ((::ERTektronixSource*)p);
   }
   static void deleteArray_ERTektronixSource(void *p) {
      delete [] ((::ERTektronixSource*)p);
   }
   static void destruct_ERTektronixSource(void *p) {
      typedef ::ERTektronixSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERTektronixSource

//______________________________________________________________________________
void ERRawToAnalyzeConverter::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERRawToAnalyzeConverter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERRawToAnalyzeConverter::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERRawToAnalyzeConverter::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERRawToAnalyzeConverter(void *p) {
      return  p ? new(p) ::ERRawToAnalyzeConverter : new ::ERRawToAnalyzeConverter;
   }
   static void *newArray_ERRawToAnalyzeConverter(Long_t nElements, void *p) {
      return p ? new(p) ::ERRawToAnalyzeConverter[nElements] : new ::ERRawToAnalyzeConverter[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERRawToAnalyzeConverter(void *p) {
      delete ((::ERRawToAnalyzeConverter*)p);
   }
   static void deleteArray_ERRawToAnalyzeConverter(void *p) {
      delete [] ((::ERRawToAnalyzeConverter*)p);
   }
   static void destruct_ERRawToAnalyzeConverter(void *p) {
      typedef ::ERRawToAnalyzeConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERRawToAnalyzeConverter

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERSimtoRaw(void *p) {
      return  p ? new(p) ::ERSimtoRaw : new ::ERSimtoRaw;
   }
   static void *newArray_ERSimtoRaw(Long_t nElements, void *p) {
      return p ? new(p) ::ERSimtoRaw[nElements] : new ::ERSimtoRaw[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERSimtoRaw(void *p) {
      delete ((::ERSimtoRaw*)p);
   }
   static void deleteArray_ERSimtoRaw(void *p) {
      delete [] ((::ERSimtoRaw*)p);
   }
   static void destruct_ERSimtoRaw(void *p) {
      typedef ::ERSimtoRaw current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERSimtoRaw

//______________________________________________________________________________
void ERLiCalibrator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERLiCalibrator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERLiCalibrator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERLiCalibrator::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERLiCalibrator(void *p) {
      return  p ? new(p) ::ERLiCalibrator : new ::ERLiCalibrator;
   }
   static void *newArray_ERLiCalibrator(Long_t nElements, void *p) {
      return p ? new(p) ::ERLiCalibrator[nElements] : new ::ERLiCalibrator[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERLiCalibrator(void *p) {
      delete ((::ERLiCalibrator*)p);
   }
   static void deleteArray_ERLiCalibrator(void *p) {
      delete [] ((::ERLiCalibrator*)p);
   }
   static void destruct_ERLiCalibrator(void *p) {
      typedef ::ERLiCalibrator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERLiCalibrator

//______________________________________________________________________________
void ERRootSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERRootSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERRootSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERRootSource::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERRootSource(void *p) {
      return  p ? new(p) ::ERRootSource : new ::ERRootSource;
   }
   static void *newArray_ERRootSource(Long_t nElements, void *p) {
      return p ? new(p) ::ERRootSource[nElements] : new ::ERRootSource[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERRootSource(void *p) {
      delete ((::ERRootSource*)p);
   }
   static void deleteArray_ERRootSource(void *p) {
      delete [] ((::ERRootSource*)p);
   }
   static void destruct_ERRootSource(void *p) {
      typedef ::ERRootSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERRootSource

//______________________________________________________________________________
void ERNXyterTreeSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNXyterTreeSource.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNXyterTreeSource::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNXyterTreeSource::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNXyterTreeSource(void *p) {
      return  p ? new(p) ::ERNXyterTreeSource : new ::ERNXyterTreeSource;
   }
   static void *newArray_ERNXyterTreeSource(Long_t nElements, void *p) {
      return p ? new(p) ::ERNXyterTreeSource[nElements] : new ::ERNXyterTreeSource[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNXyterTreeSource(void *p) {
      delete ((::ERNXyterTreeSource*)p);
   }
   static void deleteArray_ERNXyterTreeSource(void *p) {
      delete [] ((::ERNXyterTreeSource*)p);
   }
   static void destruct_ERNXyterTreeSource(void *p) {
      typedef ::ERNXyterTreeSource current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNXyterTreeSource

//______________________________________________________________________________
void ERNXyterCalibrator::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNXyterCalibrator.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNXyterCalibrator::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNXyterCalibrator::Class(),this);
   }
}

namespace ROOT {
} // end of namespace ROOT for class ::ERNXyterCalibrator

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERpmtPixelMap(void *p) {
      return  p ? new(p) ::ERpmtPixelMap : new ::ERpmtPixelMap;
   }
   static void *newArray_ERpmtPixelMap(Long_t nElements, void *p) {
      return p ? new(p) ::ERpmtPixelMap[nElements] : new ::ERpmtPixelMap[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERpmtPixelMap(void *p) {
      delete ((::ERpmtPixelMap*)p);
   }
   static void deleteArray_ERpmtPixelMap(void *p) {
      delete [] ((::ERpmtPixelMap*)p);
   }
   static void destruct_ERpmtPixelMap(void *p) {
      typedef ::ERpmtPixelMap current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERpmtPixelMap

namespace ROOT {
   static TClass *vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_Dictionary();
   static void vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_TClassManip(TClass*);
   static void *new_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p = 0);
   static void *newArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(Long_t size, void *p);
   static void delete_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p);
   static void deleteArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p);
   static void destruct_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<basic_ifstream<char,char_traits<char> >*>*)
   {
      vector<basic_ifstream<char,char_traits<char> >*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<basic_ifstream<char,char_traits<char> >*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<basic_ifstream<char,char_traits<char> >*>", -2, "vector", 210,
                  typeid(vector<basic_ifstream<char,char_traits<char> >*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<basic_ifstream<char,char_traits<char> >*>) );
      instance.SetNew(&new_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR);
      instance.SetNewArray(&newArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR);
      instance.SetDelete(&delete_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR);
      instance.SetDestructor(&destruct_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<basic_ifstream<char,char_traits<char> >*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<basic_ifstream<char,char_traits<char> >*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<basic_ifstream<char,char_traits<char> >*>*)0x0)->GetClass();
      vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<basic_ifstream<char,char_traits<char> >*> : new vector<basic_ifstream<char,char_traits<char> >*>;
   }
   static void *newArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<basic_ifstream<char,char_traits<char> >*>[nElements] : new vector<basic_ifstream<char,char_traits<char> >*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p) {
      delete ((vector<basic_ifstream<char,char_traits<char> >*>*)p);
   }
   static void deleteArray_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p) {
      delete [] ((vector<basic_ifstream<char,char_traits<char> >*>*)p);
   }
   static void destruct_vectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgR(void *p) {
      typedef vector<basic_ifstream<char,char_traits<char> >*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<basic_ifstream<char,char_traits<char> >*>

namespace ROOT {
   static TClass *vectorlETStringgR_Dictionary();
   static void vectorlETStringgR_TClassManip(TClass*);
   static void *new_vectorlETStringgR(void *p = 0);
   static void *newArray_vectorlETStringgR(Long_t size, void *p);
   static void delete_vectorlETStringgR(void *p);
   static void deleteArray_vectorlETStringgR(void *p);
   static void destruct_vectorlETStringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TString>*)
   {
      vector<TString> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TString>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TString>", -2, "vector", 210,
                  typeid(vector<TString>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETStringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TString>) );
      instance.SetNew(&new_vectorlETStringgR);
      instance.SetNewArray(&newArray_vectorlETStringgR);
      instance.SetDelete(&delete_vectorlETStringgR);
      instance.SetDeleteArray(&deleteArray_vectorlETStringgR);
      instance.SetDestructor(&destruct_vectorlETStringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TString> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<TString>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETStringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TString>*)0x0)->GetClass();
      vectorlETStringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETStringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETStringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString> : new vector<TString>;
   }
   static void *newArray_vectorlETStringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TString>[nElements] : new vector<TString>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETStringgR(void *p) {
      delete ((vector<TString>*)p);
   }
   static void deleteArray_vectorlETStringgR(void *p) {
      delete [] ((vector<TString>*)p);
   }
   static void destruct_vectorlETStringgR(void *p) {
      typedef vector<TString> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TString>

namespace ROOT {
   static TClass *maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_Dictionary();
   static void maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p = 0);
   static void *newArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p);
   static void destruct_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<basic_ifstream<char,char_traits<char> >*> >*)
   {
      map<int,vector<basic_ifstream<char,char_traits<char> >*> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<basic_ifstream<char,char_traits<char> >*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<basic_ifstream<char,char_traits<char> >*> >", -2, "map", 96,
                  typeid(map<int,vector<basic_ifstream<char,char_traits<char> >*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<basic_ifstream<char,char_traits<char> >*> >) );
      instance.SetNew(&new_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<basic_ifstream<char,char_traits<char> >*> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<int,vector<basic_ifstream<char,char_traits<char> >*> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<basic_ifstream<char,char_traits<char> >*> >*)0x0)->GetClass();
      maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<basic_ifstream<char,char_traits<char> >*> > : new map<int,vector<basic_ifstream<char,char_traits<char> >*> >;
   }
   static void *newArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<basic_ifstream<char,char_traits<char> >*> >[nElements] : new map<int,vector<basic_ifstream<char,char_traits<char> >*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p) {
      delete ((map<int,vector<basic_ifstream<char,char_traits<char> >*> >*)p);
   }
   static void deleteArray_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p) {
      delete [] ((map<int,vector<basic_ifstream<char,char_traits<char> >*> >*)p);
   }
   static void destruct_maplEintcOvectorlEbasic_ifstreamlEcharcOchar_traitslEchargRsPgRmUgRsPgR(void *p) {
      typedef map<int,vector<basic_ifstream<char,char_traits<char> >*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<basic_ifstream<char,char_traits<char> >*> >

namespace ROOT {
   static TClass *maplEintcOvectorlETStringgRsPgR_Dictionary();
   static void maplEintcOvectorlETStringgRsPgR_TClassManip(TClass*);
   static void *new_maplEintcOvectorlETStringgRsPgR(void *p = 0);
   static void *newArray_maplEintcOvectorlETStringgRsPgR(Long_t size, void *p);
   static void delete_maplEintcOvectorlETStringgRsPgR(void *p);
   static void deleteArray_maplEintcOvectorlETStringgRsPgR(void *p);
   static void destruct_maplEintcOvectorlETStringgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,vector<TString> >*)
   {
      map<int,vector<TString> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,vector<TString> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,vector<TString> >", -2, "map", 96,
                  typeid(map<int,vector<TString> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOvectorlETStringgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,vector<TString> >) );
      instance.SetNew(&new_maplEintcOvectorlETStringgRsPgR);
      instance.SetNewArray(&newArray_maplEintcOvectorlETStringgRsPgR);
      instance.SetDelete(&delete_maplEintcOvectorlETStringgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOvectorlETStringgRsPgR);
      instance.SetDestructor(&destruct_maplEintcOvectorlETStringgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,vector<TString> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<int,vector<TString> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOvectorlETStringgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,vector<TString> >*)0x0)->GetClass();
      maplEintcOvectorlETStringgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOvectorlETStringgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOvectorlETStringgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<TString> > : new map<int,vector<TString> >;
   }
   static void *newArray_maplEintcOvectorlETStringgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,vector<TString> >[nElements] : new map<int,vector<TString> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOvectorlETStringgRsPgR(void *p) {
      delete ((map<int,vector<TString> >*)p);
   }
   static void deleteArray_maplEintcOvectorlETStringgRsPgR(void *p) {
      delete [] ((map<int,vector<TString> >*)p);
   }
   static void destruct_maplEintcOvectorlETStringgRsPgR(void *p) {
      typedef map<int,vector<TString> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,vector<TString> >

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = 0);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 96,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<int,int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)0x0)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace {
  void TriggerDictionaryInitialization_G__ERBeamTimeDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/home/muzalevsky/check/beamtime/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/home/muzalevsky/check/base/",
"/home/muzalevsky/check/data/",
"/home/muzalevsky/check/data/NeuRadData",
"/home/muzalevsky/check/data/beamtime/",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/beamtime/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERBeamTimeDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERDRS4Source;
class ERTektronixSource;
class ERRawToAnalyzeConverter;
class ERSimtoRaw;
class ERLiCalibrator;
class ERRootSource;
class ERNXyterTreeSource;
class ERNXyterCalibrator;
class ERpmtPixelMap;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERBeamTimeDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef ERDRS4Source_H
#define ERDRS4Source_H

#include <stdio.h>

#include "TString.h"

#include "FairSource.h"
#include "FairRootManager.h"

#include "RawEvent.h"

typedef struct {
    char           tag[3];
    char           version;
} FHEADER;

typedef struct {
    char           time_header[4];
} THEADER;

typedef struct {
    char           bn[2];
    unsigned short board_serial_number;
} BHEADER;

typedef struct {
    char           event_header[4];
    unsigned int   event_serial_number;
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short millisecond;
    unsigned short range;   // range center in mV
} EHEADER;

typedef struct {
    char           tc[2];
    unsigned short trigger_cell;
} TCHEADER;

typedef struct {
    char           c[1];
    char           cn[3];
} CHEADER;

class ERDRS4Source : public FairSource
{
  public:
    ERDRS4Source();
    ERDRS4Source(const ERDRS4Source& source);
    virtual ~ERDRS4Source();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void SetFile(TString path){fPath = path;}
    void SetNChanels(Int_t chanels){fNChanels = chanels;}
    void SetNPoints(Int_t points){fNPoints = points;}
  private:
    FILE* f;
    TString fPath;
    FHEADER  fh;
    THEADER  th;
    BHEADER  bh;
    EHEADER  eh;
    TCHEADER tch;
    CHEADER  ch;

    unsigned int scaler;
    unsigned short voltage[1024];
    double waveform[16][4][1024], time[16][4][1024];
    float bin_width[16][4][1024];
    int i, j, b, chn, n, chn_index, n_boards;
    double t1, t2, t3, t4, dt, dt34;
    char filename[256]; //for input binary file
    char outroot[256];  //for output root file

    int ndt;
    double threshold, sumdt, sumdt2;

    RawEvent** fRawEvents;
    Int_t fNChanels;
    Int_t fNPoints;

  protected:
    Int_t ReadHeader();
  public:
    ClassDef(ERDRS4Source, 1)
};


#endif
#ifndef ERTektronixSource_H
#define ERTektronixSource_H

#include <stdio.h>
#include <map>
#include <fstream>

using namespace std;

#include "TString.h"

#include "FairSource.h"

#include "RawEvent.h"

class ERTektronixSource : public FairSource
{
  public:
    ERTektronixSource();
    ERTektronixSource(const ERTektronixSource& source);
    virtual ~ERTektronixSource();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void AddFile(Int_t chanel, TString path);
    void SetNPoints(Int_t pointsNb){fNPoints = pointsNb;}
    void SetNChanels(Int_t chanels){fNChanels = chanels;}
  private:
    map<Int_t, vector<TString> > fPaths;
    map<Int_t, vector<ifstream*> > fFiles;
    Int_t fNPoints;
    Int_t fNProcessedFiles;
    Int_t fNForProcFiles;
    Int_t fNChanels;
    map<Int_t, Int_t> fCurrentFiles;

    //Output data
    RawEvent** fRawEvents;
  public:
    ClassDef(ERTektronixSource, 1)
};


#endif
// -------------------------------------------------------------------------
// -----                  ERRawToAnalyzeConverter header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERRawToAnalyzeConverter_H
#define ERRawToAnalyzeConverter_H

#include "FairTask.h"

#include "RawEvent.h"
#include "AEvent.h"

class ERRawToAnalyzeConverter : public FairTask {

public:
  /** Default constructor **/
  ERRawToAnalyzeConverter();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERRawToAnalyzeConverter(Int_t verbose);

  /** Destructor **/
  ~ERRawToAnalyzeConverter();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  void SetNChanels(Int_t chanels){fNChanels = chanels;}
  void SetNPoints(Int_t points){fNPoints = points;}
  void SetSmooth(Int_t smpar){ if (smpar == 1) fSmoothPar = kTRUE;}

protected:
  //Input objects
  RawEvent** fRawEvents;

  //Output arrays
  AEvent** fAEvents;

  Int_t fNChanels;
  Int_t fNPoints;
  Bool_t fSmoothPar;

  const Double_t fRatio;
  const Double_t fTD; //in ns
  Double_t fNoiseMin;
  Double_t fNoiseMax;
  Int_t fNumSmoothP;	//number of points to smooth

  Int_t fEvent;

private:
  virtual void SetParContainers();
  
  ClassDef(ERRawToAnalyzeConverter,1)
};

#endif
// -------------------------------------------------------------------------
// -----                  ERLiCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERLiCalibrator_H
#define ERLiCalibrator_H

#include "FairTask.h"

#include "TLiEvent.h"

class ERLiCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERLiCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERLiCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERLiCalibrator();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

protected:
  //Input objects
  TLiEvent* fRawEvent;

  Int_t fEvent;

private:
  virtual void SetParContainers();
  
  ClassDef(ERLiCalibrator,1)
};

#endif
#ifndef ERRootSource_H
#define ERRootSource_H

#include <stdio.h>
#include <map>
#include <fstream>

using namespace std;

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "FairSource.h"

#include "TLiEvent.h"

class ERRootSource : public FairSource
{
  public:
    ERRootSource();
    ERRootSource(const ERRootSource& source);
    virtual ~ERRootSource();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void SetFile(TString path, TString treeName, TString branchName);
  private:
    TString fPath;
    TString fTreeName;
    TString fBranchName;
    TFile* fFile;
    TTree* fTree;
    //Iput data
    TLiEvent* fInEvent;
    Int_t fEvent;
  public:
    ClassDef(ERRootSource, 1)
};


#endif
/*
 * ERNXyterTreeSource.h
 *
 *  Created on: Apr 3, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERNXYTERTREESOURCE_H_
#define BEAMTIME_ERNXYTERTREESOURCE_H_

#include "FairSource.h"

#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"

#include "cls_RootEvent.h"
#include "WCalEvent.h"

#include <fstream>

using std::cout;
using std::endl;

class ERNXyterTreeSource: public FairSource {
public:
	ERNXyterTreeSource();
	virtual ~ERNXyterTreeSource();
	ClassDef(ERNXyterTreeSource,1)

	/** Virtual method Init **/
	virtual Bool_t Init();

	virtual Int_t ReadEvent(UInt_t=0);

	virtual void Close();

	virtual void Reset();

	virtual Source_Type GetSourceType(){return kFILE;}
	virtual void SetParUnpackers(){}
	virtual Bool_t InitUnpackers(){return kTRUE;}
	virtual Bool_t ReInitUnpackers(){return kTRUE;}

	void SetInFile(TString path, TString treeName, TString branchName);

	void SetCalParameters(const char* p_filename);
	void SetNonLinGraphs(const char* graph_filename);
	void PrintCalParameters();

private:
	void ImportCalParameters();
	void ImportNonLinGraphs();


private:

	TString fPath;
	TString fTreeName;
	TString fBranchName;

	TString fCalParFileName;
	TString fNonLinGraphsFileName;

	TFile* fFile;
	TTree* fTree;

	cls_RootEvent* fInEvent;
	WCalEvent *fOutEvent;

	Long64_t fEvent;

	Float_t fCalPar[64];
	const Float_t f1ePosCorrection = 1.18;
	Float_t fPedestalsCorrection[64];

	TRandom3 calRandom;

};

#endif /* BEAMTIME_ERNXYTERTREESOURCE_H_ */
/*
 * ERNXyterCalibrator.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERNXYTERCALIBRATOR_H_
#define BEAMTIME_ERNXYTERCALIBRATOR_H_

#include "TString.h"
#include "TGraph.h"
#include "TH1D.h"
//#include "Rtypes.h"
//#include "TObject.h"

class ERNXyterCalibrator : public TObject {

private: //Singleton standart
	ERNXyterCalibrator();
	virtual ~ERNXyterCalibrator();
	ClassDef(ERNXyterCalibrator,1);

public:	//Singleton standard
	static ERNXyterCalibrator& Instance(void);

public: // methods
    UInt_t ImportGraphs(TString graphFile);

    // Input value - (ped - rawVal)
    Float_t GetCalibratedVal(UInt_t ch, Float_t val);

private: // methods
//    void GenerateDummyLUTs(void);
//    void GenerateLUTs(void);
    void Deconstruct(void);

private: // data members
    TGraph* fGraph[64];

    Bool_t fConstructed; // Defines whether the histograms have been allocated

};

#endif /* BEAMTIME_ERNXYTERCALIBRATOR_H_ */
/*
 * ERpmtPixelMap.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERPMTPIXELMAP_H_
#define BEAMTIME_ERPMTPIXELMAP_H_

#include "Rtypes.h"

class ERpmtPixelMap {
public:
	ERpmtPixelMap();
	virtual ~ERpmtPixelMap();

private:
	// FEB channels are from 0 (incl.) to 63 (incl.)
	// Pixel IDs are from 1 (incl.) to 64 (incl.)

	UInt_t fPixelFromFebCh[64];
	UInt_t fFebChFromPixel[64+1];   // 0-th element not used
};

#endif /* BEAMTIME_ERPMTPIXELMAP_H_ */
// -------------------------------------------------------------------------
// -----                  ERRawToAnalyzeConverter header file          -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERSimtoRaw_H
#define ERRSimtoRaw_H

#include "FairTask.h"

#include "ERNeuRadPixelSignal.h"
#include "RawEvent.h"
#include "AEvent.h"
#include "TArrayF.h"

class ERSimtoRaw : public FairTask {

public:
  /** Default constructor **/
  ERSimtoRaw();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERSimtoRaw(Int_t NEvents);

  /** Destructor **/
  ~ERSimtoRaw();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

protected:
  //Input objects
  TClonesArray *fPixelSignals;

  //Output arrays
  RawEvent** fRawEvents;


  Int_t fNChanels;
  Int_t fNPoints;
  Int_t fEvent;

  //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
  Float_t* fResFunction;    //!
  //Хранит результирующий сигнал, как значения функции в узлах
  TArrayF fResFunctionRoot; 


/*private:
  virtual void SetParContainers();
  
  ClassDef(ERRawToAnalyzeConverter,1)*/
};

#endif

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ERDRS4Source", payloadCode, "@",
"ERLiCalibrator", payloadCode, "@",
"ERNXyterCalibrator", payloadCode, "@",
"ERNXyterTreeSource", payloadCode, "@",
"ERRawToAnalyzeConverter", payloadCode, "@",
"ERRootSource", payloadCode, "@",
"ERSimtoRaw", payloadCode, "@",
"ERTektronixSource", payloadCode, "@",
"ERpmtPixelMap", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERBeamTimeDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERBeamTimeDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERBeamTimeDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERBeamTimeDict() {
  TriggerDictionaryInitialization_G__ERBeamTimeDict_Impl();
}
