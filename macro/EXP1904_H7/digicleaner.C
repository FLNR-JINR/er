#include "./cuts/scripts/create_cuts.C"


void digicleaner(TString inFile,TString outFile) {
  create_cuts();  
  // inFile  = "/mnt/data/exp1904/digi/h7_final/h7_ct_00_0001.digi.root";
  TFile *f_in = new TFile(inFile.Data());
  if (f_in->IsZombie()) {
    cerr << "File " << inFile.Data() << " does not exist" << endl;
    exit(1);
  }
  TTree *in_tree = (TTree*)f_in->Get("er");
  if (in_tree->IsZombie()) {
    cerr << "Tree er in file" << inFile.Data() << " does not exist" << endl;
    exit(1);   
  }
  ushort nEvents = in_tree->GetEntries();

  // outFile = "/mnt/data/exp1904/cleaned/h7_final/h7_ct_00_0001.clean.root";
  TString parFile = "par.root";
  // ------------------------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // ------------------------------------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  run->WithoutGeant(); //do not init Geant
  //run->SetUserCut("EventHeader.fTrigger!=1");
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();
  run->SetEventHeader(header);  
  // ------------------------------------------------------------------------
  ERDigiCleaner* cleaner = new ERDigiCleaner();

  cleaner->SetLonelyMWPCClusterCondition();
  // "" - do not recalibrate

  // SSD20_1 
  // cleaner->Recalibrate("Telescope_1", "SSD20_1", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                      parFolder + "empty16.cal" previous amp calibration ,
  //                      parFolder + "SSD_20u_1_cal.cal" /*new amp calibration*/
  //                      // &raw2simChannelsMapping - like in digibuiler
  //                      );
  const std::map<ushort, TCutG*>& SSD20_1_gcuts = {};
  const std::map<ushort, Double_t> SSD20_1_amp_min = {
    {0, 0.2},
    {1, 0.2},
    {2, 0.2},
    {3, 0.2},
    {4, 0.2},
    {5, 0.2},
    {6, 0.2},
    {7, 0.2},
    {8, 0.2},
    {9, 0.2},
    {10, 0.2},
    {11, 0.2},
    {12, 0.2},
    {13, 0.2},
    {14, 0.2},
    {15, 0.2}
  };
  const std::map<ushort, Double_t> SSD20_1_amp_max = {};
  const std::map<ushort, Double_t> SSD20_1_time_min = {
     {0, 0.},    
    {1, 0.},
    {2, 0.},
    {3, 0.},
    {4, 0.},
    {5, 0.},
    {6, 0.},
    {7, 0.},
    {8, 0.},
    {9, 0.},
    {10, 0.},
    {11, 0.},
    {12, 0.},
    {13, 0.},
    {14, 0.},
    {15, 0.}
  };
  const std::map<ushort, Double_t> SSD20_1_time_max = {
    {0, 60.},    
    {1, 60.},
    {2, 60.},
    {3, 60.},
    {4, 60.},
    {5, 60.},
    {6, 60.},
    {7, 60.},
    {8, 60.},
    {9, 60.},
    {10, 60.},
    {11, 60.},
    {12, 60.},
    {13, 60.},
    {14, 60.},
    {15, 60.}
  };
  cleaner->SetChannelCuts("Telescope_1", "SSD20_1", SSD20_1_gcuts,SSD20_1_amp_min,SSD20_1_amp_max,
                          SSD20_1_time_min, SSD20_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_1", "SSD20_1", 1, 1);
  // SSD_1 
  //cleaner->Recalibrate("Telescope_1", "SSD_1", "" /*previous time calibration*/, ""/*new time calibration*/,
   //                    parFolder + "empty16.cal" /*previous amp calibration*/ ,
   //                    parFolder + "SSD_1m_1_cal.cal" /*new amp calibration*/
   //                    // &raw2simChannelsMapping - like in digibuiler
    //                   );
  const std::map<ushort, TCutG*>& SSD1_1_gcuts = {};
  const std::map<ushort, Double_t> SSD1_1_amp_min = {
    {0, 0.2},    
    {1, 0.2},
    {2, 0.2},
    {3, 0.2},
    {4, 0.2},
    {5, 0.2},
    {6, 0.2},
    {7, 0.2},
    {8, 0.2},
    {9, 0.2},
    {10, 0.2},
    {11, 0.2},
    {12, 0.2},
    {13, 0.2},
    {14, 0.2},
    {15, 0.2}
  };
  const std::map<ushort, Double_t> SSD1_1_amp_max = {};
  const std::map<ushort, Double_t> SSD1_1_time_min = {

  };
  const std::map<ushort, Double_t> SSD1_1_time_max = {
  };
  cleaner->SetChannelCuts("Telescope_1", "SSD_1", SSD1_1_gcuts,SSD1_1_amp_min,SSD1_1_amp_max,
                          SSD1_1_time_min, SSD1_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_1", "SSD_1", 1, 1);
  // SSDV_1 
  //cleaner->Recalibrate("Telescope_1", "SSD_V_1", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_1_v_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );
  const std::map<ushort, TCutG*>& SSDV_1_gcuts = {};
  const std::map<ushort, Double_t> SSDV_1_amp_min = {};
  const std::map<ushort, Double_t> SSDV_1_amp_max = {};
  const std::map<ushort, Double_t> SSDV_1_time_min = {
    {0, -120.},    
    {1, -120.},
    {2, -120.},
    {3, -120.},
    {4, -120.},
    {5, -120.},
    {6, -120.},
    {7, -120.},
    {8, -120.},
    {9, -120.},
    {10,-120.},
    {11,-120.},
    {12,-120.},
    {13,-120.},
    {14,-120.},
    {15,-120.}
  };
  const std::map<ushort, Double_t> SSDV_1_time_max = {
    {0, 0.},    
    {1, 0.},
    {2, 0.},
    {3, 0.},
    {4, 0.},
    {5, 0.},
    {6, 0.},
    {7, 0.},
    {8, 0.},
    {9, 0.},
    {10,0.},
    {11,0.},
    {12,0.},
    {13,0.},
    {14,0.},
    {15,0.}
  };
  cleaner->SetChannelCuts("Telescope_1", "SSD_V_1", SSDV_1_gcuts,SSDV_1_amp_min,SSDV_1_amp_max,
                          SSDV_1_time_min, SSDV_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_1", "SSD_V_1", 1, 1);
  //----------------------------------------------------------------------------------------------------------
  // SSD20_2 
  //cleaner->Recalibrate("Telescope_2", "SSD20_2", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_20u_2_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );

  cleaner->SetChannelCuts("Telescope_2", "SSD20_2", SSD20_1_gcuts,SSD20_1_amp_min,SSD20_1_amp_max,
                          SSD20_1_time_min, SSD20_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_2", "SSD20_2", 1, 1);
  // SSD_2 
  //cleaner->Recalibrate("Telescope_2", "SSD_2", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_2_cal.cal" /*new amp calibration*/
   //                    // &raw2simChannelsMapping - like in digibuiler
  //                     );

  cleaner->SetChannelCuts("Telescope_2", "SSD_2", SSD1_1_gcuts,SSD1_1_amp_min,SSD1_1_amp_max,
                          SSD1_1_time_min, SSD1_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_2", "SSD_2", 1, 1);
  // SSDV_2
  //cleaner->Recalibrate("Telescope_2", "SSD_V_2", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_2_v_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );
  cleaner->SetChannelCuts("Telescope_2", "SSD_V_2", SSDV_1_gcuts,SSDV_1_amp_min,SSDV_1_amp_max,
                          SSDV_1_time_min, SSDV_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_2", "SSD_V_2", 1, 1);
  //-----------------------------------------------------------------------------------------------------------
  // SSD20_3 
  //cleaner->Recalibrate("Telescope_3", "SSD20_3", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
   //                    parFolder + "SSD_20u_3_cal.cal" /*new amp calibration*/
   //                    // &raw2simChannelsMapping - like in digibuiler
   //                    );

  cleaner->SetChannelCuts("Telescope_3", "SSD20_3", SSD20_1_gcuts,SSD20_1_amp_min,SSD20_1_amp_max,
                          SSD20_1_time_min, SSD20_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_3", "SSD20_3", 1, 1);
  // SSD_3 
  //cleaner->Recalibrate("Telescope_3", "SSD_3", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_3_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
   //                    );

  cleaner->SetChannelCuts("Telescope_3", "SSD_3", SSD1_1_gcuts,SSD1_1_amp_min,SSD1_1_amp_max,
                          SSD1_1_time_min, SSD1_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_3", "SSD_3", 1, 1);
  // SSDV_3
  //cleaner->Recalibrate("Telescope_3", "SSD_V_3", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_3_v_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
   //                    );
  cleaner->SetChannelCuts("Telescope_3", "SSD_V_3", SSDV_1_gcuts,SSDV_1_amp_min,SSDV_1_amp_max,
                          SSDV_1_time_min, SSDV_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_3", "SSD_V_3", 1, 1);
  //-----------------------------------------------------------------------------------------------------------
  // SSD20_4
  //cleaner->Recalibrate("Telescope_4", "SSD20_4", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_20u_4_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );

  cleaner->SetChannelCuts("Telescope_4", "SSD20_4", SSD20_1_gcuts,SSD20_1_amp_min,SSD20_1_amp_max,
                          SSD20_1_time_min, SSD20_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_4", "SSD20_4", 1, 1);
  // SSD_4 
  //cleaner->Recalibrate("Telescope_4", "SSD_4", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
   //                    parFolder + "SSD_1m_4_cal.cal" /*new amp calibration*/
   //                    // &raw2simChannelsMapping - like in digibuiler
   //                    );
  const std::map<ushort, Double_t> SSD1_4_time_min = {
    {0, -100.},    
    {1, -100.},
    {2, -100.},
    {3, -100.},
    {4, -100.},
    {5, -100.},
    {6, -100.},
    {7, -100.},
    {8, -100.},
    {9, -100.},
    {10, -100.},
    {11, -100.},
    {12, -100.},
    {13, -100.},
    {14, -100.},
    {15, -100.}
  };
  const std::map<ushort, Double_t> SSD1_4_time_max = {
    {0, 200.},    
    {1, 200.},
    {2, 200.},
    {3, 200.},
    {4, 200.},
    {5, 200.},
    {6, 200.},
    {7, 200.},
    {8, 200.},
    {9, 200.},
    {10, 200.},
    {11, 200.},
    {12, 200.},
    {13, 200.},
    {14, 200.},
    {15, 200.}
  };
  cleaner->SetChannelCuts("Telescope_4", "SSD_4", SSD1_1_gcuts,SSD1_1_amp_min,SSD1_1_amp_max,
                          SSD1_4_time_min, SSD1_4_time_max);
  cleaner->SetStationMultiplicity("Telescope_4", "SSD_4", 1, 1);
  // SSDV_4
  //cleaner->Recalibrate("Telescope_4", "SSD_V_4", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty16.cal" /*previous amp calibration*/ ,
  //                     parFolder + "SSD_1m_4_v_cal.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );

  cleaner->SetChannelCuts("Telescope_4", "SSD_V_4", SSDV_1_gcuts,SSDV_1_amp_min,SSDV_1_amp_max,
                          SSDV_1_time_min, SSDV_1_time_max);
  cleaner->SetStationMultiplicity("Telescope_4", "SSD_V_4", 1, 1);
  //-------------------------------------------------------------------------------------------------------------------
  // DSSD_X
  //cleaner->Recalibrate("Central_telescope", "DSD_XY_0_X", "" /*previous time calibration*/, ""/*new time calibration*/,
  //                     parFolder + "empty32.cal" /*previous amp calibration*/ ,
  //                    parFolder + "DSSD_X.cal" /*new amp calibration*/
  //                     // &raw2simChannelsMapping - like in digibuiler
  //                     );

  const std::map<ushort, Double_t> DSSDX_amp_min= {
    {0, 2.5},    
    {1, 2.5},
    {2, 2.5},
    {3, 2.5},
    {4, 2.5},
    {5, 2.5},
    {6, 2.5},
    {7, 2.5},
    {8, 2.5},
    {9, 2.5},
    {10, 2.5},
    {11, 2.5},
    {12, 2.5},
    {13, 2.5},
    {14, 2.5},
    {15, 2.5},
    {16, 2.5},
    {17, 2.5},
    {18, 2.5},
    {19, 2.5},
    {20, 2.5},
    {21, 2.5},
    {22, 2.5},
    {23, 2.5},
    {24, 2.5},
    {25, 2.5},
    {26, 2.5},
    {27, 2.5},
    {28, 2.5},
    {29, 2.5},
    {30, 2.5},
    {31, 2.5}   
  };

  const std::map<ushort, Double_t> DSSDX_amp_max = {};
  const std::map<ushort, Double_t> DSSDX_time_min = {};
  const std::map<ushort, Double_t> DSSDX_time_max = {};  

  const std::map<ushort, TCutG*>& DSSDX_cuts = {
    {0,    dssd_x_cut[0]},
    {1,    dssd_x_cut[1]},
    {2,    dssd_x_cut[2]},
    {3,    dssd_x_cut[3]},
    {4,    dssd_x_cut[4]},
    {5,    dssd_x_cut[5]},
    {6,    dssd_x_cut[6]},
    {7,    dssd_x_cut[7]},
    {8,    dssd_x_cut[8]},
    {9,    dssd_x_cut[9]},
    {10,   dssd_x_cut[10]},
    {11,   dssd_x_cut[11]},
    {12,   dssd_x_cut[12]},
    {13,   dssd_x_cut[13]},
    {14,   dssd_x_cut[14]},
    {15,   dssd_x_cut[15]},
    {16,   dssd_x_cut[16]},
    {17,   dssd_x_cut[17]},
    {18,   dssd_x_cut[18]},
    {19,   dssd_x_cut[19]},
    {20,   dssd_x_cut[20]},
    {21,   dssd_x_cut[21]},
    {22,   dssd_x_cut[22]},
    {23,   dssd_x_cut[23]},
    {24,   dssd_x_cut[24]},
    {25,   dssd_x_cut[25]},
    {26,   dssd_x_cut[26]},
    {27,   dssd_x_cut[27]},
    {28,   dssd_x_cut[28]},
    {29,   dssd_x_cut[29]},
    {30,   dssd_x_cut[30]},
    {31,   dssd_x_cut[31]}    
  };
  cleaner->SetChannelCuts("Central_telescope", "DSD_XY_X", DSSDX_cuts,DSSDX_amp_min,DSSDX_amp_max,
                          DSSDX_time_min, DSSDX_time_max);
  cleaner->SetStationMultiplicity("Central_telescope", "DSD_XY_X", 1, 1);
  // DSSD_Y
  //cleaner->Recalibrate("Central_telescope", "DSD_XY_0_Y", "" /*previous time calibration*/, ""/*new time calibration*/,
   //                    parFolder + "empty32.cal" /*previous amp calibration*/ ,
  //                     parFolder + "DSSD_Y.cal" /*new amp calibration*/
   //                    // &raw2simChannelsMapping - like in digibuiler
   //                    );
  const std::map<ushort, TCutG*>& DSSDY_cuts = {};
  cleaner->SetChannelCuts("Central_telescope", "DSD_XY_Y", DSSDY_cuts,DSSDX_amp_min,DSSDX_amp_max,
                          DSSDX_time_min, DSSDX_time_max);  
  cleaner->SetStationMultiplicity("Central_telescope", "DSD_XY_Y", 1, 1);
  //---------------------------------------------------------------------------------------------------------------------
  // CsI
 // cleaner->Recalibrate("Central_telescope", "CsI", "" /*previous time calibration*/, ""/*new time calibration*/,
//                       parFolder + "empty16.cal" /*previous amp calibration*/ ,
 //                      parFolder + "CsI_anh.cal" /*new amp calibration*/
 //                      // &raw2simChannelsMapping - like in digibuiler
 //                      );
  const std::map<ushort, ushort> csiMapping = {
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
  const std::map<ushort, TCutG*>& CsI_cuts = {
    {0,    CsI_cut[0]},
    {1,    CsI_cut[1]},
    {2,    CsI_cut[2]},
    {3,    CsI_cut[3]},
    {4,    CsI_cut[4]},
    {5,    CsI_cut[5]},
    {6,    CsI_cut[6]},
    {7,    CsI_cut[7]},
    {8,    CsI_cut[8]},
    {9,    CsI_cut[9]},
    {10,   CsI_cut[10]},
    {11,   CsI_cut[11]},
    {12,   CsI_cut[12]},
    {13,   CsI_cut[13]},
    {14,   CsI_cut[14]},
    {15,   CsI_cut[15]}   
  };
  const std::map<ushort, Double_t> CsI_amp_min = {};
  const std::map<ushort, Double_t> CsI_amp_max = {};
  const std::map<ushort, Double_t> CsI_time_min = {};
  const std::map<ushort, Double_t> CsI_time_max = {};
  cleaner->SetChannelCuts("Central_telescope", "CsI", CsI_cuts,CsI_amp_min,CsI_amp_max,
                          CsI_time_min, CsI_time_max, &csiMapping);
  cleaner->SetStationMultiplicity("Central_telescope", "CsI", 1, 16);
  // ...

  run->AddTask(cleaner);
  // ------------------------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);
  // ------------------------------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");
  run->Init();
  run->Run(0, 1000000);
  // ------------------------------------------------------------------------
  rtdb->setOutput(parIO);
  rtdb->saveOutput();
  // ------------------------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t Central_telescopeime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;    
  cout << "Real time " << rtime << " s, CPU time " << Central_telescopeime << " s" << endl;
  cout << endl;
}