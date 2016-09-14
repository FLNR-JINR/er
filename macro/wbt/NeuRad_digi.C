void NeuRad_digi(Int_t nEvents = 1000){
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
 
  // ------------------------NeuRadDigitizer---------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERNeuRadDigitizer* digitizer = new ERNeuRadDigitizer(verbose);
  digitizer->SetPixelThreshold(2.);
  fRun->AddTask(digitizer);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  
  FairParAsciiFileIo* parInput2 = new FairParAsciiFileIo();
  TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
  NeuRadDetDigiFile += "/parameters/NeuRad_wbt.digi.par";
  parInput2->open(NeuRadDetDigiFile.Data(),"in");
  rtdb->setSecondInput(parInput2);
  
  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------
  //FairParRootFileIo*  parIo2 = new FairParRootFileIo();
  //parIo2->open(parOutFile.Data());
  rtdb->setOutput(parIo1);
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
