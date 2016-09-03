void run_sim_Li10(int nEvents = 1){
  //---------------------Files-----------------------------------------------
  TString outFile= "sim.root";
  TString parFile= "par.root";
  // ------------------------------------------------------------------------

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
  
  //------    ER Deacayer   -------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERDecay* targetDecay = new ERDecayLi9DetoLi10_Li9n_p();
  decayer->AddDecay(targetDecay);
  //-------------------------------------------------------------------------

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
  
  FairModule* target = new ERTarget("Target", kTRUE,1);
  target->SetGeometryFileName("Li10_target.geo.root");
  run->AddModule(target);

  Int_t verbose = 1;
  ERND* nd= new ERND("ERND", kTRUE,verbose);
  nd->SetGeometryFileName("ND.geo.root");
  run->AddModule(nd);

  ERDSRD* dsrd= new ERDSRD("ERDSRD", kTRUE,verbose);
  dsrd->SetGeometryFileName("DSRD.geo.root");
  run->AddModule(dsrd);
  // ------------------------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonGenerator* ionGenerator = new ERIonGenerator("Li9",3,9,3,1);
  Double32_t momentum = 0.25*9; //GeV
  ionGenerator->SetPRange(momentum, momentum);
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.0001*TMath::RadToDeg();
  ionGenerator->SetThetaRange(theta1, theta2);
  ionGenerator->SetPhiRange(0, 360);
  ionGenerator->SetBoxXYZ(-0.4,-0.4,0.4,0.4,-20.);
  primGen->AddGenerator(ionGenerator);

  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  //TDatabasePDG::Instance()->Print();
  run->SetDecayer(decayer);
  //-------------------------------------------------------------------------
  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  //-------------------------------------------------------------------------
  
  // -----   Run simulation  ------------------------------------------------
  run->Run(nEvents);
  //-------------------------------------------------------------------------

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
  //-------------------------------------------------------------------------
}
