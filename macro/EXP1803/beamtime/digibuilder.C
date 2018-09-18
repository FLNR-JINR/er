void digibuilder(Int_t nEvents = 1000000)
{
  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  inputdir = inputdir + "/input/";

	TString inFile = inputdir + "run13_0037.lmd.root";
  TString confFile = inputdir + "setup2_exp201803.xml";

	// --- Specify output file name (this is just an example)
	TString outFile = "digi.root";

  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  builder->AddFile(inFile);

  ERBeamDetUnpack* beamDetUnpack = new ERBeamDetUnpack("Beam_detector");
  
  ERTelescopeUnpack* rtUnpack = new ERTelescopeUnpack("Right_telescope");
  rtUnpack->AddDoubleSiStation("SQ_R",
                               "SQX_R","tSQX_R",
                               "SQY_R","tSQY_R",
                               inputdir + "SQX_R.cal", inputdir + "SQY_R.cal",
                               "XY");
  rtUnpack->AddCsIStation("CsI_R","CsI_R","");

  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  ltUnpack->AddDoubleSiStation("SQ_L",
                               "SQX_L","tSQX_L",
                               "SQY_L","tSQY_L",
                               inputdir + "SQX_L.cal", inputdir + "SQY_L.cal",
                               "XY");
  ltUnpack->AddSingleSiStation("SQ300",
                               "SQ300","tSQ300",
                               inputdir + "SQ20.cal",
                               "X");
  ltUnpack->AddCsIStation("CsI_L","CsI_L","");
  

  builder->AddUnpack(beamDetUnpack);
  builder->AddUnpack(rtUnpack);
  builder->AddUnpack(ltUnpack);
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
  run->Run(0,nEvents);

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