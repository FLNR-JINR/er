void Tektronix_read()
{
	// --- Specify number of events to be produced.
	// --- -1 means run until the end of the input file.
	Int_t nEvents = -1;

	// --- Specify output file name (this is just an example)
	TString outFile = "tektronix.out.root";

	// --- Source task
	ERTektronixSource* source = new ERTektronixSource();
	source->AddFile(1,"/home/vitaliy.schetinin/er/input/1000V_20mv1Wfm_Ch1.txt");
	source->AddFile(1,"/home/vitaliy.schetinin/er/input/1000V_20mv2Wfm_Ch1.txt");

	source->AddFile(2,"/home/vitaliy.schetinin/er/input/1000V_20mv1Wfm_Ch2.txt");
	source->AddFile(2,"/home/vitaliy.schetinin/er/input/1000V_20mv2Wfm_Ch2.txt");

	source->SetNPoints(1000);
	source->SetNChanels(2);

	// --- Run
	FairRunOnline *run = new FairRunOnline(source);
	run->SetOutputFile(outFile);
	run->Init();

	// --- Start run
	TStopwatch timer;
	timer.Start();
	run->Run(nEvents, 0); // run until end of input file
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
