#include "/home/muzalevskii/work/macro/h7_1904/cuts/scripts/create_cuts.C"
#include "/home/muzalevskii/work/macro/h7_1904/cuts/scripts/create_IDs.C"

void readCuts();
void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16);



Float_t pCsI_1[16],pCsI_2[16];

Int_t nCoins;

void create_cut(){

  create_IDs();
  create_cuts();

  nCoins = 0;
  // readPar("CsI_anh",pCsI_1,pCsI_2);
  // readCuts();
  // return;
  gStyle->SetOptStat(0);
  Bool_t tritium = 0;
  Bool_t he3_1 = 1;
  Bool_t he3_2 = 0;
  Bool_t he3_3 = 0;
  Bool_t he3_4 = 0;
  Bool_t tCsI_s = 0;
  Bool_t target = 0;
  Bool_t centtimes = 0;
  Bool_t CsI_tracking = 0;
  Bool_t tSQ20 = 0;
  Bool_t tSQThick = 0;
  Bool_t neutronID = 0;


  TChain *ch = new TChain("tree");

  ch->Add("/mnt/data/exp1904/analysed/selected/siTriggers/h7_ct_*.root");
  cout << ch->GetEntries() << endl;


  TChain *ch1 = new TChain("tree");
  ch1->Add("/media/ivan/data/exp1904/analysed/novPars/calibrated/finalCal/h7_ct_*_cal.root");
  cout << ch1->GetEntries() << endl;


  TString cut,hdraw;

  if (neutronID) {
    TCanvas *cNeutron = new TCanvas("cNeutron","",1000,1000);  
    cNeutron->cd();

    ch->SetMarkerColor(kBlack);      
    hdraw.Form("ND_tac:ND_amp >> (200,0,4000,200,-500,1500)");
    // hdraw.Form("X_C.:aCsI>>h%d",i);
    // gPad->SetLogz();/
    ch1->Draw(hdraw.Data(),"","col",10000000,0);
    // gPad->SetLogz();
    cNeutron->Update();


  }


  if (tCsI_s) {
    TCanvas *c1 = new TCanvas("c1","",1800,1000);  
    c1->Divide(4,4);
    for(Int_t i=0;i<16;i++) {

      c1->cd(i+1);

      ch->SetMarkerColor(kBlack);      
      hdraw.Form("arCsI[%d]:trCsI[%d]-tF5>>h%d",i,i,i);
      cut.Form("nCsI_track==%d && trCsI[%d]-tF5>200 && trCsI[%d]-tF5<600",i,i,i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      // gPad->SetLogz();/
      ch->Draw(hdraw.Data(),cut.Data(),"",500000,0);
      // gPad->SetLogz();
      c1->Update();


      ch->SetMarkerColor(kRed);      
      hdraw.Form("arCsI[%d]:trCsI[%d]-tF5>>h_cut%d",i,i,i);
      cut.Form("flagCent && nCsI_track==%d && trCsI[%d]-tF5>200 && trCsI[%d]-tF5<600",i,i,i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      // gPad->SetLogz();/
      ch->Draw(hdraw.Data(),cut.Data(),"same",500000,0);
      // gPad->SetLogz();
      c1->Update();


      CsI_cut[i]->SetLineWidth(1);
      CsI_cut[i]->SetLineColor(kRed);
      CsI_cut[i]->Draw("same");
      c1->Update();

    }
  }

  if (tritium) {

    TCanvas *c3h = new TCanvas("c3h","",1800,1000);  
    c3h->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c3h->cd(i+1);    
      cut.Form("nCsI_track==%d && flagCent",i);
      // hdraw.Form("X_C:aCsI>>h%d(300,0,150,300,2,90)",pCsI_1[i],pCsI_2[i],i);

      ch->SetMarkerColor(kBlack);
      hdraw.Form("X_C:arCsI[%d] >>h%d_cut(500,0,150,200,0,25)",i,i);
      ch->Draw(hdraw.Data(),cut.Data(),"col",10000000,0);
      c3h->Update();

      ch->SetMarkerStyle(20);
      ch->SetMarkerColor(kRed);
      cut.Form("nCsI_track==%d && flagCent && nh2 && ( (nhe3_1 && flag1) || (nhe3_2 && flag2) || (nhe3_3 && flag3) || (nhe3_4 && flag4) )",i);
      hdraw.Form("X_C:arCsI[%d]",i);
      ch->Draw(hdraw.Data(),cut.Data(),"same");
      c3h->Update();


      // cut3h[i]->SetLineColor(kBlue);
      // cut2h[i]->SetLineColor(kRed);
      // cut1h[i]->SetLineColor(kGreen);
      // cut3h[i]->Draw("same");
      // cut2h[i]->Draw("same");
      // cut1h[i]->Draw("same");
      // ch->Draw(hdraw.Data(),cut.Data(),"");
     
      c3h->Update();

    }
 
    c3h->Print("/home/muzalevskii/Desktop/de_E_triton.png");

  }

  if (target) {
    TCanvas *c4 = new TCanvas("c4","target cut",1000,1000);  
    cout << ch->Draw("fYt:fXt>>h_target(100,-30,30,100,-30,30)","nCsI>-1 && X_C>0","col") << endl;

    ch->SetMarkerColor(kRed);
    ch->Draw("fYt:fXt>>h_targetCut(100,-30,30,100,-30,30)","nCsI>-1 && X_C>0 && ((fXt-0.5)*(fXt-0.5) + (fYt-1)*(fYt-1))<9*9","same");
  }

  if (centtimes) {
    TCanvas *c3h = new TCanvas("c3h","",1800,1000);  
    c3h->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      c3h->cd(i+1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("nX_C==%d && trigger!=1",i);
      hdraw.Form("X_C:tX_C-tF5>>h%d",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);

      ch->Draw(hdraw.Data(),cut.Data(),"",1000000,0);
      c3h->Update();
      
      ch->SetMarkerColor(kRed);      
      cut.Form("nX_C==%d && flagCent",i);
      hdraw.Form("X_C:tX_C-tF5>>h%d_cut",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);

      ch->Draw(hdraw.Data(),cut.Data(),"same",1000000,0);
      c3h->Update();

      dssd_x_cut[i]->SetLineColor(kRed);
      dssd_x_cut[i]->Draw("same");


      c3h->Update();
    }
  }

  if (he3_1) {
    TCanvas *c3h_1 = new TCanvas("c3h_1","",1800,1000);
    
    Int_t i=0;

    c3h_1->cd();
    ch->SetMarkerStyle(1);
    ch->SetMarkerColor(kBlack);      
    // cut.Form("n20_1==%d && flag1",i);
    // cut.Form("flag1 && a1_1+a20_1_un<30 && a20_1<7 && n20_1==%d",i);
    cut.Form("flag4 && a1_4+a20_4_un<30 && a20_4<4");
    hdraw.Form("a20_4:a1_4+a20_4_un>>h%d1(3000,0,30,3000,0.5,4)",i);
    // hdraw.Form("X_C.:aCsI>>h%d",i);
    ch->Draw(hdraw.Data(),cut.Data(),"col");
    c3h_1->Update();

    for(Int_t i=0;i<16;i++) {
      cuthe3_4[i]->SetLineColor(kRed);
      cuthe3_4[i]->Draw("same");
    }

    c3h_1->Update();
    
  }

  if (he3_2) {
    TCanvas *c3h_2 = new TCanvas("c3h_2","",1800,1000);

    TString canName;
    TCanvas *c_arr[16];
    for(Int_t i=0;i<16;i++){
      canName.Form("cHe3_2_%d",i+1);
      c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
    }

    // c3h->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      // c3h->cd(i+1);
      c_arr[i]->cd();
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("flag2 && a1_2+a20_2_un<65 && a20_2<8 && n20_2==%d",i);
      hdraw.Form("a20_2:a1_2+a20_2_un>>h%d1",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");
      // c3h->Update();
      c_arr[i]->Update();


      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(0.7);  
      ch->SetMarkerColor(kRed);      
      // cut.Form("n20_1==%d && nhe3_1 && nh3",i);
      cut += " && nhe3_2";
      hdraw.Form("a20_2:a1_2+a20_2_un",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      nCoins+= ch->Draw(hdraw.Data(),cut.Data(),"same");
      // c3h->Update();

      cuthe3_2[i]->SetLineWidth(1);
      cuthe3_2[i]->SetLineColor(kRed);
      cuthe3_2[i]->Draw("same");
      c_arr[i]->Update();
      // c3h->Update();
    }

    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_2.pdf");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_2.pdf[");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_2.pdf");
    for(Int_t i=1;i<15;i++) {
      c_arr[i]->Print("/home/muzalevskii/Desktop/de_E_2.pdf");
    }
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_2.pdf");
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_2.pdf]");
  }

   if (he3_3) {
    TCanvas *c3h = new TCanvas("c3h_3","",1800,1000);

    TString canName;
    TCanvas *c_arr[16];
    for(Int_t i=0;i<16;i++){
      canName.Form("cHe3_3_%d",i+1);
      c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
    }

    // c3h->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      // c3h->cd(i+1);
      c_arr[i]->cd();
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("flag3 && a1_3+a20_3_un<65 && a20_3<8 && n20_3==%d",i);
      hdraw.Form("a20_3:a1_3+a20_3_un>>h%d1",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");
      // c3h->Update();
      c_arr[i]->Update();

      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(0.7);
      ch->SetMarkerColor(kRed);      
      // cut.Form("n20_1==%d && nhe3_1 && nh3",i);
      cut += " && nhe3_3";
      hdraw.Form("a20_3:a1_3+a20_3_un",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      nCoins+= ch->Draw(hdraw.Data(),cut.Data(),"same");
      // c3h->Update();

      cuthe3_3[i]->SetLineWidth(1);
      cuthe3_3[i]->SetLineColor(kRed);
      cuthe3_3[i]->Draw("same");
      c_arr[i]->Update();
      // c3h->Update();
    }
    
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_3.pdf");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_3.pdf[");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_3.pdf");
    for(Int_t i=1;i<15;i++) {
      c_arr[i]->Print("/home/muzalevskii/Desktop/de_E_3.pdf");
    }
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_3.pdf");
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_3.pdf]");
  }

  if (he3_4) {
    TCanvas *c3h = new TCanvas("c3h_4","",1800,1000);

    TString canName;
    TCanvas *c_arr[16];
    for(Int_t i=0;i<16;i++){
      canName.Form("cHe3_4_%d",i+1);
      c_arr[i] = new TCanvas(canName.Data(),"title",1800,1000);
    }

    // c3h->Divide(4,4);
    for(Int_t i=0;i<16;i++) {
      // c3h->cd(i+1);
      c_arr[i]->cd();
      ch->SetMarkerStyle(1);
      ch->SetMarkerColor(kBlack);      
      cut.Form("flag4 && a1_4+a20_4_un<65 && a20_4<8 && n20_4==%d",i);
      hdraw.Form("a20_4:a1_4+a20_4_un>>h%d1",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      ch->Draw(hdraw.Data(),cut.Data(),"");
      // c3h->Update();
      c_arr[i]->Update();


      ch->SetMarkerStyle(20);
      ch->SetMarkerSize(0.7);
      ch->SetMarkerColor(kRed);      
      // cut.Form("n20_1==%d && nhe3_1 && nh3",i);
      cut += " && nhe3_4";
      hdraw.Form("a20_4:a1_4+a20_4_un",i);
      // hdraw.Form("X_C.:aCsI>>h%d",i);
      nCoins+= ch->Draw(hdraw.Data(),cut.Data(),"same");
      // c3h->Update();

      cuthe3_4[i]->SetLineWidth(1);
      cuthe3_4[i]->SetLineColor(kRed);
      cuthe3_4[i]->Draw("same");
      c_arr[i]->Update();
      // c3h->Update();
    }
    
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_4.pdf");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_4.pdf[");
    c_arr[0]->Print("/home/muzalevskii/Desktop/de_E_4.pdf");
    for(Int_t i=1;i<15;i++) {
      c_arr[i]->Print("/home/muzalevskii/Desktop/de_E_4.pdf");
    }
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_4.pdf");
    c_arr[15]->Print("/home/muzalevskii/Desktop/de_E_4.pdf]");
  }


  if (CsI_tracking) {



    // TCanvas *c1 = new TCanvas("c1","",1800,1000);  
    // c1->Divide(4,4);
    // for(Int_t i=0;i<16;i++) {

    //   c1->cd(i+1);

    //   ch->SetMarkerColor(kBlack);      
    //   hdraw.Form("");
    //   cut.Form("nCsI_track==%d",i);
    //   ch->Draw("xCt",cut.Data(),"");
    //   // gPad->SetLogz();
    //   c1->Update();

    // }  


    // TCanvas *c2 = new TCanvas("c2","",1800,1000);  
    // c2->Divide(4,4);
    // for(Int_t i=0;i<16;i++) {

    //   c2->cd(i+1);

    //   ch->SetMarkerColor(kBlack);      

    //   cut.Form("nCsI_track==%d",i);
    //   ch->Draw("yCt",cut.Data(),"");
    //   // gPad->SetLogz();
    //   c2->Update();

    // } 

    TCanvas *c3 = new TCanvas("c3","",1800,1000);
    ch->Draw("nCsI-nCsI_track","flagCent","");

  }


  if (tSQ20) {

    TCanvas *c_sq20 = new TCanvas("c_sq20","",1000,1800);
    c_sq20->Divide(2,1);

    c_sq20->cd(1);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a20_4:t20_4-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a20_4:t20_4-tF5","flag4","same",1000000,0);

    c_sq20->cd(2);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a1_4:t1_4-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a1_4:t1_4-tF5","flag4","same",1000000,0);

return;

    c_sq20->cd(2);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a20_2:t20_2-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a20_2:t20_2-tF5","flag2","same",1000000,0);

    c_sq20->cd(3);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a20_3:t20_3-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a20_3:t20_3-tF5","flag3","same",1000000,0);

    c_sq20->cd(4);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a20_4:t20_4-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a20_4:t20_4-tF5","flag4","same",1000000,0);
  }

  if (tSQThick) {

    TCanvas *c_sqthick = new TCanvas("c_sqthick","",1000,1800);
    c_sqthick->Divide(2,2);

    c_sqthick->cd(1);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a1_1:t1_1-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a1_1:t1_1-tF5","flag1","same",1000000,0);

    c_sqthick->cd(2);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a1_2:t1_2-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a1_2:t1_2-tF5","flag2","same",1000000,0);

    c_sqthick->cd(3);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a1_3:t1_3-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a1_3:t1_3-tF5","flag3","same",1000000,0);

    c_sqthick->cd(4);
    ch->SetMarkerColor(kBlack);
    ch->Draw("a1_4:t1_4-tF5","","",1000000,0);

    ch->SetMarkerColor(kRed);
    ch->Draw("a1_4:t1_4-tF5","flag4","same",1000000,0);
    c_sqthick->Update();
  }




}

void readCuts() {

  TFile *f;
  TString cutName;

  cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/T1/li6_cut.root");
  f = new TFile(cutName.Data());
  TCutG *cut6Li_1 = (TCutG*)f->Get("CUTG");
  if (!cut6Li_1) {
    cout << "no cut " << cutName.Data() << endl;
    exit(-1);
  }    
  delete f;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/C_T/triton/h3_%d.root",i);
    f = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f->Get("CUTG");
    if (!cut3h[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }    
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/C_T/tCsI/tCsI_full_%d.root",i);
    f = new TFile(cutName.Data());
    CsI_cut[i] = (TCutG*)f->Get("CUTG");
    if (!CsI_cut[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<32;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/C_T/tX_C/tX_C_%d.root",i);
    f = new TFile(cutName.Data());
    dssd_x_cut[i] = (TCutG*)f->Get("CUTG");
    if (!dssd_x_cut[i]) {
      cout << i  << " no cut"<< endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/T1/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_1[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_1[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/T2/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/T3/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/muzalevskii/work/macro/h7_1904/cuts/T4/he3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_4[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_4[i]) {
      cout << "no cut " << cutName.Data() << endl;
      exit(-1);
    }
    delete f;
  }

  cout << " CUTS are read" << endl;
}


void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16){

  TString line;
  ifstream myfile;
  Int_t count=-2;
  TString file = "/home/muzalevskii/work/macro/h7_1904/parameters/" + fileName + ".cal";
  myfile.open(file.Data());
  while (! myfile.eof() ){
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%g %g", par1+count,par2+count);
    count++;
  }
  cout << endl << fileName.Data() << endl;
  for(Int_t i=0;i<size;i++) cout << par1[i] << " " << par2[i] << endl;

  return;
}

