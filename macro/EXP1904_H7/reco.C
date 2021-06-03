void reco(TString inFile,TString outFile) {
  //---------------------Files-----------------------------------------------
  TString geoFile = "setup_exp1904_non_uniform_ssd.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  TFile *f_in = new TFile(inFile.Data());
  if (f_in->IsZombie()) {
    cerr << "File " << inFile.Data() << " does not exist" << endl;
    exit(1);
  }
  TTree *in_tree = (TTree*)f_in->Get("er");
  if (in_tree->IsZombie()) {
    cerr << "Tree er in file" << inFile.Data() << " does not exist" << endl;
    exit(1);   
  }
  Int_t nEvents = in_tree->GetEntries();
  cout << nEvents << " will be processed" <<  endl;

  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  //run->HoldEventsCount(); //forbid different entry number in the input and output file
  run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  // ------------------------BeamDetTrackFinger--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("shapeD2");
  run->AddTask(trackFinder);
  // ------- QTelescope TrackFinder -------------------------------------------
  ERTelescopeTrackFinder* qtelescopeTrackFinder = new ERTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_X", "Telescope_1_SingleSi_SSD_1_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_2", "Telescope_2_SingleSi_SSD_2_X", "Telescope_2_SingleSi_SSD20_2_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_3", "Telescope_3_SingleSi_SSD20_3_X", "Telescope_3_SingleSi_SSD_3_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_4", "Telescope_4_SingleSi_SSD_4_X", "Telescope_4_SingleSi_SSD20_4_Y");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY");
  run->AddTask(qtelescopeTrackFinder);
  // -----------------------BeamDetTrackPID------------------------------------
  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERBeamDetPID* beamdetPid = new ERBeamDetPID(verbose);
  beamdetPid->SetBoxPID(0., 1000., 0., 1000.);
  beamdetPid->SetOffsetToF(68.475);
  beamdetPid->SetProbabilityThreshold(0);
  beamdetPid->SetIonMass(7482.5396);
  beamdetPid->SetPID(1000020080);
  run->AddTask(beamdetPid);
  // ------   QTelescope TrackPID -----------------------------------------
  ERTelescopePID* qtelescopePID = new ERTelescopePID(verbose);
  TString deStation1 = "SSD20_1";
  TString eStation1 = "SSD_1";
  Double_t normalizedThickness = 0.002; // [cm]
  Double_t DSD_thickness = 0.15; // [cm]
  qtelescopePID->SetParticle("Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y", 1000020030,
                              deStation1, eStation1, normalizedThickness);
  qtelescopePID->SetParticle("Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y", 1000020030,
                             "SSD20_2", "SSD_2", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y", 1000020030,
                             "SSD20_3", "SSD_3", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y", 1000020030,
                             "SSD20_4", "SSD_4", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y", 1000020040,
                              deStation1, eStation1, normalizedThickness);
  qtelescopePID->SetParticle("Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y", 1000020040,
                             "SSD20_2", "SSD_2", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y", 1000020040,
                             "SSD20_3", "SSD_3", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y", 1000020040,
                             "SSD20_4", "SSD_4", normalizedThickness);
  qtelescopePID->SetParticle("Central_telescope_DoubleSi_DSD_XY", 1000010030, "DSD", "CsI", DSD_thickness);
  qtelescopePID->SetEdepAccountingStrategy("DSD", ERTelescopePID::EdepAccountingStrategy::EdepFromXChannel);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_1_Y", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_1_Y", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_2_X", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_2_X", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_3_Y", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_3_Y", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_4_X", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_4_X", 15, -0.25);
  run->AddTask(qtelescopePID);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("FATAL");
  run->Init();
  // run->Run(0, nEvents);
  run->Run(0, 10000000);
  // ------------------------------------------------------------------------;
  //rtdb->setOutput(parIO);
  //rtdb->saveOutput();
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t Central_telescopeime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << Central_telescopeime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
