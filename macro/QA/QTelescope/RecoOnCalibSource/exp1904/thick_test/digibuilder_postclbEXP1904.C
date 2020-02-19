void digibuilder_postclbEXP1904(Int_t nEvents = 1443710) {
  TString intdir_converted = "/opt/keeper/stock/kozlov/calib201907/digibulder/calib_65/";
  TString intdir_settings = "../input/";
  TString rootAqqDaqFile = "calib_65_1to10.lmd.root";
  TString confFile = intdir_settings + "setupEXP1904.xml"; 
  auto file = TFile::Open((intdir_converted + rootAqqDaqFile).Data());
  auto tree = (TTree*)file->Get("stepRepackingxTree");
  nEvents = tree->GetEntriesFast(); 
  //nEvents = 100;
  // --- Specify output file name (this is just an example)
  TString outFile = "calib_65_1to10_digi.root";
  std::cout << ">>> input  file is " << rootAqqDaqFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;
  // --- Source task --------------------------------------------------------
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  builder->AddFile(intdir_converted + rootAqqDaqFile);
  // ------------------------------------------------------------------------
  ERTelescopeUnpack* telescope_1 = new ERTelescopeUnpack("Telescope_1");
  telescope_1->AddDoubleSiStation("SSD20_1",
                                  "SSD20_1","tSSD20_1",
                                  "SSD20_1","tSSD20_1",
                                  intdir_settings + "SSD_20u_1_quad_mean.cal", "",
                                  intdir_settings + "SSD_20u_1_zero.cal", "",
                                  "XY",
				  kFALSE);
  builder->AddUnpack(telescope_1);
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
