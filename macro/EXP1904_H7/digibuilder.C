TString MakeConditionBeamID();
TString MakeConditionMWPCtime();

#include "./cuts/beamCut.c"

void digibuilder(TString InFile, TString OutFile) {

  beamCut();

  TString inputdir = gSystem->Getenv("VMCWORKDIR");

  inputdir = "./parameters/";

  TString confFile = "./setupEXP1904_final.xml";

  TChain *ch = new TChain("accdaq");
  ch->Add(InFile.Data());
  Int_t nEvents = ch->GetEntries();
  //delete ch;
  cout << nEvents << " will be processed" << endl;
  std::cout << ">>> input file is " << InFile  << std::endl;
  std::cout << ">>> output file is " << OutFile << std::endl;

  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  
  // UserCuts
  TString cut("");
  cut+=("common.trigger!=1 &&");

  cut.Append((TString)cutF5->GetName() + " && ");
  cut.Append((TString)cutF3->GetName());
  cut.Append(" && " + MakeConditionMWPCtime());
  cut.Append(" && Sum$(MWPC1 > 0.) > 0.");
  cut.Append(" && Sum$(MWPC2 > 0.) > 0.");
  cut.Append(" && Sum$(MWPC3 > 0.) > 0.");
  cut.Append(" && Sum$(MWPC4 > 0.) > 0.");
  cut.Append(" && Sum$(tMWPC > 0.) == 4");
  cut.Append(" && Sum$(tF3 > 0.) == 4");
  cut.Append(" && Sum$(tF5 > 0.) == 4");
  
  // cut+=("Telescope_1_tSSD_V_1.@fData.GetEntries()==0 && ");
  // cut+=("Telescope_2_tSSD_V_2.@fData.GetEntries()==0 && ");
  // cut+=("Telescope_3_tSSD_V_3.@fData.GetEntries()==0 && ");
  // cut+=("Telescope_4_tSSD_V_4.@fData.GetEntries()==0");

  cout << " Its my cut " << endl;
  cout << cut.Data() << endl;
  builder->SetUserCut(cut.Data(),kFALSE);

  builder->AddFile(InFile);

  ERBeamDetUnpack* beamDetUnpack = new ERBeamDetUnpack("Beam_detector");
  beamDetUnpack->SetToFCalibration(0.0625, 0., 0.0625, 0.); //aF3, bF3, aF5, bF5
  beamDetUnpack->SetMWPCCalibration(0.0625,0.);
  
  std::map<ushort, ushort> inverse16Mapping = {
    {0, 15},
    {1, 14},
    {2, 13},
    {3, 12},
    {4, 11},
    {5, 10},
    {6, 9},
    {7, 8},
    {8, 7},
    {9, 6},
    {10, 5},
    {11, 4},
    {12, 3},
    {13, 2},
    {14, 1},
    {15, 0}
  };

  std::map<ushort, ushort> sq20_3Mapping = {
    {0, 0},
    {1, 1},
    {2, 2},
    {3, 3},
    {4, 4},
    {5, 5},
    {6, 6},
    {7, 8},
    {8, 7},
    {9, 9},
    {10, 10},
    {11, 11},
    {12, 12},
    {13, 13},
    {14, 14},
    {15, 15}
  };

  ERTelescopeUnpack* t1Unpack = new ERTelescopeUnpack("Telescope_1");
  t1Unpack->AddSingleSiStation("SSD20_1",
                               "SSD20_1","tSSD20_1",
                               inputdir + "SSD_20u_1_cal.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "X",&inverse16Mapping);
  t1Unpack->AddSingleSiStation("SSD_1",
                               "SSD_1","tSSD_1",
                               inputdir + "SSD_1m_1_cal.cal",inputdir + "mtdc32_pars.cal",
                               "Y",&inverse16Mapping);
  t1Unpack->AddSingleSiStation("SSD_V_1",
                               "SSD_V_1","tSSD_V_1",
                               inputdir + "SSD_1m_1_v_cal.cal",inputdir + "mtdc32_pars.cal",
                               "Y",&inverse16Mapping);

  ERTelescopeUnpack* t2Unpack = new ERTelescopeUnpack("Telescope_2");
  t2Unpack->AddSingleSiStation("SSD20_2",
                               "SSD20_2","tSSD20_2",
                               inputdir + "SSD_20u_2_cal.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "Y",&inverse16Mapping);
  t2Unpack->AddSingleSiStation("SSD_2",
                               "SSD_2","tSSD_2",
                               inputdir + "SSD_1m_2_v_cal.cal",inputdir + "mtdc32_pars.cal",
                               "X");
  t2Unpack->AddSingleSiStation("SSD_V_2",
                               "SSD_V_2","tSSD_V_2",
                               inputdir + "SSD_1m_2_v_cal.cal",inputdir + "mtdc32_pars.cal",
                               "X");

  ERTelescopeUnpack* t3Unpack = new ERTelescopeUnpack("Telescope_3");
  t3Unpack->AddSingleSiStation("SSD20_3",
                               "SSD20_3","tSSD20_3",
                               inputdir + "SSD_20u_3_cal.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "X",&sq20_3Mapping);
  t3Unpack->AddSingleSiStation("SSD_3",
                               "SSD_3","tSSD_3",
                               inputdir + "SSD_1m_3_cal.cal",inputdir + "mtdc32_pars.cal",
                               "Y");
  t3Unpack->AddSingleSiStation("SSD_V_3",
                               "SSD_V_3","tSSD_V_3",
                               inputdir + "SSD_1m_3_v_cal.cal",inputdir + "mtdc32_pars.cal",
                               "Y");

  ERTelescopeUnpack* t4Unpack = new ERTelescopeUnpack("Telescope_4");
  t4Unpack->AddSingleSiStation("SSD20_4",
                               "SSD20_4","tSSD20_4",
                               inputdir + "SSD_20u_4_cal.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "Y");
  t4Unpack->AddSingleSiStation("SSD_4",
                               "SSD_4","tSSD_4",
                               inputdir + "SSD_1m_4_cal.cal",inputdir + "mtdc32_pars.cal",
                               "X",&inverse16Mapping);
  t4Unpack->AddSingleSiStation("SSD_V_4",
                               "SSD_V_4","tSSD_V_4",
                               inputdir + "SSD_1m_4_v_cal.cal",inputdir + "mtdc32_pars.cal",
                               "X",&inverse16Mapping);

  ERTelescopeUnpack* ctUnpack = new ERTelescopeUnpack("Central_telescope");
  ctUnpack->AddDoubleSiStation("DSD",
                               "DSDX","tDSDX",
                               "DSDY","tDSDY",
                               inputdir + "DSSD_X.cal",inputdir + "mtdc32_cent_pars.cal",
                               inputdir + "DSSD_Y.cal",inputdir + "mtdc32_cent_pars.cal",
                               "XY");

  std::map<ushort, ushort> csiMapping = {
    {0, 15},
    {1, 11},
    {2, 7},
    {3, 3},
    {4, 14},
    {5, 10},
    {6, 6},
    {7, 2},
    {8, 13},
    {9, 9},
    {10, 5},
    {11, 1},
    {12, 12},
    {13, 8},
    {14, 4},
    {15, 0}
  };
  TString csi_cal = "CsI_anh.cal";
  if (InFile.Contains("h7_ct_0")) {
    csi_cal = "CsI_0.cal";
  } else if(InFile.Contains("h7_ct_1")) {
    csi_cal = "CsI_1.cal";
  } else if(InFile.Contains("h7_ct_2")) {
    csi_cal = "CsI_2.cal";
  } else if(InFile.Contains("h7_ct_3")) {
    csi_cal = "CsI_3.cal";
  } else if(InFile.Contains("h7_ct_4")) {
    csi_cal = "CsI_4.cal";
  } else if(InFile.Contains("h7_ct_5") || InFile.Contains("h7_ct_6")) {
    csi_cal = "CsI_5.cal";
  }
  ctUnpack->AddCsIStation("CsI","CsI","tCsI",inputdir + csi_cal,inputdir + "V775_caen_tdc_pars.cal", 
                          &csiMapping);


  ERNDUnpack* ndUnpack = new ERNDUnpack("ND", "ND_AMP", "ND_TIME", "ND_TAC",
                                        "", "", inputdir + "ND_tac.cal");
  
 
  builder->AddUnpack(beamDetUnpack);
  builder->AddUnpack(t1Unpack);
  builder->AddUnpack(t2Unpack);
  builder->AddUnpack(t3Unpack);
  builder->AddUnpack(t4Unpack);
  builder->AddUnpack(ctUnpack);
  builder->AddUnpack(ndUnpack);
  // --- Run
  FairRunOnline *run = new FairRunOnline(builder);
  run->SetOutputFile(OutFile);

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
  std::cout << ">>> Output file is " << OutFile << std::endl;
  std::cout << ">>> Real time " << rtime << " s, CPU time "
            << ctime << " s" << std::endl;
}

TString MakeConditionMWPCtime() {

  ////////////////////////////////////////////
  //time in MWPC
  ////////////////////////////////////////////

  TString timeF5 = "0.0625*0.25*(tF5[0] + tF5[1] + tF5[2] + tF5[3])";

  TString tMWPCX1 = "0.0625*tMWPC[0]";
  TString tMWPCY1 = "0.0625*tMWPC[1]";
  TString tMWPCX2 = "0.0625*tMWPC[2]";
  TString tMWPCY2 = "0.0625*tMWPC[3]";

  TString condition = "( ("+tMWPCX1+"-"+timeF5+")>77. && ("+tMWPCX1+"-"+timeF5+")<98."
                      +" && ("+tMWPCY1+"-"+timeF5+")>80. && ("+tMWPCY1+"-"+timeF5+")<101."
                      +" && ("+tMWPCX2+"-"+timeF5+")>78. && ("+tMWPCX2+"-"+timeF5+")<95."
                      +" && ("+tMWPCY2+"-"+timeF5+")>77. && ("+tMWPCY2+"-"+timeF5+")<98.)";

  return condition;
}


