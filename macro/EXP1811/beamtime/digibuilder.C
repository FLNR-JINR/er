TString MakeConditionBeamID();
TString MakeConditionMWPCtime();

void digibuilder(Int_t sEvent = 0 , Int_t fEvent = 100000, TString outFile = "digi.root") {

  TString inputdir = gSystem->Getenv("VMCWORKDIR");
  inputdir = inputdir + "/input/";
  TString confFile = inputdir + "setup2_exp201811.xml";

	TString inFile = inputdir + "dsd_20_l_03_0001.lmd.root";

  TChain *ch = new TChain("stepRepackingxTree");
  ch->Add(inFile.Data());
  Int_t nEvents = ch->GetEntries();
  cout << nEvents << " will be processed" << endl;
  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERDigibuilder* builder = new ERDigibuilder();
  builder->SetConfigurationFile(confFile);

  // UserCuts
  TString cut("");
  // cut+=("DetEventCommon.trigger==3 && ");
  // cut+=("Beam_detector_MWPC1.@fData.GetEntries()>0 && ");
  // cut+=("Beam_detector_MWPC2.@fData.GetEntries()>0 && ");
  // cut+=("Beam_detector_MWPC3.@fData.GetEntries()>0 && ");
  // cut+=("Beam_detector_MWPC4.@fData.GetEntries()>0 && ");

  // cut+=("Beam_detector_tMWPC.@fData.GetEntries()>0 && ");


  // cut+=("Beam_detector_F3.@fData.GetEntries()==4 && ");
  // cut+=("Beam_detector_F5.@fData.GetEntries()==4 && ");
  // cut+=("Beam_detector_tF3.@fData.GetEntries()==4 && ");
  // cut+=("Beam_detector_tF5.@fData.GetEntries()==4");

  cout << " Its my cut " << endl;
  cout << cut.Data() << endl;
  //builder->SetUserCut(cut.Data(),kTRUE);

  builder->AddFile(inFile);

  ERBeamDetUnpack* beamDetUnpack = new ERBeamDetUnpack("Beam_detector");
  beamDetUnpack->SetToFCalibration(0.125, 0., 0.125, 0.); //aF3, bF3, aF5, bF5
  beamDetUnpack->SetMWPCCalibration(0.125,0.);
  
  ERTelescopeUnpack* rtUnpack = new ERTelescopeUnpack("Right_telescope");
  rtUnpack->AddSingleSiStation("SSD20_R",
                               "SSD20_R","tSSD20_R",
                               "","",
                               "X",kFALSE);
  rtUnpack->AddSingleSiStation("SSDY_R",
                               "SSDY_R","tSSDY_R",
                               "","",
                               "Y",kFALSE);
  rtUnpack->AddSingleSiStation("SSD_R",
                               "SSD_R","tSSD_R",
                               "","",
                               "Y",kFALSE);

  ERTelescopeUnpack* ltUnpack = new ERTelescopeUnpack("Left_telescope");
  ltUnpack->AddDoubleSiStation("DSD_L",
                               "DSDX_L","tDSDX_L",
                               "DSDY_L","tDSDY_L",
                               "","",
                               "","",
                               "XY",kFALSE);
  ltUnpack->AddSingleSiStation("SSD20_L",
                               "SSD20_L","tSSD20_L",
                               "","",
                               "X",kFALSE);
  ltUnpack->AddSingleSiStation("SSD_L",
                               "SSD_L","tSSD_L",
                               "","",
                               "X",kFALSE);
  ERTelescopeUnpack* ctUnpack = new ERTelescopeUnpack("Central_telescope");
  ctUnpack->AddDoubleSiStation("DSD_C",
                               "DSDX_C","tDSDX_C",
                               "DSDY_C","tDSDY_C",
                               "","",
                               "","",
                               "XY",kFALSE);
  ctUnpack->AddCsIStation("CsI","CsI","tCsI","","",kFALSE);

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
  run->Run(sEvent,fEvent);

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

TString MakeConditionBeamID() {

  ////////////////////////////////////////////
  //beam ID
  ////////////////////////////////////////////

  TString timeF5 = "0.125*0.25*(Beam_detector_tF5.fData.fValue[0]"
      "+Beam_detector_tF5.fData.fValue[1]"
      "+Beam_detector_tF5.fData.fValue[2]"
      "+Beam_detector_tF5.fData.fValue[3])";

  TString timeF3 = "0.125*0.25*(Beam_detector_tF3.fData.fValue[0]"
      "+Beam_detector_tF3.fData.fValue[1]"
      "+Beam_detector_tF3.fData.fValue[2]"
      "+Beam_detector_tF3.fData.fValue[3])";

  TString ampF5 = "Beam_detector_F5.fData.fValue[0]"
      "+Beam_detector_F5.fData.fValue[1]"
      "+Beam_detector_F5.fData.fValue[2]"
      "+Beam_detector_F5.fData.fValue[3]";

  TString con = "";
  con.Append("(" + timeF5 + "-" + timeF3 + ")<240.");
  con.Append(" && (" + timeF5 + "-" + timeF3 + ")>200.");
  con.Append(" && " + ampF5 + "<4300.");
  con.Append(" && " + ampF5 + ">2600.");

  return con;
}

TString MakeConditionMWPCtime() {


  ////////////////////////////////////////////
  //time in MWPC
  ////////////////////////////////////////////

  TString timeF5 = "0.0625*0.25*(Beam_detector_tF5.fData.fValue[0]"
                "+Beam_detector_tF5.fData.fValue[1]"
                "+Beam_detector_tF5.fData.fValue[2]"
                "+Beam_detector_tF5.fData.fValue[3])";

  TString tMWPCX1 = "0.0625*Beam_detector_tMWPC.fData.fValue[0]";
  TString tMWPCY1 = "0.0625*Beam_detector_tMWPC.fData.fValue[1]";
  TString tMWPCX2 = "0.0625*Beam_detector_tMWPC.fData.fValue[2]";
  TString tMWPCY2 = "0.0625*Beam_detector_tMWPC.fData.fValue[3]";

  TString condition = "( ("+tMWPCX1+"-"+timeF5+")>65. && ("+tMWPCX1+"-"+timeF5+")<95."
          +" && ("+tMWPCY1+"-"+timeF5+")>65. && ("+tMWPCY1+"-"+timeF5+")<95."
          +" && ("+tMWPCX2+"-"+timeF5+")>65. && ("+tMWPCX2+"-"+timeF5+")<95."
          +" && ("+tMWPCY2+"-"+timeF5+")>65. && ("+tMWPCY2+"-"+timeF5+")<95.)";

  return condition;
}
