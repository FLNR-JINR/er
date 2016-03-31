void run_sim(int nEvents = 1){
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

  //-------- Set MC event header --------------------------------------------
  ERMCEventHeader* header = new ERMCEventHeader();
  run->SetMCEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   Create detectors  ----------------------------------------------	
  FairModule* cave= new ERCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);


  /*FairModule* target = new ERTarget("Target", kTRUE,1);
  target->SetGeometryFileName("target.geo.root");
  run->AddModule(target);
*/
  // ER muSi definition
  ERmuSi* muSi= new ERmuSi("ERmuSi", kTRUE,1);
  muSi->SetGeometryFileName("muSi.geo.root");
  run->AddModule(muSi);

  // ER Gadast definition
  /*ERGadast* gadast= new ERGadast("ERGadast", kTRUE);
  gadast->SetVerboseLevel(1);
  gadast->SetGeometryFileName("gadast.gdml");
  run->AddModule(gadast);

  // ER NeuRad definition
  /* Select verbosity level
   * 1 - only standard logs
   * 2 - Print points after each event
   * 3 - - GEANT Step information
  */
  ERNeuRad* neuRad= new ERNeuRad("ERNeuRad", kTRUE,1);
  neuRad->SetGeometryFileName("NeuRad_v2.geo.root");
  /* Select storing stepss
   * not store steps
   * SetStorePrimarySteps() - store only primary particle step
   * SetStoreAllSteps() - store all steps. WARNING - very slow
  */
  //neuRad->SetStoreAllSteps();
  run->AddModule(neuRad);
  // ------------------------------------------------------------------------
	
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  //Ion 27F
  Int_t A = 27;
  Int_t Z = 9;
  Int_t Q = 9;
  Double_t Pz = 0.7;// AGeV
  ERGenerator* generator = new ERGenerator(Z,A,Q,0.,0.,Pz,0.,0.,-10.); 
  //FairIonGenerator* bgen = new FairIonGenerator(Z,A,Q,1,0.,0.,Pz,0.,0.,-10.);

  primGen->AddGenerator(generator);
  //primGen->AddGenerator(bgen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  
  // -----   Initialize simulation run   ------------------------------------
  run->SetUserDecay(kTRUE);
  //run->SetUserDecayConfig(kTRUE);
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