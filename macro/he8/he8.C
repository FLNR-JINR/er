void he8(Int_t nEvents = 10)
{
	TString inFile = "~/er/input/t_8he_026.root";

	// --- Specify output file name (this is just an example)
	TString outFile = "out.root";

  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERRootSource* source = new ERRootSource();
  source->SetFile(inFile,"AnalysisxTree","HE8Event");

  ERTelescopeEvent* telEvent = new ERTelescopeEvent();
  source->AddEvent(telEvent);

  ERMwpcEvent* mwpcEvent = new ERMwpcEvent();
  source->AddEvent(mwpcEvent);

  ERDsrdEvent* dsrdEvent = new ERDsrdEvent();
  source->AddEvent(dsrdEvent);

  ERTofEvent* tofEvent = new ERTofEvent();
  source->AddEvent(tofEvent);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);

  ERHe8EventHeader* header = new ERHe8EventHeader();
  run->SetEventHeader(header);

  ERDsrdCalibrator* dsrdCalibrator = new ERDsrdCalibrator();
  dsrdCalibrator->SetParametersFile("/home/vitaliy.schetinin/er/input/clb4sonya.dat");
  run->AddTask(dsrdCalibrator);

  ERTelescopeCalibrator* telCalibrator = new ERTelescopeCalibrator();
  telCalibrator->SetParametersFile("/home/vitaliy.schetinin/er/input/clb4sonya.dat");
  run->AddTask(telCalibrator);

  ERTofCalibrator* tofCalibrator = new ERTofCalibrator();
  tofCalibrator->SetParametersFile("/home/vitaliy.schetinin/er/input/clb4sonya.dat");
  run->AddTask(tofCalibrator);

  ERTracker* tracker = new ERTracker(1);
  run->AddTask(tracker);
  
  // --- Start run
  TStopwatch timer;
  timer.Start();
  std::cout << ">>> Starting run..." << std::endl;
  run->Init();
  run->Run(0, nEvents); // run until end of input file

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
