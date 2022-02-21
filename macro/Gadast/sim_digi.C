void sim_digi(int nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString outFile= "sim_digi.root";
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

  ERGadast* gadast= new ERGadast("ERGadast", kTRUE, 1);
  //gadast->SetStoreSteps();
  gadast->SetGeometryFileName("partOfGadast.v1.geo.root");
  run->AddModule(gadast);
  // ------------------------------------------------------------------------

  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERGadastDigitizer* digitizer = new ERGadastDigitizer(verbose);
  //digitizer->SetCsILC(1.);
  /*
  std::map<std::pair<size_t, size_t>, float> lc;
  lc[{0, 1}] = 1.;
  digitizer->SetCsILC(lc);
  */
  std::map<std::pair<size_t, size_t>, std::vector<std::vector<std::vector<float>>>> lc;
  std::vector<std::vector<std::vector<float>>> block_1 = { { { 1., 2., 3. }, { 1., 2., 3. } },
                                                           { { 1., 2., 3. }, { 1., 2., 3. } } };
  lc[{0, 1}] = block_1;
  digitizer->SetCsILC(lc);

  //digitizer->SetCsIEdepError(0.0,0.04,0.02);
  /*
  std::map<std::pair<size_t, size_t>, float> a;
  a[{0, 1}] = 0.;
  std::map<std::pair<size_t, size_t>, float> b;
  b[{0, 1}] = 0.04;
  std::map<std::pair<size_t, size_t>, float> c;
  c[{0, 1}] = 0.02;
  digitizer->SetCsIEdepError(a, b, c);
  */
  std::map<std::pair<size_t, size_t>, std::vector<std::vector<std::vector<float>>>> a;
  std::vector<std::vector<std::vector<float>>> block_1_a = { { { 0., 0., 0. }, { 0., 0., 0. } },
                                                             { { 0., 0., 0. }, { 0., 0., 0. } } };
  a[{0, 1}] = block_1_a;

  std::map<std::pair<size_t, size_t>, std::vector<std::vector<std::vector<float>>>> b;
  std::vector<std::vector<std::vector<float>>> block_1_b = { { { 0.04, 0.04 }, { 0.04, 0.04 } },
                                                             { { 0.04, 0.04 }, { 0.04, 0.04 } } };
  b[{0, 1}] = block_1_b;

  std::map<std::pair<size_t, size_t>, std::vector<std::vector<std::vector<float>>>> c;
  std::vector<std::vector<std::vector<float>>> block_1_c = { { { 0.02 } } };
  c[{0, 1}] = block_1_c;

  digitizer->SetCsIEdepError(a, b, c);

  digitizer->SetCsITimeError(0.);
  digitizer->SetLaBrLC(1.);
  digitizer->SetLaBrEdepError(0.0,0.04,0.02);
  digitizer->SetLaBrTimeError(0.);
  run->AddTask(digitizer);
	
  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  //Изотропно в ЛАБ системе
  Int_t pdgId = 22; // gamma
  Double32_t kin_energy = 0.002; //GeV 1275 KeV
  Double_t mass = TDatabasePDG::Instance()->GetParticle(pdgId)->Mass();
  Double32_t momentum = kin_energy; //GeV
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
  boxGen->SetThetaRange(0, 0);
  boxGen->SetPRange(momentum, momentum);
  boxGen->SetPhiRange(0,0);
  boxGen->SetBoxXYZ (0.0,0.,0.,0.,0.);
  primGen->AddGenerator(boxGen);
  /*
  ERGammaGenerator* gammaGenerator = new ERGammaGenerator();
  gammaGenerator->SetBeamEnergy(0.54*17);
  gammaGenerator->SetGammaCMEnergy(0.001288);
  gammaGenerator->SetGammaCMPhiRange(0., 360.);
  gammaGenerator->SetGammaCMThetaRange(0., 180.);
  gammaGenerator->SetIon(10, 17);
  primGen->AddGenerator(gammaGenerator);
  */
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------
	
  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(kTRUE);
	
  //-------Set LOG verbosity  ----------------------------------------------- 
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  
  // -----   Initialize simulation run   ------------------------------------
  run->Init();
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

  TString geometryName = "setup.root";
  run->CreateGeometryFile(geometryName.Data());
  
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
