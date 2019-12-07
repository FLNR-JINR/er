void expCalib_sim (Int_t nEvents = 1) {
  //---------------------Files-----------------------------------------------
  TString outFile = "sim_digi_Calib.root";
  TString parFile = "par_Calib.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = "../input/QTelescopeParts_postcalibEXP1904.xml";
  TString targetGeoFileName = workDirPath + "/geometry/siSource.geo.root";
  // -----   Timer   --------------------------------------------------------
  //
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

  FairModule* target = new ERTarget("Target", kTRUE, 1);
  target->SetGeometryFileName("siSource.geo.root");
  run->AddModule(target);
   
  Int_t verbose = 0;
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeTmp");

  Double_t w_thin = 5.;
  Double_t w_thick = 6;
  Double_t l = 29;

  // ----- SSD_1 parameters --------------------------------------------------
  ERGeoSubAssembly* telescope1 = new ERGeoSubAssembly("Telescope_1", TVector3(-3*w_thick/4, w_thick / 4, l), TVector3());
  ERQTelescopeGeoComponentSingleSi* thin_1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_1", "X");
  ERQTelescopeGeoComponentSingleSi* thick_1 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_1", "Y");

  telescope1->AddComponent(thin_1, TVector3((w_thick - w_thin) / 2, -(w_thick - w_thin) / 2, 0), TVector3(0, 0, 0));
  telescope1->AddComponent(thick_1, TVector3(0, 0, 1.5), TVector3(0, 0, 0));
  // ----- SSD_1 parameters --------------------------------------------------
  ERGeoSubAssembly* telescope2 = new ERGeoSubAssembly("Telescope_2", TVector3(-w_thick / 4, -3*w_thick/4, l), TVector3());
  ERQTelescopeGeoComponentSingleSi* thin_2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_2", "Y");
  ERQTelescopeGeoComponentSingleSi* thick_2 = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD_2", "X");

  telescope2->AddComponent(thin_2, TVector3((w_thick - w_thin) / 2, (w_thick - w_thin) / 2, 0), TVector3(0, 0, 0));
  telescope2->AddComponent(thick_2, TVector3(0, 0, 1.5), TVector3(0, 0, 0));

  setupQTelescope->AddSubAssembly(telescope1);
  setupQTelescope->AddSubAssembly(telescope2);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, verbose);
  run->AddModule(qtelescope);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  Double_t  kinE_MevPerNucleon = 40.;

  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double32_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  generator->SetKinE(kin_energy);
  generator->SetPSigmaOverP(0);
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();

  generator->SetThetaSigma(0, sigmaTheta);
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0., 0., -2.);
  generator->SpreadingOnTarget(); 

  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);
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

  run->CreateGeometryFile("geo_expCalib.root");

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
