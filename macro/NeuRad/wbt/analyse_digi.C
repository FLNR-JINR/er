void analyse_digi(TString filename="results/digi.root")
{
  TFile* inFile = new TFile(filename, "READ");
  if (inFile->IsZombie()) {
    cerr << "Could not open file '" << filename << "'. Aborting." << endl;
    return;
  }

  TTree* inTree = (TTree*)inFile->Get("cbmsim");
  if (!inTree) {
    cerr << "Could not find tree 'cbmsim'. Aborting." << endl;
    return;
  }

  TBranch* neuradPhotoElectrons = inTree->GetBranch("NeuRadPhotoElectron");
  if (!neuradPhotoElectrons) {
    cerr << "Could not find branch 'NeuRadPhotoElectron'. Aborting." << endl;
    return;
  }

  TBranch* neuRadPixelSignals = inTree->GetBranch("NeuRadPixelSignal");
  if (!neuRadPixelSignals) {
    cerr << "Could not find branch 'NeuRadPixelSignal'. Aborting." << endl;
    return;
  }

}
