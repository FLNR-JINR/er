void beamtest(Int_t nEvents = 20)
{
  TString workdir = gSystem->Getenv("VMCWORKDIR");
	TString inFile = workdir + "/input/t_8he_025.root";

	// --- Specify output file name (this is just an example)
	TString outFile = "out.root";

  std::cout << ">>> input file is " << inFile  << std::endl;
  std::cout << ">>> output file is " << outFile << std::endl;

  // --- Source task
  ERRootSource* source = new ERRootSource();
  source->SetFile(inFile,"AnalysisxTree","HE8Event");
  source->AddFile(workdir + "/input/t_8he_026.root");

  ERQTelescopeEvent* qTelEvent = new ERQTelescopeEvent();
  source->AddEvent(qTelEvent);

  ERBeamDetEvent* beamDetEvent = new ERBeamDetEvent();
  source->AddEvent(beamDetEvent);

  ERRTelescopeEvent* rTelescopeEvent = new ERRTelescopeEvent();
  source->AddEvent(rTelescopeEvent);

  // --- Run
  FairRunOnline *run = new FairRunOnline(source);
  run->SetOutputFile(outFile);

  ERHe8EventHeader* header = new ERHe8EventHeader();
  run->SetEventHeader(header);

  ERRTelescopeCalibrator* rTelCalibrator = new ERRTelescopeCalibrator();
  rTelCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(rTelCalibrator);

  ERQTelescopeCalibrator* qTelCalibrator = new ERQTelescopeCalibrator();
  qTelCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(qTelCalibrator);

  ERBeamDetCalibrator* beamDetCalibrator = new ERBeamDetCalibrator();
  beamDetCalibrator->SetParametersFile(workdir + "/input/clb4sonya.dat");
  run->AddTask(beamDetCalibrator);

  ERBeamDetReconstructor* beamDetRecon = new ERBeamDetReconstructor(1);
  run->AddTask(beamDetRecon);

  ERTelescopeReconstructor* telescopeRecon = new ERTelescopeReconstructor(1);
  run->AddTask(telescopeRecon);
  
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
