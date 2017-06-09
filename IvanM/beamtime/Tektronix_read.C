void Tektronix_read()
{
	// --- Specify number of events to be produced.
	// --- -1 means run until the end of the input file.
	Int_t nEvents = -1;

	// --- Specify output file name (this is just an example)
	TString outFile = "/home/ivanm/er/IvanM/NeuRad/expData/dataTek/tektronix.out.root";

	// --- Source task
	ERTektronixSource* source = new ERTektronixSource();
	source->AddFile(1,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch12016.12.07-02.11.54.dat");
	source->AddFile(1,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch12016.12.07-08.48.05.dat");
	source->AddFile(1,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch12016.12.07-10.03.01.dat");

	source->AddFile(2,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch22016.12.07-02.11.54.dat");
	source->AddFile(2,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch22016.12.07-08.48.05.dat");
	source->AddFile(2,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch22016.12.07-10.03.01.dat");

	source->AddFile(3,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch32016.12.07-02.11.54.dat");
	source->AddFile(3,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch32016.12.07-08.48.05.dat");
	source->AddFile(3,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch32016.12.07-10.03.01.dat");

	source->AddFile(4,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch42016.12.07-02.11.54.dat");
	source->AddFile(4,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch42016.12.07-08.48.05.dat");
	source->AddFile(4,"/home/ivanm/er/IvanM/NeuRad/expData/rawDataTek/ch42016.12.07-10.03.01.dat");

	source->SetNPoints(1000);
	source->SetNChanels(4);

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
