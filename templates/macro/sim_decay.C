void sim_decay(Int_t nEvents = 10){
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
  ERDecay* targetDecay = new Decay();
  decayer->AddDecay(targetDecay);
  //-------------------------------------------------------------------------

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
  
  FairModule* target = new ERTarget("Target", kTRUE,1);
  target->SetGeometryFileName("target.geo.root");
  run->AddModule(target);
  // ------------------------------------------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  ERIonGenerator* ionGenerator = new ERIonGenerator("PrimaryIon",8,26,8,1);
  Double32_t kin_energy = 0.025*(24+8); //GeV
  Double_t mass = ionGenerator->Ion()->GetMass();
  Double32_t momentum = TMath::Sqrt(kin_energy*kin_energy + 2.*kin_energy*mass); //GeV
  ionGenerator->SetPRange(momentum, momentum);
  Double32_t theta1 = 0.;  // polar angle distribution
  Double32_t theta2 = 0.0001*TMath::RadToDeg();
  ionGenerator->SetThetaRange(theta1, theta2);
  ionGenerator->SetPhiRange(0, 360);
  ionGenerator->SetXYZ(0,0,-10.);
  primGen->AddGenerator(ionGenerator);
  run->SetGenerator(primGen);
  /*
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  //Ion 24O
  Int_t A = 24;
  Int_t Z = 8;
  Int_t Q = 8;
  Double_t Pz = 0.7;// AGeV
  FairIonGenerator* ionGenerator = new FairIonGenerator(Z,A,Q,1,0.,0.,Pz,0.,0.,-10.);

  primGen->AddGenerator(ionGenerator);
    
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
  */
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
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
}