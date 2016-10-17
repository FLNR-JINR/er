void NeuRad_digi_proof(Int_t nEvents = 10000){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "digi.root";
  TString parFile = "/home/vitaliy/er_proof/macro/NeuRad/par.root";
  // ------------------------------------------------------------------------
  
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------
  
  // -----   Digitization run   -------------------------------------------
  FairRunAnaProof *fRun= new FairRunAnaProof("workers=4");
  fRun->SetInputFile(inFile);
  fRun->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  fRun->SetEventHeader(header);
  //------------------------------------------------------------------------
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
  NeuRadDetDigiFile += "/parameters/NeuRad.digi.par";
  parInput2->open(NeuRadDetDigiFile.Data(),"in");
  rtdb->setSecondInput(parInput2);
  
  // -----   Intialise and run   --------------------------------------------
  /** Set PROOF output status, possibilities: "copy","merge"*/
  fRun->SetProofOutputStatus("merge");
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------

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
