void merge(TString file1, TString file2, TString fresult){
	TFile f1(file1);
  	TFile f2(file2);
  	TFile out(fresult, "RECREATE");

  	TTree* T1 = (TTree*) f1.Get("cbmsim");
  	TTree* T2 = (TTree*) f2.Get("cbmsim");
  
  	TList *l = new TList();
  	l->Add(T1);
  	l->Add(T1);
  	TTree *newtree = TTree::MergeTrees(l);
  	newtree->SetName("newtree");
  	out.cd();
  	newtree->Write();
}