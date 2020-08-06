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
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  // ------- QTelescope TrackFinder -------------------------------------------
  Int_t verbose = 0;
  ERTelescopeTrackFinder* qtelescopeTrackFinder = new ERTelescopeTrackFinder(verbose);
  qtelescopeTrackFinder->SetTargetPoint(0., 0., 0.);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_DoubleSi_R_XY_X", "Telescope_1_DoubleSi_R_XY_Y");
  run->AddTask(qtelescopeTrackFinder);
  // ------   QTelescope TrackPID -----------------------------------------
  ERTelescopePID* qtelescopePID = new ERTelescopePID(verbose);
  qtelescopePID->SetParticle("Telescope_1_DoubleSi_R_XY_XTelescope_1_DoubleSi_R_XY_Y", 2212);
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
  Double_t Central_telescopeime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << Central_telescopeime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
