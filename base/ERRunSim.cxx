#include "ERRunSim.h"

#include "FairBaseParSet.h"             // for FairBaseParSet
#include "FairGeoParSet.h"              // for FairGeoParSet
#include "FairField.h"                  // for FairField
#include "FairFileHeader.h"             // for FairFileHeader
#include "FairGeoInterface.h"           // for FairGeoInterface
#include "FairGeoLoader.h"              // for FairGeoLoader
#include <iostream>                     // for FairLogger, MESSAGE_ORIGIN
#include "FairMCEventHeader.h"          // for FairMCEventHeader
#include "FairMesh.h"                   // for FairMesh
#include "FairModule.h"                 // for FairModule
#include "FairParSet.h"                 // for FairParSet
#include "FairPrimaryGenerator.h"       // for FairPrimaryGenerator
#include "FairRootManager.h"            // for FairRootManager
#include "FairRunIdGenerator.h"         // for FairRunIdGenerator
#include "FairRuntimeDb.h"              // for FairRuntimeDb
#include "FairTask.h"                   // for FairTask
#include "FairTrajFilter.h"             // for FairTrajFilter

#include <iosfwd>                       // for ostream
#include "TCollection.h"                // for TIter
#include "TGeoManager.h"                // for gGeoManager
#include "TList.h"                      // for TList
#include "TObjString.h"                 // for TObjString
#include "TObject.h"                    // for TObject
#include "TROOT.h"                      // for TROOT, gROOT
#include "TSystem.h"                    // for TSystem, gSystem

#include <stdlib.h>                     // for getenv, NULL
#include <string.h>                     // for strcmp, strncmp
#include <iostream>                     // for cout, endl, ostream

using std::cout;
using std::endl;

/** default ctor*/
ERRunSim::ERRunSim(Bool_t isMaster):
fDecayer(NULL)
{

}
/** default dtor*/
ERRunSim::~ERRunSim(){

}
/**
*       Initialize the Simulation
*/
void  ERRunSim::Init(){
  /**Initialize the simulation session*/

  //CheckFlukaExec();

//  fOutFile=fRootManager->OpenOutFile(fOutname);
  std::cout << "==============  FairRunSim: Initialising simulation run ==============" << std::endl;

  FairGeoLoader* loader=new FairGeoLoader(fLoaderName->Data(), "Geo Loader");
  FairGeoInterface* GeoInterFace=loader->getGeoInterface();
  GeoInterFace->SetNoOfSets(ListOfModules->GetEntries());
  GeoInterFace->setMediaFile(MatFname.Data());
  GeoInterFace->readMedia();

  //  gSystem->cd(flout.Data());

  fApp= new ERMCApplication("Fair","The Fair VMC App",ListOfModules, MatFname);
  fApp->SetGenerator(fGen);

  // Add a Generated run ID to the FairRunTimeDb
  FairRunIdGenerator genid;
  // FairRuntimeDb *rtdb= GetRuntimeDb();
  fRunId = genid.generateId();
  fRtdb->addRun(fRunId);

  fFileHeader->SetRunId(fRunId);
  /** Add Tasks to simulation if any*/
  fApp->AddTask(fTask);

  FairBaseParSet* par=dynamic_cast<FairBaseParSet*>(fRtdb->getContainer("FairBaseParSet"));
  if (par) {
    par->SetDetList(GetListOfModules());
    par->SetGen(GetPrimaryGenerator());
    par->SetBeamMom(fBeamMom);
  }

  FairGeoParSet* geopar=dynamic_cast<FairGeoParSet*>(fRtdb->getContainer("FairGeoParSet"));
  if (geopar) {
    geopar->SetGeometry(gGeoManager);
  }

  // Set global Parameter Info

  if(fPythiaDecayer) {
    fApp->SetPythiaDecayer(fPythiaDecayer);
    if (fPythiaDecayerConfig) {
      fApp->SetPythiaDecayerConfig(fPythiaDecayerConfig);
    }
  }
  if(fUserDecay) {
    fApp->SetUserDecay(fUserDecay);
    if (fUserDecayConfig) {
      fApp->SetUserDecayConfig(fUserDecayConfig);
    }
  }
  // on/off visualisation
  if( fStoreTraj ) {
    std::cout << "Create visualisation manager " << std::endl;
    new FairTrajFilter();
  }
  if(fRadLength) {
    fApp->SetRadiationLengthReg(fRadLength);
  }
  if(fRadMap) {
    fApp->SetRadiationMapReg(fRadMap);
  }
  if(fRadGrid) {
    fApp->AddMeshList(fMeshList);
  }

  if(fField) { fField->Init(); }
  fApp->SetField(fField);
  SetFieldContainer();

  TList* containerList=fRtdb->getListOfContainers();
  TIter next(containerList);
  FairParSet* cont;
  TObjArray* ContList= new TObjArray();
  while ((cont=dynamic_cast<FairParSet*>(next()))) {
    ContList->Add(new TObjString(cont->GetName()));
  }

  par->SetContListStr(ContList);
  par->setChanged();
  par->setInputVersion(fRunId,1);

  geopar->setChanged();
  geopar->setInputVersion(fRunId,1);

  /**Set the configuration for MC engine*/
  SetMCConfig();
  fRootManager->WriteFileHeader(fFileHeader);

  if (fDecayer){
    ((ERMCApplication*)fApp)->SetDecayer(fDecayer);
    if (!fDecayer->Init())
      Fatal("ERRunSim::Init", "Decayer init problem");
  }
}

void ERRunSim::SetMCConfig()
{
  /** Private method for setting simulation and/or Geane configuration and cuts*/

  TString work = getenv("VMCWORKDIR");
  TString work_config=work+"/gconfig/";
  work_config.ReplaceAll("//","/");

  TString Lib_config= getenv("GEANT4VMC_MACRO_DIR");
  Lib_config.ReplaceAll("//","/");
  if (!Lib_config.EndsWith("/")) { Lib_config+="/"; }

  TString config_dir= getenv("CONFIG_DIR");
  config_dir.ReplaceAll("//","/");

  Bool_t AbsPath=kFALSE;

  TString LibMacro;
  TString LibFunction;
  TString ConfigMacro;
  TString cuts=fUserCuts;
  //----------------------------------------------Geant4 Config-----------------------------------------
  if(strcmp(GetName(),"TGeant4") == 0 ) {
    TString g4LibMacro="g4libs.C";
    TString g4Macro;
    if(fUserConfig.IsNull()) {
      g4Macro="g4Config.C";
      fUserConfig = g4Macro;
    } else {
      if (fUserConfig.Contains("/")) { AbsPath=kTRUE; }
      g4Macro = fUserConfig;
      std::cout << "---------------User config is used: " 
		<< g4Macro.Data() << std::endl;
    }
    if (TString(gSystem->FindFile(config_dir.Data(),g4LibMacro)) != TString("")) { //be carfull after this call the string g4LibMacro is empty if not found!!!!
      std::cout << "---User path for Configuration (g4libs.C) is used: " 
		<< config_dir.Data() << std::endl;
    } else {
      g4LibMacro=Lib_config+"g4libs.C";
    }
    LibMacro=g4LibMacro;
    LibFunction="g4libs()";
    if (!AbsPath && TString(gSystem->FindFile(config_dir.Data(),g4Macro)) != TString("")) {
      std::cout << "---User path for Configuration (g4Config.C) is used: " 
		<< config_dir.Data() << std::endl;
      ConfigMacro=g4Macro;
    } else {
      if(AbsPath) { ConfigMacro = fUserConfig; }
      else { ConfigMacro =work_config+fUserConfig; }
    }
    //----------------------------------------------Geant3 Config-----------------------------------------
  } else if(strcmp(GetName(),"TGeant3") == 0 ) {
    TString g3LibMacro="g3libs.C";
    TString g3Macro="g3Config.C";
    if(fUserConfig.IsNull()) {
      g3Macro="g3Config.C";
      fUserConfig = g3Macro;
      std::cout << "-------------- Standard Config is called ------------------------------------" << std::endl;
    } else {
      if (fUserConfig.Contains("/")) { AbsPath=kTRUE; }
      g3Macro = fUserConfig;
      std::cout << "---------------User config is used: " 
		<< g3Macro.Data() << std::endl;
    }
    if (TString(gSystem->FindFile(config_dir.Data(),g3LibMacro)) != TString("")) {
      std::cout << "---User path for Configuration (g3libs.C) is used: " 
		<< config_dir.Data() << std::endl;
    } else {
      g3LibMacro=work_config+"g3libs.C";
    }
    LibMacro=g3LibMacro;
    LibFunction="g3libs()";
    if (!AbsPath && TString(gSystem->FindFile(config_dir.Data(),g3Macro)) != TString("")) {
      std::cout << "---User path for Configuration (g3Config.C) is used: "
		<< config_dir.Data() << std::endl;
      ConfigMacro=g3Macro;
    } else {
      if(AbsPath) { ConfigMacro = fUserConfig; }
      else { ConfigMacro =work_config+fUserConfig; }
    }
    //----------------------------------------------Fluka Config-----------------------------------------
  } else if(strcmp(GetName(),"TFluka") == 0 ) {
    TString flLibMacro="fllibs.C";
    TString flMacro="flConfig.C";
    if(fUserConfig.IsNull()) {
      flMacro="flConfig.C";
      fUserConfig=flMacro;
    } else {
      if (fUserConfig.Contains("/")) { AbsPath=kTRUE; }
      flMacro = fUserConfig;
      std::cout << "---------------User config is used: " 
		<< flMacro.Data() << std::endl;
    }
    if (TString(gSystem->FindFile(config_dir.Data(), flLibMacro)) != TString("")) {
      std::cout << "---User path for Configuration (fllibs.C) is used: "
		<< config_dir.Data() << std::endl;
    } else {
      flLibMacro=work_config+"fllibs.C";
    }
    LibMacro=flLibMacro;
    LibFunction="fllibs()";
    if (!AbsPath && TString(gSystem->FindFile(config_dir.Data(),flMacro)) != TString("")) {
      std::cout << "---User path for Configuration (flConfig.C) is used: " 
		<< config_dir.Data() << std::endl;
      ConfigMacro=flMacro;
    } else {
      if(AbsPath) { ConfigMacro = fUserConfig; }
      else { ConfigMacro =work_config+fUserConfig; }
    }
  }
  //----------------------------------------------SetCuts------------------------------------------------
  if (TString(gSystem->FindFile(config_dir.Data(),cuts)) != TString("")) {
    std::cout << "---User path for Cuts and Processes (SetCuts.C) is used: "
	      << config_dir.Data() << std::endl;
  } else {
    cuts =work_config+ fUserCuts;
  }
  //--------------------------------------Now load the Config and Cuts------------------------------------
  gROOT->LoadMacro(LibMacro.Data());
  gROOT->ProcessLine(LibFunction.Data());

  gROOT->LoadMacro(ConfigMacro.Data());
  gROOT->ProcessLine("Config()");

  gROOT->LoadMacro(cuts);
  gROOT->ProcessLine("SetCuts()");

  if (fLocMagField){
    //Add local magnetic field
    TGeoVolume* vol =  gGeoManager->GetVolume(fLocMagFieldVolName);
    if (!vol)
      LOG(ERROR) << "Volume " << fLocMagFieldVolName <<
          "for local magnetic field not found in geometry" << FairLogger::endl;
    else{
      fLocMagField->Init();
      vol->SetField(fLocMagField);
    }
  }

  ((ERMCApplication*)fApp)->InitMC(ConfigMacro.Data(), cuts.Data());
}

void ERRunSim::SetField(FairField* magField,TString volName){
  fLocMagField = magField;
  fLocMagFieldVolName = volName;
}