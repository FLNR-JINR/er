// int fTriggerNum = 2;
int fTriggerNum = 1;
void reco_10he_exp(){
  //---------------------Files-----------------------------------------------

//  TString inFile = "sim_digi_mylar.root";
//  TString inFile = "sim_digi_Steel.root";
//  TString inFile = "sim_digi_1eV_Steel.root";
//  TString inFile = "sim_digi_100eV_Steel.root";
  TString inFile = "sim_digi.root";

	TString outFile = inFile;
  	inFile.Prepend("../");

  auto file = TFile::Open(inFile.Data());
  auto tree = (TTree*)file->Get("er");
  // auto tree = (TTree*)file->Get("reco");
  Int_t nEvents = tree->GetEntriesFast();//1443710

  TString parFile = "../par.root";
//  TString geoFile = "../setup_exp10he.root";
	TString geoFile = inFile;
	Ssiz_t p1 = inFile.First("/");
	geoFile.Insert(p1+1,"setup_");

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();  
  // -----   Digitization run   ---------------------------------------------
  ERRunAna *run = ERRunAna::Instance();
  run->HoldEventsCount(); //forbid different entry number in the input and output file
  TString userCut;

  
  Ssiz_t p2 = outFile.First(".");
  outFile.Replace(p2,5,".target.root");
  
  // userCut.Form("EventHeader.fTrigger==%d", fTriggerNum);
 // userCut.Form("MCEventHeader.fTrigger==%d", fTriggerNum);		//как для команды Draw
  // outFile.Form("reco_sim_digi_beamdet.trigger%d.root", fTriggerNum);

//	outFile.Form("reco_sim_digi_sin.target.root");
//	outFile.Form("reco_sim_digi_1eV.target.root");
//	outFile.Form("reco_sim_digi_ang.target.root");    
//  outFile.Form("reco_sim_digi_ionmix.target.root");
//  outFile.Form("reco_sim_digi_10.target.root");  
//  outFile.Form("reco_sim_digi_beamdet.target.root");
   
   //outFile.Form("reco_sim_digi_beamdet.mwpc1.root");
  // outFile.Form("reco_sim_digi_beamdet.mwpc2.root");
  // outFile.Form("reco_sim_digi_beamdet.si1.root");
  //outFile.Form("reco_sim_digi_beamdet.si_target.root");
//	outFile.Form("reco_sim_digi_beamdet.mwpc2_target.root");
//	outFile.Form("reco_sim_digi_beamdet.mwpc1_target.root");
  
//  run->SetUserCut(userCut.Data());
  run->SetGeomFile(geoFile);
  run->SetInputFile(inFile);
  run->SetOutputFile(outFile);


  // ------------------------BeamDetTrackFinger--------------------------------
  Int_t verbose = 1; // 1 - only standard log print, 2 - print digi information 
  ERBeamDetTrackFinder* trackFinder = new ERBeamDetTrackFinder(verbose);
  // trackFinder->SetTargetVolume("targetBodyH2"); // targetBodyH2 - embedded BeamDet target volume, tubeD2 - D2 taget volume
  trackFinder->SetTargetVolume("target3HVol"); // targetBodyH2 - embedded BeamDet target volume, tubeD2 - D2 taget volume
  ERBeamDetTrackFinder* trackFinder_mwpc1 = new ERBeamDetTrackFinder(verbose);
  trackFinder_mwpc1->SetTargetVolume("MWPC_MWPC1_1");
  ERBeamDetTrackFinder* trackFinder_mwpc2 = new ERBeamDetTrackFinder(verbose);
  trackFinder_mwpc2->SetTargetVolume("MWPC_MWPC2_2");
  ERBeamDetTrackFinder* trackFinder_si1 = new ERBeamDetTrackFinder(verbose);
  trackFinder_si1->SetTargetVolume("SingleSi_SSD20");
  ERBeamDetTrackFinder* trackFinder_si2 = new ERBeamDetTrackFinder(verbose);
  trackFinder_si2->SetTargetVolume("SingleSi_SSD20_1");
  
  run->AddTask(trackFinder);
  // run->AddTask(trackFinder_mwpc1);
  // run->AddTask(trackFinder_mwpc2);
  //run->AddTask(trackFinder_si1);
  // run->AddTask(trackFinder_si2);
  printf("\n\n\n=====================================\n\n\n================================\n");
  // -----------Runtime DataBase info ---------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo*  parIO = new FairParRootFileIo();
  parIO->open(parFile.Data(), "UPDATE");
  // rtdb->setFirstInput(parIO);
  // rtdb->setOutput(parIO);
  // rtdb->saveOutput();
  rtdb->print();
  
  // -----   Intialise and run   --------------------------------------------
  FairLogger::GetLogger()->SetLogScreenLevel("INFO");  
  run->Init();
  printf("\n\n\n=====================================\n\n\n================================\n");
//  run->Run(1, nEvents); // in Vratislav 's macro-recoMWPC, first event is triggered by 1
  run->Run(0, nEvents);
  // ------------------------------------------------------------------------;
  // rtdb->setOutput(parIO);
// //  rtdb->setOutput(parOut);
  // rtdb->saveOutput();
  // rtdb->print();
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file writen:  "    << outFile << endl;
  cout << "Parameter file writen " << parFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
