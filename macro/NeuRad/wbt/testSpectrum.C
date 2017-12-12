Double_t SPEfunc(Double_t *x, Double_t *par) {
  Double_t fitval;
  if (x[0]<63) {
    fitval = 0;
  }
  if (x[0]>=63 && x[0]<par[0]) {
    fitval = (x[0]-63) * (par[1]) / (par[0]-63) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[0]) {
    fitval = par[1]*(x[0]-par[2])*(x[0]+par[2]-2*par[3])/((par[0]-par[2])*(par[0]+par[2]-2*par[3])) + par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  if (x[0]>=par[2]) {
    fitval = par[4]*exp( -0.5*(x[0]-par[5])*(x[0]-par[5])/(par[6]*par[6]));
  }
  return fitval;
}

void testSpectrum()
{
  TF1* fSPEfunc = new TF1("fSPEfunc", "SPEfunc", 0., 2000., 7);
  fSPEfunc->SetParameters(85.8656, 30.6158, 447.112, 447.111, 52., 433., 141.);

  fSPEfunc->Draw();
}
