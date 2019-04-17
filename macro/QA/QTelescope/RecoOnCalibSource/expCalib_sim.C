void expCalib_sim (Int_t nEvents = 0) {
  //---------------------Files-----------------------------------------------
  TString outFile = "sim_digi_Calib.root";
  TString parFile = "par_Calib.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts_exp1810.xml";
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
  //-------- Set MC event header --------------------------------------------
  ERDecayMCEventHeader* decayMCheader = new ERDecayMCEventHeader();
  run->SetMCEventHeader(decayMCheader);
  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");       // Materials
  // -----   Create detectors  ----------------------------------------------   
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
   
  Int_t verbose = 0;
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  // ----- LEFT parameters ----------------------------------------------------
  Double_t radius = 35.;
  
  TVector3 rotationL(0., 0., 0.);
  Double_t xPos = radius * TMath::Sin(rotationL.Y() * TMath::DegToRad());
  Double_t yPos = 0.;
  Double_t zPos = radius * TMath::Cos(rotationL.Y() * TMath::DegToRad());
  ERGeoSubAssembly* assembly_Left = new ERGeoSubAssembly("Left_telescope", TVector3(xPos, yPos, zPos), rotationL);
  // ERQTelescopeGeoComponentSingleSi* thin_Left = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_L", 
  //                                                                                 TVector3(0., 0., -1.4), TVector3(), "X");
  ERQTelescopeGeoComponentDoubleSi* thick_Left = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_L", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  ERQTelescopeGeoPseudoDoubleSi* thin_Left = new ERQTelescopeGeoPseudoDoubleSi("SingleSi", "SingleSi_SSD20_L",
                                                                               "DoubleSi", "DoubleSi_DSD_L",
                                                                               "./input/map_sens.root",
                                                                               "./input/map_deadFront.root",
                                                                               "./input/map_deadBack.root"); 

  assembly_Left->AddComponent(thick_Left);
  assembly_Left->AddComponent(thin_Left, TVector3(0, 0, -1.4), TVector3(0, 0, 0));

  setupQTelescope->AddSubAssembly(assembly_Left);

  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, verbose);
  run->AddModule(qtelescope);

  // FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  // Double_t  kinE_MevPerNucleon = 40.;

  // Int_t Z = 2, A = 8, Q = 2;
  // TString ionName = "8He";
  // ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  // Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  // generator->SetKinE(kin_energy);
  // generator->SetPSigmaOverP(0);
  // Double32_t sigmaTheta = 0.004*TMath::RadToDeg();

  // generator->SetThetaSigma(0, sigmaTheta);
  // generator->SetPhiRange(0, 360);
  // generator->SetBoxXYZ(0, 0, 0., 0., beamStartPosition);
  // generator->SpreadingOnTarget(); 

  // primGen->AddGenerator(generator);
  // run->SetGenerator(primGen);
  //-------Set visualisation flag to true------------------------------------
  // run->SetStoreTraj(kTRUE);

  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);

  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");

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

  //run->CreateGeometryFile("setup_expCalib.root");

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
}

 
 
