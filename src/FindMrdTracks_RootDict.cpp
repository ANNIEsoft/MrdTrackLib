// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIFindMrdTracks_RootDict

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
#include "MRDSubEventClass.hh"
#include "MRDTrackClass.hh"
#include "MRDspecs.hh"
#include "MrdCell.hh"
#include "MrdCluster.hh"

// Header files passed via #pragma extra_include

namespace MRDSpecs {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *MRDSpecs_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("MRDSpecs", 0 /*version*/, "MRDspecs.hh", 12,
                     ::ROOT::Internal::DefineBehavior((void*)0,(void*)0),
                     &MRDSpecs_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *MRDSpecs_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_cMRDSubEvent(void *p = 0);
   static void *newArray_cMRDSubEvent(Long_t size, void *p);
   static void delete_cMRDSubEvent(void *p);
   static void deleteArray_cMRDSubEvent(void *p);
   static void destruct_cMRDSubEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cMRDSubEvent*)
   {
      ::cMRDSubEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cMRDSubEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cMRDSubEvent", ::cMRDSubEvent::Class_Version(), "MRDSubEventClass.hh", 20,
                  typeid(::cMRDSubEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cMRDSubEvent::Dictionary, isa_proxy, 4,
                  sizeof(::cMRDSubEvent) );
      instance.SetNew(&new_cMRDSubEvent);
      instance.SetNewArray(&newArray_cMRDSubEvent);
      instance.SetDelete(&delete_cMRDSubEvent);
      instance.SetDeleteArray(&deleteArray_cMRDSubEvent);
      instance.SetDestructor(&destruct_cMRDSubEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cMRDSubEvent*)
   {
      return GenerateInitInstanceLocal((::cMRDSubEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::cMRDSubEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *pairlEmrdclustermUcOmrdclustermUgR_Dictionary();
   static void pairlEmrdclustermUcOmrdclustermUgR_TClassManip(TClass*);
   static void *new_pairlEmrdclustermUcOmrdclustermUgR(void *p = 0);
   static void *newArray_pairlEmrdclustermUcOmrdclustermUgR(Long_t size, void *p);
   static void delete_pairlEmrdclustermUcOmrdclustermUgR(void *p);
   static void deleteArray_pairlEmrdclustermUcOmrdclustermUgR(void *p);
   static void destruct_pairlEmrdclustermUcOmrdclustermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const pair<mrdcluster*,mrdcluster*>*)
   {
      pair<mrdcluster*,mrdcluster*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(pair<mrdcluster*,mrdcluster*>));
      static ::ROOT::TGenericClassInfo 
         instance("pair<mrdcluster*,mrdcluster*>", "string", 96,
                  typeid(pair<mrdcluster*,mrdcluster*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &pairlEmrdclustermUcOmrdclustermUgR_Dictionary, isa_proxy, 4,
                  sizeof(pair<mrdcluster*,mrdcluster*>) );
      instance.SetNew(&new_pairlEmrdclustermUcOmrdclustermUgR);
      instance.SetNewArray(&newArray_pairlEmrdclustermUcOmrdclustermUgR);
      instance.SetDelete(&delete_pairlEmrdclustermUcOmrdclustermUgR);
      instance.SetDeleteArray(&deleteArray_pairlEmrdclustermUcOmrdclustermUgR);
      instance.SetDestructor(&destruct_pairlEmrdclustermUcOmrdclustermUgR);
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const pair<mrdcluster*,mrdcluster*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *pairlEmrdclustermUcOmrdclustermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const pair<mrdcluster*,mrdcluster*>*)0x0)->GetClass();
      pairlEmrdclustermUcOmrdclustermUgR_TClassManip(theClass);
   return theClass;
   }

   static void pairlEmrdclustermUcOmrdclustermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_cMRDTrack(void *p = 0);
   static void *newArray_cMRDTrack(Long_t size, void *p);
   static void delete_cMRDTrack(void *p);
   static void deleteArray_cMRDTrack(void *p);
   static void destruct_cMRDTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::cMRDTrack*)
   {
      ::cMRDTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::cMRDTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("cMRDTrack", ::cMRDTrack::Class_Version(), "MRDTrackClass.hh", 24,
                  typeid(::cMRDTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::cMRDTrack::Dictionary, isa_proxy, 4,
                  sizeof(::cMRDTrack) );
      instance.SetNew(&new_cMRDTrack);
      instance.SetNewArray(&newArray_cMRDTrack);
      instance.SetDelete(&delete_cMRDTrack);
      instance.SetDeleteArray(&deleteArray_cMRDTrack);
      instance.SetDestructor(&destruct_cMRDTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::cMRDTrack*)
   {
      return GenerateInitInstanceLocal((::cMRDTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::cMRDTrack*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *mrdcell_Dictionary();
   static void mrdcell_TClassManip(TClass*);
   static void *new_mrdcell(void *p = 0);
   static void *newArray_mrdcell(Long_t size, void *p);
   static void delete_mrdcell(void *p);
   static void deleteArray_mrdcell(void *p);
   static void destruct_mrdcell(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mrdcell*)
   {
      ::mrdcell *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::mrdcell));
      static ::ROOT::TGenericClassInfo 
         instance("mrdcell", "MrdCell.hh", 11,
                  typeid(::mrdcell), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &mrdcell_Dictionary, isa_proxy, 4,
                  sizeof(::mrdcell) );
      instance.SetNew(&new_mrdcell);
      instance.SetNewArray(&newArray_mrdcell);
      instance.SetDelete(&delete_mrdcell);
      instance.SetDeleteArray(&deleteArray_mrdcell);
      instance.SetDestructor(&destruct_mrdcell);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mrdcell*)
   {
      return GenerateInitInstanceLocal((::mrdcell*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mrdcell*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *mrdcell_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::mrdcell*)0x0)->GetClass();
      mrdcell_TClassManip(theClass);
   return theClass;
   }

   static void mrdcell_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","include/MrdCell.hh");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *mrdcluster_Dictionary();
   static void mrdcluster_TClassManip(TClass*);
   static void *new_mrdcluster(void *p = 0);
   static void *newArray_mrdcluster(Long_t size, void *p);
   static void delete_mrdcluster(void *p);
   static void deleteArray_mrdcluster(void *p);
   static void destruct_mrdcluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mrdcluster*)
   {
      ::mrdcluster *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::mrdcluster));
      static ::ROOT::TGenericClassInfo 
         instance("mrdcluster", "MrdCluster.hh", 12,
                  typeid(::mrdcluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &mrdcluster_Dictionary, isa_proxy, 4,
                  sizeof(::mrdcluster) );
      instance.SetNew(&new_mrdcluster);
      instance.SetNewArray(&newArray_mrdcluster);
      instance.SetDelete(&delete_mrdcluster);
      instance.SetDeleteArray(&deleteArray_mrdcluster);
      instance.SetDestructor(&destruct_mrdcluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mrdcluster*)
   {
      return GenerateInitInstanceLocal((::mrdcluster*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::mrdcluster*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *mrdcluster_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::mrdcluster*)0x0)->GetClass();
      mrdcluster_TClassManip(theClass);
   return theClass;
   }

   static void mrdcluster_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","include/MrdCluster.hh");
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr cMRDSubEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cMRDSubEvent::Class_Name()
{
   return "cMRDSubEvent";
}

//______________________________________________________________________________
const char *cMRDSubEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cMRDSubEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cMRDSubEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cMRDSubEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cMRDSubEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cMRDSubEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cMRDSubEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cMRDSubEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr cMRDTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *cMRDTrack::Class_Name()
{
   return "cMRDTrack";
}

//______________________________________________________________________________
const char *cMRDTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cMRDTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int cMRDTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::cMRDTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *cMRDTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cMRDTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *cMRDTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::cMRDTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void cMRDSubEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class cMRDSubEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cMRDSubEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(cMRDSubEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cMRDSubEvent(void *p) {
      return  p ? new(p) ::cMRDSubEvent : new ::cMRDSubEvent;
   }
   static void *newArray_cMRDSubEvent(Long_t nElements, void *p) {
      return p ? new(p) ::cMRDSubEvent[nElements] : new ::cMRDSubEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_cMRDSubEvent(void *p) {
      delete ((::cMRDSubEvent*)p);
   }
   static void deleteArray_cMRDSubEvent(void *p) {
      delete [] ((::cMRDSubEvent*)p);
   }
   static void destruct_cMRDSubEvent(void *p) {
      typedef ::cMRDSubEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cMRDSubEvent

namespace ROOT {
   // Wrappers around operator new
   static void *new_pairlEmrdclustermUcOmrdclustermUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<mrdcluster*,mrdcluster*> : new pair<mrdcluster*,mrdcluster*>;
   }
   static void *newArray_pairlEmrdclustermUcOmrdclustermUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) pair<mrdcluster*,mrdcluster*>[nElements] : new pair<mrdcluster*,mrdcluster*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_pairlEmrdclustermUcOmrdclustermUgR(void *p) {
      delete ((pair<mrdcluster*,mrdcluster*>*)p);
   }
   static void deleteArray_pairlEmrdclustermUcOmrdclustermUgR(void *p) {
      delete [] ((pair<mrdcluster*,mrdcluster*>*)p);
   }
   static void destruct_pairlEmrdclustermUcOmrdclustermUgR(void *p) {
      typedef pair<mrdcluster*,mrdcluster*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class pair<mrdcluster*,mrdcluster*>

//______________________________________________________________________________
void cMRDTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class cMRDTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(cMRDTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(cMRDTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_cMRDTrack(void *p) {
      return  p ? new(p) ::cMRDTrack : new ::cMRDTrack;
   }
   static void *newArray_cMRDTrack(Long_t nElements, void *p) {
      return p ? new(p) ::cMRDTrack[nElements] : new ::cMRDTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_cMRDTrack(void *p) {
      delete ((::cMRDTrack*)p);
   }
   static void deleteArray_cMRDTrack(void *p) {
      delete [] ((::cMRDTrack*)p);
   }
   static void destruct_cMRDTrack(void *p) {
      typedef ::cMRDTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::cMRDTrack

namespace ROOT {
   // Wrappers around operator new
   static void *new_mrdcell(void *p) {
      return  p ? new(p) ::mrdcell : new ::mrdcell;
   }
   static void *newArray_mrdcell(Long_t nElements, void *p) {
      return p ? new(p) ::mrdcell[nElements] : new ::mrdcell[nElements];
   }
   // Wrapper around operator delete
   static void delete_mrdcell(void *p) {
      delete ((::mrdcell*)p);
   }
   static void deleteArray_mrdcell(void *p) {
      delete [] ((::mrdcell*)p);
   }
   static void destruct_mrdcell(void *p) {
      typedef ::mrdcell current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mrdcell

namespace ROOT {
   // Wrappers around operator new
   static void *new_mrdcluster(void *p) {
      return  p ? new(p) ::mrdcluster : new ::mrdcluster;
   }
   static void *newArray_mrdcluster(Long_t nElements, void *p) {
      return p ? new(p) ::mrdcluster[nElements] : new ::mrdcluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_mrdcluster(void *p) {
      delete ((::mrdcluster*)p);
   }
   static void deleteArray_mrdcluster(void *p) {
      delete [] ((::mrdcluster*)p);
   }
   static void destruct_mrdcluster(void *p) {
      typedef ::mrdcluster current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mrdcluster

namespace ROOT {
   static TClass *vectorlEpairlETVector3cOTVector3gRsPgR_Dictionary();
   static void vectorlEpairlETVector3cOTVector3gRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlETVector3cOTVector3gRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlETVector3cOTVector3gRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlETVector3cOTVector3gRsPgR(void *p);
   static void deleteArray_vectorlEpairlETVector3cOTVector3gRsPgR(void *p);
   static void destruct_vectorlEpairlETVector3cOTVector3gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<TVector3,TVector3> >*)
   {
      vector<pair<TVector3,TVector3> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<TVector3,TVector3> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<TVector3,TVector3> >", -2, "vector", 214,
                  typeid(vector<pair<TVector3,TVector3> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEpairlETVector3cOTVector3gRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<pair<TVector3,TVector3> >) );
      instance.SetNew(&new_vectorlEpairlETVector3cOTVector3gRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlETVector3cOTVector3gRsPgR);
      instance.SetDelete(&delete_vectorlEpairlETVector3cOTVector3gRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlETVector3cOTVector3gRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlETVector3cOTVector3gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<TVector3,TVector3> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<pair<TVector3,TVector3> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlETVector3cOTVector3gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<TVector3,TVector3> >*)0x0)->GetClass();
      vectorlEpairlETVector3cOTVector3gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlETVector3cOTVector3gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlETVector3cOTVector3gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<TVector3,TVector3> > : new vector<pair<TVector3,TVector3> >;
   }
   static void *newArray_vectorlEpairlETVector3cOTVector3gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<pair<TVector3,TVector3> >[nElements] : new vector<pair<TVector3,TVector3> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlETVector3cOTVector3gRsPgR(void *p) {
      delete ((vector<pair<TVector3,TVector3> >*)p);
   }
   static void deleteArray_vectorlEpairlETVector3cOTVector3gRsPgR(void *p) {
      delete [] ((vector<pair<TVector3,TVector3> >*)p);
   }
   static void destruct_vectorlEpairlETVector3cOTVector3gRsPgR(void *p) {
      typedef vector<pair<TVector3,TVector3> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<TVector3,TVector3> >

namespace ROOT {
   static TClass *vectorlEmrdclustergR_Dictionary();
   static void vectorlEmrdclustergR_TClassManip(TClass*);
   static void *new_vectorlEmrdclustergR(void *p = 0);
   static void *newArray_vectorlEmrdclustergR(Long_t size, void *p);
   static void delete_vectorlEmrdclustergR(void *p);
   static void deleteArray_vectorlEmrdclustergR(void *p);
   static void destruct_vectorlEmrdclustergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mrdcluster>*)
   {
      vector<mrdcluster> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mrdcluster>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mrdcluster>", -2, "vector", 214,
                  typeid(vector<mrdcluster>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmrdclustergR_Dictionary, isa_proxy, 0,
                  sizeof(vector<mrdcluster>) );
      instance.SetNew(&new_vectorlEmrdclustergR);
      instance.SetNewArray(&newArray_vectorlEmrdclustergR);
      instance.SetDelete(&delete_vectorlEmrdclustergR);
      instance.SetDeleteArray(&deleteArray_vectorlEmrdclustergR);
      instance.SetDestructor(&destruct_vectorlEmrdclustergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mrdcluster> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<mrdcluster>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmrdclustergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mrdcluster>*)0x0)->GetClass();
      vectorlEmrdclustergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmrdclustergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmrdclustergR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mrdcluster> : new vector<mrdcluster>;
   }
   static void *newArray_vectorlEmrdclustergR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mrdcluster>[nElements] : new vector<mrdcluster>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmrdclustergR(void *p) {
      delete ((vector<mrdcluster>*)p);
   }
   static void deleteArray_vectorlEmrdclustergR(void *p) {
      delete [] ((vector<mrdcluster>*)p);
   }
   static void destruct_vectorlEmrdclustergR(void *p) {
      typedef vector<mrdcluster> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mrdcluster>

namespace ROOT {
   static TClass *vectorlEmrdcellgR_Dictionary();
   static void vectorlEmrdcellgR_TClassManip(TClass*);
   static void *new_vectorlEmrdcellgR(void *p = 0);
   static void *newArray_vectorlEmrdcellgR(Long_t size, void *p);
   static void delete_vectorlEmrdcellgR(void *p);
   static void deleteArray_vectorlEmrdcellgR(void *p);
   static void destruct_vectorlEmrdcellgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mrdcell>*)
   {
      vector<mrdcell> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mrdcell>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mrdcell>", -2, "vector", 214,
                  typeid(vector<mrdcell>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmrdcellgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<mrdcell>) );
      instance.SetNew(&new_vectorlEmrdcellgR);
      instance.SetNewArray(&newArray_vectorlEmrdcellgR);
      instance.SetDelete(&delete_vectorlEmrdcellgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmrdcellgR);
      instance.SetDestructor(&destruct_vectorlEmrdcellgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mrdcell> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<mrdcell>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmrdcellgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mrdcell>*)0x0)->GetClass();
      vectorlEmrdcellgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmrdcellgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmrdcellgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mrdcell> : new vector<mrdcell>;
   }
   static void *newArray_vectorlEmrdcellgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mrdcell>[nElements] : new vector<mrdcell>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmrdcellgR(void *p) {
      delete ((vector<mrdcell>*)p);
   }
   static void deleteArray_vectorlEmrdcellgR(void *p) {
      delete [] ((vector<mrdcell>*)p);
   }
   static void destruct_vectorlEmrdcellgR(void *p) {
      typedef vector<mrdcell> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mrdcell>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEcMRDTrackgR_Dictionary();
   static void vectorlEcMRDTrackgR_TClassManip(TClass*);
   static void *new_vectorlEcMRDTrackgR(void *p = 0);
   static void *newArray_vectorlEcMRDTrackgR(Long_t size, void *p);
   static void delete_vectorlEcMRDTrackgR(void *p);
   static void deleteArray_vectorlEcMRDTrackgR(void *p);
   static void destruct_vectorlEcMRDTrackgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<cMRDTrack>*)
   {
      vector<cMRDTrack> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<cMRDTrack>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<cMRDTrack>", -2, "vector", 214,
                  typeid(vector<cMRDTrack>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEcMRDTrackgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<cMRDTrack>) );
      instance.SetNew(&new_vectorlEcMRDTrackgR);
      instance.SetNewArray(&newArray_vectorlEcMRDTrackgR);
      instance.SetDelete(&delete_vectorlEcMRDTrackgR);
      instance.SetDeleteArray(&deleteArray_vectorlEcMRDTrackgR);
      instance.SetDestructor(&destruct_vectorlEcMRDTrackgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<cMRDTrack> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<cMRDTrack>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEcMRDTrackgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<cMRDTrack>*)0x0)->GetClass();
      vectorlEcMRDTrackgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEcMRDTrackgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEcMRDTrackgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cMRDTrack> : new vector<cMRDTrack>;
   }
   static void *newArray_vectorlEcMRDTrackgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<cMRDTrack>[nElements] : new vector<cMRDTrack>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEcMRDTrackgR(void *p) {
      delete ((vector<cMRDTrack>*)p);
   }
   static void deleteArray_vectorlEcMRDTrackgR(void *p) {
      delete [] ((vector<cMRDTrack>*)p);
   }
   static void destruct_vectorlEcMRDTrackgR(void *p) {
      typedef vector<cMRDTrack> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<cMRDTrack>

namespace {
  void TriggerDictionaryInitialization_FindMrdTracks_RootDict_Impl() {
    static const char* headers[] = {
"MRDSubEventClass.hh",
"MRDTrackClass.hh",
"MRDspecs.hh",
"MrdCell.hh",
"MrdCluster.hh",
0
    };
    static const char* includePaths[] = {
"/grid/fermiapp/products/larsoft/root/v6_06_08/Linux64bit+2.6-2.12-e10-nu-debug/include",
"/grid/fermiapp/products/larsoft/root/v6_06_08/Linux64bit+2.6-2.12-e10-nu-debug/include",
"/annie/app/users/moflaher/ToolAnalysis/LibFindMrdTracks2/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "FindMrdTracks_RootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$MRDSubEventClass.hh")))  cMRDSubEvent;
class __attribute__((annotate(R"ATTRDUMP(file_name@@@include/MrdCluster.hh)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$MRDTrackClass.hh")))  mrdcluster;
namespace std{template <class _T1, class _T2> struct __attribute__((annotate("$clingAutoload$string")))  pair;
}
class __attribute__((annotate(R"ATTRDUMP(INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(INCREMENT VERSION NUM EVERY TIME CLASS MEMBERS CHANGE)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$MRDTrackClass.hh")))  cMRDTrack;
class __attribute__((annotate(R"ATTRDUMP(file_name@@@include/MrdCell.hh)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$MRDTrackClass.hh")))  mrdcell;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "FindMrdTracks_RootDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "MRDSubEventClass.hh"
#include "MRDTrackClass.hh"
#include "MRDspecs.hh"
#include "MrdCell.hh"
#include "MrdCluster.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"", payloadCode, "@",
"MRDSpecs::MRDPMTExposeHeight", payloadCode, "@",
"MRDSpecs::MRDPMTRadius", payloadCode, "@",
"MRDSpecs::MRD_depth", payloadCode, "@",
"MRDSpecs::MRD_end", payloadCode, "@",
"MRDSpecs::MRD_height", payloadCode, "@",
"MRDSpecs::MRD_layer2", payloadCode, "@",
"MRDSpecs::MRD_start", payloadCode, "@",
"MRDSpecs::MRD_steel_height", payloadCode, "@",
"MRDSpecs::MRD_steel_width", payloadCode, "@",
"MRDSpecs::MRD_width", payloadCode, "@",
"MRDSpecs::alufullxlen", payloadCode, "@",
"MRDSpecs::alufullxthickness", payloadCode, "@",
"MRDSpecs::alufullylen", payloadCode, "@",
"MRDSpecs::alufullythickness", payloadCode, "@",
"MRDSpecs::alufullzlen", payloadCode, "@",
"MRDSpecs::alusteelgap", payloadCode, "@",
"MRDSpecs::heights", payloadCode, "@",
"MRDSpecs::layergap", payloadCode, "@",
"MRDSpecs::layeroffsets", payloadCode, "@",
"MRDSpecs::maxheight", payloadCode, "@",
"MRDSpecs::maxwidth", payloadCode, "@",
"MRDSpecs::mrdZlen", payloadCode, "@",
"MRDSpecs::mrdpmtfullheight", payloadCode, "@",
"MRDSpecs::mrdscintlayers", payloadCode, "@",
"MRDSpecs::nothickness", payloadCode, "@",
"MRDSpecs::numalustructs", payloadCode, "@",
"MRDSpecs::numhpanels", payloadCode, "@",
"MRDSpecs::nummrdpmts", payloadCode, "@",
"MRDSpecs::numpaddlesperpanelh", payloadCode, "@",
"MRDSpecs::numpaddlesperpanelv", payloadCode, "@",
"MRDSpecs::numpanels", payloadCode, "@",
"MRDSpecs::numplates", payloadCode, "@",
"MRDSpecs::numvetopaddles", payloadCode, "@",
"MRDSpecs::numvpanels", payloadCode, "@",
"MRDSpecs::paddle_extentsx", payloadCode, "@",
"MRDSpecs::paddle_extentsy", payloadCode, "@",
"MRDSpecs::paddle_extentsz", payloadCode, "@",
"MRDSpecs::paddle_layers", payloadCode, "@",
"MRDSpecs::paddle_orientations", payloadCode, "@",
"MRDSpecs::paddle_originx", payloadCode, "@",
"MRDSpecs::paddle_originy", payloadCode, "@",
"MRDSpecs::paddle_originz", payloadCode, "@",
"MRDSpecs::scintalugap", payloadCode, "@",
"MRDSpecs::scintbordergap", payloadCode, "@",
"MRDSpecs::scintfullxlen", payloadCode, "@",
"MRDSpecs::scintfullzlen", payloadCode, "@",
"MRDSpecs::scinthfullylen", payloadCode, "@",
"MRDSpecs::scintlgfullheight", payloadCode, "@",
"MRDSpecs::scintlgfullwidth", payloadCode, "@",
"MRDSpecs::scinttapfullheight", payloadCode, "@",
"MRDSpecs::scinttapfullwidth", payloadCode, "@",
"MRDSpecs::scintvfullylen", payloadCode, "@",
"MRDSpecs::steelfullxlen", payloadCode, "@",
"MRDSpecs::steelfullylen", payloadCode, "@",
"MRDSpecs::steelfullzlen", payloadCode, "@",
"MRDSpecs::steelscintgap", payloadCode, "@",
"MRDSpecs::tank_halfheight", payloadCode, "@",
"MRDSpecs::tank_radius", payloadCode, "@",
"MRDSpecs::tank_start", payloadCode, "@",
"MRDSpecs::tank_yoffset", payloadCode, "@",
"MRDSpecs::tankouterRadius", payloadCode, "@",
"MRDSpecs::vetopaddlesperpanel", payloadCode, "@",
"MRDSpecs::widths", payloadCode, "@",
"MRDSpecs::windowheight", payloadCode, "@",
"MRDSpecs::windowwidth", payloadCode, "@",
"cMRDSubEvent", payloadCode, "@",
"cMRDSubEvent::aspectrumv", payloadCode, "@",
"cMRDSubEvent::colorhexes", payloadCode, "@",
"cMRDSubEvent::fgIsA", payloadCode, "@",
"cMRDSubEvent::fillstaticmembers", payloadCode, "@",
"cMRDSubEvent::imgcanvas", payloadCode, "@",
"cMRDSubEvent::paddlepointers", payloadCode, "@",
"cMRDSubEvent::titleleft", payloadCode, "@",
"cMRDSubEvent::titleright", payloadCode, "@",
"cMRDSubEvent::trackcolours", payloadCode, "@",
"cMRDTrack", payloadCode, "@",
"cMRDTrack::MRDenergyvspenetration", payloadCode, "@",
"cMRDTrack::fgIsA", payloadCode, "@",
"cMRDTrack::fillstaticmembers", payloadCode, "@",
"mrdcell", payloadCode, "@",
"mrdcell::cellcounter", payloadCode, "@",
"mrdcluster", payloadCode, "@",
"mrdcluster::clustercounter", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("FindMrdTracks_RootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_FindMrdTracks_RootDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_FindMrdTracks_RootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_FindMrdTracks_RootDict() {
  TriggerDictionaryInitialization_FindMrdTracks_RootDict_Impl();
}
