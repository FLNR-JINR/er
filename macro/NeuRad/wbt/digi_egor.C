void digi_egor(Int_t nEvents = 1000)
{
  //---------------------Files-----------------------------------------------
  TString workDir = "results/";
  TString inFile  = workDir + "sim.root";
  TString parFile = workDir + "par.root";
  TString outFile = workDir + "digi.root";
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // -----   Digitization run   ---------------------------------------------
  FairRunAna* run = new FairRunAna();
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------

  //------   Set MC event header   ------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  //-------------------------------------------------------------------------

  // -----   NeuRad digitized   ---------------------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information
  ERNeuRadDigitizer* digitizer = new ERNeuRadDigitizer(verbose);
  digitizer->SetUseCrosstalks(kFALSE);
  digitizer->SetPixelJitter(0.4/2.36);
  run->AddTask(digitizer);
  // ------------------------------------------------------------------------

  // -----------Runtime DataBase info -------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  FairParAsciiFileIo* parInput1 = new FairParAsciiFileIo();

  TString erPath = gSystem->Getenv("VMCWORKDIR");

  TString NeuRadDetDigiFile = erPath + "/parameters/NeuRad_wbt.digi.par";
  parInput1->open(NeuRadDetDigiFile.Data(), "in");

  FairParRootFileIo* parInput2 = new FairParRootFileIo();
  parInput2->open(parFile.Data(), "UPDATE");

  rtdb->setFirstInput(parInput1);
  rtdb->setSecondInput(parInput2);

  //-------Set LOG verbosity  -----------------------------------------------
  FairLogger::GetLogger()->SetLogVerbosityLevel("HIGH");

  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG2");
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------
  rtdb->setOutput(parInput2);
  rtdb->saveOutput();

  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is " << outFile.Data() << "." << endl;
  cout << "Parameter file is " << parFile.Data() << "." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime
       << " s" << endl << endl;
}
