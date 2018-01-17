void exp1803_sim(Int_t nEvents = 100) {
  // --------------- Telescope T1 -------------------------------------------
  Double_t T1Aperture = 50.;
  Double_t T1Dl = 0.5;
  Double_t T1PosZ = 30.;
  Double_t T1D1 = 0.002;
  Double_t T1D2 = 0.1;
  Double_t T1Side = 100.;
  // --------------- Telescope D1 -------------------------------------------
  Double_t D1PosZ = 40.;
  Double_t D1D = 0.03;
  // --------------- BeamDet ------------------------------------------------
  Double_t BeamDetLToF = 100.;
  Double_t BeamDetPosZToF = -300;
  Double_t BeamDetLMWPC = 80.;
  Double_t BeamDetPosZMWPC = -50;
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
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
  // ------------------------------------------------------------------------
  
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------   
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXmlParametersFile(paramFileQTelescope);
  // ----- T1 parameters ----------------------------------------------------
  // ----- T1.1--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3( T1Side/2 + T1Aperture/2, 
                                         T1Side/2 - T1Aperture/2,  
                                         T1PosZ + T1D1/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3( T1Side/2 + T1Aperture/2, 
                                         T1Side/2 - T1Aperture/2,  
                                         T1PosZ + T1D1 +T1Dl + T1D2/2), "X");
  // ----- T1.2--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3( T1Side/2 - T1Aperture/2, 
                                        -T1Side/2 - T1Aperture/2,  
                                         T1PosZ + T1D1/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3( T1Side/2 - T1Aperture/2, 
                                        -T1Side/2 - T1Aperture/2,  
                                         T1PosZ + T1D1 +T1Dl + T1D2/2), "X");
  // ----- T1.3 -------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3(-T1Side/2 - T1Aperture/2, 
                                        -T1Side/2 + T1Aperture/2,  
                                         T1PosZ + T1D1/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3(-T1Side/2 - T1Aperture/2, 
                                        -T1Side/2 + T1Aperture/2,  
                                         T1PosZ + T1D1 +T1Dl + T1D2/2), "X");
  // ----- T1.4--------------------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_SD1", TVector3(-T1Side/2 + T1Aperture/2, 
                                         T1Side/2 + T1Aperture/2,  
                                         T1PosZ + T1D1/2), "X");
  setupQTelescope->AddSi("DoubleSi_SD2", TVector3(-T1Side/2 + T1Aperture/2, 
                                         T1Side/2 + T1Aperture/2,  
                                         T1PosZ + T1D1 +T1Dl + T1D2/2), "X");
  // ----- D1 parameters ----------------------------------------------------
  setupQTelescope->AddSi("DoubleSi_D1", TVector3( 0, 
                                                  0,  
                                                  D1PosZ + D1D/2), "X");
  // ------QTelescope -------------------------------------------------------
  Int_t verbose = 1;
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);
  // ------------------------------------------------------------------------
  // -----  BeamDet Setup ---------------------------------------------------
  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);
  // -----  BeamDet parameters ----------------------------------------------
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC);
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC - BeamDetLMWPC);
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);
  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,verbose);
  run->AddModule(beamdet);
  // -----   Create PrimaryGenerator   --------------------------------------
  /*FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2212; // proton  beam
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.;
  Double32_t kin_energy = .5; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.* kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta1);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 0);
  boxGen->SetBoxXYZ(0.,0.,0.,0.,-1200.);

  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);*/
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonMixGenerator* generator = new ERIonMixGenerator("6He", 2, 6, 2, 1);
  Double32_t kin_energy = 40 * 1e-3 * 28; //GeV
  generator->SetPSigma(5., 5.*0.003);
  generator->SetThetaRange(0., 0.);
  generator->SetPhiRange(0, 360);
  Double32_t distanceToTarget = 1200;
  generator->SetBoxXYZ(0, 0, 0, 0, -distanceToTarget);

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

  // ------- Decayer --------------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERDecayEXP1803* targetDecay = new ERDecayEXP1803();
  targetDecay->SetTargetVolumeName("targetH2");
  targetDecay->SetTargetThickness(0.4);
  decayer->AddDecay(targetDecay);
  run->SetDecayer(decayer);
//-------------------------------------------------------------------------
  // ------- Magnetic field -------------------------------------------------
  /*ERFieldMap* magField = new ERFieldMap("testField","A");
  run->SetField(magField);*/
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
  // ---------------------------------------------------------
  
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
