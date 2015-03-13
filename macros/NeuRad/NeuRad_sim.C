void NeuRad_sim(){
 
	// -----   Timer   --------------------------------------------------------
	TStopwatch timer;
	timer.Start();
	// ------------------------------------------------------------------------
 
	// -----   Create simulation run   ----------------------------------------
	FairRunSim* run = new FairRunSim();
	run->SetName("TGeant3");              // Transport engine
	run->SetOutputFile("out.root");          // Output file
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // ------------------------------------------------------------------------
  
	// -----   Create media   -------------------------------------------------
	run->SetMaterials("media.geo");       // Materials
	// ------------------------------------------------------------------------
  
  // -----   Create detectors  ----------------------------------------------	
	//Expert Cave definition
	FairModule* cave= new R3BCave("CAVE");
	cave->SetGeometryFileName("Expert_cave.geo");
	run->AddModule(cave);
	
	FairModule* neuRad= new ExpertNeuRad("ExpertNeuRad", kTRUE);
	neuRad->SetGeometryFileName("NeuRad.geo.root");
  //neuRad->SetGeometryFileName("neuland_v12a_14m.geo.root");
	run->AddModule(neuRad);
  // ------------------------------------------------------------------------
	
	// -----   Create PrimaryGenerator   --------------------------------------
	FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  Int_t pdgId = 2112; // neutron  beam
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 7.;
  Double32_t momentum = .01; //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(theta1, theta1);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(90,90);
  boxGen->SetXYZ(1.2, 1.2, -35.0);

  primGen->AddGenerator(boxGen);
	run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
	//-------Set visualisation flag to true------------------------------------
	run->SetStoreTraj(kTRUE);
	
	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
	
	// -----   Initialize simulation run   ------------------------------------
	run->Init();
	
	Int_t nSteps = -15000;
	gMC->SetMaxNStep(nSteps);
	
	// -----   Runtime database   ---------------------------------------------
	Bool_t kParameterMerged = kTRUE;
	FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
	parOut->open("par.root");
	rtdb->setOutput(parOut);
	rtdb->saveOutput();
	rtdb->print();
	  
	run->Run(1);
	
	// -----   Finish   -------------------------------------------------------
	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Macro finished succesfully." << endl;
  cout << "Output file is out.root" << endl;
	cout << "Parameter file is par.root" << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime
	<< "s" << endl << endl;
}
