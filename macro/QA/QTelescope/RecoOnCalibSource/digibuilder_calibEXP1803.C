void digibuilder_calibEXP1803(Int_t nEvents = 1443710)
{
  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  // inputdir = "~/soft/expertroot/macro/calibration/si_calibration/input/";
  inputdir = "./input/";

	TString inFile = inputdir + "dsd_20_l_03.lmd.root";
  TString confFile = "setup_exp1811.xml"; // check where is setup file is really stored

	// --- Specify output file name (this is just an example)
	TString outFile = "calibEXP1803.Digi.root";

  std::cout << ">>> input  file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;
  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  //builder->SetUserCut("Beam_detector_F3.@fDetMessages.GetEntriesFast() == 4",kFALSE);
  builder->AddFile(inFile);
// -------------------------------------------------------------------------------------
  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  // for the thin detector it nessesary to implement new mappin for pixelwise coefficients
  ltUnpack->AddSingleSiStation("SSD20_L",
                               "SSD20_L","tSSD20_L",
                               inputdir + "SSD20_L.cal", "", //inputdir + "tSSD20_L.cal",
                               "X");
  ltUnpack->AddDoubleSiStation("DSD_L",
                               "DSDX_L","tDSDX_L",
                               "DSDY_L","tDSDY_L",
                               inputdir + "calibCoeff_DSDX_L_dsd_l_02_0001.txt", "", //inputdir + "tDSDX_L.cal",
                               inputdir + "calibCoeff_DSDY_L_dsd_l_02_0001.txt", "", //inputdir + "tDSDY_L.cal",
                               "XY");
 // -------------------------------------------------------------------------------------  
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