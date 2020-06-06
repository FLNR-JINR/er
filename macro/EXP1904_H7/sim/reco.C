void reco(Int_t nEvents = 100000){
  //---------------------Files-----------------------------------------------
  TString inFile  = "sim_digi.root";
  TString outFile = "reco.root";
  TString parFile = "par.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  /*
  run->SetUserCut("Length$(ERQTelescopeSiPoint_Telescope_1_SingleSi_SSD_V_1_Y_2) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_2_SingleSi_SSD_V_2_X_5) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_3_SingleSi_SSD_V_3_Y_8) == 0 \
                   && Length$(ERQTelescopeSiDigi_Telescope_4_SingleSi_SSD_V_4_X_11) == 0");*/
  //run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  // ------------------------BeamDetTrackFinger--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("tubeD2");
  run->AddTask(trackFinder);
  // -----------------------BeamDetTrackPID----------------------------------s
  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERBeamDetPID* beamdetPid = new ERBeamDetPID(verbose);
  beamdetPid->SetBoxPID(0., 1000., 0., 1000.);
  beamdetPid->SetOffsetToF(0.);
  beamdetPid->SetProbabilityThreshold(0);
  beamdetPid->SetIonMass(7482.5396);
  beamdetPid->SetPID(1000020080);
  run->AddTask(beamdetPid);
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_X_0", "Telescope_1_SingleSi_SSD_1_Y_1");
  qtelescopeTrackFinder->SetHitStation("Telescope_2", "Telescope_2_SingleSi_SSD_2_X_4", "Telescope_2_SingleSi_SSD20_2_Y_3");
  qtelescopeTrackFinder->SetHitStation("Telescope_3", "Telescope_3_SingleSi_SSD20_3_X_6", "Telescope_3_SingleSi_SSD_3_Y_7");
  qtelescopeTrackFinder->SetHitStation("Telescope_4", "Telescope_4_SingleSi_SSD_4_X_10", "Telescope_4_SingleSi_SSD20_4_Y_9");
  //qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY_0");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY_0_X","Central_telescope_DoubleSi_DSD_XY_0_Y");
  run->AddTask(qtelescopeTrackFinder);
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
  qtelescopePID->SetParticle("Central_telescope_DoubleSi_DSD_XY_0_XCentral_telescope_DoubleSi_DSD_XY_0_Y",1000010030, "DSD", "CsI_0", DSD_thickness);
  //enum EdepAccountingStrategy {EdepFromXChannel, EdepFromYChannel, AverageEdep, SummarizedEdep};
  qtelescopePID->SetEdepAccountingStrategy("DSD", ERQTelescopePID::EdepAccountingStrategy::EdepFromYChannel);
  run->AddTask(qtelescopePID);
  // ------------------------ND track finder --------------------------------
  ERNDTrackFinder* NDTrackFinder = new ERNDTrackFinder();
  run->AddTask(NDTrackFinder);
  // ------------------------------------------------------------------------
  
  // ------------------------ND track finder --------------------------------
  ERNDPID* NDPID = new ERNDPID();
  run->AddTask(NDPID);
  // ------------------------------------------------------------------------

  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------;
  rtdb->setOutput(parIO);
  rtdb->saveOutput();
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
