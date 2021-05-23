void readCuts();

TCutG *cutCsI[16],*cut3h[16],*cutX_C[16];
TCutG *cuthe3_1[16],*cutSQ20_1[16],*cutSQ1_1[16];
TCutG *cuthe3_2[16],*cutSQ20_2[16],*cutSQ1_2[16];
TCutG *cuthe3_3[16],*cutSQ20_3[16],*cutSQ1_3[16];
TCutG *cuthe3_4[16],*cutSQ20_4[16],*cutSQ1_4[16];

void drawCut(){
  readCuts();

  gStyle->SetOptStat(0);
  Bool_t tritium = 0;
  Bool_t he3 = 0;
  Bool_t tCsI_s = 1;
  Bool_t target = 0;
  Bool_t centtimes = 0;
  Bool_t sq20times = 0;
  Bool_t sq20Rtimes = 0;
  Bool_t tSQ1_1 = 0;
  Bool_t Y_Lcuts = 0;
  Bool_t Y_Rcuts = 0;

  TChain *ch = new TChain("tree");
  ch->Add("/home/oem/work/data/exp1906/analysed/be_10_1-4_cut.root");

  cout << ch->GetEntries() << endl;

  TCutG *cutCsI_s[16],*cut3h[16],*cutX_L[16],*cutY_L[16],*cutSQ20_L[16],*cuthe3[16],*cutSQ20_R[16],*cutY_R[16];
  TString cutName;
  TFile *f1;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/tritonCuts/h3_%d.root",i);
    f1 = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f1->Get("CUTG");
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/CsItimeCuts/CsI_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutCsI_s[i] = (TCutG*)f1->Get("CUTG");
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutX_L/X_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutX_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutX_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/SQ20_Rcuts/sq20_R_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutSQ20_R[i] = (TCutG*)f1->Get("CUTG");
    if (!cutSQ20_R[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutY_L/Y_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutY_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutY_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/SQ20_Lcuts/sq20_L_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutSQ20_L[i] = (TCutG*)f1->Get("CUTG");
    if (!cutSQ20_L[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/helium3/he3_%d.root",i);
    f1 = new TFile(cutName.Data());
    cuthe3[i] = (TCutG*)f1->Get("CUTG");
    if (!cuthe3[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }
  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1811/cutY_R/Y_R_%d.root",i);
    f1 = new TFile(cutName.Data());
    cutY_R[i] = (TCutG*)f1->Get("CUTG");
    if (!cutY_R[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f1;
  }


  TString cut,hdraw;

  if (tritium) {
    TCanvas *c1 = new TCanvas("c1","",1800,1000);  
    c1->Divide(4,4);
    for(Int_t i=0;i<16;i++) {

      c1->cd(i+1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("nCsI==%d && flagCent",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"col");
      c1->Update();

      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kGreen);      
      cut.Form("nCsI==%d && nh3",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h_red%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");
      c1->Update();
     
      ch->SetMarkerStyle(20);
      ch->SetMarkerColor(kRed); 
      ch->SetMarkerSize(0.4);          
      cut.Form("nCsI==%d && nh3 && nhe3_1",i);
      // hdraw.Form("X_C.:aCsI>>h%d(300,0,4000,800,0,60)",i);
      hdraw.Form("X_C.:aCsI>>h_coin%d(500,0,4048,500,1,30)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");
      c1->Update();

    }

  }

 
  if (tCsI_s) {
    TCanvas *c3 = new TCanvas("c3","",1800,1000);  
    c3->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c3->cd(i+1);
      // {Int_t i=6;
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("nCsI==%d",i);
      hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_1_%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");

      ch->SetMarkerColor(kRed);
      cut.Form("nCsI==%d && flagCent",i);
      hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_h3_%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      // ch->SetMarkerColor(kGreen);
      // cut.Form("nCsI==%d && trigger!=1 && nheIsotope && flagCent",i);
      // hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_alpha_%d",i);
      // ch->Draw(hdraw.Data(),cut.Data(),"same");

      // ch->SetMarkerColor(kBlack);
      // cutCsI_s[i]->Draw("same");
      // cutCsI_s[i]->SetLineColor(kBlack);
      // cutCsI_s[i]->SetLineWidth(2);
      // ch->SetMarkerColor(kRed);
      // ch->SetMarkerStyle(20);
      // ch->SetMarkerSize(0.1); 


      c3->Update();
    }

    // TCanvas *c3_1 = new TCanvas("c3_1","",1800,1000);  
    // c3_1->Divide(4,4);
    // for(Int_t i=0;i<16;i++) {
    //   c3_1->cd(i+1);
    //   cut.Form("nCsI==%d && trigger==1",i);
    //   // tree->Draw("DSDX_C:aCsI_s","nCsI_s==0","", 1004737, 0);
    //   hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_1_%d_trigg1(700,-400,300,3000,0,3000)",i);
    //   ch->Draw(hdraw.Data(),cut.Data(),"col",1000000,0);
    //   cutCsI_s[i]->Draw("same");
    //   cutCsI_s[i]->SetLineColor(kBlack);
    //   cutCsI_s[i]->SetLineWidth(2);
    //   ch->SetMarkerColor(kRed);
    //   ch->SetMarkerStyle(20);
    //   ch->SetMarkerSize(0.1); 

    //   // cut.Form("nCsI==%d && flagCent",i);
    //   // hdraw.Form("aCsI:tCsI-tF5 >> h_CsI_2_%d(200,-400,300,200,0,3000)");
    //   // ch->Draw(hdraw.Data(),cut.Data(),"same",,1000000,0);

    //   c3_1->Update();
    // }

    // c3->Print("/home/user/Desktop/redmine/CsI_cuts.png");
  }



  if (centtimes) {

    TCanvas *c5 = new TCanvas("c5","",1800,1000);
    c5->Divide(4,4);

    for(Int_t i=0;i<16;i++) {
      // Int_t i = 15;
      c5->cd(i+1);

      cutName.Form("nX_C==%d && flagCent",i);
      ch->SetMarkerColor(kBlack);  
      ch->Draw("X_C:tX_C - tF5",cutName.Data(),"");

      ch->SetMarkerColor(kRed);
      cutName = cutName + TString(" && nh3");
      ch->Draw("X_C:tX_C - tF5",cutName.Data(),"same");

      cutX_C[i]->SetLineWidth(2);
      cutX_C[i]->Draw("same");

      c5->Update();
    }

  }

  if (sq20times) {

    TCanvas *c6 = new TCanvas("c6","sq20_L times",1800,1000);
    c6->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
    // { 
    //   Int_t i = 0;
      c6->cd(i+1);
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);
      cut.Form("n20_4==%d && flag4",i);
      hdraw.Form("a20_4:t20_4-tF5 >> h_tsq20_%d(300,0,120,300,0,20)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");

      ch->SetMarkerColor(kRed);
       ch->SetMarkerStyle(20);
      cut.Form("n20_4==%d && nhe3_4",i);
      hdraw.Form("a20_4:t20_4-tF5 >> h_tsq20_red_%d(300,0,120,300,0,20)",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      c6->Update();
    }
    // c6->Print("/home/user/Desktop/pictures/t20_L_all.png");

  }


if (tSQ1_1) {
    TCanvas *c7 = new TCanvas("c7","",1800,1000);  
    c7->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c7->cd(i+1);
    // { Int_t i=0;
      ch->SetMarkerStyle(1);
      // ch->SetMarkerSize(1.);

      cut.Form("n1_1==%d && flag1",i);
      hdraw.Form("a1_1:t1_1-tF5 >> h_X_L_%d(2000,0,100,2000,-10,60)",i);
      ch->SetMarkerColor(kBlack);
      ch->Draw(hdraw.Data(),cut.Data(),"col");

      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(0.5);
      cut.Form("n1_1==%d && nhe3_1",i);
      hdraw.Form("a1_1:t1_1-tF5 >> h_X_Lhe3_%d(2000,0,100,2000,-10,60)",i);
      ch->SetMarkerColor(kRed);
      ch->Draw(hdraw.Data(),cut.Data(),"same");

      c7->Update();
    }
  }


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
    cuthe3_1[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_1[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/C_T/tCsI/tCsI_%d.root",i);
    f = new TFile(cutName.Data());
    cutCsI[i] = (TCutG*)f->Get("CUTG");
    if (!cutCsI[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/draw/tX_C/tX_C_%d.root",i);
    f = new TFile(cutName.Data());
    cutX_C[i] = (TCutG*)f->Get("CUTG");
    if (!cutX_C[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T1/tSQ20/tSQ20_%d.root",i);
    f = new TFile(cutName.Data());
    cutSQ20_1[i] = (TCutG*)f->Get("CUTG");
    if (!cutSQ20_1[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T1/SQ1_1/tSQ1_%d.root",i);
    f = new TFile(cutName.Data());
    cutSQ1_1[i] = (TCutG*)f->Get("CUTG");
    if (!cutSQ1_1[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T2/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T3/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/oem/work/macro/he8_1904/cuts/T4/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_4[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_4[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

}
