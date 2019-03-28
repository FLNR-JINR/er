void recoAndSimCoordAndAng_track(Int_t nEvents = 10000) {
  //---------------------Files-----------------------------------------------
  TString inFile = "sim_digi.root";
  TString outFile = "track.root";
  TString parFile = "par.root";
  TString parOutFile = "parOut.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  // ------------------------BeamDetTrackFinder-----------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  trackFinder->SetTargetVolume("targetBodyH2");
  run->AddTask(trackFinder);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parInput);
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
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
  // ------------------------------------------------------------------------
}
