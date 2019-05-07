void digibuilder_exp1811(Int_t nEvents = 100000)
{
  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  inputdir = inputdir + "/showBeam_VC/raw/";

	TString inFile = inputdir + "he8_10_0010.root";
  TString confFile = inputdir + "setup_exp1811.xml";

  // --- Specify output file name (this is just an example)
  TString outFile = "~/Acculinna/ER/showBeam_VC/analysed/he8_10_0010.Ivan.root";

  std::cout << ">>> input  file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  TString cut("");
  cut+=("DetEventCommon.trigger!=1 && ");
  cut+=("Beam_detector_MWPC1.@fData.GetEntries()>0 && ");
  //cut+=("Beam_detector_MWPC1.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC2.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC3.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC4.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_F3.@fData.GetEntries()==4 && ");
  cut+=("Beam_detector_F5.@fData.GetEntries()==4");

  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);
  //builder->SetUserCut("Beam_detector_F3.@fData.GetEntriesFast() == 4",kFALSE);
  builder->SetUserCut(cut.Data(), kFALSE);
  builder->AddFile(inFile);

// -------------------------------------------------------------------------------------
  ERBeamDetUnpack* beamDetUnpack = new ERBeamDetUnpack("Beam_detector");
  beamDetUnpack->SetTimeCalibrationConst(0.0625);
// -------------------------------------------------------------------------------------
 
  ERTelescopeUnpack* rtUnpack = new ERTelescopeUnpack("Right_telescope");
  rtUnpack->AddSingleSiStation("SSD20_R",
                               "SSD20_R","tSSD20_R",
                               inputdir + "SSD20_R.cal", "", //inputdir + "tSSD20_R.cal",
                               "X");

  rtUnpack->AddSingleSiStation("SSDY_R",
                               "SSDY_R","tSSDY_R",
                               inputdir + "SSDY_R.cal", "", // inputdir + "tSSDY_R.cal",
                               "Y");

  rtUnpack->AddSingleSiStation("SSD_R",
                               "SSD_R","tSSD_R",
                               inputdir + "SSD_R.cal", "", //inputdir + "tSSD_R.cal",
                               "Y");
// -------------------------------------------------------------------------------------
  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  ltUnpack->AddSingleSiStation("SSD20_L",
                               "SSD20_L","tSSD20_L",
                               inputdir + "SSD20_L.cal", "", //inputdir + "tSSD20_L.cal",
                               "X");

  ltUnpack->AddDoubleSiStation("DSD_L",
                               "DSDX_L","tDSDX_L",
                               "DSDY_L","tDSDY_L",
                               inputdir + "DSDX_L.cal", "", //inputdir + "tDSDX_L.cal",
                               inputdir + "DSDY_L.cal", "", //inputdir + "tDSDY_L.cal",
                               "XY");
 
  ltUnpack->AddSingleSiStation("SSD_L",
                               "SSD_L","tSSD_L",
                               inputdir + "SSD_L.cal", "", //inputdir + "tSSD_L.cal",
                               "X");
  
// -------------------------------------------------------------------------------------
  ERTelescopeUnpack* ctUnpack = new ERTelescopeUnpack("Central_telescope");
  ctUnpack->AddDoubleSiStation("DSD_C",
                               "DSDX_C","tDSDX_C",
                               "DSDY_C","tDSDY_C",
                               inputdir + "DSDX_C.cal", "", //inputdir + "tDSDX_C.cal",
                               inputdir + "DSDY_C.cal", "", //inputdir + "tDSDY_C.cal",
                               "XY");

  ctUnpack->AddCsIStation("CsI","CsI","tCsI",inputdir + "CsI.cal","");
// -------------------------------------------------------------------------------------  
  builder->AddUnpack(beamDetUnpack);
  builder->AddUnpack(rtUnpack);
  builder->AddUnpack(ltUnpack);
  builder->AddUnpack(ctUnpack);  

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