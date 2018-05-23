void calib_first(Int_t runId, Double_t kinE_MevPerNucleon) {
  Int_t nEvents = 1000;

  // --------------- Telescope T1 -------------------------------------------
  Double_t T1Dl = 0.5;         // [cm]      
  Double_t T1PosZ = 10.;       // [cm] 
  Double_t T1D1Thick = 0.002;  // [cm]  
  Double_t T1D2Thick = 0.1;    // [cm] 
  Double_t T1Side = 6.2;       // [cm] coincides with SD1 side length from /db/QTelescope/QTelescopeParts.xml
  Double_t T1Aperture = 3.1;   // [cm]  
  // --------------- Telescope D1 -------------------------------------------
  Double_t D1PosZ = 20.;       // [cm]
  Double_t D1Thick = 0.03;     // [cm]
  // --------------- BeamDet ------------------------------------------------
  Double_t BeamDetLToF = 1500.;     // [cm] 
  Double_t BeamDetPosZToF = -50;  // [cm] 
  Double_t BeamDetLMWPC = 32.;     // [cm]
  Double_t BeamDetPosZMWPC = -8;  // [cm]  
  // --------------- Beam start position ------------------------------------
  Double_t beamStartPosition = -40.1;  // [cm]
  // --------------- Target -------------------------------------------------
  Double_t targetH2Thickness = 0.4;  // [cm] coincides with target thickness in /macro/geo/create..._geo.C
  //---------------------Files-----------------------------------------------
  TString outFile;
  TString parFile;
  outFile.Form("sim_digi%d.root", runId);
  parFile.Form("par%d.root", runId);
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
  TString paramFileBeamDet = workDirPath
                         + "/db/BeamDet/BeamDetParts.xml";
  TString targetGeoFileName = workDirPath + "/geometry/target_CD2_geo.root";
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
  // -----  BeamDet Setup ---------------------------------------------------
  // -----  QTelescope Setup ------------------------------------------------
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeCalib");
  // ----- CalibSi1 parameters ----------------------------------------------------
  Double_t xPos, yPos, zPos;
  Double_t radius = 0.;
  TVector3 rotationT1(0., 0., 0.);
  ERGeoSubAssembly* telescpoeAss = new ERGeoSubAssembly("SiDet");
  ERGeoSubAssembly* sourceAss = new ERGeoSubAssembly("Source");
  ERQTelescopeGeoComponentDoubleSi* Q_L = new ERQTelescopeGeoComponentDoubleSi("DoubleSi", 
                                                                               "DoubleSi_SD2_calib", 
                                                                               "X");
  vector<ERQTelescopeGeoComponentDoubleSi*> deadQ_L;
  ERQTelescopeGeoComponentSingleSi* alphaSourceCover;
  alphaSourceCover = new ERQTelescopeGeoComponentSingleSi("SingleSi", 
                                                          "SingleSi_calib", 
                                                          "X");
  for (int i = 0; i < 10; i++) {
    deadQ_L.push_back(new ERQTelescopeGeoComponentDoubleSi("DoubleSi", 
                                                           "DoubleSi_SD2_calib_dead", 
                                                           "X"));
  }

  int j = 1;
  for (auto itDead : deadQ_L) {
    telescpoeAss->AddComponent(itDead, TVector3(0., 0., - (0.9996/2) - j * 0.00004 + 0.00002), TVector3());
    j++;
  }
  sourceAss->AddComponent(alphaSourceCover, TVector3(0., 0., 0.), TVector3());
  telescpoeAss->AddComponent(Q_L, TVector3(0., 0., 0.0002), TVector3());

  xPos = radius * TMath::Sin(rotationT1.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotationT1.Y() * TMath::DegToRad());
  setupQTelescope->AddSubAssembly(telescpoeAss, TVector3(xPos, yPos, zPos), rotationT1);
  setupQTelescope->AddSubAssembly(sourceAss, TVector3(0., 0., -40), rotationT1);
  // ------QTelescope -------------------------------------------------------
  ERQTelescope* qtelescope= new ERQTelescope("ERQTelescope", kTRUE,verbose);
  run->AddModule(qtelescope);

  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  // Int_t     Z = 1, A = 3, Q = 1;
  // TString   ionName = "3H";
  Int_t Z = 2, A = 4, Q = 2;
  TString ionName = "4He"; 

  ERIonMixGenerator* generator = new ERIonMixGenerator(ionName, Z, A, Q, 1);
  Double_t kin_energy = kinE_MevPerNucleon * 1e-3 * A; //GeV
  // generator->SetKinE(kin_energy);
  // generator->SetPSigmaOverP(0);
  sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetKinERange(kin_energy, kin_energy);
  generator->SetThetaRange(0, 6.45483);
  generator->SetCosTheta();
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(0, 0, 0, 0, beamStartPosition);
  // generator->SpreadingOnTarget(); 

  primGen->AddGenerator(generator);

  run->SetGenerator(primGen);

  ERQTelescopeDigitizer* qtelescopeDigitizer = new ERQTelescopeDigitizer(verbose);
  qtelescopeDigitizer->SetSiElossThreshold(0);
  qtelescopeDigitizer->SetSiElossSigma(0);
  qtelescopeDigitizer->SetSiTimeSigma(0);

  qtelescopeDigitizer->SetCsIElossThreshold(0);
  qtelescopeDigitizer->SetCsIElossSigma(0);
  qtelescopeDigitizer->SetCsITimeSigma(0);
  run->AddTask(qtelescopeDigitizer);
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
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
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
}