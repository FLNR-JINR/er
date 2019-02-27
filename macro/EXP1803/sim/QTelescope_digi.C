void QTelescope_digi(Int_t nEvents = 100){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "digiQTele.root";
  TString parFile = "par.root";
  TString parOutFile = "parOut.root";
  // ------------------------------------------------------------------------
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  //------------------------------------------------------------------------
  // ------------------------BeamDetDigitizer---------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERQTelescopeDigitizer* digitizer = new ERQTelescopeDigitizer(verbose);
  digitizer->SetSiElossThreshold(0);
  digitizer->SetSiElossSigma(0);
  digitizer->SetSiTimeSigma(0);

  digitizer->SetCsIElossThreshold(0);
  digitizer->SetCsIElossSigma(0);
  digitizer->SetCsITimeSigma(0);

  run->AddTask(digitizer);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");
  
  rtdb->setFirstInput(parInput);
  
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  //FairParRootFileIo*  parIo2 = new FairParRootFileIo();
  //parIo2->open(parOutFile.Data());
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
