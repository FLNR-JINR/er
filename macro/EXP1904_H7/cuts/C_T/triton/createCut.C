void readCuts();
void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16);

TCutG *cutCsI[16],*cut3h[16],*cutX_C[32];
TCutG *cuthe3_1[16],*cutSQ20_1[16],*cutSQ1_1[16];
TCutG *cuthe3_2[16],*cutSQ20_2[16],*cutSQ1_2[16];
TCutG *cuthe3_3[16],*cutSQ20_3[16],*cutSQ1_3[16];
TCutG *cuthe3_4[16],*cutSQ20_4[16],*cutSQ1_4[16];

Float_t pCsI_1[16],pCsI_2[16];

Int_t nCoins;

void createCut(){
  nCoins = 0;
  readPar("CsI",pCsI_1,pCsI_2);
  readCuts();
  // return;
// return;
  gStyle->SetOptStat(0);
  Bool_t tritium = 1;

  TChain *ch = new TChain("tree");

  ch->Add("/media/ivan/data/exp1904/analysed/novPars/selected/h7_*.root");
  cout << ch->GetEntries() << endl;

  TString cut,hdraw;

  // if (tritium) {
  Int_t i = 4 ;
  TCanvas *c3h = new TCanvas("c3h","",1800,1000);  
  // c3h->Divide(4,4);
  // c3h->cd(i+1);    
    cut.Form("flagCent && nCsI_track==%d",i);
    // hdraw.Form("X_C:aCsI>>h%d(300,0,150,300,2,90)",pCsI_1[i],pCsI_2[i],i);

    hdraw.Form("X_C:(arCsI[%d]-%f)/%f >>h%d(500,0,4000,200,0,25)",i,pCsI_1[i],pCsI_2[i],i);
    // hdraw.Form("X_C.:aCsI>>h%d",i);
    ch->SetMarkerColor(kBlack);
    ch->SetMarkerStyle(7);
    ch->Draw(hdraw.Data(),cut.Data(),"",70000000);


    // cut.Form("flagCent && nCsI_track==%d && nh3",i);
    // // hdraw.Form("X_C:aCsI>>h%d(300,0,150,300,2,90)",pCsI_1[i],pCsI_2[i],i);

    // hdraw.Form("X_C:(arCsI[%d]-%f)/%f >>h%d_cut(500,0,4000,200,0,25)",i,pCsI_1[i],pCsI_2[i],i);
    // // hdraw.Form("X_C.:aCsI>>h%d",i);
    // ch->SetMarkerColor(kRed);
    // ch->SetMarkerStyle(7);
    // ch->Draw(hdraw.Data(),cut.Data(),"same");

    // cut3h[i]->Draw("same");


    c3h->Update();
}

void readCuts() {

  TFile *f;
  TString cutName;

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts/C_T/triton/h3_%d.root",i);
    f = new TFile(cutName.Data());
    cut3h[i] = (TCutG*)f->Get("CUTG");
    if (!cut3h[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }    
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts_MKpars/he3_1/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_1[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_1[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts/C_T/tCsI/tCsI_full_%d.root",i);
    f = new TFile(cutName.Data());
    cutCsI[i] = (TCutG*)f->Get("CUTG");
    if (!cutCsI[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<32;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts/C_T/tX_C/tX_C_%d.root",i);
    f = new TFile(cutName.Data());
    cutX_C[i] = (TCutG*)f->Get("CUTG");
    if (!cutX_C[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts/T1/tSQ20/tSQ20_%d.root",i);
    f = new TFile(cutName.Data());
    cutSQ20_1[i] = (TCutG*)f->Get("CUTG");
    if (!cutSQ20_1[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts/T1/SQ1_1/tSQ1_%d.root",i);
    f = new TFile(cutName.Data());
    cutSQ1_1[i] = (TCutG*)f->Get("CUTG");
    if (!cutSQ1_1[i]) {
      cout << i  << " no cut"<< endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts_MKpars/he3_2/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_2[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_2[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts_MKpars/he3_3/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_3[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_3[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  for(Int_t i=0;i<16;i++) {
    cutName.Form("/home/ivan/work/macro/he8_1904/cuts_MKpars/he3_4/he3_%d.root",i);
    f = new TFile(cutName.Data());
    cuthe3_4[i] = (TCutG*)f->Get("CUTG");
    if (!cuthe3_4[i]) {
      cout << "no cut " << cutName.Data() << endl;
      return;
    }
    delete f;
  }

  cout << " CUTS are read" << endl;
}

void readPar(TString fileName,Float_t *par1,Float_t *par2,Int_t size=16){

  TString line;
  ifstream myfile;
  Int_t count=0;
  TString file = "/home/ivan/work/soft/er/input/exp1904_pars/" + fileName + ".cal";
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

