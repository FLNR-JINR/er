void reco(TString inFile,TString outFile) {
// void reco() {
  //---------------------Files-----------------------------------------------
  // TString inFile  = "digi.root";
  // TString outFile = "reco.root";
  TString parFile = "par.root";
  TString geoFile = "setup_exp1904_non_uniform_ssd.root";
  // TString geoFile = "/home/muzalevskii/work/expertroot/macro/EXP1904_H7/sim_nonuniform/setup_exp1904_non_uniform_ssd.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  

  // TString inFile = "/mnt/data/exp1904/ERanalysis/cleaned/h7_ect_62_0007.cleaned.root";
  // TString outFile = "/mnt/data/exp1904/ERanalysis/testReco/test/h7_ect_62_0007.cleaned.root";

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
  /*
  run->SetUserCut("Length$(ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_V_1_Y_2) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD_V_2_X_5) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD_V_3_Y_8) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD_V_4_X_11) == 0");
  */
  /*
  run->SetUserCut("Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_V_1_Y_2) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD_V_2_X_5) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD_V_3_Y_8) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD_V_4_X_11) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1) == 1 \
                   && Length$(ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD20_1_X_0) == 1");*/
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
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_X_0", "Telescope_1_SingleSi_SSD_1_Y_1");
  qtelescopeTrackFinder->SetHitStation("Telescope_2", "Telescope_2_SingleSi_SSD_2_X_4", "Telescope_2_SingleSi_SSD20_2_Y_3");
  qtelescopeTrackFinder->SetHitStation("Telescope_3", "Telescope_3_SingleSi_SSD20_3_X_6", "Telescope_3_SingleSi_SSD_3_Y_7");
  qtelescopeTrackFinder->SetHitStation("Telescope_4", "Telescope_4_SingleSi_SSD_4_X_10", "Telescope_4_SingleSi_SSD20_4_Y_9");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY_0");
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
  //run->AddTask(beamdetPid);
  // ------   QTelescope TrackPID -----------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);
  TString deStation1 = "SSD20_1";
  TString eStation1 = "SSD_1";
  Double_t normalizedThickness = 0.002; // [cm]
  Double_t DSD_thickness = 0.15; // [cm]
  qtelescopePID->SetParticle("Telescope_1_SingleSi_SSD20_1_X_0Telescope_1_SingleSi_SSD_1_Y_1", 1000020030,
                              deStation1, eStation1, normalizedThickness);
  qtelescopePID->SetParticle("Telescope_2_SingleSi_SSD_2_X_4Telescope_2_SingleSi_SSD20_2_Y_3", 1000020030,
                             "SSD20_2", "SSD_2", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_3_SingleSi_SSD20_3_X_6Telescope_3_SingleSi_SSD_3_Y_7", 1000020030,
                             "SSD20_3", "SSD_3", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_4_SingleSi_SSD_4_X_10Telescope_4_SingleSi_SSD20_4_Y_9", 1000020030,
                             "SSD20_4", "SSD_4", normalizedThickness);
  qtelescopePID->SetParticle("Central_telescope_DoubleSi_DSD_XY_0", 1000010030, "DSD", "CsI", DSD_thickness);
  qtelescopePID->SetEdepAccountingStrategy("DSD", ERQTelescopePID::EdepAccountingStrategy::EdepFromXChannel);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_1_Y", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_1_Y", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_2_X_4", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_2_X_4", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_3_Y_7", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_3_Y_7", 15, -0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_4_X_10", 0, 0.25);
  qtelescopeTrackFinder->SetTrackPositionCorrection("SSD_4_X_10", 15, -0.25);
  run->AddTask(qtelescopePID);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  ///FairParRootFileIo*  parIO = new FairParRootFileIo();
  //parIO->open(parFile.Data(), "UPDATE");
  //rtdb->setFirstInput(parIO);
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
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << Central_telescopeime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
