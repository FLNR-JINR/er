void be6_sim(Int_t nEvents = 1){
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
  run->SetName("TGeant4");
  run->SetOutputFile(outFile.Data());
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
  
  FairModule* target = new ERTarget("Target", kTRUE,1);
  target->SetGeometryFileName("target.3h.geo.root");
  run->AddModule(target);

  //------    ER Decayer   -------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERTextDecay* decay = new ERTextDecay("6Beto4He2p");
  decay->SetInputIon(4,6,4);
  decay->SetOutputIon(2,4,2);
  decay->AddOutputParticle(2212);
  decay->AddOutputParticle(2212);
  decay->SetUniformPos(-0.1,0.1);
  decay->SetFileName("generators/6be_chex_noal_0_v2.dat");
  decay->SetDecayVolume("target3H");
  decayer->AddDecay(decay);
  run->SetDecayer(decayer);
  //-------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonGenerator* ionGenerator = new ERIonGenerator("6Be",4,6,4,1);
  Double32_t kin_energy = 0.0325*6; //GeV
  Double_t mass = ionGenerator->Ion()->GetMass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  ionGenerator->SetPRange(momentum, momentum);
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.;
  ionGenerator->SetThetaRange(theta1, theta2);
  ionGenerator->SetPhiRange(0, 360);
  ionGenerator->SetBoxXYZ(-0.1,-0.1,0.1,0.1,-1534);
  primGen->AddGenerator(ionGenerator);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
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