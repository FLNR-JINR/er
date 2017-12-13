void drawRes(TString filename="results/digi.root")
{
  TFile* inFile = new TFile(filename, "READ");
  TTree* inTree = (TTree*)inFile->Get("cbmsim");
  //inTree->StartViewer();

  TCanvas* canv = new TCanvas("canv", "canv", 100, 100, 1800, 800); // x, y, w, h
  canv->Divide(2);

  canv->cd(1);
  TH1D* histoAmpsF = new TH1D("histoAmpsF", "histoAmpsF", 200, 0., 20000.);
  inTree->Draw("EventHeader.fNeuRadSumAmplitudeF >> histoAmpsF", "EventHeader.fNeuRadSumAmplitudeF>0");

  canv->cd(2);
  TH1D* histoAmpsB = new TH1D("histoAmpsB", "histoAmpsB", 200, 0., 20000.);
  inTree->Draw("EventHeader.fNeuRadSumAmplitudeB >> histoAmpsB", "EventHeader.fNeuRadSumAmplitudeB>0");
}
