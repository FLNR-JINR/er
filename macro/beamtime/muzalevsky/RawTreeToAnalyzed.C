//#include "FairRunOnline.h"

//using namespace std;

void RawTreeToAnalyzed()
{
	TString inFile = "WPT_tests/680pF_11okt_measure_with_opt_grease/HV1000_Sci_mounted_Am241in_front_10cm_thr32_nxtrim1_11oct_30min_0000_tree.root";

	// --- Specify number of events to be produced.
	// --- -1 means run until the end of the input file.
	Int_t nEvents = 10000;

	// --- Specify output file name (this is just an example)
	TString outFile = "out.root";

	std::cout << ">>> input file is " << inFile  << std::endl;
	std::cout << ">>> output file is " << outFile << std::endl;

	// --- Source task
	ERNXyterTreeSource* source = new ERNXyterTreeSource();
	source->SetInFile(inFile,"theTree","theBranch.");
//	source->ImportCalParameters("/home/vratik/workspace/NeuRad_tests/parameters/cal1e_HV1000_nonLin.par");

	// --- Run
	FairRunOnline *run = new FairRunOnline(source);
	run->SetOutputFile(outFile);


//	ERLiCalibrator* calibrator = new ERLiCalibrator(1);
//	run->AddTask(calibrator);

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
