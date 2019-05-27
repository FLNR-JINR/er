void merge(TString workDir=".")
{
	TChain ch("er");

	TString inputFile;
	inputFile.Form("%s/digi_*.root", workDir.Data());
	ch.Add(inputFile);

	TString outputFile;
	outputFile.Form("digi_merge.root");
	ch.Merge(outputFile);
}
