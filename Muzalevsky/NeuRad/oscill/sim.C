void sim(Int_t nEvents = 10000){
  //---------------------Files-----------------------------------------------
  TString outFile= "tests/sim.root";
  TString parFile= "tests/par.root";
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

  //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* header = new ERMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   Create detectors  ---------------------------------------------- 
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);
  
  ERCollimator* collimator = new ERCollimator();
  collimator->SetGeometryFileName("collimator.geo.root");
  run->AddModule(collimator);

  // ER NeuRad definition
  /* Select verbosity level
   * 1 - only standard logs
   * 2 - Print points after each event
   * 3 - - GEANT Step information
  */
  Int_t verbose = 1;
  ERNeuRad* neuRad= new ERNeuRad("ERNeuRad", kTRUE,verbose);
  neuRad->SetGeometryFileName("NeuRad.v4.geo.root");
  /* Select storing stepss
   * not store steps
   * SetStorePrimarySteps() - store only primary particle step
   * SetStoreAllSteps() - store all steps. WARNING - very slow
  */
  neuRad->SetStorePrimarySteps();
  run->AddModule(neuRad);
  // ------------------------------------------------------------------------
  
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  // Int_t pdgId = 2112; // neutron  beam
  // Double32_t theta1 = 0.;  // polar angle distribution
  // Double32_t theta2 = 7.;
  // Double32_t kin_energy = .500; //GeV
  Int_t pdgId = 22; // neutron  beam
  Double32_t theta1 = 180.;  // polar angle distribution
  Double32_t theta2 = 180.;
  Double32_t kin_energy = 0.001333; //GeV

  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta1);
  boxGen->SetPRange(momentum, momentum);
  // boxGen->SetPhiRange(90, 90);
  boxGen->SetPhiRange(0, 0);
  // boxGen->SetBoxXYZ(0.,0,0.3,0.3,0.);
  boxGen->SetBoxXYZ(-0.149,-0.149,0.149,0.149,19.4);
  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
  
  //-------Set LOG verbosity  ----------------------------------------------- 
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
  neuRad->WriteHistos();
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
