TString getFileNameFromPath (TString const &path) {
  TString fileName(path(path.Last('/') + 1, path.Length())); // file name with extension
  // remove file extension. Extension is considered as part of the name after last "." symbol
  fileName.Remove(fileName.Last('.'), fileName.Length());    
  return fileName;
}

void digibuilder_postclbEXP1904(Int_t nEvents = 1443710)
{

  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  // inputdir = "~/soft/expertroot/macro/calibration/si_calibration/input/";
  gSystem->Exec("mkdir -p ./input");
  inputdir = "./mnt/data_nas/exp201904/data/postclb/";

  vector<TString> lmdFiles = {
                               "alltel_90_0001.lmd",
                               "alltel_90_0002.lmd",
                               "alltel_90_0003.lmd",
                               "alltel_90_0004.lmd",
                               "alltel_90_0005.lmd",
                               "alltel_90_0006.lmd",
                               "alltel_90_0007.lmd",
                               "alltel_90_0008.lmd"
                             };

  TString confFile = "./input/setupEXP1904.xml"; // check where is setup file is really stored
  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("stepRepackingxTree");
  nEvents = tree->GetEntriesFast();//1443710

	// --- Specify output file name (this is just an example)
	TString outFile = "alltel_90_7f_db_1.root";

  std::cout << ">>> input  file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;
  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  //builder->SetUserCut("Beam_detector_F3.@fDetMessages.GetEntriesFast() == 4",kFALSE);
  for (auto const &itFile: lmdFiles) {
    builder->AddFile(inputdir + itFile);
  }
// -------------------------------------------------------------------------------------
  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  // for the thin detector it nessesary to implement new mappin for pixelwise coefficients
  ltUnpack->AddSingleSiStation("SSD20_1",
                               "SSD20_1","tSSD20_1",
                               inputdir + "SSD20_1_geant.cal", "", //inputdir + "tSSD20_L.cal",
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