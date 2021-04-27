void create_cut(){

  TChain *ch1 = new TChain("er"); //e4
  ch1->Add("/home/oem/work/data/exp1904/digi/h7/h7_*"); 

  ch1->SetMarkerSize(0.7);
  // ch1->SetMarkerColor(kRed);
  ch1->SetMarkerStyle(20);

  TString draw,cut;
  // Int_t i=0;

  TCanvas *c1 = new TCanvas("c1","",1800,1000);  
  c1->Divide(8,4);
  for(Int_t i=0;i<32;i++) {
    c1->cd(i+1);

    cut.Form("ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_X.fStripNb==%d && EventHeader.fTrigger!=1",i);
    draw.Form("1000*ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_X.fEdep:ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_X.fTime-0.5*BeamDetToFDigi2.fTime >> hX%d(200,-200,100,400,0,4000)",i);

    ch1->Draw(draw.Data(),cut.Data(),"col", 2000000, 0);

    c1->Update();
  }
  c1->Print("/home/oem/Desktop/pics/exp1904/DSD_X_amptime.png");

  TCanvas *c3 = new TCanvas("c3","",1800,1000);  
  c3->Divide(8,4);
  for(Int_t i=0;i<32;i++) {
    c3->cd(i+1);

    cut.Form("ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_Y.fStripNb==%d && EventHeader.fTrigger!=1",i);
    draw.Form("1000*ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_Y.fEdep:ERQTelescopeSiDigi_Central_telescope_DoubleSi_DSD_XY_0_Y.fTime-0.5*BeamDetToFDigi2.fTime >> hY%d(200,-200,100,400,0,4000)",i);

    ch1->Draw(draw.Data(),cut.Data(),"col", 2000000, 0);

    c3->Update();
  }

  c3->Print("/home/oem/Desktop/pics/exp1904/DSD_Y_amptime.png");

  TCanvas *c2 = new TCanvas("c2","",1800,1000);  
  c2->Divide(4,4);
  for(Int_t i=0;i<16;i++) {
  // { Int_t i=5;
    c2->cd(i+1);

    cut.Form("ERQTelescopeCsIDigi_Central_telescope_CsI_0.fBlockNb==%d && EventHeader.fTrigger!=1",i);
    draw.Form("ERQTelescopeCsIDigi_Central_telescope_CsI_0.fEdep:ERQTelescopeCsIDigi_Central_telescope_CsI_0.fTime-0.5*BeamDetToFDigi2.fTime >> h%d(400,200,1000,400,0,4000)",i);
// draw.Form("1000*ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1.fEdep:ERQTelescopeSiDigi_Telescope_1_SingleSi_SSD_1_Y_1.fTime-0.5*BeamDetToFDigi2.fTime >> h%d",i);    

    ch1->Draw(draw.Data(),cut.Data(),"col", 2000000, 0);

    c2->Update();
  }

  c2->Print("/home/oem/Desktop/pics/exp1904/DSD_X_amptime.png");

  return;
}