void Gadast_digi(Int_t nEvents = 100){
  //---------------------Files-----------------------------------------------
  TString inFile = "sim.root";
  TString outFile = "digiGadast.root";
  TString parFile = "par.root";
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
  ERGadastDigitizer* digitizer = new ERGadastDigitizer(verbose);
  fRun->AddTask(digitizer);
  // ------------------------------------------------------------------------
  
  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIo1);
  
  FairParAsciiFileIo* parInput2 = new FairParAsciiFileIo();
  TString GadastDetDigiFile = gSystem->Getenv("VMCWORKDIR");
  GadastDetDigiFile += "/parameters/Gadast.digi.par";
  parInput2->open(GadastDetDigiFile.Data(),"in");
  rtdb->setSecondInput(parInput2);
  
  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  // ------------------------------------------------------------------------
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
