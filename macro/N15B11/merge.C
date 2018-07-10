void merge(TString workDir="output_parallel")
{
	TChain ch("er");

	TString inputFile;
	inputFile.Form("%s/sim_*.root", workDir.Data());
	ch.Add(inputFile);

	TString outputFile;
	outputFile.Form("sim_merge.root");
	ch.Merge(outputFile);
}
