void reco(TString inFile, TString outFile) {
  //---------------------Files-----------------------------------------------

  TString geoFile = "input/setup.root";

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
  qtelescopeTrackFinder->SetHitStation("Telescope_left", "Telescope_left_DoubleSi_DSD_L_XY");
  qtelescopeTrackFinder->SetHitStation("Telescope_right", "Telescope_right_SingleSi_SSD_20_X", "Telescope_right_DoubleSi_DSD_R_XY_Y");
  run->AddTask(qtelescopeTrackFinder);
  // -----------------------BeamDetTrackPID------------------------------------
  Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "6He";
  ERBeamDetPID* beamdetPid = new ERBeamDetPID(verbose);
  beamdetPid->SetBoxPID(0., 1000., 0., 1000.);
  beamdetPid->SetOffsetToF(69.93);
  beamdetPid->SetProbabilityThreshold(0);
  beamdetPid->SetIonMass(5605.54);
  beamdetPid->SetPID(1000020060);
  run->AddTask(beamdetPid);
  // ------   QTelescope TrackPID -----------------------------------------
  ERTelescopePID* qtelescopePID = new ERTelescopePID(verbose);

  std::map<TString, std::pair<Int_t, Double_t>> activeStations_telRight = {
      {"SSD_20", {-1, 0.}},
      {"DSD_R", {-1, .0}},
      {"Rv", {-1, .0}}
  };
  qtelescopePID->SetParticle("Telescope_right_SingleSi_SSD_20_XTelescope_right_DoubleSi_DSD_R_XY_Y", 1000020060, "SSD_20", "DSD_R",
   activeStations_telRight, 0.002,{}, {"SSD_20"});
  qtelescopePID->SetParticle("Telescope_right_SingleSi_SSD_20_XTelescope_right_DoubleSi_DSD_R_XY_Y", 1000020040, "SSD_20", "DSD_R",
   activeStations_telRight, 0.002,{}, {"SSD_20"});  
  qtelescopePID->SetEdepAccountingStrategy("DSD_R", ERTelescopePID::EdepAccountingStrategy::EdepFromYChannel);  

  std::map<TString, std::pair<Int_t, Double_t>> activeStations_telLeft = {
      {"DSD_L", {-1, .0}},
      {"CsI", {-1, .0}}
  };
  qtelescopePID->SetParticle("Telescope_left_DoubleSi_DSD_L_XY", 1000020040, "DSD_L", "CsI", activeStations_telLeft, 0.1);
  qtelescopePID->SetParticle("Telescope_left_DoubleSi_DSD_L_XY", 1000020060, "DSD_L", "CsI", activeStations_telLeft, 0.1);
  qtelescopePID->SetEdepAccountingStrategy("DSD_L", ERTelescopePID::EdepAccountingStrategy::EdepFromYChannel);

  run->AddTask(qtelescopePID);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("FATAL");
  run->Init();
  run->Run(0, nEvents);
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
