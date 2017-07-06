// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedImuzalevskydIcheckdIbuilddIdatadIG__ERDataDict

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
#include "NeuRadData/ERNeuRadPoint.h"
#include "NeuRadData/ERNeuRadPhotoElectron.h"
#include "NeuRadData/ERNeuRadPixelSignal.h"
#include "NeuRadData/ERNeuRadDigi.h"
#include "NeuRadData/ERNeuRadStep.h"
#include "NeuRadData/ERNeuRadHit.h"
#include "NeuRadData/ERNeuRadHitWBT.h"
#include "muSiData/ERmuSiPoint.h"
#include "muSiData/ERmuSiHit.h"
#include "muSiData/ERmuSiTrack.h"
#include "muSiData/ERmuSiVertex.h"
#include "GadastData/ERGadastCsIPoint.h"
#include "GadastData/ERGadastLaBrPoint.h"
#include "GadastData/ERGadastStep.h"
#include "GadastData/ERGadastDigi.h"
#include "ERStack.h"
#include "ERMCTrack.h"
#include "ERMCEventHeader.h"
#include "EREventHeader.h"
#include "ERLi10EventHeader.h"
#include "ERLi10MCEventHeader.h"
#include "NDData/ERNDPoint.h"
#include "NDData/ERNDHit.h"
#include "DSRDData/ERDSRDPoint.h"
#include "DSRDData/ERDSRDHit.h"
#include "beamtime/RawEvent.h"
#include "beamtime/AEvent.h"
#include "beamtime/TGo4EventElement.h"
#include "beamtime/TLiEvent.h"
#include "beamtime/cls_RootHit.h"
#include "beamtime/cls_RootEvent.h"
#include "beamtime/WCalHit.h"
#include "beamtime/WCalEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_ERStack(void *p = 0);
   static void *newArray_ERStack(Long_t size, void *p);
   static void delete_ERStack(void *p);
   static void deleteArray_ERStack(void *p);
   static void destruct_ERStack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERStack*)
   {
      ::ERStack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERStack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERStack", ::ERStack::Class_Version(), "invalid", 1152,
                  typeid(::ERStack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERStack::Dictionary, isa_proxy, 4,
                  sizeof(::ERStack) );
      instance.SetNew(&new_ERStack);
      instance.SetNewArray(&newArray_ERStack);
      instance.SetDelete(&delete_ERStack);
      instance.SetDeleteArray(&deleteArray_ERStack);
      instance.SetDestructor(&destruct_ERStack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERStack*)
   {
      return GenerateInitInstanceLocal((::ERStack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERStack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERMCTrack(void *p = 0);
   static void *newArray_ERMCTrack(Long_t size, void *p);
   static void delete_ERMCTrack(void *p);
   static void deleteArray_ERMCTrack(void *p);
   static void destruct_ERMCTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERMCTrack*)
   {
      ::ERMCTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERMCTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERMCTrack", ::ERMCTrack::Class_Version(), "ERMCTrack.h", 38,
                  typeid(::ERMCTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERMCTrack::Dictionary, isa_proxy, 4,
                  sizeof(::ERMCTrack) );
      instance.SetNew(&new_ERMCTrack);
      instance.SetNewArray(&newArray_ERMCTrack);
      instance.SetDelete(&delete_ERMCTrack);
      instance.SetDeleteArray(&deleteArray_ERMCTrack);
      instance.SetDestructor(&destruct_ERMCTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERMCTrack*)
   {
      return GenerateInitInstanceLocal((::ERMCTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERMCTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERMCEventHeader(void *p = 0);
   static void *newArray_ERMCEventHeader(Long_t size, void *p);
   static void delete_ERMCEventHeader(void *p);
   static void deleteArray_ERMCEventHeader(void *p);
   static void destruct_ERMCEventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERMCEventHeader*)
   {
      ::ERMCEventHeader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERMCEventHeader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERMCEventHeader", ::ERMCEventHeader::Class_Version(), "invalid", 1521,
                  typeid(::ERMCEventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERMCEventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::ERMCEventHeader) );
      instance.SetNew(&new_ERMCEventHeader);
      instance.SetNewArray(&newArray_ERMCEventHeader);
      instance.SetDelete(&delete_ERMCEventHeader);
      instance.SetDeleteArray(&deleteArray_ERMCEventHeader);
      instance.SetDestructor(&destruct_ERMCEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERMCEventHeader*)
   {
      return GenerateInitInstanceLocal((::ERMCEventHeader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERMCEventHeader*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EREventHeader(void *p = 0);
   static void *newArray_EREventHeader(Long_t size, void *p);
   static void delete_EREventHeader(void *p);
   static void deleteArray_EREventHeader(void *p);
   static void destruct_EREventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EREventHeader*)
   {
      ::EREventHeader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EREventHeader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EREventHeader", ::EREventHeader::Class_Version(), "invalid", 1553,
                  typeid(::EREventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EREventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::EREventHeader) );
      instance.SetNew(&new_EREventHeader);
      instance.SetNewArray(&newArray_EREventHeader);
      instance.SetDelete(&delete_EREventHeader);
      instance.SetDeleteArray(&deleteArray_EREventHeader);
      instance.SetDestructor(&destruct_EREventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EREventHeader*)
   {
      return GenerateInitInstanceLocal((::EREventHeader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EREventHeader*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERLi10EventHeader(void *p = 0);
   static void *newArray_ERLi10EventHeader(Long_t size, void *p);
   static void delete_ERLi10EventHeader(void *p);
   static void deleteArray_ERLi10EventHeader(void *p);
   static void destruct_ERLi10EventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERLi10EventHeader*)
   {
      ::ERLi10EventHeader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERLi10EventHeader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERLi10EventHeader", ::ERLi10EventHeader::Class_Version(), "invalid", 1588,
                  typeid(::ERLi10EventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERLi10EventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::ERLi10EventHeader) );
      instance.SetNew(&new_ERLi10EventHeader);
      instance.SetNewArray(&newArray_ERLi10EventHeader);
      instance.SetDelete(&delete_ERLi10EventHeader);
      instance.SetDeleteArray(&deleteArray_ERLi10EventHeader);
      instance.SetDestructor(&destruct_ERLi10EventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERLi10EventHeader*)
   {
      return GenerateInitInstanceLocal((::ERLi10EventHeader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERLi10EventHeader*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERLi10MCEventHeader(void *p = 0);
   static void *newArray_ERLi10MCEventHeader(Long_t size, void *p);
   static void delete_ERLi10MCEventHeader(void *p);
   static void deleteArray_ERLi10MCEventHeader(void *p);
   static void destruct_ERLi10MCEventHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERLi10MCEventHeader*)
   {
      ::ERLi10MCEventHeader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERLi10MCEventHeader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERLi10MCEventHeader", ::ERLi10MCEventHeader::Class_Version(), "invalid", 1619,
                  typeid(::ERLi10MCEventHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERLi10MCEventHeader::Dictionary, isa_proxy, 4,
                  sizeof(::ERLi10MCEventHeader) );
      instance.SetNew(&new_ERLi10MCEventHeader);
      instance.SetNewArray(&newArray_ERLi10MCEventHeader);
      instance.SetDelete(&delete_ERLi10MCEventHeader);
      instance.SetDeleteArray(&deleteArray_ERLi10MCEventHeader);
      instance.SetDestructor(&destruct_ERLi10MCEventHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERLi10MCEventHeader*)
   {
      return GenerateInitInstanceLocal((::ERLi10MCEventHeader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERLi10MCEventHeader*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadPoint(void *p = 0);
   static void *newArray_ERNeuRadPoint(Long_t size, void *p);
   static void delete_ERNeuRadPoint(void *p);
   static void deleteArray_ERNeuRadPoint(void *p);
   static void destruct_ERNeuRadPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadPoint*)
   {
      ::ERNeuRadPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadPoint", ::ERNeuRadPoint::Class_Version(), "invalid", 26,
                  typeid(::ERNeuRadPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadPoint) );
      instance.SetNew(&new_ERNeuRadPoint);
      instance.SetNewArray(&newArray_ERNeuRadPoint);
      instance.SetDelete(&delete_ERNeuRadPoint);
      instance.SetDeleteArray(&deleteArray_ERNeuRadPoint);
      instance.SetDestructor(&destruct_ERNeuRadPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadPoint*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadPhotoElectron(void *p = 0);
   static void *newArray_ERNeuRadPhotoElectron(Long_t size, void *p);
   static void delete_ERNeuRadPhotoElectron(void *p);
   static void deleteArray_ERNeuRadPhotoElectron(void *p);
   static void destruct_ERNeuRadPhotoElectron(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadPhotoElectron*)
   {
      ::ERNeuRadPhotoElectron *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadPhotoElectron >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadPhotoElectron", ::ERNeuRadPhotoElectron::Class_Version(), "invalid", 161,
                  typeid(::ERNeuRadPhotoElectron), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadPhotoElectron::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadPhotoElectron) );
      instance.SetNew(&new_ERNeuRadPhotoElectron);
      instance.SetNewArray(&newArray_ERNeuRadPhotoElectron);
      instance.SetDelete(&delete_ERNeuRadPhotoElectron);
      instance.SetDeleteArray(&deleteArray_ERNeuRadPhotoElectron);
      instance.SetDestructor(&destruct_ERNeuRadPhotoElectron);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadPhotoElectron*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadPhotoElectron*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadPhotoElectron*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadPixelSignal(void *p = 0);
   static void *newArray_ERNeuRadPixelSignal(Long_t size, void *p);
   static void delete_ERNeuRadPixelSignal(void *p);
   static void deleteArray_ERNeuRadPixelSignal(void *p);
   static void destruct_ERNeuRadPixelSignal(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadPixelSignal*)
   {
      ::ERNeuRadPixelSignal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadPixelSignal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadPixelSignal", ::ERNeuRadPixelSignal::Class_Version(), "invalid", 216,
                  typeid(::ERNeuRadPixelSignal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadPixelSignal::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadPixelSignal) );
      instance.SetNew(&new_ERNeuRadPixelSignal);
      instance.SetNewArray(&newArray_ERNeuRadPixelSignal);
      instance.SetDelete(&delete_ERNeuRadPixelSignal);
      instance.SetDeleteArray(&deleteArray_ERNeuRadPixelSignal);
      instance.SetDestructor(&destruct_ERNeuRadPixelSignal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadPixelSignal*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadPixelSignal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadPixelSignal*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadDigi(void *p = 0);
   static void *newArray_ERNeuRadDigi(Long_t size, void *p);
   static void delete_ERNeuRadDigi(void *p);
   static void deleteArray_ERNeuRadDigi(void *p);
   static void destruct_ERNeuRadDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadDigi*)
   {
      ::ERNeuRadDigi *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadDigi >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadDigi", ::ERNeuRadDigi::Class_Version(), "invalid", 331,
                  typeid(::ERNeuRadDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadDigi::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadDigi) );
      instance.SetNew(&new_ERNeuRadDigi);
      instance.SetNewArray(&newArray_ERNeuRadDigi);
      instance.SetDelete(&delete_ERNeuRadDigi);
      instance.SetDeleteArray(&deleteArray_ERNeuRadDigi);
      instance.SetDestructor(&destruct_ERNeuRadDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadDigi*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadDigi*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadDigi*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadStep(void *p = 0);
   static void *newArray_ERNeuRadStep(Long_t size, void *p);
   static void delete_ERNeuRadStep(void *p);
   static void deleteArray_ERNeuRadStep(void *p);
   static void destruct_ERNeuRadStep(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadStep*)
   {
      ::ERNeuRadStep *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadStep >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadStep", ::ERNeuRadStep::Class_Version(), "invalid", 421,
                  typeid(::ERNeuRadStep), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadStep::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadStep) );
      instance.SetNew(&new_ERNeuRadStep);
      instance.SetNewArray(&newArray_ERNeuRadStep);
      instance.SetDelete(&delete_ERNeuRadStep);
      instance.SetDeleteArray(&deleteArray_ERNeuRadStep);
      instance.SetDestructor(&destruct_ERNeuRadStep);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadStep*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadStep*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadStep*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadHit(void *p = 0);
   static void *newArray_ERNeuRadHit(Long_t size, void *p);
   static void delete_ERNeuRadHit(void *p);
   static void deleteArray_ERNeuRadHit(void *p);
   static void destruct_ERNeuRadHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadHit*)
   {
      ::ERNeuRadHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadHit", ::ERNeuRadHit::Class_Version(), "invalid", 511,
                  typeid(::ERNeuRadHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadHit::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadHit) );
      instance.SetNew(&new_ERNeuRadHit);
      instance.SetNewArray(&newArray_ERNeuRadHit);
      instance.SetDelete(&delete_ERNeuRadHit);
      instance.SetDeleteArray(&deleteArray_ERNeuRadHit);
      instance.SetDestructor(&destruct_ERNeuRadHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadHit*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNeuRadHitWBT(void *p = 0);
   static void *newArray_ERNeuRadHitWBT(Long_t size, void *p);
   static void delete_ERNeuRadHitWBT(void *p);
   static void deleteArray_ERNeuRadHitWBT(void *p);
   static void destruct_ERNeuRadHitWBT(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNeuRadHitWBT*)
   {
      ::ERNeuRadHitWBT *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNeuRadHitWBT >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNeuRadHitWBT", ::ERNeuRadHitWBT::Class_Version(), "invalid", 544,
                  typeid(::ERNeuRadHitWBT), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNeuRadHitWBT::Dictionary, isa_proxy, 4,
                  sizeof(::ERNeuRadHitWBT) );
      instance.SetNew(&new_ERNeuRadHitWBT);
      instance.SetNewArray(&newArray_ERNeuRadHitWBT);
      instance.SetDelete(&delete_ERNeuRadHitWBT);
      instance.SetDeleteArray(&deleteArray_ERNeuRadHitWBT);
      instance.SetDestructor(&destruct_ERNeuRadHitWBT);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNeuRadHitWBT*)
   {
      return GenerateInitInstanceLocal((::ERNeuRadHitWBT*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNeuRadHitWBT*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiPoint(void *p = 0);
   static void *newArray_ERmuSiPoint(Long_t size, void *p);
   static void delete_ERmuSiPoint(void *p);
   static void deleteArray_ERmuSiPoint(void *p);
   static void destruct_ERmuSiPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiPoint*)
   {
      ::ERmuSiPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiPoint", ::ERmuSiPoint::Class_Version(), "invalid", 581,
                  typeid(::ERmuSiPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiPoint) );
      instance.SetNew(&new_ERmuSiPoint);
      instance.SetNewArray(&newArray_ERmuSiPoint);
      instance.SetDelete(&delete_ERmuSiPoint);
      instance.SetDeleteArray(&deleteArray_ERmuSiPoint);
      instance.SetDestructor(&destruct_ERmuSiPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiPoint*)
   {
      return GenerateInitInstanceLocal((::ERmuSiPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiHit(void *p = 0);
   static void *newArray_ERmuSiHit(Long_t size, void *p);
   static void delete_ERmuSiHit(void *p);
   static void deleteArray_ERmuSiHit(void *p);
   static void destruct_ERmuSiHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiHit*)
   {
      ::ERmuSiHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiHit", ::ERmuSiHit::Class_Version(), "invalid", 678,
                  typeid(::ERmuSiHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiHit::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiHit) );
      instance.SetNew(&new_ERmuSiHit);
      instance.SetNewArray(&newArray_ERmuSiHit);
      instance.SetDelete(&delete_ERmuSiHit);
      instance.SetDeleteArray(&deleteArray_ERmuSiHit);
      instance.SetDestructor(&destruct_ERmuSiHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiHit*)
   {
      return GenerateInitInstanceLocal((::ERmuSiHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiTrack(void *p = 0);
   static void *newArray_ERmuSiTrack(Long_t size, void *p);
   static void delete_ERmuSiTrack(void *p);
   static void deleteArray_ERmuSiTrack(void *p);
   static void destruct_ERmuSiTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiTrack*)
   {
      ::ERmuSiTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiTrack", ::ERmuSiTrack::Class_Version(), "invalid", 706,
                  typeid(::ERmuSiTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiTrack::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiTrack) );
      instance.SetNew(&new_ERmuSiTrack);
      instance.SetNewArray(&newArray_ERmuSiTrack);
      instance.SetDelete(&delete_ERmuSiTrack);
      instance.SetDeleteArray(&deleteArray_ERmuSiTrack);
      instance.SetDestructor(&destruct_ERmuSiTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiTrack*)
   {
      return GenerateInitInstanceLocal((::ERmuSiTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERmuSiVertex(void *p = 0);
   static void *newArray_ERmuSiVertex(Long_t size, void *p);
   static void delete_ERmuSiVertex(void *p);
   static void deleteArray_ERmuSiVertex(void *p);
   static void destruct_ERmuSiVertex(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERmuSiVertex*)
   {
      ::ERmuSiVertex *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERmuSiVertex >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERmuSiVertex", ::ERmuSiVertex::Class_Version(), "invalid", 730,
                  typeid(::ERmuSiVertex), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERmuSiVertex::Dictionary, isa_proxy, 4,
                  sizeof(::ERmuSiVertex) );
      instance.SetNew(&new_ERmuSiVertex);
      instance.SetNewArray(&newArray_ERmuSiVertex);
      instance.SetDelete(&delete_ERmuSiVertex);
      instance.SetDeleteArray(&deleteArray_ERmuSiVertex);
      instance.SetDestructor(&destruct_ERmuSiVertex);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERmuSiVertex*)
   {
      return GenerateInitInstanceLocal((::ERmuSiVertex*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERmuSiVertex*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastCsIPoint(void *p = 0);
   static void *newArray_ERGadastCsIPoint(Long_t size, void *p);
   static void delete_ERGadastCsIPoint(void *p);
   static void deleteArray_ERGadastCsIPoint(void *p);
   static void destruct_ERGadastCsIPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastCsIPoint*)
   {
      ::ERGadastCsIPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastCsIPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastCsIPoint", ::ERGadastCsIPoint::Class_Version(), "invalid", 773,
                  typeid(::ERGadastCsIPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastCsIPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastCsIPoint) );
      instance.SetNew(&new_ERGadastCsIPoint);
      instance.SetNewArray(&newArray_ERGadastCsIPoint);
      instance.SetDelete(&delete_ERGadastCsIPoint);
      instance.SetDeleteArray(&deleteArray_ERGadastCsIPoint);
      instance.SetDestructor(&destruct_ERGadastCsIPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastCsIPoint*)
   {
      return GenerateInitInstanceLocal((::ERGadastCsIPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastCsIPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastLaBrPoint(void *p = 0);
   static void *newArray_ERGadastLaBrPoint(Long_t size, void *p);
   static void delete_ERGadastLaBrPoint(void *p);
   static void deleteArray_ERGadastLaBrPoint(void *p);
   static void destruct_ERGadastLaBrPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastLaBrPoint*)
   {
      ::ERGadastLaBrPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastLaBrPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastLaBrPoint", ::ERGadastLaBrPoint::Class_Version(), "invalid", 882,
                  typeid(::ERGadastLaBrPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastLaBrPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastLaBrPoint) );
      instance.SetNew(&new_ERGadastLaBrPoint);
      instance.SetNewArray(&newArray_ERGadastLaBrPoint);
      instance.SetDelete(&delete_ERGadastLaBrPoint);
      instance.SetDeleteArray(&deleteArray_ERGadastLaBrPoint);
      instance.SetDestructor(&destruct_ERGadastLaBrPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastLaBrPoint*)
   {
      return GenerateInitInstanceLocal((::ERGadastLaBrPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastLaBrPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastStep(void *p = 0);
   static void *newArray_ERGadastStep(Long_t size, void *p);
   static void delete_ERGadastStep(void *p);
   static void deleteArray_ERGadastStep(void *p);
   static void destruct_ERGadastStep(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastStep*)
   {
      ::ERGadastStep *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastStep >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastStep", ::ERGadastStep::Class_Version(), "invalid", 989,
                  typeid(::ERGadastStep), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastStep::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastStep) );
      instance.SetNew(&new_ERGadastStep);
      instance.SetNewArray(&newArray_ERGadastStep);
      instance.SetDelete(&delete_ERGadastStep);
      instance.SetDeleteArray(&deleteArray_ERGadastStep);
      instance.SetDestructor(&destruct_ERGadastStep);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastStep*)
   {
      return GenerateInitInstanceLocal((::ERGadastStep*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastStep*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERGadastDigi(void *p = 0);
   static void *newArray_ERGadastDigi(Long_t size, void *p);
   static void delete_ERGadastDigi(void *p);
   static void deleteArray_ERGadastDigi(void *p);
   static void destruct_ERGadastDigi(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERGadastDigi*)
   {
      ::ERGadastDigi *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERGadastDigi >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERGadastDigi", ::ERGadastDigi::Class_Version(), "invalid", 1077,
                  typeid(::ERGadastDigi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERGadastDigi::Dictionary, isa_proxy, 4,
                  sizeof(::ERGadastDigi) );
      instance.SetNew(&new_ERGadastDigi);
      instance.SetNewArray(&newArray_ERGadastDigi);
      instance.SetDelete(&delete_ERGadastDigi);
      instance.SetDeleteArray(&deleteArray_ERGadastDigi);
      instance.SetDestructor(&destruct_ERGadastDigi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERGadastDigi*)
   {
      return GenerateInitInstanceLocal((::ERGadastDigi*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERGadastDigi*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNDPoint(void *p = 0);
   static void *newArray_ERNDPoint(Long_t size, void *p);
   static void delete_ERNDPoint(void *p);
   static void deleteArray_ERNDPoint(void *p);
   static void destruct_ERNDPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNDPoint*)
   {
      ::ERNDPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNDPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNDPoint", ::ERNDPoint::Class_Version(), "invalid", 1656,
                  typeid(::ERNDPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNDPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERNDPoint) );
      instance.SetNew(&new_ERNDPoint);
      instance.SetNewArray(&newArray_ERNDPoint);
      instance.SetDelete(&delete_ERNDPoint);
      instance.SetDeleteArray(&deleteArray_ERNDPoint);
      instance.SetDestructor(&destruct_ERNDPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNDPoint*)
   {
      return GenerateInitInstanceLocal((::ERNDPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNDPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERNDHit(void *p = 0);
   static void *newArray_ERNDHit(Long_t size, void *p);
   static void delete_ERNDHit(void *p);
   static void deleteArray_ERNDHit(void *p);
   static void destruct_ERNDHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERNDHit*)
   {
      ::ERNDHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERNDHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERNDHit", ::ERNDHit::Class_Version(), "invalid", 1752,
                  typeid(::ERNDHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERNDHit::Dictionary, isa_proxy, 4,
                  sizeof(::ERNDHit) );
      instance.SetNew(&new_ERNDHit);
      instance.SetNewArray(&newArray_ERNDHit);
      instance.SetDelete(&delete_ERNDHit);
      instance.SetDeleteArray(&deleteArray_ERNDHit);
      instance.SetDestructor(&destruct_ERNDHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERNDHit*)
   {
      return GenerateInitInstanceLocal((::ERNDHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERNDHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDSRDPoint(void *p = 0);
   static void *newArray_ERDSRDPoint(Long_t size, void *p);
   static void delete_ERDSRDPoint(void *p);
   static void deleteArray_ERDSRDPoint(void *p);
   static void destruct_ERDSRDPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDSRDPoint*)
   {
      ::ERDSRDPoint *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDSRDPoint >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDSRDPoint", ::ERDSRDPoint::Class_Version(), "invalid", 1797,
                  typeid(::ERDSRDPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDSRDPoint::Dictionary, isa_proxy, 4,
                  sizeof(::ERDSRDPoint) );
      instance.SetNew(&new_ERDSRDPoint);
      instance.SetNewArray(&newArray_ERDSRDPoint);
      instance.SetDelete(&delete_ERDSRDPoint);
      instance.SetDeleteArray(&deleteArray_ERDSRDPoint);
      instance.SetDestructor(&destruct_ERDSRDPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDSRDPoint*)
   {
      return GenerateInitInstanceLocal((::ERDSRDPoint*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDSRDPoint*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ERDSRDHit(void *p = 0);
   static void *newArray_ERDSRDHit(Long_t size, void *p);
   static void delete_ERDSRDHit(void *p);
   static void deleteArray_ERDSRDHit(void *p);
   static void destruct_ERDSRDHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ERDSRDHit*)
   {
      ::ERDSRDHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ERDSRDHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ERDSRDHit", ::ERDSRDHit::Class_Version(), "invalid", 1891,
                  typeid(::ERDSRDHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ERDSRDHit::Dictionary, isa_proxy, 4,
                  sizeof(::ERDSRDHit) );
      instance.SetNew(&new_ERDSRDHit);
      instance.SetNewArray(&newArray_ERDSRDHit);
      instance.SetDelete(&delete_ERDSRDHit);
      instance.SetDeleteArray(&deleteArray_ERDSRDHit);
      instance.SetDestructor(&destruct_ERDSRDHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ERDSRDHit*)
   {
      return GenerateInitInstanceLocal((::ERDSRDHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ERDSRDHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RawEvent(void *p = 0);
   static void *newArray_RawEvent(Long_t size, void *p);
   static void delete_RawEvent(void *p);
   static void deleteArray_RawEvent(void *p);
   static void destruct_RawEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RawEvent*)
   {
      ::RawEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RawEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RawEvent", ::RawEvent::Class_Version(), "invalid", 1937,
                  typeid(::RawEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RawEvent::Dictionary, isa_proxy, 4,
                  sizeof(::RawEvent) );
      instance.SetNew(&new_RawEvent);
      instance.SetNewArray(&newArray_RawEvent);
      instance.SetDelete(&delete_RawEvent);
      instance.SetDeleteArray(&deleteArray_RawEvent);
      instance.SetDestructor(&destruct_RawEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RawEvent*)
   {
      return GenerateInitInstanceLocal((::RawEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RawEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AEvent(void *p = 0);
   static void *newArray_AEvent(Long_t size, void *p);
   static void delete_AEvent(void *p);
   static void deleteArray_AEvent(void *p);
   static void destruct_AEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AEvent*)
   {
      ::AEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AEvent", ::AEvent::Class_Version(), "invalid", 2065,
                  typeid(::AEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::AEvent::Dictionary, isa_proxy, 4,
                  sizeof(::AEvent) );
      instance.SetNew(&new_AEvent);
      instance.SetNewArray(&newArray_AEvent);
      instance.SetDelete(&delete_AEvent);
      instance.SetDeleteArray(&deleteArray_AEvent);
      instance.SetDestructor(&destruct_AEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AEvent*)
   {
      return GenerateInitInstanceLocal((::AEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLiEvent(void *p = 0);
   static void *newArray_TLiEvent(Long_t size, void *p);
   static void delete_TLiEvent(void *p);
   static void deleteArray_TLiEvent(void *p);
   static void destruct_TLiEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLiEvent*)
   {
      ::TLiEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLiEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLiEvent", ::TLiEvent::Class_Version(), "invalid", 2372,
                  typeid(::TLiEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLiEvent::Dictionary, isa_proxy, 4,
                  sizeof(::TLiEvent) );
      instance.SetNew(&new_TLiEvent);
      instance.SetNewArray(&newArray_TLiEvent);
      instance.SetDelete(&delete_TLiEvent);
      instance.SetDeleteArray(&deleteArray_TLiEvent);
      instance.SetDestructor(&destruct_TLiEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLiEvent*)
   {
      return GenerateInitInstanceLocal((::TLiEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TLiEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TGo4EventElement(void *p = 0);
   static void *newArray_TGo4EventElement(Long_t size, void *p);
   static void delete_TGo4EventElement(void *p);
   static void deleteArray_TGo4EventElement(void *p);
   static void destruct_TGo4EventElement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TGo4EventElement*)
   {
      ::TGo4EventElement *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TGo4EventElement >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TGo4EventElement", ::TGo4EventElement::Class_Version(), "invalid", 2255,
                  typeid(::TGo4EventElement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TGo4EventElement::Dictionary, isa_proxy, 4,
                  sizeof(::TGo4EventElement) );
      instance.SetNew(&new_TGo4EventElement);
      instance.SetNewArray(&newArray_TGo4EventElement);
      instance.SetDelete(&delete_TGo4EventElement);
      instance.SetDeleteArray(&deleteArray_TGo4EventElement);
      instance.SetDestructor(&destruct_TGo4EventElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TGo4EventElement*)
   {
      return GenerateInitInstanceLocal((::TGo4EventElement*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TGo4EventElement*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cls_RootHit(void *p = 0);
   static void *newArray_cls_RootHit(Long_t size, void *p);
   static void delete_cls_RootHit(void *p);
   static void deleteArray_cls_RootHit(void *p);
   static void destruct_cls_RootHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cls_RootHit*)
   {
      ::cls_RootHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cls_RootHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cls_RootHit", ::cls_RootHit::Class_Version(), "invalid", 2453,
                  typeid(::cls_RootHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cls_RootHit::Dictionary, isa_proxy, 4,
                  sizeof(::cls_RootHit) );
      instance.SetNew(&new_cls_RootHit);
      instance.SetNewArray(&newArray_cls_RootHit);
      instance.SetDelete(&delete_cls_RootHit);
      instance.SetDeleteArray(&deleteArray_cls_RootHit);
      instance.SetDestructor(&destruct_cls_RootHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cls_RootHit*)
   {
      return GenerateInitInstanceLocal((::cls_RootHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::cls_RootHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_cls_RootEvent(void *p = 0);
   static void *newArray_cls_RootEvent(Long_t size, void *p);
   static void delete_cls_RootEvent(void *p);
   static void deleteArray_cls_RootEvent(void *p);
   static void destruct_cls_RootEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cls_RootEvent*)
   {
      ::cls_RootEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cls_RootEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cls_RootEvent", ::cls_RootEvent::Class_Version(), "invalid", 2498,
                  typeid(::cls_RootEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cls_RootEvent::Dictionary, isa_proxy, 4,
                  sizeof(::cls_RootEvent) );
      instance.SetNew(&new_cls_RootEvent);
      instance.SetNewArray(&newArray_cls_RootEvent);
      instance.SetDelete(&delete_cls_RootEvent);
      instance.SetDeleteArray(&deleteArray_cls_RootEvent);
      instance.SetDestructor(&destruct_cls_RootEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cls_RootEvent*)
   {
      return GenerateInitInstanceLocal((::cls_RootEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::cls_RootEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_WCalHit(void *p = 0);
   static void *newArray_WCalHit(Long_t size, void *p);
   static void delete_WCalHit(void *p);
   static void deleteArray_WCalHit(void *p);
   static void destruct_WCalHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WCalHit*)
   {
      ::WCalHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WCalHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("WCalHit", ::WCalHit::Class_Version(), "invalid", 2545,
                  typeid(::WCalHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::WCalHit::Dictionary, isa_proxy, 4,
                  sizeof(::WCalHit) );
      instance.SetNew(&new_WCalHit);
      instance.SetNewArray(&newArray_WCalHit);
      instance.SetDelete(&delete_WCalHit);
      instance.SetDeleteArray(&deleteArray_WCalHit);
      instance.SetDestructor(&destruct_WCalHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WCalHit*)
   {
      return GenerateInitInstanceLocal((::WCalHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WCalHit*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_WCalEvent(void *p = 0);
   static void *newArray_WCalEvent(Long_t size, void *p);
   static void delete_WCalEvent(void *p);
   static void deleteArray_WCalEvent(void *p);
   static void destruct_WCalEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WCalEvent*)
   {
      ::WCalEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WCalEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("WCalEvent", ::WCalEvent::Class_Version(), "invalid", 2589,
                  typeid(::WCalEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::WCalEvent::Dictionary, isa_proxy, 4,
                  sizeof(::WCalEvent) );
      instance.SetNew(&new_WCalEvent);
      instance.SetNewArray(&newArray_WCalEvent);
      instance.SetDelete(&delete_WCalEvent);
      instance.SetDeleteArray(&deleteArray_WCalEvent);
      instance.SetDestructor(&destruct_WCalEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WCalEvent*)
   {
      return GenerateInitInstanceLocal((::WCalEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WCalEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ERStack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERStack::Class_Name()
{
   return "ERStack";
}

//______________________________________________________________________________
const char *ERStack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERStack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERStack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERStack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERStack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERStack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERStack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERStack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERMCTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERMCTrack::Class_Name()
{
   return "ERMCTrack";
}

//______________________________________________________________________________
const char *ERMCTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERMCTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERMCTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERMCTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERMCEventHeader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERMCEventHeader::Class_Name()
{
   return "ERMCEventHeader";
}

//______________________________________________________________________________
const char *ERMCEventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCEventHeader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERMCEventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERMCEventHeader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERMCEventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCEventHeader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERMCEventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERMCEventHeader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EREventHeader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EREventHeader::Class_Name()
{
   return "EREventHeader";
}

//______________________________________________________________________________
const char *EREventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EREventHeader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EREventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EREventHeader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EREventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EREventHeader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EREventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EREventHeader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERLi10EventHeader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERLi10EventHeader::Class_Name()
{
   return "ERLi10EventHeader";
}

//______________________________________________________________________________
const char *ERLi10EventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10EventHeader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERLi10EventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10EventHeader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERLi10EventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10EventHeader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERLi10EventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10EventHeader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERLi10MCEventHeader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERLi10MCEventHeader::Class_Name()
{
   return "ERLi10MCEventHeader";
}

//______________________________________________________________________________
const char *ERLi10MCEventHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10MCEventHeader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERLi10MCEventHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERLi10MCEventHeader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERLi10MCEventHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10MCEventHeader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERLi10MCEventHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERLi10MCEventHeader*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadPoint::Class_Name()
{
   return "ERNeuRadPoint";
}

//______________________________________________________________________________
const char *ERNeuRadPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadPhotoElectron::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadPhotoElectron::Class_Name()
{
   return "ERNeuRadPhotoElectron";
}

//______________________________________________________________________________
const char *ERNeuRadPhotoElectron::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPhotoElectron*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadPhotoElectron::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPhotoElectron*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadPhotoElectron::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPhotoElectron*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadPhotoElectron::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPhotoElectron*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadPixelSignal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadPixelSignal::Class_Name()
{
   return "ERNeuRadPixelSignal";
}

//______________________________________________________________________________
const char *ERNeuRadPixelSignal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPixelSignal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadPixelSignal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPixelSignal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadPixelSignal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPixelSignal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadPixelSignal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadPixelSignal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadDigi::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadDigi::Class_Name()
{
   return "ERNeuRadDigi";
}

//______________________________________________________________________________
const char *ERNeuRadDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigi*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigi*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigi*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadDigi*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadStep::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadStep::Class_Name()
{
   return "ERNeuRadStep";
}

//______________________________________________________________________________
const char *ERNeuRadStep::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadStep*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadStep::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadStep*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadStep::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadStep*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadStep::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadStep*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadHit::Class_Name()
{
   return "ERNeuRadHit";
}

//______________________________________________________________________________
const char *ERNeuRadHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNeuRadHitWBT::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNeuRadHitWBT::Class_Name()
{
   return "ERNeuRadHitWBT";
}

//______________________________________________________________________________
const char *ERNeuRadHitWBT::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitWBT*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNeuRadHitWBT::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitWBT*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNeuRadHitWBT::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitWBT*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNeuRadHitWBT::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNeuRadHitWBT*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiPoint::Class_Name()
{
   return "ERmuSiPoint";
}

//______________________________________________________________________________
const char *ERmuSiPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiHit::Class_Name()
{
   return "ERmuSiHit";
}

//______________________________________________________________________________
const char *ERmuSiHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiTrack::Class_Name()
{
   return "ERmuSiTrack";
}

//______________________________________________________________________________
const char *ERmuSiTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERmuSiVertex::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERmuSiVertex::Class_Name()
{
   return "ERmuSiVertex";
}

//______________________________________________________________________________
const char *ERmuSiVertex::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertex*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERmuSiVertex::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertex*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERmuSiVertex::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertex*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERmuSiVertex::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERmuSiVertex*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastCsIPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastCsIPoint::Class_Name()
{
   return "ERGadastCsIPoint";
}

//______________________________________________________________________________
const char *ERGadastCsIPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastCsIPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastCsIPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastCsIPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastCsIPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastCsIPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastCsIPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastCsIPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastLaBrPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastLaBrPoint::Class_Name()
{
   return "ERGadastLaBrPoint";
}

//______________________________________________________________________________
const char *ERGadastLaBrPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastLaBrPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastLaBrPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastLaBrPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastLaBrPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastLaBrPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastLaBrPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastLaBrPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastStep::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastStep::Class_Name()
{
   return "ERGadastStep";
}

//______________________________________________________________________________
const char *ERGadastStep::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastStep*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastStep::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastStep*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastStep::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastStep*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastStep::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastStep*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERGadastDigi::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERGadastDigi::Class_Name()
{
   return "ERGadastDigi";
}

//______________________________________________________________________________
const char *ERGadastDigi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigi*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERGadastDigi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigi*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERGadastDigi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigi*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERGadastDigi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERGadastDigi*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNDPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNDPoint::Class_Name()
{
   return "ERNDPoint";
}

//______________________________________________________________________________
const char *ERNDPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNDPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNDPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNDPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERNDHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERNDHit::Class_Name()
{
   return "ERNDHit";
}

//______________________________________________________________________________
const char *ERNDHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERNDHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERNDHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERNDHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERNDHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERNDHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDSRDPoint::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDSRDPoint::Class_Name()
{
   return "ERDSRDPoint";
}

//______________________________________________________________________________
const char *ERDSRDPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDPoint*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDSRDPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDPoint*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDSRDPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDPoint*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDSRDPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDPoint*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ERDSRDHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ERDSRDHit::Class_Name()
{
   return "ERDSRDHit";
}

//______________________________________________________________________________
const char *ERDSRDHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ERDSRDHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ERDSRDHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ERDSRDHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ERDSRDHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RawEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RawEvent::Class_Name()
{
   return "RawEvent";
}

//______________________________________________________________________________
const char *RawEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RawEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RawEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RawEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RawEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AEvent::Class_Name()
{
   return "AEvent";
}

//______________________________________________________________________________
const char *AEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLiEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLiEvent::Class_Name()
{
   return "TLiEvent";
}

//______________________________________________________________________________
const char *TLiEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLiEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLiEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLiEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLiEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLiEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLiEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLiEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TGo4EventElement::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TGo4EventElement::Class_Name()
{
   return "TGo4EventElement";
}

//______________________________________________________________________________
const char *TGo4EventElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGo4EventElement*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TGo4EventElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TGo4EventElement*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TGo4EventElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGo4EventElement*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TGo4EventElement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TGo4EventElement*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cls_RootHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cls_RootHit::Class_Name()
{
   return "cls_RootHit";
}

//______________________________________________________________________________
const char *cls_RootHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cls_RootHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cls_RootHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cls_RootHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cls_RootHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cls_RootHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cls_RootHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cls_RootHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cls_RootEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cls_RootEvent::Class_Name()
{
   return "cls_RootEvent";
}

//______________________________________________________________________________
const char *cls_RootEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cls_RootEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cls_RootEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cls_RootEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cls_RootEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cls_RootEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cls_RootEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cls_RootEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr WCalHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *WCalHit::Class_Name()
{
   return "WCalHit";
}

//______________________________________________________________________________
const char *WCalHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCalHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int WCalHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCalHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WCalHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCalHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WCalHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCalHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr WCalEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *WCalEvent::Class_Name()
{
   return "WCalEvent";
}

//______________________________________________________________________________
const char *WCalEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCalEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int WCalEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WCalEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WCalEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCalEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WCalEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WCalEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ERStack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERStack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERStack::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERStack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERStack(void *p) {
      return  p ? new(p) ::ERStack : new ::ERStack;
   }
   static void *newArray_ERStack(Long_t nElements, void *p) {
      return p ? new(p) ::ERStack[nElements] : new ::ERStack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERStack(void *p) {
      delete ((::ERStack*)p);
   }
   static void deleteArray_ERStack(void *p) {
      delete [] ((::ERStack*)p);
   }
   static void destruct_ERStack(void *p) {
      typedef ::ERStack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERStack

//______________________________________________________________________________
void ERMCTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERMCTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERMCTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERMCTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERMCTrack(void *p) {
      return  p ? new(p) ::ERMCTrack : new ::ERMCTrack;
   }
   static void *newArray_ERMCTrack(Long_t nElements, void *p) {
      return p ? new(p) ::ERMCTrack[nElements] : new ::ERMCTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERMCTrack(void *p) {
      delete ((::ERMCTrack*)p);
   }
   static void deleteArray_ERMCTrack(void *p) {
      delete [] ((::ERMCTrack*)p);
   }
   static void destruct_ERMCTrack(void *p) {
      typedef ::ERMCTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERMCTrack

//______________________________________________________________________________
void ERMCEventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERMCEventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERMCEventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERMCEventHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERMCEventHeader(void *p) {
      return  p ? new(p) ::ERMCEventHeader : new ::ERMCEventHeader;
   }
   static void *newArray_ERMCEventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::ERMCEventHeader[nElements] : new ::ERMCEventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERMCEventHeader(void *p) {
      delete ((::ERMCEventHeader*)p);
   }
   static void deleteArray_ERMCEventHeader(void *p) {
      delete [] ((::ERMCEventHeader*)p);
   }
   static void destruct_ERMCEventHeader(void *p) {
      typedef ::ERMCEventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERMCEventHeader

//______________________________________________________________________________
void EREventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class EREventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(EREventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(EREventHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EREventHeader(void *p) {
      return  p ? new(p) ::EREventHeader : new ::EREventHeader;
   }
   static void *newArray_EREventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::EREventHeader[nElements] : new ::EREventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_EREventHeader(void *p) {
      delete ((::EREventHeader*)p);
   }
   static void deleteArray_EREventHeader(void *p) {
      delete [] ((::EREventHeader*)p);
   }
   static void destruct_EREventHeader(void *p) {
      typedef ::EREventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EREventHeader

//______________________________________________________________________________
void ERLi10EventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERLi10EventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERLi10EventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERLi10EventHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERLi10EventHeader(void *p) {
      return  p ? new(p) ::ERLi10EventHeader : new ::ERLi10EventHeader;
   }
   static void *newArray_ERLi10EventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::ERLi10EventHeader[nElements] : new ::ERLi10EventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERLi10EventHeader(void *p) {
      delete ((::ERLi10EventHeader*)p);
   }
   static void deleteArray_ERLi10EventHeader(void *p) {
      delete [] ((::ERLi10EventHeader*)p);
   }
   static void destruct_ERLi10EventHeader(void *p) {
      typedef ::ERLi10EventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERLi10EventHeader

//______________________________________________________________________________
void ERLi10MCEventHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERLi10MCEventHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERLi10MCEventHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERLi10MCEventHeader::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERLi10MCEventHeader(void *p) {
      return  p ? new(p) ::ERLi10MCEventHeader : new ::ERLi10MCEventHeader;
   }
   static void *newArray_ERLi10MCEventHeader(Long_t nElements, void *p) {
      return p ? new(p) ::ERLi10MCEventHeader[nElements] : new ::ERLi10MCEventHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERLi10MCEventHeader(void *p) {
      delete ((::ERLi10MCEventHeader*)p);
   }
   static void deleteArray_ERLi10MCEventHeader(void *p) {
      delete [] ((::ERLi10MCEventHeader*)p);
   }
   static void destruct_ERLi10MCEventHeader(void *p) {
      typedef ::ERLi10MCEventHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERLi10MCEventHeader

//______________________________________________________________________________
void ERNeuRadPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadPoint(void *p) {
      return  p ? new(p) ::ERNeuRadPoint : new ::ERNeuRadPoint;
   }
   static void *newArray_ERNeuRadPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadPoint[nElements] : new ::ERNeuRadPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadPoint(void *p) {
      delete ((::ERNeuRadPoint*)p);
   }
   static void deleteArray_ERNeuRadPoint(void *p) {
      delete [] ((::ERNeuRadPoint*)p);
   }
   static void destruct_ERNeuRadPoint(void *p) {
      typedef ::ERNeuRadPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadPoint

//______________________________________________________________________________
void ERNeuRadPhotoElectron::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadPhotoElectron.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadPhotoElectron::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadPhotoElectron::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadPhotoElectron(void *p) {
      return  p ? new(p) ::ERNeuRadPhotoElectron : new ::ERNeuRadPhotoElectron;
   }
   static void *newArray_ERNeuRadPhotoElectron(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadPhotoElectron[nElements] : new ::ERNeuRadPhotoElectron[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadPhotoElectron(void *p) {
      delete ((::ERNeuRadPhotoElectron*)p);
   }
   static void deleteArray_ERNeuRadPhotoElectron(void *p) {
      delete [] ((::ERNeuRadPhotoElectron*)p);
   }
   static void destruct_ERNeuRadPhotoElectron(void *p) {
      typedef ::ERNeuRadPhotoElectron current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadPhotoElectron

//______________________________________________________________________________
void ERNeuRadPixelSignal::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadPixelSignal.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadPixelSignal::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadPixelSignal::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadPixelSignal(void *p) {
      return  p ? new(p) ::ERNeuRadPixelSignal : new ::ERNeuRadPixelSignal;
   }
   static void *newArray_ERNeuRadPixelSignal(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadPixelSignal[nElements] : new ::ERNeuRadPixelSignal[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadPixelSignal(void *p) {
      delete ((::ERNeuRadPixelSignal*)p);
   }
   static void deleteArray_ERNeuRadPixelSignal(void *p) {
      delete [] ((::ERNeuRadPixelSignal*)p);
   }
   static void destruct_ERNeuRadPixelSignal(void *p) {
      typedef ::ERNeuRadPixelSignal current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadPixelSignal

//______________________________________________________________________________
void ERNeuRadDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadDigi(void *p) {
      return  p ? new(p) ::ERNeuRadDigi : new ::ERNeuRadDigi;
   }
   static void *newArray_ERNeuRadDigi(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadDigi[nElements] : new ::ERNeuRadDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadDigi(void *p) {
      delete ((::ERNeuRadDigi*)p);
   }
   static void deleteArray_ERNeuRadDigi(void *p) {
      delete [] ((::ERNeuRadDigi*)p);
   }
   static void destruct_ERNeuRadDigi(void *p) {
      typedef ::ERNeuRadDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadDigi

//______________________________________________________________________________
void ERNeuRadStep::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadStep.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadStep::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadStep::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadStep(void *p) {
      return  p ? new(p) ::ERNeuRadStep : new ::ERNeuRadStep;
   }
   static void *newArray_ERNeuRadStep(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadStep[nElements] : new ::ERNeuRadStep[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadStep(void *p) {
      delete ((::ERNeuRadStep*)p);
   }
   static void deleteArray_ERNeuRadStep(void *p) {
      delete [] ((::ERNeuRadStep*)p);
   }
   static void destruct_ERNeuRadStep(void *p) {
      typedef ::ERNeuRadStep current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadStep

//______________________________________________________________________________
void ERNeuRadHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadHit(void *p) {
      return  p ? new(p) ::ERNeuRadHit : new ::ERNeuRadHit;
   }
   static void *newArray_ERNeuRadHit(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadHit[nElements] : new ::ERNeuRadHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadHit(void *p) {
      delete ((::ERNeuRadHit*)p);
   }
   static void deleteArray_ERNeuRadHit(void *p) {
      delete [] ((::ERNeuRadHit*)p);
   }
   static void destruct_ERNeuRadHit(void *p) {
      typedef ::ERNeuRadHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadHit

//______________________________________________________________________________
void ERNeuRadHitWBT::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNeuRadHitWBT.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNeuRadHitWBT::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNeuRadHitWBT::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNeuRadHitWBT(void *p) {
      return  p ? new(p) ::ERNeuRadHitWBT : new ::ERNeuRadHitWBT;
   }
   static void *newArray_ERNeuRadHitWBT(Long_t nElements, void *p) {
      return p ? new(p) ::ERNeuRadHitWBT[nElements] : new ::ERNeuRadHitWBT[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNeuRadHitWBT(void *p) {
      delete ((::ERNeuRadHitWBT*)p);
   }
   static void deleteArray_ERNeuRadHitWBT(void *p) {
      delete [] ((::ERNeuRadHitWBT*)p);
   }
   static void destruct_ERNeuRadHitWBT(void *p) {
      typedef ::ERNeuRadHitWBT current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNeuRadHitWBT

//______________________________________________________________________________
void ERmuSiPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiPoint(void *p) {
      return  p ? new(p) ::ERmuSiPoint : new ::ERmuSiPoint;
   }
   static void *newArray_ERmuSiPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiPoint[nElements] : new ::ERmuSiPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiPoint(void *p) {
      delete ((::ERmuSiPoint*)p);
   }
   static void deleteArray_ERmuSiPoint(void *p) {
      delete [] ((::ERmuSiPoint*)p);
   }
   static void destruct_ERmuSiPoint(void *p) {
      typedef ::ERmuSiPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiPoint

//______________________________________________________________________________
void ERmuSiHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiHit(void *p) {
      return  p ? new(p) ::ERmuSiHit : new ::ERmuSiHit;
   }
   static void *newArray_ERmuSiHit(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiHit[nElements] : new ::ERmuSiHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiHit(void *p) {
      delete ((::ERmuSiHit*)p);
   }
   static void deleteArray_ERmuSiHit(void *p) {
      delete [] ((::ERmuSiHit*)p);
   }
   static void destruct_ERmuSiHit(void *p) {
      typedef ::ERmuSiHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiHit

//______________________________________________________________________________
void ERmuSiTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiTrack(void *p) {
      return  p ? new(p) ::ERmuSiTrack : new ::ERmuSiTrack;
   }
   static void *newArray_ERmuSiTrack(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiTrack[nElements] : new ::ERmuSiTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiTrack(void *p) {
      delete ((::ERmuSiTrack*)p);
   }
   static void deleteArray_ERmuSiTrack(void *p) {
      delete [] ((::ERmuSiTrack*)p);
   }
   static void destruct_ERmuSiTrack(void *p) {
      typedef ::ERmuSiTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiTrack

//______________________________________________________________________________
void ERmuSiVertex::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERmuSiVertex.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERmuSiVertex::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERmuSiVertex::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERmuSiVertex(void *p) {
      return  p ? new(p) ::ERmuSiVertex : new ::ERmuSiVertex;
   }
   static void *newArray_ERmuSiVertex(Long_t nElements, void *p) {
      return p ? new(p) ::ERmuSiVertex[nElements] : new ::ERmuSiVertex[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERmuSiVertex(void *p) {
      delete ((::ERmuSiVertex*)p);
   }
   static void deleteArray_ERmuSiVertex(void *p) {
      delete [] ((::ERmuSiVertex*)p);
   }
   static void destruct_ERmuSiVertex(void *p) {
      typedef ::ERmuSiVertex current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERmuSiVertex

//______________________________________________________________________________
void ERGadastCsIPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastCsIPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastCsIPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastCsIPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastCsIPoint(void *p) {
      return  p ? new(p) ::ERGadastCsIPoint : new ::ERGadastCsIPoint;
   }
   static void *newArray_ERGadastCsIPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastCsIPoint[nElements] : new ::ERGadastCsIPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastCsIPoint(void *p) {
      delete ((::ERGadastCsIPoint*)p);
   }
   static void deleteArray_ERGadastCsIPoint(void *p) {
      delete [] ((::ERGadastCsIPoint*)p);
   }
   static void destruct_ERGadastCsIPoint(void *p) {
      typedef ::ERGadastCsIPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastCsIPoint

//______________________________________________________________________________
void ERGadastLaBrPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastLaBrPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastLaBrPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastLaBrPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastLaBrPoint(void *p) {
      return  p ? new(p) ::ERGadastLaBrPoint : new ::ERGadastLaBrPoint;
   }
   static void *newArray_ERGadastLaBrPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastLaBrPoint[nElements] : new ::ERGadastLaBrPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastLaBrPoint(void *p) {
      delete ((::ERGadastLaBrPoint*)p);
   }
   static void deleteArray_ERGadastLaBrPoint(void *p) {
      delete [] ((::ERGadastLaBrPoint*)p);
   }
   static void destruct_ERGadastLaBrPoint(void *p) {
      typedef ::ERGadastLaBrPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastLaBrPoint

//______________________________________________________________________________
void ERGadastStep::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastStep.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastStep::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastStep::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastStep(void *p) {
      return  p ? new(p) ::ERGadastStep : new ::ERGadastStep;
   }
   static void *newArray_ERGadastStep(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastStep[nElements] : new ::ERGadastStep[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastStep(void *p) {
      delete ((::ERGadastStep*)p);
   }
   static void deleteArray_ERGadastStep(void *p) {
      delete [] ((::ERGadastStep*)p);
   }
   static void destruct_ERGadastStep(void *p) {
      typedef ::ERGadastStep current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastStep

//______________________________________________________________________________
void ERGadastDigi::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERGadastDigi.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERGadastDigi::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERGadastDigi::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERGadastDigi(void *p) {
      return  p ? new(p) ::ERGadastDigi : new ::ERGadastDigi;
   }
   static void *newArray_ERGadastDigi(Long_t nElements, void *p) {
      return p ? new(p) ::ERGadastDigi[nElements] : new ::ERGadastDigi[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERGadastDigi(void *p) {
      delete ((::ERGadastDigi*)p);
   }
   static void deleteArray_ERGadastDigi(void *p) {
      delete [] ((::ERGadastDigi*)p);
   }
   static void destruct_ERGadastDigi(void *p) {
      typedef ::ERGadastDigi current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERGadastDigi

//______________________________________________________________________________
void ERNDPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNDPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNDPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNDPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNDPoint(void *p) {
      return  p ? new(p) ::ERNDPoint : new ::ERNDPoint;
   }
   static void *newArray_ERNDPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERNDPoint[nElements] : new ::ERNDPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNDPoint(void *p) {
      delete ((::ERNDPoint*)p);
   }
   static void deleteArray_ERNDPoint(void *p) {
      delete [] ((::ERNDPoint*)p);
   }
   static void destruct_ERNDPoint(void *p) {
      typedef ::ERNDPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNDPoint

//______________________________________________________________________________
void ERNDHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERNDHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERNDHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERNDHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERNDHit(void *p) {
      return  p ? new(p) ::ERNDHit : new ::ERNDHit;
   }
   static void *newArray_ERNDHit(Long_t nElements, void *p) {
      return p ? new(p) ::ERNDHit[nElements] : new ::ERNDHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERNDHit(void *p) {
      delete ((::ERNDHit*)p);
   }
   static void deleteArray_ERNDHit(void *p) {
      delete [] ((::ERNDHit*)p);
   }
   static void destruct_ERNDHit(void *p) {
      typedef ::ERNDHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERNDHit

//______________________________________________________________________________
void ERDSRDPoint::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDSRDPoint.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDSRDPoint::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDSRDPoint::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDSRDPoint(void *p) {
      return  p ? new(p) ::ERDSRDPoint : new ::ERDSRDPoint;
   }
   static void *newArray_ERDSRDPoint(Long_t nElements, void *p) {
      return p ? new(p) ::ERDSRDPoint[nElements] : new ::ERDSRDPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDSRDPoint(void *p) {
      delete ((::ERDSRDPoint*)p);
   }
   static void deleteArray_ERDSRDPoint(void *p) {
      delete [] ((::ERDSRDPoint*)p);
   }
   static void destruct_ERDSRDPoint(void *p) {
      typedef ::ERDSRDPoint current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDSRDPoint

//______________________________________________________________________________
void ERDSRDHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class ERDSRDHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ERDSRDHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(ERDSRDHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ERDSRDHit(void *p) {
      return  p ? new(p) ::ERDSRDHit : new ::ERDSRDHit;
   }
   static void *newArray_ERDSRDHit(Long_t nElements, void *p) {
      return p ? new(p) ::ERDSRDHit[nElements] : new ::ERDSRDHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_ERDSRDHit(void *p) {
      delete ((::ERDSRDHit*)p);
   }
   static void deleteArray_ERDSRDHit(void *p) {
      delete [] ((::ERDSRDHit*)p);
   }
   static void destruct_ERDSRDHit(void *p) {
      typedef ::ERDSRDHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ERDSRDHit

//______________________________________________________________________________
void RawEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class RawEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RawEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(RawEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RawEvent(void *p) {
      return  p ? new(p) ::RawEvent : new ::RawEvent;
   }
   static void *newArray_RawEvent(Long_t nElements, void *p) {
      return p ? new(p) ::RawEvent[nElements] : new ::RawEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_RawEvent(void *p) {
      delete ((::RawEvent*)p);
   }
   static void deleteArray_RawEvent(void *p) {
      delete [] ((::RawEvent*)p);
   }
   static void destruct_RawEvent(void *p) {
      typedef ::RawEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RawEvent

//______________________________________________________________________________
void AEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class AEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(AEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(AEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AEvent(void *p) {
      return  p ? new(p) ::AEvent : new ::AEvent;
   }
   static void *newArray_AEvent(Long_t nElements, void *p) {
      return p ? new(p) ::AEvent[nElements] : new ::AEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_AEvent(void *p) {
      delete ((::AEvent*)p);
   }
   static void deleteArray_AEvent(void *p) {
      delete [] ((::AEvent*)p);
   }
   static void destruct_AEvent(void *p) {
      typedef ::AEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AEvent

//______________________________________________________________________________
void TLiEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLiEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TLiEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(TLiEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLiEvent(void *p) {
      return  p ? new(p) ::TLiEvent : new ::TLiEvent;
   }
   static void *newArray_TLiEvent(Long_t nElements, void *p) {
      return p ? new(p) ::TLiEvent[nElements] : new ::TLiEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLiEvent(void *p) {
      delete ((::TLiEvent*)p);
   }
   static void deleteArray_TLiEvent(void *p) {
      delete [] ((::TLiEvent*)p);
   }
   static void destruct_TLiEvent(void *p) {
      typedef ::TLiEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TLiEvent

//______________________________________________________________________________
void TGo4EventElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class TGo4EventElement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TGo4EventElement::Class(),this);
   } else {
      R__b.WriteClassBuffer(TGo4EventElement::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TGo4EventElement(void *p) {
      return  p ? new(p) ::TGo4EventElement : new ::TGo4EventElement;
   }
   static void *newArray_TGo4EventElement(Long_t nElements, void *p) {
      return p ? new(p) ::TGo4EventElement[nElements] : new ::TGo4EventElement[nElements];
   }
   // Wrapper around operator delete
   static void delete_TGo4EventElement(void *p) {
      delete ((::TGo4EventElement*)p);
   }
   static void deleteArray_TGo4EventElement(void *p) {
      delete [] ((::TGo4EventElement*)p);
   }
   static void destruct_TGo4EventElement(void *p) {
      typedef ::TGo4EventElement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TGo4EventElement

//______________________________________________________________________________
void cls_RootHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class cls_RootHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cls_RootHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(cls_RootHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cls_RootHit(void *p) {
      return  p ? new(p) ::cls_RootHit : new ::cls_RootHit;
   }
   static void *newArray_cls_RootHit(Long_t nElements, void *p) {
      return p ? new(p) ::cls_RootHit[nElements] : new ::cls_RootHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_cls_RootHit(void *p) {
      delete ((::cls_RootHit*)p);
   }
   static void deleteArray_cls_RootHit(void *p) {
      delete [] ((::cls_RootHit*)p);
   }
   static void destruct_cls_RootHit(void *p) {
      typedef ::cls_RootHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cls_RootHit

//______________________________________________________________________________
void cls_RootEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class cls_RootEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cls_RootEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(cls_RootEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cls_RootEvent(void *p) {
      return  p ? new(p) ::cls_RootEvent : new ::cls_RootEvent;
   }
   static void *newArray_cls_RootEvent(Long_t nElements, void *p) {
      return p ? new(p) ::cls_RootEvent[nElements] : new ::cls_RootEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_cls_RootEvent(void *p) {
      delete ((::cls_RootEvent*)p);
   }
   static void deleteArray_cls_RootEvent(void *p) {
      delete [] ((::cls_RootEvent*)p);
   }
   static void destruct_cls_RootEvent(void *p) {
      typedef ::cls_RootEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cls_RootEvent

//______________________________________________________________________________
void WCalHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class WCalHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(WCalHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(WCalHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_WCalHit(void *p) {
      return  p ? new(p) ::WCalHit : new ::WCalHit;
   }
   static void *newArray_WCalHit(Long_t nElements, void *p) {
      return p ? new(p) ::WCalHit[nElements] : new ::WCalHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_WCalHit(void *p) {
      delete ((::WCalHit*)p);
   }
   static void deleteArray_WCalHit(void *p) {
      delete [] ((::WCalHit*)p);
   }
   static void destruct_WCalHit(void *p) {
      typedef ::WCalHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::WCalHit

//______________________________________________________________________________
void WCalEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class WCalEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(WCalEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(WCalEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_WCalEvent(void *p) {
      return  p ? new(p) ::WCalEvent : new ::WCalEvent;
   }
   static void *newArray_WCalEvent(Long_t nElements, void *p) {
      return p ? new(p) ::WCalEvent[nElements] : new ::WCalEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_WCalEvent(void *p) {
      delete ((::WCalEvent*)p);
   }
   static void deleteArray_WCalEvent(void *p) {
      delete [] ((::WCalEvent*)p);
   }
   static void destruct_WCalEvent(void *p) {
      typedef ::WCalEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::WCalEvent

namespace {
  void TriggerDictionaryInitialization_G__ERDataDict_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/opt/fr_16.06_inst/include",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/data",
"/home/muzalevsky/check/data/NeuRadData",
"/home/muzalevsky/check/data/muSiData",
"/home/muzalevsky/check/data/NDData",
"/home/muzalevsky/check/data/beamtime",
"/opt/fs_may16p1_inst/include/root",
"/opt/fs_may16p1_inst/include",
"/opt/fs_may16p1_inst/include/root",
"/home/muzalevsky/check/build/data/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "G__ERDataDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class ERStack;
class __attribute__((annotate("$clingAutoload$ERMCTrack.h")))  ERMCTrack;
class ERMCEventHeader;
class EREventHeader;
class ERLi10EventHeader;
class ERLi10MCEventHeader;
class ERNeuRadPoint;
class ERNeuRadPhotoElectron;
class ERNeuRadPixelSignal;
class ERNeuRadDigi;
class ERNeuRadStep;
class ERNeuRadHit;
class ERNeuRadHitWBT;
class ERmuSiPoint;
class ERmuSiHit;
class ERmuSiTrack;
class ERmuSiVertex;
class ERGadastCsIPoint;
class ERGadastLaBrPoint;
class ERGadastStep;
class ERGadastDigi;
class ERNDPoint;
class ERNDHit;
class ERDSRDPoint;
class ERDSRDHit;
class RawEvent;
class AEvent;
class TLiEvent;
class TGo4EventElement;
class cls_RootHit;
class cls_RootEvent;
class WCalHit;
class WCalEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "G__ERDataDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// -------------------------------------------------------------------------
// -----                      ERNeuRadPoint header file                -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


/**  ERNeuRadPoint.h
 **/


#ifndef ERNeuRadPoint_H
#define ERNeuRadPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERNeuRadPoint : public FairMCPoint
{

 public:

  /** Default constructor **/
  ERNeuRadPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberInModuleNb number of fiber in module
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   *@param lightYield    Energy deposit [MeV]
   **/
  ERNeuRadPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberNb, Int_t pixelNb, Int_t moduleNb, 
		  Double_t mass,
		  TVector3 posIn, TVector3 posInLoc,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t timeIn, Double_t timeOut,Double_t trackLength, Double_t eLoss,
      Double_t lightYield, Int_t pid, Double_t charge) ;
      
  /** Copy constructor **/
  ERNeuRadPoint(const ERNeuRadPoint&);
  

  /** Destructor **/
  virtual ~ERNeuRadPoint();


  ERNeuRadPoint& operator=(const ERNeuRadPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID() const { return fEventID; }
  Int_t GetMot0TrackID()   const { return fMot0TrackID; }
  Double_t GetXIn()   const { return fX; }
  Double_t GetYIn()   const { return fY; }
  Double_t GetZIn()   const { return fZ; }
  Double_t GetXInLocal()   const { return fXlocal; }
  Double_t GetYInLocal()   const { return fYlocal; }
  Double_t GetZInLocal()   const { return fZlocal; }
  Double_t GetXOut()  const { return fX_out; }
  Double_t GetYOut()  const { return fY_out; }
  Double_t GetZOut()  const { return fZ_out; }
  Double_t GetPxIn()  const { return fPx; }
  Double_t GetPyIn()  const { return fPy; }
  Double_t GetPzIn()  const { return fPz; }
  Double_t GetPxOut() const { return fPx_out; }
  Double_t GetPyOut() const { return fPy_out; }
  Double_t GetPzOut() const { return fPz_out; }
  Double_t GetPIn()   const;
  Double_t GetPOut()  const;
  Double_t GetMass() const { return fMass; }
  Double_t GetLightYield() const {return fLightYield;}
  Int_t GetModuleNb(){return fModuleNb;}
  Int_t GetPixelNb(){return fPixelNb;}
  Int_t GetFiberNb() const {return fFiberNb;}
  Int_t GetPID() const {return fPID;}
  Double_t GetCharge() const {return fCharge;}
  Double_t GetTime() const {return fTimeIn;}
  Double_t GetTimeIn() const {return fTimeIn;}
  Double_t GetTimeOut() const {return fTimeOut;}
  Double_t GetLength() const;

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  //@todo проверить эти функции
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;
  Double_t GetP(Double_t pointLen) const;
  Double_t GetTime(Double_t pointLen) const;

  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /** Modifies  **/
  void SetLightYield(Double_t ly) {fLightYield = ly;}
  void SetTimeIn(Double_t t){fTimeIn = t;}
  void SetXIn(Double_t xin) {fX = xin;}
  void SetXOut(Double_t xout) {fX_out = xout;}
  void SetYIn(Double_t yin) {fY = yin;}
  void SetYOut(Double_t yout) {fY_out = yout;}  
  void SetZIn(Double_t zin) {fZ = zin;}
  void SetZOut(Double_t zout) {fZ_out = zout;}
  
 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Int_t fFiberNb;
  Int_t fPixelNb;
  Int_t fModuleNb;
  Double_t fMass;
  Double32_t fXlocal,  fYlocal,  fZlocal;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Double_t fTimeIn, fTimeOut;
  Double_t fLightYield;
  Int_t fPID;
  Double_t fCharge;
  Double_t fTrackLength;

  ClassDef(ERNeuRadPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                      ERNeuRadPhotoElectron header file           -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPhotoElectron_H
#define ERNeuRadPhotoElectron_H

#include "FairMultiLinkedData.h"

class ERNeuRadPhotoElectron : public FairMultiLinkedData{
	private:
    Int_t fIndex;
		Int_t fSide; //0 - front , 1 - back
		Double_t fCathodeTime;
		Double_t fAnodeTime;
		Int_t fPhotonCount;
		Double_t fAmplitude;
      Double_t fLYTime;
	public:
		/** Default constructor **/
		ERNeuRadPhotoElectron();
		ERNeuRadPhotoElectron(Int_t index, Int_t side, Double_t lyTime, Double_t cathode_time, Double_t anode_time, Int_t photon_count, 
							Double_t amplitude);

		/** Destructor **/
		~ERNeuRadPhotoElectron();

    /** Accessors **/
    Int_t     Index()        {return fIndex;}
    Int_t     Side()         {return fSide;}
    Double_t   LYTime()       {return fLYTime;}
    Double_t  CathodeTime()  {return fCathodeTime;}
    Double_t  AnodeTime()    {return fAnodeTime;}
    Int_t     PhotonCount()  {return fPhotonCount;}
    Double_t  Amplitude()    {return fAmplitude;}

    void Print();

	ClassDef(ERNeuRadPhotoElectron,1)
};

#endif

// -------------------------------------------------------------------------
// -----                      ERNeuRadPixelSignal header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPixelSignal_H
#define ERNeuRadPixelSignal_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayF.h"

#include <map>

#include "FairMultiLinkedData.h"

#include "ERNeuRadPhotoElectron.h"

class ERNeuRadPixelSignal : public FairMultiLinkedData {

protected:

  Int_t fPixelNb;
  Int_t fModuleNb;
  Int_t fSide;
  //Массив амплитуд фотоэлектронов сигнала
  TArrayF fPEAmplitudes;     
  //Суммарная амплитуда фотоэлектронов сигнала
  Double_t fPEAmplitudesSum;
  //Массив времен прихоа на анод фотоэлектронов сигнала
  TArrayF fPEAnodeTimes;
  //Массив длин одноэлектронных сигналов
  Int_t *fPETimes;     //!
  //Количество фотоэлектронов в сигнале
  Int_t fPECount;

  Int_t fCurFPoint;

  Double_t fStartTime;
  Double_t fFinishTime;

  //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
  Float_t* fResFunction;    //!
  //Хранит результирующий сигнал, как значения функции в узлах
  TArrayF fResFunctionRoot; 

  Double_t OnePEFunction(Double_t time, Double_t amplitude);
  Int_t OnePETime(Double_t amplitude);
public:

        Double_t GetStartTime(){
 	       return fStartTime;
        }

        Double_t GetFinishTime(){
	        return fFinishTime;
        }

	Float_t GetPEamp(Int_t i) {
                return fPEAmplitudes[i];
	}
        Float_t GetPEtime(Int_t i) {
                return fPEAnodeTimes[i];
        }
	Int_t GetPECount() {
		return fPECount;
	}

  /** Default constructor **/
  ERNeuRadPixelSignal();

  /** Constructor with arguments **/
  ERNeuRadPixelSignal(Int_t iModule, Int_t iPixel, Int_t fpoints_count, Int_t side);

  virtual ~ERNeuRadPixelSignal();
  
  virtual void AddPhotoElectron(ERNeuRadPhotoElectron* fpoint);

  virtual void Generate();

  virtual bool Exist(){return fCurFPoint > 0;}
  
  TArrayF* ResultSignal() {return &fResFunctionRoot;}

  //пока заглушки
  virtual std::vector<Double_t> Intersections(Double_t discriminatorThreshold);

  virtual Double_t MaxInteg(const Double_t window, const Double_t dt) {return -1.;}
  virtual Double_t Integ(const Double_t start,const Double_t finish);
  virtual Double_t FirstInteg(const Double_t window);
  virtual Double_t FullInteg(){return Integ(fStartTime,fFinishTime);}
  virtual Double_t Mean(const Double_t time) {return -1.;}

  virtual Double_t StartTime() {return fStartTime;} 
  virtual Double_t FinishTime() {return fFinishTime;}

  virtual Float_t ThresholdTime(Float_t peThreshold);

  virtual Double_t OnePEIntegral();

  virtual Int_t PECount() {return fPECount;}

  Double_t  dT() {return cdT;}

  Int_t ModuleNb() const {return fModuleNb;}
  Int_t PixelNb() const {return fPixelNb;}
  Int_t Side() const {return fSide;}
  Double_t AmplitudesSum() const {return fPEAmplitudesSum;}


    //constants
  //Гранулирование сигнала по времени
  static const Double_t cdT; //ns
  
  ClassDef(ERNeuRadPixelSignal,1);
};

#endif
// -------------------------------------------------------------------------
// -----                      ERNeuRadDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadDigi_H
#define ERNeuRadDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERNeuRadDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERNeuRadDigi();
  
  /** Constructor with arguments
   *@param id  Index of digi in collection
   *@param frontTDC Front Time to Digi Converter value [ns]
   *@param backTDC  Back Time to Digi Converter value [ns]
   *@param TDC      Time to Digi Converter value [ns]
   *@param QDC      Charge to Digi Converter value [MeV]
   *@param FinberIndex fiber in bundle number
   *@param ModuleIndex bundle number
   **/
  ERNeuRadDigi(Int_t id, Double_t frontTDC, Double_t backTDC, Double_t tdc,
                  Double_t qdc, Int_t bundle, Int_t fiber, Int_t side);
                   
  /** Copy constructor **/
  ERNeuRadDigi(const ERNeuRadDigi&);

  /** Destructor **/
  virtual ~ERNeuRadDigi();

  ERNeuRadDigi& operator=(const ERNeuRadDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  void SetTDC(Double_t time){fTDC = time;}
  Double_t TDC(){return fTDC;}

  void SetFrontTDC(Double_t time){fFrontTDC = time;}
  Double_t FrontTDC(){return fFrontTDC;}
  
  void SetBackTDC(Double_t time){fBackTDC = time;}
  Double_t BackTDC() const {return fBackTDC;}

  void SetQDC(Double_t charge){fQDC = charge;}
  Double_t QDC(){return fQDC;}

  void SetID(Int_t id){fID = id;}
  Int_t ID(){return fID;}
  
  void SetFiberIndex(Int_t fiber){fFiberIndex = fiber;}
  Int_t FiberIndex(){return fFiberIndex;}

  void SetModuleIndex(Int_t bundle){fModuleIndex = bundle;}
  Int_t ModuleIndex(){return fModuleIndex;}

  Int_t Side() {return fSide;}
  
 protected:
  Int_t fID;
  Double32_t fFrontTDC;
  Double32_t fBackTDC;
  Double32_t fTDC;
  Double32_t fQDC;
  Int_t fFiberIndex;
  Int_t fModuleIndex;
  Int_t fSide; //0 - front, 1 - back

  ClassDef(ERNeuRadDigi,1)

};

#endif
// -------------------------------------------------------------------------
// -----                      ERNeuRadStep header file             -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


/**  ERNeuRadStep.h
 **/

#ifndef ERNeuRadStep_H
#define ERNeuRadStep_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayI.h"


#include "FairMultiLinkedData.h"

#include "ERMCTrack.h"

class ERNeuRadStep : public FairMultiLinkedData
{
 public:

  /** Default constructor **/
  ERNeuRadStep();

  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberNb number of fiber in module
   *@param pos    Coordinates at entrance of point [cm]
   *@param mom    Momentum of track first step[GeV]
   *@param tof      Time since event start [ns]
   *@param length Track length since creation [cm]
   *@param pid   
   **/
  ERNeuRadStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberNb,
      Int_t pixelNb,
      Int_t moduleNb,
		  TVector3 pos, 
      TVector3 mom, 
		  Double_t tof, 
      Double_t length, 
      Int_t pid,
      Double_t mass,
      ExpertTrackingStatus trackStatus,
      Double_t eLoss,
      Double_t charge,
      TArrayI  processID);


  /** Copy constructor **/
  ERNeuRadStep(const ERNeuRadStep&);


  /** Destructor **/
  virtual ~ERNeuRadStep();
  
  ERNeuRadStep& operator=(const ERNeuRadStep&) { return *this; }
  
  /** Accessors **/
  Int_t GetFiberNb() const {return fFiberNb;}
  Int_t GetPixelNb() const {return fPixelNb;}
  Int_t GetModuleNb() const {return fModuleNb;}
  Double_t GetX() const {return fX;}
  Double_t GetY() const {return fY;}
  Double_t GetZ() const {return fZ;}
  void Print();
  
  static ExpertTrackingStatus GetTrackStatus();
  
 protected:
  
  Int_t fEventID;
  Int_t fStepNr;
  Int_t fTrackID;
  Int_t fMot0TrackID;
  Int_t fFiberNb;
  Int_t fPixelNb;
  Int_t fModuleNb;
  Double32_t fX,  fY,  fZ;
  Double32_t fPx, fPy, fPz;
  Double32_t fTOF;
  Double32_t fLength;
  Int_t fPID;
  Double_t fMass;
  ExpertTrackingStatus fTrackStatus;
  Double_t fEloss;
  Double_t fCharge;
  TArrayI fProcessID;
  

  ClassDef(ERNeuRadStep,1)
};
#endif
// -------------------------------------------------------------------------
// -----                        ERNeuRadHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHit_H
#define ERNeuRadHit_H

#include "FairHit.h"

class ERNeuRadHit : public FairHit {
private:
	Int_t fFiberIndex;
	Int_t fModuleIndex;
	Int_t fID;
	Float_t fTime;
public:
	ERNeuRadHit(){}
	ERNeuRadHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,  Int_t refIndex,
				 Int_t ModuleIndex, Int_t FiberIndex, Float_t time);

	Int_t Fiber()  const {return fFiberIndex;}
	Int_t Module() const {return fModuleIndex;}
	Int_t ID()     const {return fID;}
	Float_t Time() const {return fTime;}

	ClassDef(ERNeuRadHit, 1)

};

#endif

// -------------------------------------------------------------------------
// -----                        ERNeuRadHitWBT header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadHitWBT_H
#define ERNeuRadHitWBT_H

#include "ERNeuRadHit.h"

class ERNeuRadHitWBT : public ERNeuRadHit {
private:
	Float_t fQInteg;
public:
	ERNeuRadHitWBT(){}
	ERNeuRadHitWBT(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos,  Int_t refIndex,
				 Int_t ModuleIndex, Int_t FiberIndex, Float_t time, Float_t QInteg);

	Float_t QInteg() {return fQInteg;}

	ClassDef(ERNeuRadHitWBT, 1)

};

#endif

// -------------------------------------------------------------------------
// -----                      ERmuSiPoint header file                -----
// -----                  Created data  developerName                  -----
// -------------------------------------------------------------------------


/**  ERmuSiPoint.h
 **/


#ifndef ERmuSiPoint_H
#define ERmuSiPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERmuSiPoint : public FairMCPoint 
{

 public:

  /** Default constructor **/
  ERmuSiPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERmuSiPoint(Int_t index, Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t station);


  /** Copy constructor **/
  ERmuSiPoint(const ERmuSiPoint&);


  /** Destructor **/
  virtual ~ERmuSiPoint();


  ERmuSiPoint& operator=(const ERmuSiPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }
  Int_t Station()               const { return fStation;}
  Int_t Index()               const { return fIndex;}

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fStation;
  Int_t fIndex;
  Double_t fPAngle;

  ClassDef(ERmuSiPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                        ERmuSiHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSIHIT_H
#define ERMUSIHIT_H

#include "FairHit.h"

class ERmuSiHit : public FairHit {
private:
	Int_t fStation;
	Int_t fID;
public:
	ERmuSiHit(){}
	ERmuSiHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station);

	Int_t Station() const {return fStation;}
	Int_t ID() const {return fID;}

	ClassDef(ERmuSiHit, 1)

};

#endif

// -------------------------------------------------------------------------
// -----                        ERmuSiTrack header file                -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSITRACK_H
#define ERMUSITRACK_H

#include "ERmuSiHit.h"

class ERmuSiTrack : public TObject{
private:
	ERmuSiHit fHits[3];
public:
	ERmuSiTrack(){}

	Int_t AddHit(Int_t station, ERmuSiHit hit);
	ERmuSiHit* Hit(Int_t iStation){return &(fHits[iStation]);}
	ClassDef(ERmuSiTrack, 1)
};

#endif

// -------------------------------------------------------------------------
// -----                        ERmuSiVertex header file                -----
// -----                     Created 04/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERMUSIVERTEX_H
#define ERMUSIVERTEX_H

#include "TObject.h"

class ERmuSiVertex : public TObject{
private:
	Int_t fTracksID[3];
	Int_t fTracksNb;
	Float_t fX, fY, fZ;
public:
	ERmuSiVertex();
	ERmuSiVertex(Float_t x, Float_t y, Float_t z);
	
	void AddTrack(Int_t id);

	Int_t TrackNb() const {return fTracksNb;}
	Int_t Track(Int_t id) const;
	Float_t X() const {return fX;}
	Float_t Y() const {return fY;}
	Float_t Z() const {return fZ;}

	void SetX(Float_t x){fX = x;}
	void SetY(Float_t y){fY = y;}
	void SetZ(Float_t z){fZ = z;}

	ClassDef(ERmuSiVertex, 1)
};

#endif

// -------------------------------------------------------------------------
// -----                      ERGadastCsIPoint header file                -----
// -----                  Created data  Vitaliy Schetinin              -----
// -------------------------------------------------------------------------



#ifndef ERGadastCsIPoint_H
#define ERGadastCsIPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

class ERGadastCsIPoint : public FairMCPoint 
{

 public:
 
  /** Default constructor **/
  ERGadastCsIPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERGadastCsIPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t pdg,
      Int_t wall, Int_t block, Int_t cell, Int_t meshEl);


  /** Copy constructor **/
  ERGadastCsIPoint(const ERGadastCsIPoint&);


  /** Destructor **/
  virtual ~ERGadastCsIPoint();


  ERGadastCsIPoint& operator=(const ERGadastCsIPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }
  Int_t GetWall()            const { return fWall; }
  Int_t GetBlock()            const { return fBlock; }
  Int_t GetCell()            const { return fCell; }
  Int_t GetMeshEl()         const {return fMeshEl;}


  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fPDG;
  Int_t fWall;
  Int_t fBlock;
  Int_t fCell;
  Int_t fMeshEl;

  ClassDef(ERGadastCsIPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                      ERGadastLaBrPoint header file                -----
// -----                  Created data  Vitaliy Schetinin              -----
// -------------------------------------------------------------------------



#ifndef ERGadastLaBrPoint_H
#define ERGadastLaBrPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

class ERGadastLaBrPoint : public FairMCPoint 
{

 public:
 
  /** Default constructor **/
  ERGadastLaBrPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERGadastLaBrPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t pdg,  Int_t meshEl);


  /** Copy constructor **/
  ERGadastLaBrPoint(const ERGadastLaBrPoint&);


  /** Destructor **/
  virtual ~ERGadastLaBrPoint();


  ERGadastLaBrPoint& operator=(const ERGadastLaBrPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }
  Int_t GetMeshEl()         const {return fMeshEl;}

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fPDG;
  Int_t fMeshEl;

  ClassDef(ERGadastLaBrPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                      ERGadastStep header file             -----
// -----                  Created 03/16  by v. Schetinin            -----
// -------------------------------------------------------------------------


/**  ERGadastStep.h
 **/

#ifndef ERGadastStep_H
#define ERGadastStep_H


#include "TObject.h"
#include "TVector3.h"
#include "TArrayI.h"


#include "FairMultiLinkedData.h"

#include "ERMCTrack.h"

class ERGadastStep : public FairMultiLinkedData
{
 public:

  /** Default constructor **/
  ERGadastStep();

  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param fiberInBundleNb number of fiber in bundle
   *@param pos    Coordinates at entrance of point [cm]
   *@param mom    Momentum of track first step[GeV]
   *@param tof      Time since event start [ns]
   *@param length Track length since creation [cm]
   *@param pid   
   **/
  ERGadastStep(Int_t eventID, Int_t stepNr,Int_t trackID,
		  Int_t mot0trackID,
      Int_t fiberInBundleNb,
		  TVector3 pos, 
      TVector3 mom, 
		  Double_t tof, 
      Double_t length, 
      Int_t pid,
      Double_t mass,
      ExpertTrackingStatus trackStatus,
      Double_t eLoss,
      Double_t charge,
      TArrayI  processID);


  /** Copy constructor **/
  ERGadastStep(const ERGadastStep&);


  /** Destructor **/
  virtual ~ERGadastStep();
  
  ERGadastStep& operator=(const ERGadastStep&) { return *this; }
  
  /** Accessors **/
  Double_t GetFiberInBundleNb() const {return fFiberInBundleNb;}
  Double_t GetX() const {return fX;}
  Double_t GetY() const {return fY;}
  Double_t GetZ() const {return fZ;}
  void Print();
  
  static ExpertTrackingStatus GetTrackStatus();
  
 protected:
  
  Int_t fEventID;
  Int_t fStepNr;
  Int_t fTrackID;
  Int_t fMot0TrackID;
  Int_t fFiberInBundleNb;
  Double32_t fX,  fY,  fZ;
  Double32_t fPx, fPy, fPz;
  Double32_t fTOF;
  Double32_t fLength;
  Int_t fPID;
  Double_t fMass;
  ExpertTrackingStatus fTrackStatus;
  Double_t fEloss;
  Double_t fCharge;
  TArrayI fProcessID;
  

  ClassDef(ERGadastStep,1)
};
#endif
// -------------------------------------------------------------------------
// -----                      ERGadastDigi header file             -----
// -----                  Created 03/15  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERGadastDigi_H
#define ERGadastDigi_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERGadastDigi : public FairMultiLinkedData
{
 public:
  /** Default constructor **/
  ERGadastDigi();
  
  /** Constructor with arguments
   **/
  ERGadastDigi(Int_t id, Float_t Edep);
                   
  /** Copy constructor **/
  ERGadastDigi(const ERGadastDigi&);

  /** Destructor **/
  virtual ~ERGadastDigi();

  ERGadastDigi& operator=(const ERGadastDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

  /* Accessors */
  Int_t ID () const {return fID;}
  Float_t Edep() const {return fEdep;}
  
 protected:
  Int_t fID;
  Float_t fEdep;
  
  ClassDef(ERGadastDigi,1)

};

#endif
// -------------------------------------------------------------------------
// -----                       ERStack header file                     -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------


/** ERStack.h
 *@author V.Scheitnin <sch_vitaliy@mail.ru>
 **
 ** Version  11/12/15 by V.Schetinin
 **
 ** This class handles the particle stack for the transport simulation.
 ** For the stack FILO functunality, it uses the STL stack. To store
 ** the tracks during transport, a TParticle arry is used.
 ** At the end of the event, tracks satisfying the filter criteria
 ** are copied to a ERMCTrack array, which is stored in the output.
 **
 ** The filtering criteria for the output tracks are:
 ** - primary tracks are stored in any case.
 ** - secondary tracks are stored if they have a minimal number of
 **   points (sum of all detectors) and a minimal energy, or are the
 **
 ** The storage of secondaries can be switched off.
 ** The storage of all mothers can be switched off.
 ** By default, the minimal number of points is 1 and the energy cut is 0.
 **/


#ifndef ERStack_H
#define ERStack_H


#include "ERDetectorList.h"
#include "FairGenericStack.h"

#include <map>
#include <stack>

class TClonesArray;

class ERStack : public FairGenericStack
{

 public:

  /** Default constructor
   *param size  Estimated track number
   **/
  ERStack(Int_t size = 100);


  /** Destructor  **/
  virtual ~ERStack();

  virtual void PushTrack(Int_t toBeDone, Int_t parentID, Int_t pdgCode,
			 Double_t px, Double_t py, Double_t pz,
			 Double_t e, Double_t vx, Double_t vy,
			 Double_t vz, Double_t time, Double_t polx,
			 Double_t poly, Double_t polz, TMCProcess proc,
			 Int_t& ntr, Double_t weight, Int_t is);
  /** Add a TParticle to the stack.
   ** Declared in TVirtualMCStack
   *@param toBeDone  Flag for tracking
   *@param parentID  Index of mother particle
   *@param pdgCode   Particle type (PDG encoding)
   *@param px,py,pz  Momentum components at start vertex [GeV]
   *@param e         Total energy at start vertex [GeV]
   *@param vx,vy,vz  Coordinates of start vertex [cm]
   *@param time      Start time of track [s]
   *@param polx,poly,polz Polarisation vector
   *@param proc      Production mechanism (VMC encoding)
   *@param ntr       Track number (filled by the stack)
   *@param weight    Particle weight
   *@param is        Generation status code (whatever that means)
   **/
  virtual void PushTrack(Int_t toBeDone, Int_t parentID, Int_t pdgCode,
			 Double_t px, Double_t py, Double_t pz,
			 Double_t e, Double_t vx, Double_t vy,
			 Double_t vz, Double_t time, Double_t polx,
			 Double_t poly, Double_t polz, TMCProcess proc,
			 Int_t& ntr, Double_t weight, Int_t is,Int_t secondparentID);


  /** Get next particle for tracking from the stack.
   ** Declared in TVirtualMCStack
   *@param  iTrack  index of popped track (return)
   *@return Pointer to the TParticle of the track
   **/
  virtual TParticle* PopNextTrack(Int_t& iTrack);


  /** Get primary particle by index for tracking from stack
   ** Declared in TVirtualMCStack
   *@param  iPrim   index of primary particle
   *@return Pointer to the TParticle of the track
   **/
  virtual TParticle* PopPrimaryForTracking(Int_t iPrim);


  /** Set the current track number
   ** Declared in TVirtualMCStack
   *@param iTrack  track number
   **/
  virtual void SetCurrentTrack(Int_t iTrack)   { fCurrentTrack     = iTrack; }


  /** Get total number of tracks
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetNtrack() const { return fNParticles; }


  /** Get number of primary tracks
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetNprimary() const { return fNPrimaries; }


  /** Get the current track's particle
   ** Declared in TVirtualMCStack
   **/
  virtual TParticle* GetCurrentTrack() const;


  /** Get the number of the current track
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetCurrentTrackNumber() const { return fCurrentTrack; }


  /** Get the track number of the parent of the current track
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetCurrentParentTrackNumber() const;


  /** Add a TParticle to the fParticles array **/
  virtual void AddParticle(TParticle* part);


  /** Fill the MCTrack output array, applying filter criteria **/
  virtual void FillTrackArray();


  /** Update the track index in the MCTracks and MCPoints **/
  virtual void UpdateTrackIndex(TRefArray* detArray);


  /** Resets arrays and stack and deletes particles and tracks **/
  virtual void Reset();


  /** Register the MCTrack array to the Root Manager  **/
  virtual void Register();


  /** Output to screen
   **@param iVerbose: 0=events summary, 1=track info
   **/
  virtual void Print(Int_t iVerbose=0) const;


  /** Modifiers  **/
  void StoreSecondaries(Bool_t choice = kTRUE) { fStoreSecondaries = choice; }
  void SetMinPoints(Int_t min)                 { fMinPoints        = min;    }
  void SetEnergyCut(Double_t eMin)             { fEnergyCut        = eMin;   }
  void StoreMothers(Bool_t choice = kTRUE)     { fStoreMothers     = choice; }


  /** Increment number of points for the current track in a given detector
   *@param iDet  Detector unique identifier
   **/
  void AddPoint(DetectorId iDet);


  /** Increment number of points for an arbitrary track in a given detector
   *@param iDet    Detector unique identifier
   *@param iTrack  Track number
   **/
  void AddPoint(DetectorId iDet, Int_t iTrack);


  /** Accessors **/
  TParticle* GetParticle(Int_t trackId) const;
  TClonesArray* GetListOfParticles() { return fParticles; }



 private:


  /** STL stack (FILO) used to handle the TParticles for tracking **/
  std::stack<TParticle*>  fStack;           //!


  /** Array of TParticles (contains all TParticles put into or created
   ** by the transport
   **/
  TClonesArray* fParticles;            //!


  /** Array of CbmMCTracks containg the tracks written to the output **/
  TClonesArray* fTracks;


  /** STL map from particle index to storage flag  **/
  std::map<Int_t, Bool_t>           fStoreMap;        //!
  std::map<Int_t, Bool_t>::iterator fStoreIter;       //!


  /** STL map from particle index to track index  **/
  std::map<Int_t, Int_t>            fIndexMap;        //!
  std::map<Int_t, Int_t>::iterator  fIndexIter;       //!


  /** STL map from track index and detector ID to number of MCPoints **/
  std::map<std::pair<Int_t, Int_t>, Int_t> fPointsMap;     //!


  /** Some indizes and counters **/
  Int_t fCurrentTrack;  //! Index of current track
  Int_t fNPrimaries;    //! Number of primary particles
  Int_t fNParticles;    //! Number of entries in fParticles
  Int_t fNTracks;       //! Number of entries in fTracks
  Int_t fIndex;         //! Used for merging


  /** Variables defining the criteria for output selection **/
  Bool_t     fStoreSecondaries;
  Int_t      fMinPoints;
  Double32_t fEnergyCut;
  Bool_t     fStoreMothers;


  /** Mark tracks for output using selection criteria  **/
  void SelectTracks();

  ERStack(const ERStack&);
  ERStack& operator=(const ERStack&);

  ClassDef(ERStack,1)


};

#endif

// -------------------------------------------------------------------------
// -----                      ERMCTrack header file                    -----
// -----                  Created 11/05/15  by V. Schetinin            -----
// -------------------------------------------------------------------------

/** ERMCTrack.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing Monte Carlo tracks processed by the ERStack.
 ** A MCTrack can be a primary track put into the simulation or a
 ** secondary one produced by the transport through decay or interaction.
 **
 **
 **/


#ifndef ERMCTRACK_H_
#define ERMCTRACK_H_

#include "ERDetectorList.h"

#include "TObject.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
 
enum ExpertTrackingStatus{
    Entering,
    Exiting,
    Inside,
    Stop
};


class TParticle;

class ERMCTrack : public TObject
{

 public:

  /**  Default constructor  **/
  ERMCTrack();


  /**  Standard constructor  **/
  ERMCTrack(Int_t pdgCode, Int_t motherID, Double_t px, Double_t py,
	     Double_t pz, Double_t x, Double_t y, Double_t z,
	     Double_t t, Int_t nPoints);

  /**  Copy constructor  **/
  ERMCTrack(const ERMCTrack& track);


  /**  Constructor from TParticle  **/
  ERMCTrack(TParticle* particle);


  /**  Destructor  **/
  virtual ~ERMCTrack();


  /**  Output to screen  **/
  void Print(Int_t iTrack=0) const;

  //Int_t AccNeuRad() const {return GetNPoints(kNEURAD);}
  /**  Accessors  **/
  Int_t    GetPdgCode()  const { return fPdgCode; }
  Int_t    GetMotherId() const { return fMotherId; }
  Double_t GetPx()       const { return fPx; }
  Double_t GetPy()       const { return fPy; }
  Double_t GetPz()       const { return fPz; }
  Double_t GetStartX()   const { return fStartX; }
  Double_t GetStartY()   const { return fStartY; }
  Double_t GetStartZ()   const { return fStartZ; }
  Double_t GetStartT()   const { return fStartT; }
  Double_t GetMass()     const;
  Double_t GetCharge()   const;
  Double_t GetEnergy()   const;
  Double_t GetPt()       const { return TMath::Sqrt(fPx*fPx+fPy*fPy); }
  Double_t GetP() 		 const { return TMath::Sqrt(fPx*fPx+fPy*fPy+fPz*fPz); }
  Double_t GetRapidity() const;
  void GetMomentum(TVector3& momentum) const;
  void Get4Momentum(TLorentzVector& momentum) const;
  void GetStartVertex(TVector3& vertex) const;


  /** Accessors to the number of MCPoints in the detectors **/
  Int_t GetNPoints(DetectorId detId)  const;


  /**  Modifiers  **/
  void SetMotherId(Int_t id) { fMotherId = id; }
  void SetNPoints(Int_t iDet, Int_t np);



private:

  /**  PDG particle code  **/
  Int_t  fPdgCode;

  /**  Index of mother track. -1 for primary particles.  **/
  Int_t  fMotherId;

  /** Momentum components at start vertex [GeV]  **/
  Double32_t fPx, fPy, fPz;

  /** Coordinates of start vertex [cm, ns]  **/
  Double32_t fStartX, fStartY, fStartZ, fStartT;

  Int_t fNPoints;
  Double_t fMass;
  Double_t fEnergy;

  ClassDef(ERMCTrack,2);

};



// ==========   Inline functions   ========================================

inline Double_t ERMCTrack::GetEnergy() const {
  Double_t mass = GetMass();
  return TMath::Sqrt(mass*mass + fPx*fPx + fPy*fPy + fPz*fPz );
}


inline void ERMCTrack::GetMomentum(TVector3& momentum) const {
  momentum.SetXYZ(fPx,fPy,fPz);
}


inline void ERMCTrack::Get4Momentum(TLorentzVector& momentum) const {
  momentum.SetXYZT(fPx,fPy,fPz,GetEnergy());
}


inline void ERMCTrack::GetStartVertex(TVector3& vertex) const {
  vertex.SetXYZ(fStartX,fStartY,fStartZ);
}

#endif /* ERMCTRACK_H_ */
// -------------------------------------------------------------------------
// -----                      ERMCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERMCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERMCEVENTHEADER_H_
#define ERMCEVENTHEADER_H_

#include "FairMCEventHeader.h"

class ERMCEventHeader : public FairMCEventHeader {
private:
	Double_t fTargetReactionPos;
	Double_t fDirectReactionPos;
	Float_t fNeuRadELoss;
	Float_t fNeuRadLY;
public:
	ERMCEventHeader();
	void SetTargetReactionPos(Double_t pos){fTargetReactionPos = pos;}
	void SetDirectReactionPos(Double_t pos){fDirectReactionPos = pos;}
	void SetNeuRadEloss(Float_t eloss) {fNeuRadELoss = eloss;}
  	void SetNeuRadLY(Float_t ly)       {fNeuRadLY = ly;}

	Double_t TargetReactionPos()const {return fTargetReactionPos;}
	Double_t DirectReactionPos()const {return fDirectReactionPos;}
	Float_t NeuRadEloss()const {return fNeuRadELoss;}
	Float_t NeuRadLY()const {return fNeuRadLY;}

	ClassDef(ERMCEventHeader,1)
};

#endif// -------------------------------------------------------------------------
// -----                        EREventHeader header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef EREventHeader_H
#define EREventHeader_H

#include "FairEventHeader.h"

class EREventHeader : public FairEventHeader {
private:
	Int_t fNeuRadPECountF;
	Int_t fNeuRadPECountB;
	Float_t fNeuRadSumAmplitudeF;
	Float_t fNeuRadSumAmplitudeB;

public:
	EREventHeader();

	void SetNeuRadPECountF(Int_t peCount) {fNeuRadPECountF = peCount;}
	void SetNeuRadPECountB(Int_t peCount) {fNeuRadPECountB = peCount;}
	void SetNeuRadSumAmplitudeF(Float_t sum) {fNeuRadSumAmplitudeF = sum;}
	void SetNeuRadSumAmplitudeB(Float_t sum) {fNeuRadSumAmplitudeB = sum;}

	Int_t NeuRadPECountF() const {return fNeuRadPECountF;}
	Int_t NeuRadPECountB() const {return fNeuRadPECountB;}
	Float_t NeuRadSumAmplitudeF() const {return fNeuRadSumAmplitudeF;}
	Float_t NeuRadSumAmplitudeB() const {return fNeuRadSumAmplitudeB;}

	ClassDef(EREventHeader, 1)

};

#endif// -------------------------------------------------------------------------
// -----                        ERLi10EventHeader header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERLi10EventHeader_H
#define ERLi10EventHeader_H

#include "FairEventHeader.h"

class ERLi10EventHeader : public FairEventHeader {
private:
	Float_t fPEnergy;
	Float_t fNEnergy;
public:
	ERLi10EventHeader();

	void SetPEnergy(Float_t e) {fPEnergy = e;}
	void SetNEnergy(Float_t e) {fNEnergy = e;}

	ClassDef(ERLi10EventHeader, 1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERLi10MCEventHeader header file              -----
// -----                  Created 03/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

/** ERLi10MCEventHeader.h
 *@author V.Schetinin <sch_vitaliy@mail.ru>
 **
 ** Data class for storing specific information about event
 **/

#ifndef ERLi10MCEventHeader_H
#define ERLi10MCEventHeader_H

#include "FairMCEventHeader.h"

class ERLi10MCEventHeader : public FairMCEventHeader {
private:
	Float_t fReactionPos;
	Float_t fReactionTime;
public:
	ERLi10MCEventHeader();
	void SetReactionPos(Double_t pos){fReactionPos = pos;}
	void SetReactionTime(Double_t time) {fReactionTime = time;}

	Float_t ReactionPos() const {return fReactionPos;}
	Float_t ReactionTime() const {return fReactionTime;}

	ClassDef(ERLi10MCEventHeader,1)
};

#endif
// -------------------------------------------------------------------------
// -----                      ERNDPoint header file                -----
// -----                  Created data  developerName                  -----
// -------------------------------------------------------------------------


/**  ERNDPoint.h
 **/


#ifndef ERNDPoint_H
#define ERNDPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERNDPoint : public FairMCPoint 
{

 public:

  /** Default constructor **/
  ERNDPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERNDPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t stilbenNr, Float_t lightYield);


  /** Copy constructor **/
  ERNDPoint(const ERNDPoint&);


  /** Destructor **/
  virtual ~ERNDPoint();


  ERNDPoint& operator=(const ERNDPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  Int_t StilbenNr() const {return fStilbenNr;}
  Float_t LightYield() const {return fLightYield;}

  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fStilbenNr;
  Float_t fLightYield;

  ClassDef(ERNDPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                        ERNDHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERNDHit_H
#define ERNDHit_H

#include "FairHit.h"

class ERNDHit : public FairHit{
private:
	Int_t fID;
	Float_t fLightYield;
	Float_t fTime;
	Float_t fNeutronProb;
public:
	ERNDHit(){}
	ERNDHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, 
				Int_t point_index, Float_t lightYield, Float_t time, Float_t neutronProb);

	Int_t ID() const {return fID;}

	Float_t LightYield() const {return fLightYield;}
	Float_t Time() const {return fTime;}
	Float_t NeutronProb() const {return fNeutronProb;}


	ClassDef(ERNDHit, 1)

};

#endif

// -------------------------------------------------------------------------
// -----                      ERDSRDPoint header file                -----
// -----                  Created data  developerName                  -----
// -------------------------------------------------------------------------


/**  ERDSRDPoint.h
 **/


#ifndef ERDSRDPoint_H
#define ERDSRDPoint_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"


class ERDSRDPoint : public FairMCPoint 
{

 public:

  /** Default constructor **/
  ERDSRDPoint();


  /** Constructor with arguments
   *@param EventID  Index of Event
   *@param trackID  Index of MCTrack
   *@param mot0trackID Index of Mother MCTrack
   *@param posIn    Ccoordinates at entrance of point [cm]
   *@param posOut   Coordinates at exit of point [cm]
   *@param momIn    Momentum of track at entrance [GeV]
   *@param momOut   Momentum of track at exit [GeV]
   *@param tof      Time since event start [ns]
   *@param length   Track length since creation [cm]
   *@param eLoss    Energy deposit [KeV]
   **/
  ERDSRDPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss, Int_t sector,Int_t sensor);


  /** Copy constructor **/
  ERDSRDPoint(const ERDSRDPoint&);


  /** Destructor **/
  virtual ~ERDSRDPoint();


  ERDSRDPoint& operator=(const ERDSRDPoint&) { return *this; }


  /** Accessors **/
  Int_t GetEventID()            const { return fEventID; }
  Int_t GetMot0TrackID()        const { return fMot0TrackID; }
  Double_t GetXIn()             const { return fX; }
  Double_t GetYIn()             const { return fY; }
  Double_t GetZIn()             const { return fZ; }
  Double_t GetXOut()            const { return fX_out; }
  Double_t GetYOut()            const { return fY_out; }
  Double_t GetZOut()            const { return fZ_out; }
  Double_t GetPxOut()           const { return fPx_out; }
  Double_t GetPyOut()           const { return fPy_out; }
  Double_t GetPzOut()           const { return fPz_out; }
  Double_t GetMass()            const { return fMass; }

  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }


  /** Point coordinates at given z from linear extrapolation **/
  Double_t GetX(Double_t z) const;
  Double_t GetY(Double_t z) const;
  Int_t Sector() const {return fSector;}
  Int_t Sensor() const {return fSensor;}

  /** Check for distance between in and out **/
  Bool_t IsUsable() const;

  /** Output to screen **/
  virtual void Print(const Option_t* opt = 0) const;

 protected:
  
  Int_t fEventID;
  Int_t fMot0TrackID;
  Double_t fMass;
  Double32_t fX_out,  fY_out,  fZ_out;
  Double32_t fPx_out, fPy_out, fPz_out;
  Int_t fSector, fSensor;

  ClassDef(ERDSRDPoint,1)
};
#endif
// -------------------------------------------------------------------------
// -----                        ERDSRDHit header file                   -----
// -----                     Created 03/16  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERDSRDHit_H
#define ERDSRDHit_H

#include "FairHit.h"

class ERDSRDHit : public FairHit{
private:
	Int_t fID;
	Float_t fEloss;
	Float_t fTime;
public:
	ERDSRDHit(){}
	ERDSRDHit(Int_t id, Int_t detID, TVector3& pos, TVector3& dpos, Int_t point_index, Float_t eloss, Float_t time);

	Int_t ID() const {return fID;}
	Float_t Eloss() const {return fEloss;}
	Float_t Time() const {return fTime;}


	ClassDef(ERDSRDHit, 1)

};

#endif

/*
 * RawEvent.h
 *
 *  Created on: Dec 27, 2016
 *      Author: vratik
 */

#ifndef DATACLASSES_RAWEVENT_H_
#define DATACLASSES_RAWEVENT_H_

#include <iostream>

//#include "Rtypes.h"
#include "TError.h"
#include "TGraph.h"
#include "TArrayD.h"
#include "TNamed.h"
#include "TArrayF.h"


//#define NCELLS 1024

using std::cout;
using std::endl;

class RawEvent: public TNamed {

private:
//	Double_t fAmp[NCELLS]; //array for raw amplitudes
//	Double_t fTime[NCELLS]; //array for raw times

	TArrayD fAmp;	//array for raw amplitudes
	TArrayD fTime;	//array for raw times

	//Массив амплитуд фотоэлектронов сигнала
	Float_t fPEAmplitudes[1000];
	//Массив времен прихоа на анод фотоэлектронов сигнала
	Float_t fPEAnodeTimes[1000];
	//Количество фотоэлектронов в сигнале
	Int_t fPECount;
	Double_t fStartTime;
	Double_t fFinishTime;
	const Int_t fNPoints;

//	TGraph *gAmp;

//public:
//	TArrayD fAmpA;

public:
	RawEvent();
	RawEvent(const Int_t npoints);
	virtual ~RawEvent();
	ClassDef(RawEvent,1);

	void Reset();
	//Resets arrays to zeros

	const Double_t* GetAmp() const { return fAmp.GetArray(); }
	
	const Double_t* GetTime() const { return fTime.GetArray(); }

	void SetAmp(Double_t a, Int_t i);
	//Takes amplitude (raw data, voltage from binary file) 
	//and places it in the array fAmp

	void SetTime(Double_t t, Int_t i);
	//Takes time (raw data, times from binary file) 
	//and places it in the array fTime

	void PrintAmp(Int_t i);
	//Prints i amplitudes (to make sense i shold be NCELLS)

	void PrintTime(Int_t i);

	Double_t GetTime(Int_t i);

	Double_t GetAmp(Int_t i);

	Double_t& Amp(Int_t i){return fAmp[i];}

//	void InvertAmp(Double_t a, Int_t i);
	//Inverts the amplitudes i.e. makes from negative singals 
	//posititve signals and vise versa.

//	void SetGraphs();
//	TGraph* GetGraph() {
//		return gAmp;
//	}


	void SetPEtime(Float_t a, Int_t i);
	void SetPEamp(Float_t a, Int_t i); 
	void SetPECount(Int_t i);

        Float_t GetPEamp(Int_t i) {
                return fPEAmplitudes[i];
        }
        Float_t GetPEtime(Int_t i) {
                return fPEAnodeTimes[i];
        }
        Int_t GetPECount() {
                return fPECount;
        }
	void SetStartTime(Double_t t);
	Double_t GetStartTime() {
		return fStartTime;
	}

        void SetFinishTime(Double_t t);
        Double_t GetFinishTime() {
                return fFinishTime;
        }


private:
	void Init();

};

#endif /* DATACLASSES_RAWEVENT_H_ */
/*
 * AEvent.h
 *
 *  Created on: Dec 28, 2016
 *      Author: daria
 */

#ifndef DATACLASSES_AEVENT_H_
#define DATACLASSES_AEVENT_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "TError.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TNamed.h"
//#include "TMath.h"


#include "RawEvent.h"

//#define NCELLS 1024

using std::cout;
using std::endl;

class AEvent : public TNamed {

private:

	const Int_t fNPoints;	//!
	TArrayD fAmpPos;	//array for raw amplitudes
	TArrayD fTime;		//array for raw times

	Double_t fAmpMax;
	Double_t fTimeAmpMax;
	Double_t fZeroLevel;
	Double_t fEdgeXi;
	Double_t fEdgeSlope;	//slope coefficient for the rising edge of the signal
	Double_t fTime10;		//time of 10% of rising edge amplitude in ns
	Double_t fTime90;		//time of 10% of rising edge amplitude in ns
	Double_t fTimeMid;		//time point between fTime90 and fTime10
	Double_t fAmpMid;		//amplitude in fTimeMid point. Used in ToT on 50% of rising edge determination
	Double_t fToT;

	TArrayD fAmpCFD;	//array for CFD amplitudes (attenuated, inversed and delayed)
	Double_t fTimeCFD;			//zero-crossing time
	Double_t fChargeCFD;	//
	Double_t fChargeLED;	//charge of the signal in Coulomb
	Double_t fChargeTF;	//

	Double_t fTimeLED;	//time of LED threshold crossing
	Double_t fTimeFront;	//time of front fit crossing zero level

	TGraph *fGraphSignal;
	TGraph *fGraphCFD;

	RawEvent *fInputEvent;		//!

	Double_t fCFratio;		//!
	Double_t fCFtimeDelay;		//!

	Double_t fNoiseRangeMin;	//!
	Double_t fNoiseRangeMax;	//!
	Int_t fWinSize;			//!

	 //Массив амплитуд фотоэлектронов сигнала
        Float_t fPEAmplitudes[1000];
        //Массив времен прихоа на анод фотоэлектронов сигнала
        Float_t fPEAnodeTimes[1000];
        //Количество фотоэлектронов в сигнале
        Int_t fPECount;
	// start time of the signal
	Double_t fStartTime;
	Double_t fFinishTime;
public:
	AEvent();
	AEvent(const Int_t npoints);
	virtual ~AEvent();
	ClassDef(AEvent,1);
	
	Double_t GetT_10();
	//returns time of 10% of rising edge amplitude in ns

	Double_t GetT_90();
	//returns time of 90% of rising edge amplitude in ns

	Double_t GetEdgeSlope();
	//returns slope coefficient for the rising edge of the signal

	Double_t GetEdgeXi();

	Double_t GetfCFD();

	Double_t GetfLED();

	Double_t GetOnefTime(Int_t i);

	Double_t GetOnefAmpPos(Int_t i);

	void ProcessEvent(Bool_t bSmooth = kFALSE);
	void SetInputEvent(RawEvent** event);

	void SetCFratio(Double_t ratio) { fCFratio = ratio; };	
	//CFD set attenuation coefficient

	void SetCFtimeDelay(Double_t timeDelay) { fCFtimeDelay = timeDelay; };	
	//CFD set time delay (in points)

	void SetNoiseRange(Double_t min, Double_t max) { fNoiseRangeMin = min; fNoiseRangeMax = max; };
	//Set noise range to be used in FindZeroLevel()

	void SetSmoothPoints(Int_t numofp) {fWinSize = numofp; };
	//set number of smoothing points

	void SetToT();
	//calculate time-over-threshold. the threshold value is fTimeMid - middle point of the rising edge
	//ToT is calculated within 15 ns range in order to consider all the weirdness of the signal form

	void Reset();
	 //Resets arrays to zeros

	TGraph* GetGraphCFD() {

		return fGraphCFD;
	}
	//draws CFD graphs

	TGraph* GetGraphSignal() {

		return fGraphSignal;
	}
	//draws signal shape graphs

	void FindFrontProperties();

	Double_t FindZeroLevel();
	//for zero level correction. one parameter fit between pmin and pmax
	//returns fit parameter i.e. number on which amplitude should be corrected

	void SetChargeCFD(Int_t tmin = -3, Int_t tmax = 17);
	//calculates charge of the signal (i.e. its integral 
	//in range of (tmin,tmax) in ns)
	//CFD time is taken as a start point

	void SetChargeLED(Int_t tmin = -3, Int_t tmax = 17);

	void SetChargeTF(Int_t tmin = -3, Int_t tmax = 17);
	//calculates charge of the signal (i.e. its integral
	//in range of (tmin,tmax) in ns)
	//time of front fit crossing zero is taken as a start point

	//void SetLED(Double_t threshold = 0.001);

	void SetLED(Double_t threshold = 0.02);

	//leading edge discriminator

        void SetPEtime(Float_t a, Int_t i);
        void SetPEamp(Float_t a, Int_t i);
        void SetPECount(Int_t i);

        Float_t GetPEamp(Int_t i);
        Float_t GetPEtime(Int_t i);
        Int_t GetPECount();             
	void ObtainPE();
	Double_t GetStartTime();
	void SetStartTime(Double_t t);
        Double_t GetFinishTime();
        void SetFinishTime(Double_t t);

private:
	void Init();
	void SetMaxAmplitudes();
	void SetGraphs();
	void SmoothGraphs();
	void SetCFD(); 	//constant fraction discriminator method
};

#endif /* DATACLASSES_AEVENT_H_ */
// $Id: TGo4EventElement.h 1488 2015-06-08 11:32:46Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum f�r Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#ifndef TGO4EVENTELEMENT_H
#define TGO4EVENTELEMENT_H

#include "TNamed.h"

class TGo4EventSource;
class TBranch;
class TTree;

/** The abstract base class for the data elements of which the
 * unpacked events (or detector structure data, resp) are composed.
 * Any event structure consists of eventelements that are linked back to
 * their parent element.
 * The user event class is the top parent level event element;
 * unlimited levels of event elements as subevents might be
 * managed by this user event.
 * The way of subelement organization (e.g.
 * user event class members, TObjArray,...) is not restricted here.
 * @interface
 * @author J. Adamczewski
 * @since 12/2000*/

class TGo4EventElement : public TNamed {
   public:

      TGo4EventElement();

      TGo4EventElement(const char* name);
      TGo4EventElement(const char* aName, const char* aTitle, Short_t aBaseCat=0);

      virtual ~TGo4EventElement(void);

      /** Switches the valid state of this event. */
      void SetValid(Bool_t on) { fbIsValid = on; }

      /** Returns the valid state of this event. */
      Bool_t IsValid() const { return fbIsValid; }

      /** Setter for the event source that is currently used by the Fill method. */
      void SetEventSource(TGo4EventSource * src) { fxEventSource = src; }

      /** Setter for the parent event structure reference. */
      void SetParent(TGo4EventElement * par) { fxParent = par; }

      void SetKeepContents(Bool_t on=kTRUE) { fbKeepContents = on; }

      inline TGo4EventElement* GetParent() const { return fxParent; }

      virtual TGo4EventElement* GetChild(const char* name);

      inline TGo4EventSource * GetEventSource() const { return fxEventSource; }

      /** Check if event source is valid. Tests the pointer for
       * zero, and if not zero, compares classname of the
       * event source with given string. To be used from the
       * Fill method and from the analysis method  init event classes. */
      Bool_t CheckEventSource(const char* classname);

      /** Method called by the event owner (analysis step) to fill the
       * event element from the set event source. Event source can
       * be the source of the analysis step (if this is a raw event) or
       * the event processor (if this is a reduced event).
       * Fill method has to specify how the event source writes to the
       * members of this event structure. Either by calling methods of
       * the source (like myevent.a=fxEventSource->GetA(); etc ), or by
       * direct copy to the member (like fxEventSource->FillMemoryAt(&myevent.a);) */
      virtual Int_t Fill();

      /** Method called on initialization of event classes.
       * User might check the correct type of the eventsource here and assign this
       * to a subclass pointer. */
      virtual Int_t Init();

      /** Method prints content of the event */
      virtual void PrintEvent();

      virtual void makeBranch(TBranch *parent) {}
      virtual void deactivate();
      virtual void activate();

      virtual void Clear(Option_t *t="");
      virtual void clearAll(Int_t) {}
      virtual Bool_t isComposed() { return kFALSE; }
      virtual Short_t getId() { return fIdentifier; }
      virtual void setDebug (Bool_t debug) { fDebug=debug; }
      virtual TGo4EventElement& operator[](Int_t) { return *this; }

      virtual void Print(Option_t* option = "") const;

      /** Use this method to map event structure with the Tree branch(es) */
      virtual void synchronizeWithTree(TTree *tree, TGo4EventElement** var_ptr = 0);

      virtual Int_t activateBranch(TBranch *branch, Int_t index=0, TGo4EventElement** var_ptr = 0);

      virtual TTree* CreateSampleTree(TGo4EventElement** sample = 0);

      void ShowSampleTree();

   private:

      /** Indicates if this event is valid or not. If false, the event store
       * will not write the event.
       * We still stream this, since it may be used to flag validity of composite sub events*/
      Bool_t fbIsValid;

      /** The higher level event element that owns this instance
       * @supplierCardinality 1 */
      TGo4EventElement * fxParent; //!

      /** The external event source  instance which is used to fill this event structure
       * from the raw event. Lazy initialization as zero; the pointer is set by
       * analysis only for top level Event Elements with subevents. Might be exchanged
       * on the fly to switch the unpack of different subevents into the same
       * event structure.
       * @supplierCardinality 0..1*/
      TGo4EventSource * fxEventSource; //!

   protected:
      Short_t fIdentifier; // Identifier
      Bool_t fDebug; //! Debug level

      /** helper flag to supress Clear if event shall be kept*/
      Bool_t fbKeepContents; //!

   ClassDef(TGo4EventElement,3)
};

#endif //TGO4EVENTELEMENT_H
//===================================================================
//== TLiEvent.h:	exp0211
//== Version: 2011-02-01
//===================================================================

#ifndef TLiEVENT_H
#define TLiEVENT_H

#include "TGo4EventElement.h"

class TLiEvent : public TGo4EventElement 
{
public:
	TLiEvent();
	TLiEvent(const char* name);
	virtual ~TLiEvent();

	/** Method called by the framework to clear the event element. */
	void Clear(Option_t *t="");
	unsigned short SQ11[16];
	unsigned short SQ12[32];
	unsigned short SQ13[16];
	unsigned short SQ21[16];
	unsigned short SQ22[32];
	unsigned short SQ23[16];
	unsigned short ANR[16];
	unsigned short ANS[16];
	unsigned short ANC[32];
	unsigned short TSQ11[16];
	unsigned short TSQ12[32];
	unsigned short TSQ21[16];
	unsigned short TSQ22[32];
	unsigned short TANR[16];
	unsigned short TANS[16];
	unsigned short unusedQDC[32];
	unsigned short unusedTDC[16];
	unsigned short SCR[16];
	unsigned short nx1, ny1, nx2, ny2; // wires
	unsigned short x1[32],y1[32],x2[32],y2[32]; // wires
	unsigned short adc1[16];
	unsigned short adc2[16];
	unsigned short adc3[16];
	unsigned short adc4[16];
	unsigned short NeuTDC[32];
	unsigned short Amp_TDC[32];
	unsigned int time;
		
	int qdcF3L;
	int qdcF3R;
	int qdcF4L;
	int qdcF4R;
	int qdcF4U;
	int qdcF4D;
	int qdcF5L;
	int qdcF5R;
	int qdcF5U;
	int qdcF5D;
	int tdcF3L;
	int tdcF3R;
	int tdcF4L;
	int tdcF4R;
	int tdcF4U;
	int tdcF4D;
	int tdcF5L;
	int tdcF5R;
	int tdcF5U;
	int tdcF5D;
	int nevent;
	int trigger;
	int subevents;
	int evsize;
int mainTRIGGER;
int beamTRIGGER;
int annTRIGGER;
ClassDef(TLiEvent,1)
};
#endif //TEVENT_H



#ifndef CLS_ROOTHIT_H
#define CLS_ROOTHIT_H

/*
 * AdcVal is actually pedestal minus raw adc val.
 * This value has physical meaning.
 */

#include <TObject.h>

class cls_RootHit : public TObject
{
public: // methods

    // Default constructor should not be used
    cls_RootHit();
    virtual ~cls_RootHit();

    // Constructor with full parameters list
    cls_RootHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc, Float_t p_adcCalib);
    // Copy constructor
    cls_RootHit(cls_RootHit* p_sourceHit);

    void Clear(Option_t *option="");

    cls_RootHit &operator=(const cls_RootHit &orig);

    UChar_t GetChannel() {return fChannel;}
    Int_t GetAdcVal() {return fAdcVal;}

private: // data members

    ULong64_t fTimestamp;
    //UChar_t fFebID;        // hooks for further development
    //UChar_t fChipID;       // hooks for further development
    UChar_t fChannel;
    UShort_t fRawAdcVal;
    Int_t fAdcVal;           // (pedestal -raw adc val)
    Float_t fAdcCalibrated; // After calibration using LUTs


public:
    ClassDef(cls_RootHit,1)
};

#endif // CLS_ROOTHIT_H
#ifndef CLS_ROOTEVENT_H
#define CLS_ROOTEVENT_H

#include "TNamed.h"
#include <TClonesArray.h>

class cls_RootHit;

class cls_RootEvent : public TNamed
{

public: // methods

	cls_RootEvent();
	virtual ~cls_RootEvent();

	void Clear(Option_t *option ="");

	cls_RootHit* AddHit(cls_RootHit* p_sourceHit);
	cls_RootHit* AddHit(ULong64_t p_ts, UChar_t p_ch, UShort_t p_rawAdc, Int_t p_adc, Float_t p_adcCalib);

	void SetID(ULong_t p_id) { fEventID = p_id; }

	UShort_t GetNumOfHits() {return fNumOfHits;}
//	Int_t GetAdcVal(UChar_t channel);
	TClonesArray* GetHits() {return fHits;}

private: // data members

    TClonesArray* fHits; //-> TClonesArray of cls_RootHit objects
    UShort_t fNumOfHits; //   Number of hits - actually the size of the 'fHits'

    ULong_t fEventID;

public:
    ClassDef(cls_RootEvent,1)
};

#endif // CLS_ROOTEVENT_H
/*
 * WCalHit.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef DATA_BEAMTIME_WCALHIT_H_
#define DATA_BEAMTIME_WCALHIT_H_

#include "TObject.h"

//todo: delete
#include <iostream>

class WCalHit : public TObject {
public:
	WCalHit();
	WCalHit(UChar_t p_ch, Int_t p_adc, Float_t p_adcNonLin, Float_t p_adcNonLinCorr, Float_t p_adcCalib);
	virtual ~WCalHit();
	ClassDef(WCalHit,1);

	void Clear(Option_t *option="");

	WCalHit &operator=(const WCalHit &orig);

private:
	void SetXY();

private:
    UChar_t fChannel;
//    UShort_t fRawAdcVal;
    Int_t fAdcVal;			// (pedestal -raw adc val)
    Float_t fAdcNonLin;		//corrected for non-linearity
    Float_t fAdcNonLinCorr;	//corrected for non-linearity and pedestal shift
    Float_t fAdcCal;	// After calibration using LUTs

    Int_t fXpixel;			//number of x row (vertical)
    Int_t fYpixel;			//number of x row (or column) (horizontal)

    UInt_t fPixelFromFebCh[64]; //!
};

#endif /* DATA_BEAMTIME_WCALHIT_H_ */
/*
 * WCalEvent.h
 *
 *  Created on: Apr 6, 2017
 *      Author: vratik
 */

#ifndef DATA_BEAMTIME_WCALEVENT_H_
#define DATA_BEAMTIME_WCALEVENT_H_

#include "TNamed.h"
#include "TClonesArray.h"
#include "WCalHit.h"

class WCalEvent : public TNamed {
public:
	WCalEvent();
	virtual ~WCalEvent();
	ClassDef(WCalEvent,1);

    void Clear(Option_t *option ="");

//    cls_RootHit* AddHit(cls_RootHit* p_sourceHit);
//    WCalHit* AddHit(UChar_t p_ch, Float_t p_adcCalib);
    WCalHit* AddHit(UChar_t p_ch, Int_t p_adc, Float_t p_adcNonLin, Float_t p_adcNonLinCorr, Float_t p_adcCalib);

//    void SetID(ULong_t p_id) { fEventID = p_id; }
	void SetNumOfHits(UShort_t nhits) {fNumOfHits = nhits;};

private: // data members

    TClonesArray* fHits; //-> TClonesArray of WCalHit objects
    UShort_t fNumOfHits; //   Number of hits - actually the size of the 'fHits'

//    ULong_t fEventID;

};

#endif /* DATA_BEAMTIME_WCALEVENT_H_ */

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AEvent", payloadCode, "@",
"ERDSRDHit", payloadCode, "@",
"ERDSRDPoint", payloadCode, "@",
"EREventHeader", payloadCode, "@",
"ERGadastCsIPoint", payloadCode, "@",
"ERGadastDigi", payloadCode, "@",
"ERGadastLaBrPoint", payloadCode, "@",
"ERGadastStep", payloadCode, "@",
"ERLi10EventHeader", payloadCode, "@",
"ERLi10MCEventHeader", payloadCode, "@",
"ERMCEventHeader", payloadCode, "@",
"ERMCTrack", payloadCode, "@",
"ERNDHit", payloadCode, "@",
"ERNDPoint", payloadCode, "@",
"ERNeuRadDigi", payloadCode, "@",
"ERNeuRadHit", payloadCode, "@",
"ERNeuRadHitWBT", payloadCode, "@",
"ERNeuRadPhotoElectron", payloadCode, "@",
"ERNeuRadPixelSignal", payloadCode, "@",
"ERNeuRadPoint", payloadCode, "@",
"ERNeuRadStep", payloadCode, "@",
"ERStack", payloadCode, "@",
"ERmuSiHit", payloadCode, "@",
"ERmuSiPoint", payloadCode, "@",
"ERmuSiTrack", payloadCode, "@",
"ERmuSiVertex", payloadCode, "@",
"RawEvent", payloadCode, "@",
"TGo4EventElement", payloadCode, "@",
"TLiEvent", payloadCode, "@",
"WCalEvent", payloadCode, "@",
"WCalHit", payloadCode, "@",
"cls_RootEvent", payloadCode, "@",
"cls_RootHit", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("G__ERDataDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_G__ERDataDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_G__ERDataDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_G__ERDataDict() {
  TriggerDictionaryInitialization_G__ERDataDict_Impl();
}
