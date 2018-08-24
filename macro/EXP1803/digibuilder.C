void digibuilder(Int_t nEvents = 20)
{
  TString workdir = gSystem->Getenv("VMCWORKDIR");
	TString inFile = workdir + "/input/run13_0037.lmd.root";
  TString confFile = workdir + "/input/setup2_exp201803.xml";

	// --- Specify output file name (this is just an example)
	TString outFile = "run13_0037.root";

  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  builder->AddFile(inFile);

  ERBeamDetUnpack* beamDetUnpack = new ERBeamDetUnpack("Beam_detector");
  builder->AddUnpack(beamDetUnpack);
  // --- Run
  FairRunOnline *run = new FairRunOnline(builder);
  run->SetOutputFile(outFile);

  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> Starting run..." << std::endl;
  run->Init();
  run->Run(0,1);

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