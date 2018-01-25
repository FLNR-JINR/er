void he8_sim(Int_t nEvents = 10){
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

  ERBeamDet* beamDet= new ERBeamDet("ERBeamDet", kTRUE,1);
  beamDet->SetGeometryFileName("beamdet.v1.geo.root");
  run->AddModule(beamDet);

  ERQTelescope* QTelescope= new ERQTelescope("ERQTelescope", kTRUE,1);
  QTelescope->SetGeometryFileName("QTelescope.v2.geo.root");
  run->AddModule(QTelescope);

  ERRTelescope* RTelescope= new ERRTelescope("ERRTelescope", kTRUE,1);
  RTelescope->SetGeometryFileName("RTelescope.v2.geo.root"); 
  run->AddModule(RTelescope);

  // ------------------------------------------------------------------------
  //------    ER Decayer   -------------------------------------------------
  ERDecayer* decayer = new ERDecayer();
  ERTextDecay* decay = new ERTextDecay("10Heto8He2n");
  decay->SetInputIon(2,10,2);
  decay->AddOutputIon(2,8,2);
  decay->AddOutputParticle(2112);
  decay->AddOutputParticle(2112);
  decay->SetUniformPos(0.5,0.7);
  decay->SetFileName("generators/generator_10He_decay.dat");
  decay->SetDecayVolume("target3H");
  decayer->AddDecay(decay);
  run->SetDecayer(decayer);
  //-------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  ERIonGenerator* ionGenerator = new ERIonGenerator("10He",2,10,2,1);
  Double32_t kin_energy = 0.025*9; //GeV
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