void digi1(Int_t nEvents = 400000){
  //---------------------Files-----------------------------------------------
  TString inFile = "/home/muzalevsky/work/dataER/simNeuRad/simСoll1.root";
  TString outFile = "/home/muzalevsky/work/dataER/simNeuRad/6000_1.94_Cross/digi1.root";
  TString parFile = "/home/muzalevsky/work/dataER/simNeuRad/parColl1.root";
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
  ERNeuRadDigitizer* digitizer = new ERNeuRadDigitizer(verbose);
  digitizer->SetUseCrosstalks(kTRUE);
  digitizer->SetPixelJitter(0.28/2.36);
  fRun->AddTask(digitizer);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParAsciiFileIo* parInput1 = new FairParAsciiFileIo();
  TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
  NeuRadDetDigiFile += "/parameters/NeuRad.digi.v4.par";
  parInput1->open(NeuRadDetDigiFile.Data(),"in");

  FairParRootFileIo*  parInput2 = new FairParRootFileIo();
  parInput2->open(parFile.Data(), "UPDATE");
  
  rtdb->setFirstInput(parInput1);
  rtdb->setSecondInput(parInput2);
  
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------
  rtdb->setOutput(parInput2);
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
