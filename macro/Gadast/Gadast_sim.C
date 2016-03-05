void Gadast_sim(int nEvents = 10000){
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

  ERGadast* gadast= new ERGadast("ERGadast", kTRUE);
  gadast->SetVerboseLevel(3);
  gadast->SetGeometryFileName("gadast.gdml");
  run->AddModule(gadast);
  // ------------------------------------------------------------------------
	
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  //Изотропно в ЛАБ системе
  Int_t pdgId = 22; // gamma
  Double32_t kin_energy = 0.001275; //GeV 1275 KeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = kin_energy; //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(0, 90);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0, 360.);
  boxGen->SetBoxXYZ (0.0,0.,0.,0.,0.);
  primGen->AddGenerator(boxGen);
/*
  ERGammaGenerator* gammaGenerator = new ERGammaGenerator();
  gammaGenerator->SetBeamEnergy(0.5*26);
  gammaGenerator->SetGammaCMEnergy(0.0005);
  gammaGenerator->SetGammaCMPhiRange(0., 360.);
  gammaGenerator->SetGammaCMThetaRange(0., 90.);
  gammaGenerator->SetIon(8, 26);
  primGen->AddGenerator(gammaGenerator);*/

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