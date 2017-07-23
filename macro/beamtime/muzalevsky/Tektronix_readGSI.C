void Tektronix_readGSI()
{
	// --- Specify number of events to be produced.
	// --- -1 means run until the end of the input file.
	Int_t nEvents = -1;

	// --- Specify output file name (this is just an example)
	TString outFile = "/store/ivan/dataEXP/tektronix.out1.root";

// 
	ERTektronixSource* source = new ERTektronixSource();
	TString filename1,filename2; //	i=0;
	const Int_t nfiles = 20000;
	for(Int_t i=0; i<nfiles; i++) {
		filename1.Form("/store/ivan/dataEXP/muzalevsky/1000V_trigg20mv_20kentr/1000V_20mv%dWfm_Ch1.txt",i+1);
		filename2.Form("/store/ivan/dataEXP/muzalevsky/1000V_trigg20mv_20kentr/1000V_20mv%dWfm_Ch2.txt",i+1);
		source->AddFile(1,filename1.Data());
		source->AddFile(2,filename2.Data());
	}
	// --- Source task
	
	/*source->AddFile(1,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch12016.12.07-02.11.54.dat");
	source->AddFile(1,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch12016.12.07-08.48.05.dat");
	source->AddFile(1,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch12016.12.07-10.03.01.dat");

	source->AddFile(2,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch22016.12.07-02.11.54.dat");
	source->AddFile(2,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch22016.12.07-08.48.05.dat");
	source->AddFile(2,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch22016.12.07-10.03.01.dat");

	source->AddFile(3,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch32016.12.07-02.11.54.dat");
	source->AddFile(3,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch32016.12.07-08.48.05.dat");
	source->AddFile(3,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch32016.12.07-10.03.01.dat");

	source->AddFile(4,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch42016.12.07-02.11.54.dat");
	source->AddFile(4,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch42016.12.07-08.48.05.dat");
	source->AddFile(4,"/home/vitaliy/NeuRad_tests/data/rawDataTektronix/ch42016.12.07-10.03.01.dat");*/

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
