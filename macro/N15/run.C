void run(Int_t nEvents = 1000)
{
  //---------------------Files--------------------------------------------------
  TString outFile= "run.root";
  TString parFile= "par.root";

  // -----   Timer   -----------------------------------------------------------
  TStopwatch timer;
  timer.Start(); 

  // -----   Create simulation run   -------------------------------------------
  ERRunSim* run = new ERRunSim();
  /** Select transport engine
  * TGeant3
  * TGeant4
  **/
  run->SetName("TGeant4");
  run->SetOutputFile(outFile.Data());

  // -----   Runtime database   ------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  // -----   Create media   ----------------------------------------------------
  run->SetMaterials("N15.media.geo");

  //-------- Set MC event header -----------------------------------------------
  ERDecayMCEventHeader* header = new ERDecayMCEventHeader();
  run->SetMCEventHeader(header);

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

  ERDetector* detector= new ERDetector("TestDetector1", kTRUE, verbose);
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

  //-------------------------------------------------------------------------
  // -----   Create PrimaryGenerator   -----------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonMixGenerator* generator = new ERIonMixGenerator("15N", Z, A, Q, 1);
  Double32_t kin_energy = 0.043; //GeV
  generator->SetKinESigma(kin_energy, 0);
  generator->SpreadingOnTarget();
  Double32_t theta = 0;
  Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(theta, sigmaTheta);
  generator->SetPhiRange(0, 360);
  Double32_t distanceToTarget = 200;
  Double32_t sigmaOnTarget = 0.5;
  generator->SetSigmaXYZ(0, 0, -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  generator->SetBoxXYZ(-0.4,-0.4,0.4,0.4, -distanceToTarget);
  primGen->AddGenerator(generator);
  run->SetGenerator(primGen);

  //-------Set visualisation flag to true---------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Digitization tasks --------------------------------------------------
  ERDigitizer* digitizer = new ERDigitizer("TestDetector1",verbose);
  digitizer->AddError("gas1",0.,0.0,0.0);
  digitizer->AddError("plastic1",0.0,0.,0.);
  run->AddTask(digitizer);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

  //-------Initialize simulation run--------------------------------------------
  run->Init();

  //-------Runtime database-----------------------------------------------------
  Bool_t kParameterMerged = kTRUE; 
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data()); 
  rtdb->setOutput(parOut); 
  rtdb->saveOutput(); 
  rtdb->print();

  // ------Run simulation/digitization------------------------------------------
  run->Run(nEvents);

  // -----   Finish   ----------------------------------------------------------
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
