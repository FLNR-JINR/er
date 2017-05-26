void he8(Int_t nEvents = 10)
{
  TString workdir = gSystem->Getenv("VMCWORKDIR");
	TString inFile = workdir + "/input/t_8he_026.root";

	// --- Specify output file name (this is just an example)
	TString outFile = "out.root";

  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERRootSource* source = new ERRootSource();
  source->SetFile(inFile,"AnalysisxTree","HE8Event");

  ERTelescopeEvent* telEvent = new ERTelescopeEvent();
  source->AddEvent(telEvent);

  ERBeamDetEvent* beamDetEvent = new ERBeamDetEvent();
  source->AddEvent(beamDetEvent);

  ERDsrdEvent* dsrdEvent = new ERDsrdEvent();
  source->AddEvent(dsrdEvent);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);

  ERHe8EventHeader* header = new ERHe8EventHeader();
  run->SetEventHeader(header);

  ERDsrdCalibrator* dsrdCalibrator = new ERDsrdCalibrator();
  dsrdCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(dsrdCalibrator);

  ERTelescopeCalibrator* telCalibrator = new ERTelescopeCalibrator();
  telCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(telCalibrator);

  ERBeamDetCalibrator* beamDetCalibrator = new ERBeamDetCalibrator();
  beamDetCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(beamDetCalibrator);

  ERBeamDetReconstructor* beamDetRecon = new ERBeamDetReconstructor(1);
  run->AddTask(beamDetRecon);

  ERTelescopeReconstructor* telescopeRecon = new ERTelescopeReconstructor(1);
  run->AddTask(telescopeRecon);

  /*ERHe8Analyzer* analyzer = new ERHe8Analyzer(1);
  run->AddTask(analyzer);*/
  
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
