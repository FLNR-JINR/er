void merge(TString workDir="../output_parallel")
{
	TChain ch("er");

	TString inputFile;
	inputFile.Form("%s/sim_*.root", workDir.Data());
	ch.Add(inputFile);

	TString outputFile;
	outputFile.Form("%s/sim_merge.root", "output_digi");
	ch.Merge(outputFile);
}
