void run(Int_t nEvents = 1000){
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
  // -----   Create detectors  -------------------------------------------------
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
  detector->SetGeometryFileName("N15.base.geo.root");
  detector->AddSensetive("gas");
  detector->AddSensetive("plastic");
  run->AddModule(detector);
  
  ERDetector* detector1= new ERDetector("TestDetector2", kTRUE,verbose);
  detector1->SetGeometryFileName("N15.base.geo_2.root");
  detector1->AddSensetive("gas1");
  detector1->AddSensetive("plastic1");
  run->AddModule(detector1);
  
  ERDetector* detector2= new ERDetector("TestDetector3", kTRUE,verbose);
  detector2->SetGeometryFileName("N15.base.geo_4.root");
  detector2->AddSensetive("gas2");
  detector2->AddSensetive("plastic2");
  run->AddModule(detector2);
  
  //------    ER Decayer   -----------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERTextDecay* decay = new ERTextDecay("15Nto15N11B");
  decay->SetInputIon(7,15,7);
  decay->AddOutputIon(7,15,7);
  decay->AddOutputIon(5,11,5);
  decay->SetUniformPos(-0.00035,0.00035);
  decay->SetStep(0.00001); //0.1 micron
  decay->SetFileName("CMom_B11_N15.txt");
  decay->SetDecayVolume("targetB11");
  decay->SetTargetReactionMass(16*0.94);
  decayer->AddDecay(decay);
  run->SetDecayer(decayer);
  // -----   Create PrimaryGenerator   -----------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  //Ion 15N
  Int_t A = 15;
  Int_t Z = 7;
  Int_t Q = 3;
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
  digitizer->AddError("gas",0.,0.0,0.0);
  digitizer->AddError("plastic",0.0,0.,0.);
  run->AddTask(digitizer);

  ERDigitizer* digitizer2 = new ERDigitizer("TestDetector2",verbose);
  digitizer2->AddError("gas1",0.,0.0,0.0);
  digitizer2->AddError("plastic1",0.0,0.,0.);
  run->AddTask(digitizer2);

  ERDigitizer* digitizer3 = new ERDigitizer("TestDetector3",verbose);
  digitizer3->AddError("gas2",0.,0.0,0.0);
  digitizer3->AddError("plastic2",0.0,0.,0.);
  run->AddTask(digitizer3);
  //-------Set LOG verbosity  --------------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  //-------Initialize simulation run--------------------------------------------
  run->Init();
  //-------Runtime database-----------------------------------------------------
  Bool_t kParameterMerged = kTRUE; 
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data()); 
  rtdb->setOutput(parOut); 
  rtdb->saveOutput(); 
  rtdb->print(); // 
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
