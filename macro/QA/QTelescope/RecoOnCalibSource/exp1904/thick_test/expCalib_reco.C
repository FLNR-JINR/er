void expCalib_reco(){
  //---------------------Files-----------------------------------------------
  // TString inFile = "../input/calib_1mm_90_1to8_digi_quad.root";
  TString inFile = "../input/calib_1mm_90_1to8_digi_lin.root";
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  Int_t nEvents = tree->GetEntriesFast();//1443710:
  // nEvents = 20;
  TString parFile = "par_Calib.root";
  TString geoFile = "geo_expCalib.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  TString userCut;
  TString outFile;
  outFile.Form("expCalib_reco_1.root");
  run->SetGeomFile("geo_expCalib.root");
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  Int_t verbose = 1;
  // ------- QTelescope TrackFinder -------------------------------------------
  ERQTelescopeTrackFinder* qtelescopeTrackFinder = new ERQTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  //qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_Y_0",
  //                                                    "Telescope_1_SingleSi_SSD_1_X_1");
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD_1_X_0", "Fictious");
  qtelescopeTrackFinder->SetStripEdepRange(0.65e-3, 8.35e-3);          // [GeV]
  //qtelescopeTrackFinder->SetStripEdepRange(0.0097, 100.);   // [GeV]
  //qtelescopeTrackFinder->SetEdepDiffXY(5.);                 // [GeV]
  //qtelescopeTrackFinder->SetEdepMaxDiffXY(4e-3); 
  qtelescopeTrackFinder->SetEdepMaxDiffXY(1000); 
  run->AddTask(qtelescopeTrackFinder);

  // ------- QTelescope PID -------------------------------------------
  ERQTelescopePID* qtelescopePID = new ERQTelescopePID(verbose);

  //qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD20_1_Y_0Telescope_1_SingleSi_SSD_1_X_1", 1000020040);
  qtelescopePID->SetStationParticle("Telescope_1_SingleSi_SSD_1_X_0Fictious", 1000020040);

  run->AddTask(qtelescopePID); 
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parInput);
  // -----   Intialise and run   --------------------------------------------
  // FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  //FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  //run->HoldEventsCount();
  run->MarkFill(kFALSE);
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------;
  rtdb->setOutput(parInput);
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

  // ---------
}
