void expCalib_sim (Int_t nEvents = 100) {
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
  ERQTelescopeGeoComponentSingleSi* thin_Left = new ERQTelescopeGeoComponentSingleSi("SingleSi", "SingleSi_SSD20_L", 
                                                                                  TVector3(0., 0., -1.4), TVector3(), "X");
  ERQTelescopeGeoComponentDoubleSi* thick_Left = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", "DoubleSi_DSD_L", 
                                                                                  TVector3(0., 0., 0.), TVector3(), "X");
  assembly_Left->AddComponent(thin_Left);
  assembly_Left->AddComponent(thick_Left);

  setupQTelescope->AddSubAssembly(assembly_Left);

  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t Z = 2, A = 4, Q = 2;
  TString ionName = "4He"; 

  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  // Double_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetKinERange(kin_energy, kin_energy);
  // Double_t fronalCathesis = (detSideSize / 2) * sqrt(2/*1 + pow(cos(abs(rotAngle * TMath::DegToRad())),2)*/) + (sqrt(2) * (sourceCloudDiam + abs(xShift)) / 2);
  Double_t fronalCathesis = (detSideSize / 2);
  Double_t maxTheta = abs(atan(fronalCathesis / sourcePos));
  generator->SetThetaRange(0, 0./*maxTheta*TMath::RadToDeg()*/);
  generator->SetCosTheta();
  generator->SetPhiRange(0, 360);
  // generator->SetBoxXYZ(-sourceCloudDiam/2 + xShift, -sourceCloudDiam/2, sourceCloudDiam/2 + xShift, sourceCloudDiam/2, sourcePos);
  // generator->SetBoxXYZ(0., 0., 0., 0., sourcePos);
  generator->SetRoundXY(0.6, 2.72, 0.33, sourcePos);
  primGen->AddGenerator(generator);

  run->SetGenerator(primGen);
  //-------Set visualisation flag to true------------------------------------
  // run->SetStoreTraj(kTRUE);

  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(20*1e-3);
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

 
 
