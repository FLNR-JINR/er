void sim(Int_t nEvents = 10){
	TString dir = getenv("VMCWORKDIR");
	TString tutdir = dir + "/../Tutorial1";

	TString tut_geomdir = tutdir + "/geometry/";
  	gSystem->Setenv("GEOMPATH",tut_geomdir.Data());

  	TString tut_configdir = tutdir + "/gconfig/";
  	gSystem->Setenv("CONFIG_DIR",tut_configdir.Data());
  	cout << tut_geomdir << endl;
  	TString outFile = "sim.root";
  	TString parFile = "par.root";

  	// -----   Create simulation run   ----------------------------------------
  	FairRunSim* run = new FairRunSim();
  	run->SetName("TGeant3");;             // Transport engine
  	run->SetOutputFile(outFile);          // Output file
  	FairRuntimeDb* rtdb = run->GetRuntimeDb();
  	// ------------------------------------------------------------------------
    // -----   Create media   -------------------------------------------------
    run->SetMaterials("media.geo");       // Materials
    // ------------------------------------------------------------------------

    // -----   Create geometry   ----------------------------------------------
    FairModule* cave= new FairCave("CAVE");
    cave->SetGeometryFileName("cave.geo"); 
    run->AddModule(cave);

    Det* det= new Det("Det", kTRUE,1);
    det->SetGeometryFileName("det.geo.root");
    run->AddModule(det);
    //--------------------------------------------------------------------------------

    //-------------------------------------------------------------------------

  // -----   Create PrimaryGenerator   --------------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  FairBoxGenerator* boxGen = new FairBoxGenerator(2212, 1);
  Double_t momentum = 1; //GeV
  boxGen->SetThetaRange (-25,25);
  boxGen->SetPRange(momentum,momentum);
  boxGen->SetPhiRange(0.,180.);
  primGen->AddGenerator(boxGen);
  run->SetGenerator(primGen);
  // ------------------------------------------------------------------------

  //-------Set visualisation flag to true------------------------------------
    run->SetStoreTraj(kTRUE);

  // -----   Initialize simulation run   ------------------------------------
  run->Init();
  // ------------------------------------------------------------------------

  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(parFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ------------------------------------------------------------------------
     
  // -----   Start run   ----------------------------------------------------
  run->Run(nEvents);
}
