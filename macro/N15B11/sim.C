void sim(Int_t nEvents = 100, Int_t index = 0, TString outDir="output", Double_t angle = 20.)
{
  gRandom->SetSeed(index);

  //---------------------Files-----------------------------------------------
  TString outFile;
  outFile.Form("%s/sim_%d.root", outDir.Data(), index);
  TString parFile;
  parFile.Form("%s/par_%d.root", outDir.Data(), index);
  // ------------------------------------------------------------------------

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

  // ------------------------------------------------------------------------
  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------

  // -----   Create media   -------------------------------------------------
  run->SetMaterials("N15.media.geo");       // Materials
  // ------------------------------------------------------------------------

  //-------- Set MC event header --------------------------------------------
  ERDecayMCEventHeader* header = new ERDecayMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  FairModule* collimator = new ERCollimator(); // "N15B11_collimator", "N15B11_collimator"
  collimator->SetGeometryFileName("N15.collimator.root");
  run->AddModule(collimator);

  ERDetector* target = new ERTarget("N15B11_target", kTRUE, 1);
  target->SetGeometryFileName("N15.target.root");
  run->AddModule(target);

  FairDetector* detector = new ERN15B11Detector("N15B11detector", kTRUE, 1);
  detector->SetGeometryFileName("N15B11_detector.geo.root");
  run->AddModule(detector);

  //------    ER Decayer   -------------------------------------------------
  //Ion 15N
  Int_t A = 15;
  Int_t Z = 7;
  Int_t Q = 3;

  ERDecayer* decayer = new ERDecayer();
  ERElasticScattering* scattering = new ERElasticScattering("15Nto15N11B");

  scattering->SetInputIon(Z,A,Q);
  scattering->SetTargetIon(5, 11, 5);
  scattering->SetThetaCDF("cos_tetta_cross.txt");
  scattering->SetUniformPos(-0.00035,0.00035);
  scattering->SetStep(0.00001); //0.1 micron
  scattering->SetDecayVolume("cave"); //targetB11
  scattering->SetDetectorsSlot(angle, 4.*0.262822833);
  scattering->SetPhiRange(-20., 20.);

  decayer->AddDecay(scattering);
  run->SetDecayer(decayer);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
  generator->SetKinERange(0.0427094, 0.0436017); // 0.0427094 : 0.0436017

  Double32_t theta = 0.;
  Double32_t sigmaTheta = 5e-3*TMath::RadToDeg();
  generator->SetThetaSigma(theta, sigmaTheta); // theta = 0., sigma = 5 mrad

  //generator->SetThetaRange(0., 0.); // -2 : 2
  generator->SetPhiRange(0., 180.); // 0 : 180

  Double32_t distanceToTarget = 50.; // work: 50 cm, test 0.5 micron: 0.00005+0.00035
  generator->SetBoxXYZ(-0.5, -0.5, 0.5, 0.5, -distanceToTarget); // Xmin = -0.5, Ymin = -0.5, Xmax = 0.5, , Ymax = 0.5, Z

  primGen->AddGenerator(generator);

  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE); // kFALSE

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

  //------- Initialize simulation run ---------------------------------------
  run->Init();
  Int_t nSteps = -15000;

  //--- Runtime database ----------------------------------------------------
  Bool_t kParameterMerged = kTRUE;    /** @brief Returns curent theta in CM for Primary Ion. **/
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
  // cout << "Energy " << momentum << "; mass " << mass << endl;
  cout << "Interactions number in target: " << scattering->GetInteractNumInTarget() << endl;
  cout << "dPhi range: " << scattering->GetdPhi() << endl;
  cout << "ThetaCM Mean for N15: " << scattering->GetThetaCMMeanPri() << endl;
  cout << "ThetaCM Mean for B11: " << scattering->GetThetaCMMeanTar() << endl;
  cout.precision(12);
  cout << "summ: "<< scattering->GetCDFRangesSum() << endl;
}
