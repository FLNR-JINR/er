void create_cut(){

  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/h7_0_cut.root");
  cout << ch->GetEntries() << endl;

  TCanvas *c3h = new TCanvas("c3h","",1800,1000);  

  TString hdraw,cut;

  Int_t i = 31;

  ch->SetMarkerColor(kBlack);      
  cut.Form("nX_C==%d",i);
  hdraw.Form("X_C:tX_C-tF5>>h%d(1000,-200,100,1000,-10,100)",i);

  ch->Draw(hdraw.Data(),cut.Data(),"");
  c3h->Update();

  return;
}