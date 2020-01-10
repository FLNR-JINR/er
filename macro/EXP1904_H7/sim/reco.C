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
  run->SetUserCut("Length$(ERQTelescopeSiPoint_T1_SingleSi_SSD_veto_Y_2) == 0 \
                   && Length$(ERQTelescopeSiDigi_T2_SingleSi_SSD_veto_X_5) == 0 \
                   && Length$(ERQTelescopeSiDigi_T3_SingleSi_SSD_veto_Y_8) == 0 \
                   && Length$(ERQTelescopeSiDigi_T4_SingleSi_SSD_veto_X_11) == 0");
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
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetHitStation("T1", "T1_SingleSi_SSD20_X_0", "T1_SingleSi_SSD_Y_1");
  qtelescopeTrackFinder->SetHitStation("T2", "T2_SingleSi_SSD_X_4", "T2_SingleSi_SSD20_Y_3");
  qtelescopeTrackFinder->SetHitStation("T3", "T3_SingleSi_SSD20_X_6", "T3_SingleSi_SSD_Y_7");
  qtelescopeTrackFinder->SetHitStation("T4", "T4_SingleSi_SSD_X_10", "T4_SingleSi_SSD20_Y_9");
  qtelescopeTrackFinder->SetHitStation("CT", "CT_DoubleSi_DSD_XY_0");
  run->AddTask(qtelescopeTrackFinder);
  // -----------------------BeamDetTrackPID----------------------------------s
  Int_t Z = 2, A = 8, Q = 2;
  TString ionName = "8He";
  ERBeamDetPID* beamdetPid = new ERBeamDetPID(verbose);
  beamdetPid->SetBoxPID(0., 1000., 0., 1000.);
  beamdetPid->SetOffsetToF(0.);
  beamdetPid->SetProbabilityThreshold(0);
  beamdetPid->SetIonMass(6.5696366);
  beamdetPid->SetPID(1000020080);
  run->AddTask(beamdetPid);
  // ------   QTelescope TrackPID -----------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);
  qtelescopePID->SetStationParticle("T1_SingleSi_SSD20_X_0T1_SingleSi_SSD_Y_1", 1000020030);
  qtelescopePID->SetStationParticle("T2_SingleSi_SSD_X_4T2_SingleSi_SSD20_Y_3", 1000020030);
  qtelescopePID->SetStationParticle("T3_SingleSi_SSD20_X_6T3_SingleSi_SSD_Y_7", 1000020030);
  qtelescopePID->SetStationParticle("T4_SingleSi_SSD_X_10T4_SingleSi_SSD20_Y_9", 1000020030);
  qtelescopePID->SetStationParticle("CT_DoubleSi_DSD_XY_0", 1000010030);
  run->AddTask(qtelescopePID);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
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
