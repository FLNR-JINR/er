void exp1803_reco(Int_t nEvents = 100) {
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "reco.root";
  TString parFile = "par.root";
  TString parOutFile = "parOut.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run= new ERRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  

   ERBeamDetDigitizer* beamDetDigitizer = new ERBeamDetDigitizer(1);
  // beamDetDigitizer->SetMWPCElossThreshold(0.006);
  // beamDetDigitizer->SetToFElossThreshold(0.006);
  // beamDetDigitizer->SetToFElossSigmaOverEloss(0);
  // beamDetDigitizer->SetToFTimeSigma(1e-10);
  run->AddTask(beamDetDigitizer);


  // ------------------------BeamDetDigitizer--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  run->AddTask(trackFinder);
  // -----------------------BeamDetTrackPID----------------------------------
  Int_t Z = 2, A = 6, Q = 2;
  TString ionName = "6He";
  ERBeamDetPID* pid = new ERBeamDetPID(verbose);
  pid->SetIonMass(5.60554);
  pid->SetBoxPID(0., 1000., 0., 1000.);
  pid->SetOffsetToF(0.);
  pid->SetProbabilityThreshold(0);
  pid->SetPID(1000020060);
  run->AddTask(pid);  
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
