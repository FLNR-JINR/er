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

  FairDetector* detector = new ERN15B11Detector("N15B11detector", kTRUE);
  detector->SetGeometryFileName("N15B11_detector.geo.root");
  run->AddModule(detector);

  //------    ER Decayer   -------------------------------------------------
  //Ion 15N
  Int_t A = 15;
  Int_t Z = 7;
  Int_t Q = 3;

  ERDecayer* decayer = new ERDecayer();
  ERN15B11ElasticScattering* scattering = new ERN15B11ElasticScattering("15Nto15N11B");

  scattering->SetInputIon(Z,A,Q);
  scattering->SetTargetIon(5,11,5);
  scattering->SetThetaCDF("cos_tetta_cross.txt");
  scattering->SetUniformPos(-0.00035,0.00035);
  scattering->SetStep(0.00001); //0.1 micron
  scattering->SetDecayVolume("targetB11");
  scattering->SetDetAngle(angle); // argumetn is an angle of detector position in Lab
  //scattering->SetThetaRange(18.4, 19.4);
  //scattering->SetPhiRange(0., 0.);

  decayer->AddDecay(scattering);
  run->SetDecayer(decayer);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
  //Double32_t kin_energy = 0.043; // GeV
  //generator->SetPSigma(6.7835, 6.7835*0.003);
  //generator->SetKinESigma(kin_energy, 0.);
  generator->SetKinERange(0.0427094, 0.0436017);
  //generator->SpreadingOnTarget();

  //Double32_t theta = 0.;
  //Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  //generator->SetThetaSigma(0., 0.);

  generator->SetThetaRange(-2., 2.); // -2 : 2
  generator->SetPhiRange(0., 360.); // 0 : 360

  Double32_t distanceToTarget = 50.;
  Double32_t sigmaOnTarget = 0.;
  //generator->SetSigmaXYZ(0., 0., -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  generator->SetBoxXYZ(0.,0., 0.,0., -distanceToTarget); // Xmin, Xmax, Ymin, Ymax, Z

  //generator->AddBackgroundIon("26P", 15, 26, 15, 0.25);
  //generator->AddBackgroundIon("26S", 16, 26, 16, 0.25);
  //generator->AddBackgroundIon("24Si", 14, 24, 14, 0.25);

  primGen->AddGenerator(generator);

  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

  //------- Initialize simulation run ---------------------------------------
  run->Init();
  Int_t nSteps = -15000;

  //--- Runtime database ----------------------------------------------------
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
  // cout << "Energy " << momentum << "; mass " << mass << endl;
  cout << "Interactions number in target: " << scattering->GetInteractNumInTarget() << endl;
  cout << "dPhi range: " << scattering->GetdPhi() << endl;
}
