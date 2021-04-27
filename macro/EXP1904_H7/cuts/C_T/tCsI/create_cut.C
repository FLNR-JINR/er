void create_cut(){

  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/h7_0_cut.root");
  cout << ch->GetEntries() << endl; 

  TString hdraw,cut;
  TCanvas *c1 = new TCanvas("c1","",1800,1000); 

  Int_t i = 11;

  ch->SetMarkerColor(kBlack);      
  hdraw.Form("arCsI[%d]:trCsI[%d]-tF5>>h%d(1000,200,700,4000,0,4000)",i,i,i);
  cut.Form("flagCent && nCsI_track==%d",i);
  // hdraw.Form("X_C.:aCsI>>h%d",i);
  // gPad->SetLogz();/
  ch->Draw(hdraw.Data(),cut.Data(),"");
  // gPad->SetLogz();
  c1->Update();

  return;
}