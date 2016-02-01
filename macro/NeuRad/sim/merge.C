void merge(TString input, TString fresult){
	TObjArray *files = input.Tokenize(",");
	TList* l = new TList();
	for (int i =0; i < files->GetEntriesFast(); i++){
		TFile *f  = new TFile( ((TObjString *)(files->At(i)))->String());
		TTree *t = (TTree*) f->Get("cbmsim");
		l->Add(t);
	}
	
  	TFile out(fresult, "RECREATE");
  	TTree *newtree = TTree::MergeTrees(l);
  	newtree->SetName("newtree");
  	out.cd();
  	newtree->Write();
}
