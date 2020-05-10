void simAlphaElossSiLayer_digi_dir(Double_t kinE_MevPerNucleon,
                          TString detDbId, 
                          Double_t rotAngle = 0, 
                          Double_t sourcePos= -39,
                          Double_t xShift = 0,
                          Int_t nEvents = 1000) 
{
  // --------------- Beam initials ------------------------------------
  Double_t detSideSize = 5.; // detector side size from the db-file
  Double_t sourceCloudDiam = 0.5; //  scattering of a particle start position in the X-Y plane
  //---------------------Files-----------------------------------------------
  TString outFile;
  TString parFile;
  outFile.Form("simAlpha%dener%s.root", (int)(kinE_MevPerNucleon*4e3), 
                                                detDbId.Data());
  parFile.Form("simAlpha%dener%s_par.root", (int)(kinE_MevPerNucleon*4e3), 
                                                   detDbId.Data());
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = "./inputs/QTelescopeDB.xml";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer; 
  timer.Start();
  // ------------------------------------------------------------------------
  // -----   Create simulation run   ----------------------------------------
  ERRunSim* run = new ERRunSim();
  // FairRun* run = new ERRunSim();  
  FairRun *runBase = run;
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
  setupQTelescope->SetGeoName("QTelescopeCalib");
  // ----- CalibSi parameters ----------------------------------------------------
  ERGeoSubAssembly* telescpoeAss = new ERGeoSubAssembly("SiDet");
  ERQTelescopeGeoComponentSingleSi* Q_L = new ERQTelescopeGeoComponentSingleSi("SingleSi", 
                                                                               detDbId, 
                                                                               "X");
  telescpoeAss->AddComponent(Q_L, TVector3(0., 0., 0.), TVector3());
  
  Double_t xPos, yPos, zPos;
  Double_t radius = 0.;
  TVector3 rotation1mm(0., rotAngle, 0.);
  xPos = radius * TMath::Sin(rotation1mm.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotation1mm.Y() * TMath::DegToRad());
  setupQTelescope->AddSubAssembly(telescpoeAss, TVector3(xPos, yPos, zPos), rotation1mm);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE, verbose);
  run->AddModule(qtelescope);
  // ------QTelescope digitizer----------------------------------------------
  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);
  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t Z = 2, A = 4, Q = 2;
  TString ionName = "Alpha"; 

  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  // Double_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetKinERange(kin_energy, kin_energy);
  // Double_t fronalCathesis = (detSideSize / 2) * sqrt(2/*1 + pow(cos(abs(rotAngle * TMath::DegToRad())),2)*/) + (sqrt(2) * (sourceCloudDiam + abs(xShift)) / 2);
  Double_t fronalCathesis = (detSideSize / 2);
  Double_t maxTheta = abs(atan(sqrt(2)*fronalCathesis / sourcePos));
  // generator->SetThetaRange(0, maxTheta*TMath::RadToDeg());
  generator->SetThetaRange(0, 0);
  generator->SetCosTheta();
  generator->SetPhiRange(0, 360);
  // generator->SetBoxXYZ(-sourceCloudDiam/2 + xShift, -sourceCloudDiam/2, sourceCloudDiam/2 + xShift, sourceCloudDiam/2, sourcePos);
  // double centerX = 2.72; //[mm]
  // double centerY = 0.33; //[mm]
  // generator->SetBoxXYZ(0.272 - 0.3, 0.033 - 0.3, 0.272 + 0.3, 0.033 + 0.3, sourcePos);
  // generator->SetRoundXY (0.6, 0.272, 0.033, sourcePos);
  // generator->SetRoundXY (0.6, 0.272, 0.033, sourcePos);
  generator->SetRoundXY (0, 0, 0, sourcePos);
  primGen->AddGenerator(generator);

  run->SetGenerator(primGen);
  //-------Set visualisation flag to true------------------------------------
  // run->SetStoreTraj(kTRUE);
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
  //gMC->SetMaxNStep(nSteps);
  run->CreateGeometryFile("setup.root");
  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
  return 0;
}