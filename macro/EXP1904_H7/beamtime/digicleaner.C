void digicleaner(Int_t nEvents = 100000) {
  TString inFile  = "digi.root";
  TString outFile = "digi_new.root";
  TString parFile = "par.root";
  TString inputdir = "/opt/er/macro/EXP1904_H7/beamtime/input/";
  // ------------------------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // ------------------------------------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  run->WithoutGeant(); //do not init Geant
  //run->SetUserCut("");
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  ERBeamTimeEventHeader* header = new ERBeamTimeEventHeader();
  run->SetEventHeader(header);
  // ------------------------------------------------------------------------
  ERDigiCleaner* cleaner = new ERDigiCleaner();

  cleaner->SetLonelyMWPCClusterCondition();
  // "" - do not recalibrate
  cleaner->Recalibrate("Telescope_1", "SSD_1", "" /*previous time calibration*/, ""/*new time calibration*/,
                       inputdir + "mtdc32_pars.cal" /*previous amp calibration*/ ,
                       inputdir + "mtdc32_pars.cal" /*new amp calibration*/
                       //&raw2simChannelsMapping - like in digibuiler
                       );
  cleaner->RecalibrateWithTAC("ND", "ND", "" /*previous time calibration*/, ""/*new time calibration*/,
                       inputdir + "ND.cal" /*previous amp calibration*/ ,
                       inputdir + "ND.cal" /*new amp calibration*/,
                       inputdir + "tacND.cal" /*previous tac calibration*/ ,
                       inputdir + "tacND.cal" /*new tac calibration*/
                       //&raw2simChannelsMapping - like in digibuiler
                       );                     
  // ...
  TCutG *ssd1_0_cut = new TCutG("SSD_1_0_cut",6);
  ssd1_0_cut->SetPoint(0,-0.3586207,1.509534);
  ssd1_0_cut->SetPoint(1,-1.894181,-0.529661);
  ssd1_0_cut->SetPoint(2,0.07780173,-1.21822);
  ssd1_0_cut->SetPoint(3,-1.0375,-0.07944915);
  ssd1_0_cut->SetPoint(4,0.756681,0.1853814);
  ssd1_0_cut->SetPoint(5,-0.3586207,1.509534);
  TCutG* ssd1_1_cut = ssd1_0_cut;
  const std::map<Int_t, TCutG*>& SSD_1_gcuts = {
    {0,  ssd1_0_cut},
    {1,  ssd1_1_cut}
  };
  const std::map<Int_t, Double_t> SSD_1_amp_min = {
    {1, 0.1}
  };
  const std::map<Int_t, Double_t> SSD_1_amp_max = {
    {1, 10}
  };
  const std::map<Int_t, Double_t> SSD_1_time_min, SSD_1_time_max;
  cleaner->SetChannelCuts("Telescope_1", "SSD_1", SSD_1_gcuts, SSD_1_amp_min, SSD_1_amp_max,
                          SSD_1_time_min, SSD_1_time_max);
  // ...
  cleaner->SetStationMultiplicity("Telescope_1", "SSD_1", 1);
  cleaner->SetStationMultiplicity("Telescope_1", "SSD_V_1", 0);
  // ...
  run->AddTask(cleaner);
  // ------------------------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);
  // ------------------------------------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  run->Init();
  run->Run(0, nEvents);
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