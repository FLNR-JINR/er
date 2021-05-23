void readCuts();

TCutG *cutCsI[16],*cut3h[16];
TCutG *cuthe3[16];

void drawID() {
  
  readCuts();

  TChain *ch = new TChain("tree");
  ch->Add("/home/oem/work/data/exp1904/analysed/h7/h7_all_cut.root");  
  cout << "Found " << ch->GetEntries() << " entries" << endl;

  TCanvas *c1 = new TCanvas("c1","",1800,1000);  
  // ch->Draw("a20_1:a1_1>>id1(500,1.,40,250,0.1,5)","flag1","col");
  // c1->Print("/home/oem/Desktop/pics/exp1904/de_E_1.pdf");

  // TCanvas *c2 = new TCanvas("c2","",1800,1000);  
  // ch->Draw("a20_2:a1_2>>id2(500,1.,40,250,0.1,5)","flag2","col");
  // c2->Print("/home/oem/Desktop/pics/exp1904/de_E_2.pdf");

  // TCanvas *c3 = new TCanvas("c3","",1800,1000);  
  // ch->Draw("a20_3:a1_3>>id3(500,1.,40,250,0.1,5)","flag3","col");
  // c3->Print("/home/oem/Desktop/pics/exp1904/de_E_3.pdf");

  // TCanvas *c4 = new TCanvas("c4","",1800,1000);  
  // ch->Draw("a20_4:a1_4>>id4(500,1.,40,250,0.1,5)","flag4","col");
  // c4->Print("/home/oem/Desktop/pics/exp1904/de_E_4.pdf");

// // return;
  TString canName,cut,hDraw;
 //  TCanvas *c_arr[16];
 //  for(Int_t i=0;i<16;i++){
 //    canName.Form("c%d",i+1);
 //    c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
 //  }

 //  for(Int_t i=0;i<16;i++) {
 // // { Int_t i =14;
 //    c_arr[i]->cd();
 //    // c1->cd(i+1);
 //    ch->SetMarkerSize(0.5);
 //    ch->SetMarkerColor(kBlack);
 //    cut.Form("nCsI==%d",i);
 //    // hDraw.Form("a20_4:a1_4>>h%d(500,0.3,50,500,0.3,5)",i);
 //    hDraw.Form("X_C:aCsI>>CentId_%d(500,0,4048,500,1,50)",i);
 //    ch->Draw(hDraw.Data(),cut.Data(),"col");
 //    c_arr[i]->Update();
 //  }

 //  c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf");
 //  c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf[");
 //  c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf");
 //  for(Int_t i=1;i<15;i++) {
 //    c_arr[i]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf");
 //  }
 //  c_arr[15]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf");
 //  c_arr[15]->Print("/home/oem/Desktop/pics/exp1904/de_E_cent.pdf]");

  // TString canName,cut,hDraw;
  // TCanvas *c_arr[16];
  // for(Int_t i=0;i<16;i++){
  //   canName.Form("c%d",i+1);
  //   c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
  // }

  // for(Int_t i=0;i<16;i++) {
 {Int_t i = 1;
    // c_arr[i]->cd();

    cut.Form("n20_2==%d && flag2",i);
    hDraw.Form("a20_2:a1_2 >>h%d(500,0.5,40,250,0.1,5)",i);
    ch->Draw(hDraw.Data(),cut.Data(),"col");

    ch->SetMarkerStyle(20);
    ch->SetMarkerSize(0.9);
    ch->SetMarkerColor(kGreen);
    cut.Form("n20_2==%d && flag2 && nh3",i);
    hDraw.Form("a20_2:a1_2 >>hred%d(500,0.5.,40,250,0.1,5)",i);
    ch->Draw(hDraw.Data(),cut.Data(),"same");

    // ch->SetMarkerSize(1.3);
    // ch->SetMarkerColor(kRed);
    // cut.Form("n20_2==%d && flag2 && nh3 && nhe3_1",i);
    // hDraw.Form("a20_2:a1_2 >>hcoin%d(500,0.5,40,250,0.1,5)",i);
    // ch->Draw(hDraw.Data(),cut.Data(),"same");

    cuthe3[i]->Draw("same");

    // c_arr[i]->Update();
  }

  // c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf");
  // c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf[");
  // c_arr[0]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf");
  // for(Int_t i=1;i<15;i++) {
  //   c_arr[i]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf");
  // }
  // c_arr[15]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf");
  // c_arr[15]->Print("/home/oem/Desktop/pics/exp1904/de_E_1_all.pdf]");


  return;
}

void readCuts() {

  TFile *f;
  TString cutName;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/C_T/triton/h3_%d.root",i);
    f = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f->Get("CUTG");
    if (!cut3h[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }    
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T1/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

}
