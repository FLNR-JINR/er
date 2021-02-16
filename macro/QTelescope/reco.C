void reco(Int_t nEvents = 100000){
  //---------------------Files-----------------------------------------------
  TString inFile  = "sim_digi.root";
  TString outFile = "reco.root";
  TString parFile = "par.root";
  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);
  // ------------------------------------------------------------------------
  //-------- Set MC event header --------------------------------------------
  EREventHeader* header = new EREventHeader();
  run->SetEventHeader(header);
  // ------- QTelescope TrackFinder -------------------------------------------
  ERTelescopeTrackFinder* qtelescopeTrackFinder = new ERTelescopeTrackFinder(0);
  qtelescopeTrackFinder->SetInteractionPosition(0., 0., 0.);
  qtelescopeTrackFinder->SetHitStation("Telescope_1", "Telescope_1_SingleSi_SSD20_1_X", "Telescope_1_SingleSi_SSD_1_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_2", "Telescope_2_SingleSi_SSD_2_X", "Telescope_2_SingleSi_SSD20_2_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_3", "Telescope_3_SingleSi_SSD20_3_X", "Telescope_3_SingleSi_SSD_3_Y");
  qtelescopeTrackFinder->SetHitStation("Telescope_4", "Telescope_4_SingleSi_SSD_4_X", "Telescope_4_SingleSi_SSD20_4_Y");
  //qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY_0");
  qtelescopeTrackFinder->SetHitStation("Central_telescope", "Central_telescope_DoubleSi_DSD_XY_X","Central_telescope_DoubleSi_DSD_XY_Y");
  run->AddTask(qtelescopeTrackFinder);
  // ------   QTelescope TrackPID -----------------------------------------
  ERTelescopePID* qtelescopePID = new ERTelescopePID(0);
  TString deStation1 = "SSD20_1";
  TString eStation1 = "SSD_1";
  Double_t normalizedThickness = 0.002; // [cm]
  Double_t DSD_thickness = 0.15; // [cm]
  qtelescopePID->SetParticle("Telescope_1_SingleSi_SSD20_1_XTelescope_1_SingleSi_SSD_1_Y", 1000020030,
                              deStation1, eStation1, normalizedThickness);
  qtelescopePID->SetParticle("Telescope_2_SingleSi_SSD_2_XTelescope_2_SingleSi_SSD20_2_Y", 1000020030,
                             "SSD20_2", "SSD_2", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_3_SingleSi_SSD20_3_XTelescope_3_SingleSi_SSD_3_Y", 1000020030,
                             "SSD20_3", "SSD_3", normalizedThickness);
  qtelescopePID->SetParticle("Telescope_4_SingleSi_SSD_4_XTelescope_4_SingleSi_SSD20_4_Y", 1000020030,
                             "SSD20_4", "SSD_4", normalizedThickness);
  qtelescopePID->SetParticle("Central_telescope_DoubleSi_DSD_XY_XCentral_telescope_DoubleSi_DSD_XY_Y",1000010030, "DSD", "CsI", DSD_thickness);
  //enum EdepAccountingStrategy {EdepFromXChannel, EdepFromYChannel, AverageEdep, SummarizedEdep};
  qtelescopePID->SetEdepAccountingStrategy("DSD", ERTelescopePID::EdepAccountingStrategy::EdepFromYChannel);
  run->AddTask(qtelescopePID);
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  rtdb->setFirstInput(parIO);
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  run->Init();
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------;
  rtdb->setOutput(parIO);
  rtdb->saveOutput();
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t Central_telescopeime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << Central_telescopeime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
