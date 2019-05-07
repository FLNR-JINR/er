void exp1811_reco(Int_t nEvents = 10000000){
  //---------------------Files-----------------------------------------------
  TString inFile  = "sim_digi.root";
  TString outFile = "sim_reco.root";
  TString parFile = "par.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  //run->SetUserCut("EventHeader.fTrigger==1");
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
  // -----------------------BeamDetTrackPID----------------------------------
  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERBeamDetPID* pid = new ERBeamDetPID(verbose);
  pid->SetBoxPID(0., 1000., 0., 1000.);
  pid->SetOffsetToF(0.);
  pid->SetProbabilityThreshold(0);
  pid->SetIonMass(7.5061760); //????
  pid->SetPID(1000020080);
  run->AddTask(pid);
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);

  qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_SingleSi_SSD20_L_X_0");
  qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_DoubleSi_DSD_L_XY_0");
  qtelescopeTrackFinder->SetHitStation("Left_telescope", "Left_telescope_SingleSi_SSD_L_X_0");

  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_C_XY_0");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_CsI_0");

  qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSD20_R_X_0");
  qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSDY_R_Y_0");
  qtelescopeTrackFinder->SetHitStation("Right_telescope","Right_telescope_SingleSi_SSD_R_Y_0");

  qtelescopeTrackFinder->SetStripEdepRange(0., 100.);          // [GeV]
  //qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  //qtelescopeTrackFinder->SetStripEdepRange(0.0097, 100.);   // [GeV]
  //qtelescopeTrackFinder->SetEdepDiffXY(5.);                 // [GeV]
  qtelescopeTrackFinder->SetEdepMaxDiffXY(0.5); 
  run->AddTask(qtelescopeTrackFinder);
  // ------- QTelescope PID -------------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);

  // qtelescopePID->SetUserCut("ERQTelescopeSiDigi_T2_DoubleSi_SD2_XY_1_X.fEdep>0.009");

  qtelescopePID->SetStationParticle("Central_telescope_DoubleSi_DSD_C_XY_0", 1000010030);
  qtelescopePID->SetStationParticle("Central_telescope_CsI_0", 1000010030);

  qtelescopePID->SetStationParticle("Left_telescope_SingleSi_SSD20_L_X_0", 1000020030);
  qtelescopePID->SetStationParticle("Left_telescope_DoubleSi_DSD_L_XY_0", 1000020030);
  qtelescopePID->SetStationParticle("Left_telescope_SingleSi_SSD_L_X_0", 1000020030);

  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSD20_R_X_0", 1000020030);
  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSDY_R_Y_0", 1000020030);
  qtelescopePID->SetStationParticle("Right_telescope_SingleSi_SSD_R_Y_0", 1000020030);

  run->AddTask(qtelescopePID); 
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
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
