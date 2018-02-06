void exp1803_sim(Int_t nEvents = 100) {
  // --------------- Telescope T1 -------------------------------------------
  Double_t T1Dl = 0.5;         // [cm]      
  Double_t T1PosZ = 10.;       // [cm] 
  Double_t T1D1Thick = 0.002;  // [cm]  
  Double_t T1D2Thick = 0.1;    // [cm] 
  Double_t T1Side = 6.2;       // [cm]  this parameter should coincide with SD1 side length from /db/QTelescope/QTelescopeParts.xml
  Double_t T1Aperture = 3.1;   // [cm]  
  // --------------- Telescope D1 -------------------------------------------
  Double_t D1PosZ = 20.;       // [cm]
  Double_t D1Thick = 0.03;     // [cm]
  // --------------- BeamDet ------------------------------------------------
  Double_t BeamDetLToF = 100.;
  Double_t BeamDetPosZToF = -300;
  Double_t BeamDetLMWPC = 80.;
  Double_t BeamDetPosZMWPC = -50;
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = 0;
  // --------------- Target -------------------------------------------------
  Double_t targetH2Thickness = 0.4;  // this parameter should coincide with target H2 thickness in /macro/geo/create_GadastEXP1803_geo.C 
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/target.h2.geo.root";
  TString gadastGeoFileName = workDirPath + "/geometry/partOfGadast.v1.geo.root";
  TString ndGeoFileName = workDirPath + "/geometry/ND.geo.root";
  TString magnetGeoFileName = workDirPath + "/geometry/magnet.geo.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
 
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // ------------------------------------------------------------------------
  
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials

  // -----   Create detectors  ----------------------------------------------   
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  // -----   Create target  -------------------------------------------------
  FairModule* target = new ERTarget("targetH2", kTRUE, 1);
  target->SetGeometryFileName(targetGeoFileName);
  run->AddModule(target);

  // -----   Create Part of Gadast ------------------------------------------
  ERGadast* gadast = new ERGadast("PartofGadast", kTRUE, 1);
  gadast->SetGeometryFileName(gadastGeoFileName);
  run->AddModule(gadast);

  // -----   Create Stilbene wall -------------------------------------------
  ERND* neutronDetector = new ERND("StilbeneWall", kTRUE, 1);
  neutronDetector->SetGeometryFileName(ndGeoFileName);
  run->AddModule(neutronDetector);

  // -----   Create Magnet geometry -----------------------------------------
  FairModule* magnet = new ERTarget("magnet", 1, kTRUE);
  magnet->SetGeometryFileName(magnetGeoFileName);
  run->AddModule(magnet);

  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXmlParametersFile(paramFileQTelescope);

  // ----- T1 parameters ----------------------------------------------------
  // ----- T1.1--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3( T1Side/2 + T1Aperture/2, 
                                                   T1Side/2 - T1Aperture/2,  
                                                   T1PosZ + T1D1Thick/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3( T1Side/2 + T1Aperture/2, 
                                                   T1Side/2 - T1Aperture/2,  
                                                   T1PosZ + T1D1Thick +T1Dl + T1D2Thick/2), "X");
  // ----- T1.2--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3( T1Side/2 - T1Aperture/2, 
                                                  -T1Side/2 - T1Aperture/2,  
                                                   T1PosZ + T1D1Thick/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3( T1Side/2 - T1Aperture/2, 
                                                  -T1Side/2 - T1Aperture/2,  
                                                   T1PosZ + T1D1Thick +T1Dl + T1D2Thick/2), "X");
  // ----- T1.3 -------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3(-T1Side/2 - T1Aperture/2, 
                                                  -T1Side/2 + T1Aperture/2,  
                                                   T1PosZ + T1D1Thick/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3(-T1Side/2 - T1Aperture/2, 
                                                  -T1Side/2 + T1Aperture/2,  
                                                   T1PosZ + T1D1Thick +T1Dl + T1D2Thick/2), "X");
  // ----- T1.4--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3(-T1Side/2 + T1Aperture/2, 
                                                   T1Side/2 + T1Aperture/2,  
                                                   T1PosZ + T1D1Thick/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3(-T1Side/2 + T1Aperture/2, 
                                                   T1Side/2 + T1Aperture/2,  
                                                   T1PosZ + T1D1Thick +T1Dl + T1D2Thick/2), "X");

  // ----- D1 parameters ----------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_D1", TVector3( 0, 
                                                  0,  
                                                  D1PosZ + D1Thick/2), "X");

  // ------QTelescope -------------------------------------------------------
  Int_t verbose = 1;
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);

  // -----  BeamDet Setup ---------------------------------------------------
  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);

  // -----  BeamDet parameters ----------------------------------------------
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC);
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC - BeamDetLMWPC);
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);
  // //setupBeamDet->SetSensitiveTarget();

  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);
  // -----   Create PrimaryGenerator   --------------------------------------

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 40.;
  // Int_t     Z = 1, A = 3, Q = 1;
  // TString   ionName = "3H";
  Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "6He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  generator->SetThetaRange(0., 0.);
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0, 0, -beamStartPosition);

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);
  // ------- Decayer --------------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERDecayEXP1803* targetDecay = new ERDecayEXP1803();
  targetDecay->SetTargetVolumeName("tubeH2");
  targetDecay->SetTargetThickness(targetH2Thickness);
  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);

  // ------- Magnetic field -------------------------------------------------
  ERFieldMap* magField = new ERFieldMap("testField","A"); //exp1803Field, testField
  run->SetField(magField, "magnet");
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
    
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;
  //gMC->SetMaxNStep(nSteps);
    
  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  
  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
}
