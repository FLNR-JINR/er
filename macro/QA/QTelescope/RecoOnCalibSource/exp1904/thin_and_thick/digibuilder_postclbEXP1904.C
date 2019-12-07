void digibuilder_postclbEXP1904(Int_t nEvents = 1443710) {
  //TString intdir_converted = "/opt/keeper/stock/kozlov/calib201907/digibulder/alltel_90/";
  TString intdir_converted = "/opt/keeper/stock/kozlov/calib201907/digibulder/calib_90_all/";
  //TString intdir_converted = "../input/";
  TString intdir_settings = "../input/";

  //TString rootAqqDaqFile = "alltel_90_1to8.root";
  //TString rootAqqDaqFile = "alltel_90.lml.root";
  TString rootAqqDaqFile = "calib_90_all.lml.root";

  TString confFile = "../input/setupEXP1904.xml"; // check where is setup file is really stored
  auto file = TFile::Open((intdir_converted + rootAqqDaqFile).Data());
  auto tree = (TTree*)file->Get("stepRepackingxTree");
  nEvents = tree->GetEntriesFast();//1443710

  // --- Specify output file name (this is just an example)
  //TString outFile = "alltel_90.root";
  TString outFile = "calib_90_all.root";

  std::cout << ">>> input  file is " << rootAqqDaqFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;
  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  builder->AddFile(intdir_converted + rootAqqDaqFile);
// -------------------------------------------------------------------------------------
  ERTelescopeUnpack* telescope_1 = new ERTelescopeUnpack("Telescope_1");
  ERTelescopeUnpack* telescope_2 = new ERTelescopeUnpack("Telescope_2");
  // for the thin detector it nessesary to implement new mappin for pixelwise coefficients
  telescope_1->AddSingleSiStation("SSD_1",
                                  "SSD_1", "tSSD_1",
                                  intdir_settings + "SSD_1m_1.cal", "",
                                  "Y",
				  kFALSE);
  telescope_2->AddSingleSiStation("SSD_2",
                                  "SSD_2", "tSSD_2",
                                  intdir_settings + "SSD_1m_2.cal", "",
                                  "X",
				  kFALSE);
 // -------------------------------------------------------------------------------------  
  telescope_1->AddSingleSiStation("SSD20_1",
                                  "SSD20_1", "tSSD20_1",
                                  intdir_settings + "SSD_20u_1.cal", "",
                                  "X",
                                  kFALSE);
  telescope_2->AddSingleSiStation("SSD20_2",
                                  "SSD20_2", "tSSD20_2",
                                  intdir_settings + "SSD_20u_2.cal", "",
                                  "Y",
                                  kFALSE);
  builder->AddUnpack(telescope_1);
  builder->AddUnpack(telescope_2);
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
  run->Run(0, nEvents);

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
