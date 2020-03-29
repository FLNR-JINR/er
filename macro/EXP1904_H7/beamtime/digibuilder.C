TString MakeConditionBeamID();
TString MakeConditionMWPCtime();

#include "beamCut.C"

void digibuilder(TString InFile, TString OutFile) {

  beamCut();

  TString inputdir = "/opt/er/macro/EXP1904_H7/beamtime/input/";

  TString confFile = "/opt/er/macro/EXP1904_H7/beamtime/input/setupEXP1904_final.xml";

  TChain *ch = new TChain("stepRepackingxTree");
  ch->Add(InFile.Data());
  Int_t nEvents = ch->GetEntries();
  delete ch;
  cout << nEvents << " will be processed" << endl;
  std::cout << ">>> input file is " << InFile  << std::endl;
  std::cout << ">>> output file is " << OutFile << std::endl;

  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);

  // UserCuts
  TString cut("");
  //cut+=("DetEventCommon.trigger==1 && ");
  cut+=("Beam_detector_MWPC1.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC2.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC3.@fData.GetEntries()>0 && ");
  cut+=("Beam_detector_MWPC4.@fData.GetEntries()>0 && ");

  cut+=("Beam_detector_tMWPC.@fData.GetEntries()>0 && ");

  cut+=("Beam_detector_tF3.@fData.GetEntries()==4 && ");  
  cut+=("Beam_detector_tF5.@fData.GetEntries()==4 && ");   
  cut+=("Beam_detector_F3.@fData.GetEntries()==4 && ");
  cut+=("Beam_detector_F5.@fData.GetEntries()==4 && ");

  cut.Append((TString)cutF5->GetName() + " && ");
  cut.Append((TString)cutF3->GetName());

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
  
  ERTelescopeUnpack* t1Unpack = new ERTelescopeUnpack("Telescope_1");
  t1Unpack->AddSingleSiStation("SSD20_1",
                               "SSD20_1","tSSD20_1",
                               inputdir + "empty16.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "X");
  t1Unpack->AddSingleSiStation("SSD_1",
                               "SSD_1","tSSD_1",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "Y");
  t1Unpack->AddSingleSiStation("SSD_V_1",
                               "SSD_V_1","tSSD_V_1",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "Y");

  ERTelescopeUnpack* t2Unpack = new ERTelescopeUnpack("Telescope_2");
  t2Unpack->AddSingleSiStation("SSD20_2",
                               "SSD20_2","tSSD20_2",
                               inputdir + "empty16.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "Y");
  t2Unpack->AddSingleSiStation("SSD_2",
                               "SSD_2","tSSD_2",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "X");
  t2Unpack->AddSingleSiStation("SSD_V_2",
                               "SSD_V_2","tSSD_V_2",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "X");

  ERTelescopeUnpack* t3Unpack = new ERTelescopeUnpack("Telescope_3");
  t3Unpack->AddSingleSiStation("SSD20_3",
                               "SSD20_3","tSSD20_3",
                               inputdir + "empty16.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "X");
  t3Unpack->AddSingleSiStation("SSD_3",
                               "SSD_3","tSSD_3",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "Y");
  t3Unpack->AddSingleSiStation("SSD_V_3",
                               "SSD_V_3","tSSD_V_3",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "Y");

  ERTelescopeUnpack* t4Unpack = new ERTelescopeUnpack("Telescope_4");
  t4Unpack->AddSingleSiStation("SSD20_4",
                               "SSD20_4","tSSD20_4",
                               inputdir + "empty16.cal",inputdir + "V775_caen_tdc_pars.cal",
                               "Y");
  t4Unpack->AddSingleSiStation("SSD_4",
                               "SSD_4","tSSD_4",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "X");
  t4Unpack->AddSingleSiStation("SSD_V_4",
                               "SSD_V_4","tSSD_V_4",
                               inputdir + "empty16.cal",inputdir + "mtdc32_pars.cal",
                               "X");

  ERTelescopeUnpack* ctUnpack = new ERTelescopeUnpack("Central_telescope");
  ctUnpack->AddDoubleSiStation("DSD",
                               "DSDX","tDSDX",
                               "DSDY","tDSDY",
                               inputdir + "empty32.cal",inputdir + "mtdc32_pars.cal",
                               inputdir + "empty32.cal",inputdir + "mtdc32_pars.cal",
                               "XY");
  std::map<Int_t, Int_t> csiChannelMapping = {
    {8, 0},
    {12, 1}
    //...
  };
  ctUnpack->AddCsIStation("CsI","CsI","tCsI",inputdir + "empty16.cal",inputdir + "V775_caen_tdc_pars.cal", 
                          &csiChannelMapping);

  ERNDUnpack* ndUnpack = new ERNDUnpack("ND", "ND_AMP", "ND_TIME", "ND_TAC",
                                        "", "", "");
  
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
  run->Run(0,nEvents);

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
