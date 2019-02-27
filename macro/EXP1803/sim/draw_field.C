void draw_field()
{
  TString fieldName = "field_v12b";
  Double_t fzmin =  -300.;    // along z axis
  Double_t fzmax =   300.;
  ERFieldMap* field = new ERFieldMap("testField","A");
  field->Init();
  field->Print();
  // -------  Create graphs and histograms  -------------------------------
  Int_t fnz = TMath::Nint( (fzmax-fzmin) * 3. );
  Double_t* z = new Double_t [fnz+1];
  Double_t* Bx =new Double_t [fnz+1];
  Double_t targZ = 0.;
  Double_t x, y;
  Double_t by;//, bz, bt, b;
  //  Double_t fdx = (fxmax-fxmin) / Double_t(fnx);
  //  Double_t fdy = (fymax-fymin) / Double_t(fny);
  Double_t fdz = (fzmax-fzmin) / Double_t(fnz);
  // ---> Loop over z axis (x=y=0)
  x = y = 0.;
  Double_t fieldval[3];
  Double_t point[3];
  for (Int_t iz=0; iz<=fnz; iz++) {
    z[iz] = fzmin + Double_t(iz) * fdz;
//    Bx[iz] = field->GetBx(x,y,z[iz]) / 10.;
    point[0]=x;
    point[1]=y;
    point[2]=z[iz];
    field->GetFieldValue(point, fieldval);
    Bx[iz] = fieldval[0];
  }
  TCanvas* c1 = new TCanvas("c1", "canvas", 768, 1152);
  TGraph* hBx1 = new TGraph(fnz+1,z,Bx);
  hBx1->SetLineColor(4);
  hBx1->SetLineWidth(2);
  hBx1->SetTitle("x = y = 0");
  hBx1->GetXaxis()->SetTitle("z [cm]");
  hBx1->GetYaxis()->SetTitle("B_{x} [T]");
  hBx1->Draw("ACP");
  max_ = hBx1->GetHistogram()->GetMaximum();
  min_ = hBx1->GetHistogram()->GetMinimum();
  TLine* l1 = new TLine(targZ, min_, targZ, max_);
  l1->Draw();
  c1->Update();
}
