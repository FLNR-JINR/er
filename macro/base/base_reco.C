void base_reco(){

	TString inFile = "sim.root";
	TString outFile = "digi.root";
	TString parFile = "par.root";
	TString parOutFile = "parOut.root";
	
	TString geoPath = gSystem->Getenv("VMCWORKDIR");

	FairLogger::GetLogger()->SetLogScreenLevel("DEBUG2");

	ERRunAna*run = new ERRunAna();
	run->SetGeomFile("all.root");
	run->SetInputFile(inFile);
	run->SetOutputFile(outFile);
	run->Init();
	run->Run(0,1);
	run->Calc();
}