void sim(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
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

  ERCollimator* collimator = new ERCollimator();
  collimator->SetGeometryFileName("N15.collimator.root");
  run->AddModule(collimator);

  FairModule* target = new ERTarget("Target", kTRUE,1);
  target->SetGeometryFileName("N15.target.root");
  run->AddModule(target);

  Int_t verbose = 0;

  ERDetector* detector= new ERDetector("TestDetector1", kTRUE,verbose);
  detector->SetGeometryFileName("N15.base.geo_1.root");
  detector->AddSensetive("gas1");
  detector->AddSensetive("plastic1");
  run->AddModule(detector);
  
  //------    ER Decayer   -------------------------------------------------
  //Ion 15N
  Int_t A = 15;
  Int_t Z = 7;
  Int_t Q = 3;

  ERDecayer* decayer = new ERDecayer();
  ERElasticScattering* scattering = new ERElasticScattering("15Nto15N11B");
  
  scattering->SetInputIon(Z,A,Q);
  scattering->SetTargetIon(5,11,5);

  scattering->SetThetaCDF("cos_tetta_cross.txt");

  scattering->SetUniformPos(-0.00035,0.00035);
  scattering->SetStep(0.00001); //0.1 micron
  scattering->SetDecayVolume("targetB11");

  scattering->SetThetaRange(23, 25);
  scattering->SetPhiRange(0, 0);

  decayer->AddDecay(scattering);
  run->SetDecayer(decayer);
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
  Double32_t kin_energy = 0.043; //GeV
  //generator->SetKinESigma(kin_energy, 0);
  //generator->SetPSigma(6.7835, 6.7835*0.003);
  generator->SetKinESigma(kin_energy, 0);

  generator->SpreadingOnTarget();

  Double32_t theta = 0;
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(theta, 0);
  
  generator->SetPhiRange(0, 360);

  Double32_t distanceToTarget = 200;
  Double32_t sigmaOnTarget = 0.;
  generator->SetSigmaXYZ(0, 0, -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  generator->SetBoxXYZ(0,0,0,0, -distanceToTarget);

  //generator->AddBackgroundIon("26P", 15, 26, 15, 0.25);
  //generator->AddBackgroundIon("26S", 16, 26, 16, 0.25);
  //generator->AddBackgroundIon("24Si", 14, 24, 14, 0.25);

  primGen->AddGenerator(generator);
 /*
  Int_t pdgId = 2212; // 
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.;
  //Double32_t momentum = 0.05; //GeV
  Double32_t kin_energy = .05; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV

  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0,360);
  boxGen->SetXYZ(0.,0., -100.);

  primGen->AddGenerator(boxGen);
  */  
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  // —– Initialize simulation run ———————————— 
  run->Init();
  Int_t nSteps = -15000;

  // —– Runtime database ——————————————— 
  Bool_t kParameterMerged = kTRUE; 
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data()); 
  rtdb->setOutput(parOut); 
  rtdb->saveOutput(); 
  rtdb->print(); // 
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
}
