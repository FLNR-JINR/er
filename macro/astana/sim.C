void sim(Int_t nEvents = 1)
{
  //---------------------Files-----------------------------------------------
  TString outFile = "sim.root";
  TString parFile = "par.root";
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
  run->SetMaterials("media.geo");       // Materials
  // ------------------------------------------------------------------------

  //-------- Set MC event header --------------------------------------------
  ERDecayMCEventHeader* header = new ERDecayMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  FairModule* target = new ERTarget("astana_target_CD2", kTRUE, 1);
  target->SetGeometryFileName("astana_target_CD2_geo.root");
  run->AddModule(target);

  ERDetector* detector = new ERDetector("Si_detector", kTRUE);
  detector->SetGeometryFileName("astana_Si_detector_geo.root");
  detector->AddSensetive("station");
  run->AddModule(detector);

  // 1 - only standard log print, 2 - print digi information 
  ERDigitizer* digitizer = new ERDigitizer("Si_detector",2);
  digitizer->AddError("station",0.,0.0,0.0);
  run->AddTask(digitizer);

  //------    ER Decayer   -------------------------------------------------
  //Ion 13C
  Int_t A = 13;
  Int_t Z = 6;
  Int_t Q = 6;

  ERDecayer* decayer = new ERDecayer();
  ERElasticScattering* scattering = new ERElasticScattering("13Cto13C2H");

  scattering->SetInputIon(Z,A,Q);
  scattering->SetTargetIon(1,2,1);
  //scattering->SetThetaCDF("cos_tetta_cross.txt");
  scattering->SetUniformPos(-0.0005,-0.0005);
  scattering->SetStep(0.00001); //0.1 micron
  scattering->SetDecayVolume("boxCD");
  //scattering->SetThetaRange(20., 21.); //TODO !!!!
  scattering->SetPhiRange(0., 360.);

  decayer->AddDecay(scattering);
  run->SetDecayer(decayer);

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonMixGenerator* generator = new ERIonMixGenerator("13C", Z, A, Q, 1);
  Double32_t kin_energy = 0.012*13; // GeV
  generator->SetKinESigma(kin_energy, 0.);

  //Double32_t theta = 0.;
  //Double32_t sigmaTheta = 0.004*TMath::RadToDeg();
  generator->SetThetaSigma(0., 0.);
  generator->SetPhiRange(0., 360.);

  Double32_t distanceToTarget = 200.;
  Double32_t sigmaOnTarget = .0;
  generator->SpreadingOnTarget();
  generator->SetSigmaXYZ(0., 0., -distanceToTarget, sigmaOnTarget, sigmaOnTarget);
  generator->SetBoxXYZ(0.,0., 0.,0., -distanceToTarget); // Xmin, Xmax, Ymin, Ymax, Z


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
}
