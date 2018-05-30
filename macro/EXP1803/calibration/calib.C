void calib(TString runId, 
           Double_t kinE_MevPerNucleon, 
           Double_t rotAngle = 0, 
           Double_t sourcePos= -39, 
           Int_t nEvents = 1000) 
{
  // --------------- Beam start position ------------------------------------
  // Double_t sourcePos= -39;  // [cm]
  Double_t deadFrontThicknessMax = 0.0004; // [cm]
  Double_t detectorThickness = 0.1; // [cm]
  Double_t deadLayerFragments = 10;
  Double_t detSideSize = 6.4;
  Double_t sourceCoverThck = 0.00003;
  Double_t sourceCloudDiam = 0.3;
  //---------------------Files-----------------------------------------------
  TString outFile;
  TString parFile;
  outFile.Form("sim_digi_calib_%s.root", runId.Data());
  parFile.Form("par_calib_%s.root", runId.Data());
  TString workDirPath = gSystem->Getenv("VMCWORKDIR");
  TString paramFileQTelescope = workDirPath
                         + "/db/QTelescope/QTelescopeParts.xml";
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
  Double_t detectorSensVolThickness = detectorThickness - deadFrontThicknessMax;
  Double_t deadLayerFragmentsThickness = deadFrontThicknessMax / deadLayerFragments;
  ERQTelescopeSetup* setupQTelescope = ERQTelescopeSetup::Instance();
  setupQTelescope->SetXMLParametersFile(paramFileQTelescope);
  setupQTelescope->SetGeoName("QTelescopeCalib");
  // ----- CalibSi1 parameters ----------------------------------------------------
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
  for (int i = 0; i < deadLayerFragments; i++) {
    deadQ_L.push_back(new ERQTelescopeGeoComponentDoubleSi("DoubleSi", 
                                                           "DoubleSi_SD2_calib_dead", 
                                                           "X"));
  }

  int j = 0;
  for (auto itDead : deadQ_L) {
    Double_t offsetZ = -(detectorThickness/2) 
                     + j * deadLayerFragmentsThickness + deadLayerFragmentsThickness/2;
    telescpoeAss->AddComponent(itDead, TVector3(0., 0., offsetZ), TVector3());
    j++;
    cout << "deadLayerFragments offset " << offsetZ << "; n = " << j << endl;
  }
  sourceAss->AddComponent(alphaSourceCover, TVector3(0., 0., 0.), TVector3());
  telescpoeAss->AddComponent(Q_L, TVector3(0., 0., deadFrontThicknessMax), TVector3());

  Double_t xPos, yPos, zPos;
  Double_t radius = 0.;
  TVector3 rotation1mm(0., rotAngle, 0.);
  xPos = radius * TMath::Sin(rotation1mm.Y() * TMath::DegToRad());
  yPos = 0.;
  zPos = radius * TMath::Cos(rotation1mm.Y() * TMath::DegToRad());
  setupQTelescope->AddSubAssembly(telescpoeAss, TVector3(xPos, yPos, zPos), rotation1mm);
  setupQTelescope->AddSubAssembly(sourceAss, TVector3(0., 0., sourcePos + sourceCoverThck/2), TVector3());
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
  Double_t fronalCathesis = (detSideSize / 2) * sqrt(1 + pow(cos(abs(rotAngle * TMath::DegToRad())),2)) + (sqrt(2) * sourceCloudDiam / 2);
  Double_t maxTheta = abs(atan(fronalCathesis / sourcePos));
  cout << "Max theta " << maxTheta << endl;
  generator->SetThetaRange(0, maxTheta * TMath::RadToDeg());
  generator->SetCosTheta();
  generator->SetPhiRange(0, 360);
  generator->SetBoxXYZ(-sourceCloudDiam/2, -sourceCloudDiam/2, sourceCloudDiam/2, sourceCloudDiam/2, sourcePos);
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
  cout << "Output file is sim.root" << endl;
  cout << "Parameter file is par.root" << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
          << "s" << endl << endl;
}