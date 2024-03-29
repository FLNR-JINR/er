void digibuilder(Int_t nEvents = 1000000, 
                  TString inFile = "run13_0037.lmd.root",
                  TString confFile = "setup2_exp201803.xml",
                  TString outFile = "digi.root" )
{
  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  inputdir = inputdir + "/input/";

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
                               inputdir + "SQX_R.cal",inputdir + "tSQX_R.cal",
                               inputdir + "SQY_R.cal",inputdir + "tSQY_R.cal",
                               "XY",
                               kFALSE);
  rtUnpack->AddCsIStation("CsI_R","CsI_R","tCsI_R",inputdir + "csi_r_ec.clb","",kFALSE);

  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  ltUnpack->AddDoubleSiStation("SQ_L",
                               "SQX_L","tSQX_L",
                               "SQY_L","tSQY_L",
                               inputdir + "SQX_L.cal", inputdir + "tSQX_L.cal",
                               inputdir + "SQY_L.cal", inputdir + "tSQY_L.cal",
                               "XY",kFALSE);
  ltUnpack->AddSingleSiStation("SQ300",
                               "SQ300","tSQ300",
                               inputdir + "SQ20.cal", inputdir + "tSQ20.cal",
                               "X",kFALSE);
  ltUnpack->AddCsIStation("CsI_L","CsI_L","tCsI_L","","",kFALSE);
  

  builder->AddUnpack(beamDetUnpack);
  builder->AddUnpack(rtUnpack);
  builder->AddUnpack(ltUnpack);
  // --- Run
  FairRunOnline *run = new FairRunOnline(builder);
  run->SetOutputFile(outFile);

  ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();
  run->SetEventHeader(header);

  // -----   Logger settings --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");

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