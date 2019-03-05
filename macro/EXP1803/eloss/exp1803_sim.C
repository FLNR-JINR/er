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
  Double_t BeamDetLToF = 1500.;     // [cm] 
  Double_t BeamDetPosZToF = -50;    // [cm] 
  Double_t BeamDetLMWPC = 32.;      // [cm]
  Double_t BeamDetPosZMWPC = -8;    // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetH2Thickness = 0.4;    // [cm] this parameter should coincide with target H2 thickness in /macro/geo/create_GadastEXP1803_geo.C
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

  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);

  // -----  BeamDet parameters ----------------------------------------------
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);       // 
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);                     //  BeamDet parts should be added in ascending order   
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC - BeamDetLMWPC);   //  of Z-coordinate of part.
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC);                  // 
 // //setupBeamDet->SetSensitiveTarget();

  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,1);
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
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(0, 0);
 // generator->SetThetaRange(0., 5.);
 // generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0, 0, beamStartPosition);
  generator->SpreadingOnTarget();

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);
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
