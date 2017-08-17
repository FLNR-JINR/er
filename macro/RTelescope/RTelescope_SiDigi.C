void RTelescope_SiDigi(Int_t nEvents = 1000){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "digi.root";
  TString parFile = "par.root";
  TString parOutFile = "parOut.root";
  // ------------------------------------------------------------------------
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Digitization run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  fRun->SetEventHeader(header);
  //------------------------------------------------------------------------
  // ------------------------NeuRadDigitizer---------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information
  ERRTelescopeSiDigitizer* digitizer = new ERRTelescopeSiDigitizer(verbose);
  fRun->AddTask(digitizer);
  // ------------------------------------------------------------------------

  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

  FairParRootFileIo*  parInput = new FairParRootFileIo();
  parInput->open(parFile.Data(), "UPDATE");

  rtdb->setFirstInput(parInput);

  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  fRun->Init();
  fRun->Run(0, nEvents);
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
