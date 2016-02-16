void Gadast_sim(int nEvents = 1){
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

  Int_t verbose = 1;
  ERGadast* gadast= new ERGadast("ERGadast", kTRUE,verbose);
  gadast->SetGeometryFileName("gadast.root");
  run->AddModule(gadast);
  // ------------------------------------------------------------------------
	
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2112; // neutron  beam
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 7.;
  Double32_t momentum = .400; //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta1);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(90,90);
  boxGen->SetXYZ(0.15,0.15, -51.0);

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
  gMC->SetMaxNStep(nSteps);
	
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