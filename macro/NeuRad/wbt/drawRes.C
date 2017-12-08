void drawRes(TString filename="results/sim.root")
{
  TFile* inFile = new TFile(filename, "READ");
  TTree* inTree = (TTree*)inFile->Get("cbmsim");
  inTree->StartViewer();
}
