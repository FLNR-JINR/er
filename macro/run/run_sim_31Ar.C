void run_sim_31Ar(int nEvents = 1000){
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
  ERDecay* targetDecay = new ERDecay31Arto30Ar();
  decayer->AddDecay(targetDecay);
  ERDecay30Arto28S2p* directDecay = new ERDecay30Arto28S2p();
  directDecay->SetDirectReactionTauCM(200.);
  decayer->AddDecay(directDecay);
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
  target->SetGeometryFileName("target.geo.root");
  run->AddModule(target);

  // ER muSi definition
  ERmuSi* muSi= new ERmuSi("ERmuSi", kTRUE,1);
  muSi->SetGeometryFileName("muSi.geo.root");
  run->AddModule(muSi);

  // ER Gadast definition
  ERGadast* gadast= new ERGadast("ERGadast", kTRUE);
  gadast->SetVerboseLevel(1);
  gadast->SetGeometryFileName("gadast.gdml");
  //run->AddModule(gadast);

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
  //run->AddModule(neuRad);
  // ------------------------------------------------------------------------
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  //Ion 27F
  Int_t A = 31;
  Int_t Z = 18;
  Int_t Q = 18;
  Double_t Pz = 1.;// AGeV
  FairIonGenerator* ionGenerator = new FairIonGenerator(Z,A,Q,1,0.,0.,Pz,0.,0.,-10.);

  primGen->AddGenerator(ionGenerator);
    
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  //------Set magnetic field ------------------------------------------------
  FairConstField* fMagField = new FairConstField();
  fMagField->SetField(10.,0.,0.);//values are in kG, 1T = 10kG
  fMagField->SetFieldRegion(-200,200.,-200.,200.,1000.,2500.); // in cm
  run->SetField(fMagField);
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
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
