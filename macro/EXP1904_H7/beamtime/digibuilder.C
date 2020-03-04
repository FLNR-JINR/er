void digibuilder(Int_t sEvent = 0 , Int_t fEvent = 300000, TString outFile = "digi.root") {
  TString confFile = "setupEXP1904_new.xml";
  TString inFile = "h7_ct_41_0013.root";

  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  builder->AddFile(inFile);

  ERNDUnpack* ndUnpack = new ERNDUnpack("ND", "ND_AMP", "ND_TIME", "ND_TAC",
                                        ""/*amp cal file*/, ""/*time cal file*/, ""/*tac cal file*/, kFALSE);
  builder->AddUnpack(ndUnpack);

  // --- Run
  FairRunOnline *run = new FairRunOnline(builder);
  run->SetOutputFile(outFile);

  ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();
  run->SetEventHeader(header);

    // -----   Logger settings --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> Starting run..." << std::endl;
  run->Init();
  run->Run(sEvent,fEvent);

  timer.Stop();
  
  // --- End-of-run info
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << ">>> Macro finished successfully." << std::endl;
  std::cout << ">>> Output file is " << outFile << std::endl;
  std::cout << ">>> Real time " << rtime << " s, CPU time "
            << ctime << " s" << std::endl;

}