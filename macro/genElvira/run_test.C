void run_test(Int_t nEvents = 1000)
{
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
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

  // -----   Create detectors  ----------------------------------------------
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);


  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

/*  Int_t pdgId = 22; // gamma  beam
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.;
  Double32_t kin_energy = 661.7e-6; //GeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta2);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 180.);
  boxGen->SetBoxXYZ(0., 0., 0., 0., -10.);
  primGen->AddGenerator(boxGen);*/

  Int_t pdgId = 2112; // 2112 - neutron
  EREasFuncOfAnglesGenerator* newGen = new EREasFuncOfAnglesGenerator(pdgId);
  newGen->SetThetaRange(0., 180.);
  newGen->SetPhiRange(0., 360.);
  newGen->SetBoxXYZ(0., 0., 0., 0., -10.);
  newGen->SetUserParameters(13., 1., 16., 9.5, 6.3);
  newGen->SetCosTheta();
  primGen->AddGenerator(newGen);

  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  Int_t nSteps = -15000;
  //gMC->SetMaxNStep(nSteps);

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ---------------------------------------------------------

  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);

  // -----   Finish   -------------------------------------------------------
  //neuRad->WriteHistos();
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
