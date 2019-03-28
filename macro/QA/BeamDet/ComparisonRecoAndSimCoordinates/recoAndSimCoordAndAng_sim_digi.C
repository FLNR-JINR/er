void recoAndSimCoordAndAng_sim_digi(Int_t nEvents = 10000) {
  // --------------- BeamDet ------------------------------------------------
  Double_t BeamDetLToF = 1500.;     // [cm] 
  Double_t BeamDetPosZToF = -50;    // [cm] 
  Double_t BeamDetLMWPC = 32.;      // [cm]
  Double_t BeamDetPosZMWPC = -8;    // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -1600;  // [cm]
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi.root";
  TString parFile= "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
  **/
  run->SetName("TGeant4");              // Transport engine
  run->SetOutputFile(outFile.Data());          // Output file
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
   //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* eventHeader = new ERMCEventHeader();
  run->SetMCEventHeader(eventHeader);
 // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // -----   Create detectors  ----------------------------------------------   
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
  // -----  BeamDet setup class ----------------------------------------------
  ERBeamDetSetup* setupBeamDet = ERBeamDetSetup::Instance();
  setupBeamDet->SetXmlParametersFile(paramFileBeamDet);
  // -----  BeamDet parameters ----------------------------------------------
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF - BeamDetLToF);       // 
  setupBeamDet->AddToF("ToF1", BeamDetPosZToF);                     //  BeamDet parts should be added in ascending order   
  setupBeamDet->AddMWPC("MWPC1", BeamDetPosZMWPC - BeamDetLMWPC);   //  of a Z-coordinate of a part.
  setupBeamDet->AddMWPC("MWPC2", BeamDetPosZMWPC);                  // 
  setupBeamDet->SetSensitiveTarget();
  // ------BeamDet ----------------------------------------------------------
  ERBeamDet* beamdet= new ERBeamDet("ERBeamDet", kTRUE,1);
  run->AddModule(beamdet);
  // ------BeamDet digitizer-------------------------------------------------
  ERBeamDetDigitizer* beamDetDigitizer = new ERBeamDetDigitizer(1);
  // beamDetDigitizer->SetMWPCElossThreshold(0.8*1e-6);
  run->AddTask(beamDetDigitizer);
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Double_t  kinE_MevPerNucleon = 40.;
  Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "6He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaRange(0, sigmaTheta);
  generator->SetPhiRange(0., 360.);
  generator->SetBoxXYZ(-2, -2, 2, 2, beamStartPosition);
  generator->SpreadingOnTarget();

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("ERROR");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");  
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;
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
