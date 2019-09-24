void nonUniformCalibRun_sim (Int_t nEvents = 1) {
  //---------------------Files-----------------------------------------------
  TString outFile = "sim_digi.root";
  TString parFile = "par.root";
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts2.xml";
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

  FairModule* target = new ERTarget("Target", kTRUE,1);
  // target->SetGeometryFileName("target.SiSource.geo.root");
  // target->SetGeometryFileName("target_CD2_geo.root");
  // run->AddModule(target);
   
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
  ERQTelescopeGeoComponentSingleSi* thin_Left = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_L", 
                                                                                  TVector3(0., 0., -1.4), TVector3(), "X");
  ERQTelescopeGeoComponentDoubleSi* thick_Left = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_L", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  // ERQTelescopeGeoNonUniformSingleSi* thin_Left = new ERQTelescopeGeoNonUniformSingleSi("SingleSi", "SingleSi_SSD20_L",
  //                                                                              "DoubleSi", "DoubleSi_DSD_L", "Y",
  //                                                                              "./input/map_sens.root",
  //                                                                              "./input/map_deadFront.root",
  //                                                                              "./input/map_deadBack.root"); 
  assembly_Left->AddComponent(thick_Left);
  assembly_Left->AddComponent(thick_Left, TVector3(0, 0, -1.4), TVector3(70, 45, 60));
  // assembly_Left->AddComponent(thin_Left, TVector3(0, 0, -1.4), TVector3(0, 45, 45));

  setupQTelescope->AddSubAssembly(assembly_Left);

  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, verbose);
  run->AddModule(qtelescope);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t Z = 2, A = 4, Q = 2;
  TString ionName = "4He"; 
//  
  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double_t kin_energy = 6.0024 * 1e-3; //GeV
  // Double_t kin_energy = 7.6869 * 1e-3; //GeV
  generator->SetKinERange(kin_energy, kin_energy);
  Double_t fronalCathesis = (5.8 / 2);
  Double_t maxTheta = abs(atan(fronalCathesis / zPos));
//  generator->SetThetaRange(0, maxTheta*TMath::RadToDeg());
  generator->SetThetaRange(0,0);
  generator->SetCosTheta();
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(-0., -0., 0., 0., 0.);
  primGen->AddGenerator(generator);


  // ERIonMixGenerator* generator_high = new ERIonMixGenerator(ionName + "_high", Z, A, Q, 1);
  // kin_energy = 7.6869 * 1e-3 * A; //GeV
  // generator_high->SetKinERange(kin_energy, kin_energy);
  // fronalCathesis = (5. / 2);
  // maxTheta = abs(atan(fronalCathesis / zPos));
  // generator_high->SetThetaRange(0, maxTheta*TMath::RadToDeg());
  // generator_high->SetCosTheta();
  // generator_high->SetPhiRange(0, 360);
  // generator_high->SetRoundXY(0., 0., 0., 0.);
  // primGen->AddGenerator(generator_high);


  run->SetGenerator(primGen);
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(20e-6);
  // qtelescopeDigitizer->SetSiElossSigma(0.);
  // qtelescopeDigitizer->SetSiElossSigma(20e-3);
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

  // run->CreateGeometryFile("geo_exp.root");

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
