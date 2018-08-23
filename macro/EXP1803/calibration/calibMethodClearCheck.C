/*В первом приближении не выполняется интерполяция по нескольким точкам. 
  По крайним точкам берется линейная аппроксимация для произодной функции энергопотерь
  в слоях детектора.*/

vector<Double_t> fCalibEnergies = {4.7844e-3, 7.6869e-3};
vector<vector<pair<Double_t, Double_t>>> fChannels(2); // <EnergyInd, StripNb<MinPeak, MaxPeak>>
// vector<Double_t> fChannels; // отсчеты АЦП

vector<Double_t> fm(2);    // |
vector<Double_t> fn(2);    // | All coefs a temporary and mast be changed to unified arrays of parameters
vector<Double_t> fk(2);    // |

Double_t  fDeadLayer;
Int_t     fUpperSubAddress_X = 32;
Int_t     fUpperSubAddress_Y = 16;
Int_t     fSumChannelCount = 2048;
          
TString   fTreeName = "er";
TString   fBranchName = "NeEvent.";
TString   fLeafName_X = "SQX_L";
TString   fLeafName_Y = "SQY_L";
          
Int_t     fLowerChannel = 0;
Int_t     fUpperChannel = 0;
          
vector<TString> fExpRootFile = {
                                "sim_digi_calib_chdirect1.root",
                                "sim_digi_calib_chdirect4.root",
                                "sim_digi_calib_chrot1.root",
                                "sim_digi_calib_chrot4.root"
                               };
TString   fOutRootFileDir = "out_direct.root";
TString   fOutRootFileRot = "out_rot.root";
TString   fChannelFileNames = "channelsDirect.txt";

// Each file contains data for one energy value
std::vector<TString> fMCDataFiles = {
                                     "sim_digi_calib_direct1.root",
                                     "sim_digi_calib_direct4.root",
                                     "sim_digi_calib_rot1.root",
                                     "sim_digi_calib_rot4.root"
                                    };


vector<Double_t> fLowEnergyThresholdChannel; // algorithm described below finds minimal bin which is identified as min value righter than zero

TTree*  fTree;
TString fTmpHistParams;


// cout << "vect " << fLowEnergyThresholdChannel[5] << endl;
Double_t  fFitMinSigma = 15;       //pouziva se, private
Double_t  fFitPeakThreshold = 0.3;

ofstream fNumclCoefsFile;

void InitTree (TString rootFileName) {
  TFile *inputFile = new TFile(rootFileName.Data());
  if ( !inputFile->IsOpen() ) {
    Error("calibMacro", "File %s was not opened and won't be processed", rootFileName.Data());
    return;
  }

  fTree = (TTree*)inputFile->Get(fTreeName);
  if (!fTree) {
    Error("calibMacro", "fTree %s was not found in file %s", fTreeName.Data(), rootFileName.Data());
    return;
  }
}

/*Подсчет количества каналов АЦП для каждого стрипа/пикселя*/
void SearchChanels (Int_t upperSubAddress_X, Int_t upperSubAddress_Y, TString rootFileName, Int_t fileInd) {
  // InitTree(rootFileName);
  // fTmpHistParams.Form("(%d, %d, %d)", 2048, 100, 300);
  // TString  canvasName = "";
  // ofstream calibFile;
  // ofstream chFile;
  // chFile.open("fChannelFileName");
  // calibFile.open("calib.par");
 
  for (Int_t subAddr_X = 0; subAddr_X < upperSubAddress_X; subAddr_X++) {
    // TString varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_0_X.fEdep";
    // TString cutToChooseStrip;
    // cutToChooseStrip.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_0_X.fStripNb==%d", subAddr_X);

    // canvasName.Form("specrumInStrip_F%d_%d", fileInd, subAddr_X);
    // TCanvas* spectrumCanvas = new TCanvas(canvasName);
    // spectrumCanvas->cd(1);

    // TString varInd_X;
    // varInd_X.Form("[%d]", subAddr_X);
    // TSpectrum sc; //by default for 100 peaks
    // varExp = varExp  + ">>tmpHistToGetPeaks" + fTmpHistParams; 
    // cout << "Strip number " << subAddr_X << endl;

    // fTree->Draw(varExp, cutToChooseStrip, "");
    // TH1F *tmpHistToGetPeaks = (TH1F*)gDirectory->Get("tmpHistToGetPeaks");
    // tmpHistToGetPeaks->Rebin(-2);
    // sc.Search(tmpHistToGetPeaks, fFitMinSigma, "", fFitPeakThreshold);
    // Int_t peaksCount = sc.GetNPeaks();
    // Double_t* peaksPos = sc.GetPositionX();
    // // initializing of a vector by specrum finding results
    // vector<Double_t> *peaksChannels = new vector<Double_t>(peaksPos, peaksPos + peaksCount);
    // sort(peaksChannels->begin(), peaksChannels->end());
    // cout << "NPeaks " << peaksCount << endl;
    // cout << "\t peakPos ";
    // for (int i = 0; i < peaksCount; i++) {
    //   cout << peaksChannels->at(i) << " ";
    // }
    // cout <<  endl;
    if(!(fileInd % 2)) {
      cout << "lil" << endl;
      // fChannels[fileInd/2].push_back(pair<Double_t, Double_t>(peaksChannels->front(), 0.));
      fChannels[fileInd/2].push_back(pair<Double_t, Double_t>(0., 0.));
    } else {
      cout << "lil2" << endl;
      // fChannels[(fileInd-1)/2][subAddr_X].second = peaksChannels->front();
      fChannels[(fileInd-1)/2][subAddr_X].second = 0;
    }
    // spectrumCanvas->SaveAs(canvasName + ".root");
    // delete spectrumCanvas;
    // }    
  }
}

void EquationSolverNewton (Int_t stripNb, Double_t eps, Int_t iterLimit) {
  Double_t eta_1  = fn[1] - fn[0];
  Double_t mu_1   = fm[1] - fm[0];

  fChannels[0][stripNb].first  = 137.49;//(int)((4.7844 * 1e-3 - 161.002 * 0.0003 * 0.0003 - 1.48196 * 0.0003 - 2e-4) / 3e-5);
  fChannels[0][stripNb].second = 238.47;//(int)((7.6869 * 1e-3 - 58.6571 * 0.0003 * 0.0003 - 1.08467 * 0.0003 - 2e-4) / 3e-5);
  fChannels[1][stripNb].first  = 131.24;//(int)((4.7844 * 1e-3 - 161.002 * 0.000417049 * 0.000417049 - 1.48196 * 0.000417049 - 2e-4) / 3e-5);
  fChannels[1][stripNb].second = 234.07;//(int)((7.6869 * 1e-3 - 58.6571 * 0.000417049 * 0.000417049 - 1.08467 * 0.000417049 - 2e-4) / 3e-5);

  Double_t nu_1  = fChannels[0][stripNb].first - fChannels[0][stripNb].second;
  Double_t nu_2  = fChannels[1][stripNb].first - fChannels[1][stripNb].second;
  Double_t epsE = fCalibEnergies[0] - fCalibEnergies[1];
// Double_t eta2  = fn[1] - fn[0];
  // Double_t mu2   = fm[1] - fm[0];
  // nu_2 = (4.7844 * 1e-3 - 161.002 * 0.0003 * 0.0003           + 1.48196 * 0.0003      - 2e-4) / 3e-5;
  // nu_1 = (7.6869 * 1e-3 - 161.002 * 0.000417049 * 0.000417049 + 1.48196 * 0.000417049 - 2e-4) / 3e-5;
  cout << endl;
  cout << eta_1 << " " << mu_1 << " " << nu_1 << " " << nu_2 << " " << epsE << endl;
  Double_t a = fmax(epsE / nu_2, epsE / nu_1);//fmax((epsE - eta_1*eta_1/4/mu_1) / nu_2, (epsE - eta_1*eta_1/4/mu_1) / nu_1) + 1e-7;
  cout << "Начальное приближение " << a << endl; //", либо " << fmax(epsE / nu_2, epsE / nu_1) + 1e-7 << endl;
  Double_t aPrev = eps + 1 + a;

  int iterCount = 0;
  while (abs(a - aPrev) > eps) {
    aPrev = a;
    iterCount++;
    if (iterCount > iterLimit) {
      cout << "Too many iterations " << endl;
      break;
    }
    
    Double_t F;
    Double_t f;
    // cout << "preF " << fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*a + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)))) << endl;
    
    F = fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*a + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a))))
      + fn[0]/2/mu_1 * (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)))
      - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * a;

    // F = fm[2]/2/pow(mu_1, 2) * (eta_1 + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - 2*mu_1*(epsE - nu_2*a)))
    //   - fn[2]/2/mu_1 * (eta_1 + (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a))))
    //   - fm[0]/2/pow(mu_1, 2) * (eta_1 + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)) - 2*mu_1*(epsE - nu_1*a)))
    //   + fn[0]/2/mu_1 * (eta_1 + (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a))))
    //   - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * a;
    
    // cout << "F1 = " << F << "; f = " << f << endl;
    // a += 1e-5;
    // F = fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*a + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a))))
    //   + fn[0]/2/mu_1 * (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)))
    //   - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * a;
    cout << "Fdf = " << F << "; f = " << f << endl;

    f = fm[0]/mu_1*(nu_2-nu_1 + eta_1 * (nu_2/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - nu_1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a))))
      + fn[0] * (nu_1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)) - nu_2/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a))) 
      - (fChannels[0][stripNb].first - fChannels[1][stripNb].first);

    // f = fm[2]*nu_1/mu_1 * (eta_1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)) - 1)
    //   + nu_1*fn[2]*(1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*a)))
    //   - fm[0]*nu_1/mu_1 * (eta_1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)) - 1)
    //   - nu_1*fn[0]*(1/sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*a)))
    //   - (fChannels[0][stripNb].first - fChannels[1][stripNb].first);

    a -= F / f;
    cout << "a = " << a << endl;
  }
  // a = 0.00003;
  Double_t d1 = (-eta_1 - sqrt(pow(eta_1, 2) - 4*mu_1*(epsE - nu_1*a))) / 2 / (mu_1);
  Double_t d2 = (-eta_1 - sqrt(pow(eta_1, 2) - 4*mu_1*(epsE - nu_2*a))) / 2 / (mu_1);
  Double_t b = fCalibEnergies[0] - fm[0] * pow(d1, 2) - fn[0] * d1 - a * fChannels[0][stripNb].first;
  cout << "b variants : " << fCalibEnergies[0] - fm[0] * pow(d1, 2) - fn[0] * d1 - a * fChannels[0][stripNb].first  << " "
                          << fCalibEnergies[1] - fm[1] * pow(d1, 2) - fn[1] * d1 - a * fChannels[1][stripNb].first  << " "
                          << fCalibEnergies[0] - fm[0] * pow(d2, 2) - fn[0] * d2 - a * fChannels[0][stripNb].second << " "
                          << fCalibEnergies[1] - fm[1] * pow(d2, 2) - fn[1] * d2 - a * fChannels[1][stripNb].second << endl;
  cout << "Strip " << stripNb << ": " << setw(4) 
       << a << " " << b << " " << d1 << " " << d2 << " , " << iterCount << " iterations" << endl;
  cout << "   N_1 = " << fChannels[0][stripNb].first << "; N_2 = " << fChannels[0][stripNb].second 
       << "; N_1* = " << fChannels[1][stripNb].first << "; N_2* = " << fChannels[1][stripNb].second 
       << "; || N_1 - N_2 = " << fChannels[0][stripNb].first - fChannels[0][stripNb].second 
       << "; N_1* - N_2* = " << fChannels[1][stripNb].first - fChannels[1][stripNb].second << endl
       << "  N_1* - N_1 = " << fChannels[1][stripNb].first - fChannels[0][stripNb].first
       << "; N_2* - N_2 = " << fChannels[1][stripNb].second - fChannels[0][stripNb].second << endl ;
  fNumclCoefsFile << stripNb << setw(4) << ": " << setprecision(5) << a << setw(12) << setprecision(5) << b << setw(12)
                  << setprecision(5) << d1 << setw(12) << setprecision(5) << d2 << " , ";
  fNumclCoefsFile << " " << fChannels[0][stripNb].first << setw(8) << fChannels[0][stripNb].second 
     << setw(8) << fChannels[1][stripNb].first << setw(8) << fChannels[1][stripNb].second 
     << setw(8) << fChannels[0][stripNb].first - fChannels[0][stripNb].second 
     << setw(8) << fChannels[1][stripNb].first - fChannels[1][stripNb].second << setw(8)
     << setw(8) << fChannels[1][stripNb].first - fChannels[0][stripNb].first
     << setw(8) << fChannels[1][stripNb].second - fChannels[0][stripNb].second << endl ;
  // if (stripNb == 0) {
  //   Double_t grA = 1.89e-05, grB = 0.00246391, grD1 = -0.00128995, grD2 = -0.00114981;
    Double_t numclA = a, numclB = b, numclD1 = d1, numclD2 = d2;
  //   cout << "Graph disparity " << "\t {";
  //   cout << fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*grA + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*grA)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*grA))))
  //         + fn[0]/2/mu_1 * (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*grA)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*grA)))
  //         - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * grA << "}" << endl;
  //   // cout << (grA * fChannels[0][stripNb].first  + grB + fm[0] * pow(grD1, 2) + fn[0] * grD1 + fk[0] - fCalibEnergies[0]) << ", ";
  //   // cout << (grA * fChannels[0][stripNb].second + grB + fm[1] * pow(grD1, 2) + fn[1] * grD1 + fk[1] - fCalibEnergies[1]) << ", ";
  //   // cout << (grA * fChannels[1][stripNb].first  + grB + fm[0] * pow(grD2, 2) + fn[0] * grD2 + fk[0] - fCalibEnergies[0]) << ", ";
  //   // cout << (grA * fChannels[1][stripNb].second + grB + fm[1] * pow(grD2, 2) + fn[1] * grD2 + fk[1] - fCalibEnergies[1]) << "}" << endl;

    cout << "Numerical disparity solution" << "\t {";
    cout << (numclA * fChannels[0][stripNb].first  + numclB + fm[0] * pow(numclD1, 2) + fn[0] * numclD1 + fk[0] - fCalibEnergies[0]) << ", ";
    cout << (numclA * fChannels[0][stripNb].second + numclB + fm[1] * pow(numclD1, 2) + fn[1] * numclD1 + fk[1] - fCalibEnergies[1]) << ", ";
    cout << (numclA * fChannels[1][stripNb].first  + numclB + fm[0] * pow(numclD2, 2) + fn[0] * numclD2 + fk[0] - fCalibEnergies[0]) << ", ";
    cout << (numclA * fChannels[1][stripNb].second + numclB + fm[1] * pow(numclD2, 2) + fn[1] * numclD2 + fk[1] - fCalibEnergies[1]) << "}" << endl;
    cout << fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*numclA + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*numclA)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*numclA))))
          + fn[0]/2/mu_1 * (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*numclA)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*numclA)))
          - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * numclA << "}" << endl;
    cout << "Numerical disparity origin" << "\t {";
    cout << (0.00003 * fChannels[0][stripNb].first  + 0.0002 + fm[0] * pow(0.0003, 2) + fn[0] * 0.0003 + fk[0] - fCalibEnergies[0]) << ", ";
    cout << (0.00003 * fChannels[0][stripNb].second + 0.0002 + fm[1] * pow(0.0003, 2) + fn[1] * 0.0003 + fk[1] - fCalibEnergies[1]) << ", ";
    cout << (0.00003 * fChannels[1][stripNb].first  + 0.0002 + fm[0] * pow(0.00042, 2) + fn[0] * 0.00042 + fk[0] - fCalibEnergies[0]) << ", ";
    cout << (0.00003 * fChannels[1][stripNb].second + 0.0002 + fm[1] * pow(0.00042, 2) + fn[1] * 0.00042 + fk[1] - fCalibEnergies[1]) << "}" << endl;
    cout << fm[0]/2/pow(mu_1, 2) * (2*mu_1*(nu_2-nu_1)*0.00003 + eta_1*(sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*0.00003)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*0.00003))))
          + fn[0]/2/mu_1 * (sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_1*0.00003)) - sqrt(eta_1*eta_1-4*mu_1*(epsE-nu_2*0.00003)))
          - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * 0.00003 << "}" << endl;
  // }
}

// Function for getting approximation of an eloss law in Si layer of an arbitary shape
void MonteCarloEdep (Int_t energyInd, Int_t sensDeadCount,  Double_t maxDeadThickness, TString fileNameMC) {
  // cout << "LOG" << endl;
  TCanvas* tmpCanvas = new TCanvas("tmpCanvas");
  TCanvas* canvasMeanEdepByStrip = new TCanvas("MeanEdepByStrip");
  tmpCanvas->cd(1);
  std::vector<TH1F*> histsMeanEdepByStrip;

  std::vector<Double_t> x(sensDeadCount + 1);
  std::vector<Double_t> y(sensDeadCount + 1);
  std::vector<Double_t> d_x(sensDeadCount + 1);
  std::vector<Double_t> d_y(sensDeadCount + 1);
  Int_t    stripNb = fUpperSubAddress_X / 2;

  TFile* inFile = new TFile(fileNameMC);
  fTree = (TTree*)inFile->Get("er");


  histsMeanEdepByStrip.push_back(new TH1F("hMeanEdepByStrip",
                                          "MeanEdepByStrip",
                                          sensDeadCount, 0, sensDeadCount));

  TString varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep";
  TString cutToChooseStrip = "";


  Double_t lengthMul = (fileNameMC.Contains("rot")) ? 1. / cos(44 * TMath::DegToRad()) : 1;
  cout << "lengthMul " << lengthMul << endl;
  for (int sensDead = sensDeadCount; sensDead > 0; sensDead--) {
    varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep";
    cutToChooseStrip = "";
    for (int d = 1; d < sensDead; d++) {
      TString tmpVarExp;
      tmpVarExp.Form(" + ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_%d_X.fEdep", d);
      varExp += tmpVarExp;
    }
    cutToChooseStrip.Form("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb==%d", stripNb);
    tmpCanvas->cd(1);
   // cutToChooseStrip += " && " + varExp + " > 0.006";
    TString histName;
    histName.Form("hTmpHistToGetStripMeanEdep_%d", sensDead);
    fTree->Draw(varExp+">>"+histName, cutToChooseStrip);
    TH1F *tmpHistToGetStripMeanEdep = (TH1F*)gDirectory->Get(histName);
    Double_t meanValue = tmpHistToGetStripMeanEdep->GetMean();
    x[sensDead] = maxDeadThickness / sensDeadCount * sensDead;
    y[sensDead] = meanValue;
    if (sensDead != sensDeadCount) {
      d_x[sensDead + 1] = maxDeadThickness * lengthMul / sensDeadCount * (sensDead + 1) - 0.00002 * lengthMul;
      d_y[sensDead + 1] = (y[sensDead + 1] - y[sensDead]) / (4e-5 * lengthMul);
    }
    histsMeanEdepByStrip.back()->SetBinContent(sensDead, meanValue);
  }
  d_x[1] = x[1] - 0.00002 * lengthMul;
  d_y[1] = y[1] / (4e-5 * lengthMul);

  // Quadratic approximation coeffs of an eloss function 
  fm[energyInd] = (d_y[sensDeadCount] - d_y[1]) / 2 / (d_x[sensDeadCount] - d_x[1]);
  fn[energyInd] = (d_y[1] * d_x[sensDeadCount] - d_y[sensDeadCount] * d_x[1]) / (d_x[sensDeadCount] - d_x[1]);
  fk[energyInd] = 0;

  static TCanvas* eLdepCanvas = new TCanvas("edepCanvas");
  static auto mg  = new TMultiGraph();
  static std::vector<TGraph*> gr_dVect;
  eLdepCanvas->cd(1);
  // TGraph* gr_d = new TGraph(sensDeadCount + 1, &d_x[0], &d_y[0]);
  gr_dVect.push_back(new TGraph(sensDeadCount + 1, &d_x[0], &d_y[0]));
  gr_dVect.back()->RemovePoint(0);
  gr_dVect.back()->SetLineColor(energyInd);
  gr_dVect.back()->SetLineWidth(2);
  gr_dVect.back()->SetMarkerColor(energyInd);
  gr_dVect.back()->SetMarkerSize(1.1);
  gr_dVect.back()->SetMarkerStyle(21);
  gr_dVect.back()->SetName(fileNameMC);
  TFitResultPtr fitRes = gr_dVect.back()->Fit("pol1","S");
  cout << "Fit params " << fitRes->Parameter(0) << ", " << fitRes->Parameter(1) / 2. << endl;
  fm[energyInd] = fitRes->Parameter(1) / 2.;
  fn[energyInd] = fitRes->Parameter(0);
  fk[energyInd] = 0;
  if (energyInd > 2) {
    cout << "Ratio maxLen " << gr_dVect[2]->GetXaxis()->GetXmax() / gr_dVect[0]->GetXaxis()->GetXmax() << endl;
  }
  // gr_dVect->back()->Draw("AC*");
  mg->Add(gr_dVect.back(),"PL");
  // cout << d_y[1] << endl;
  // cout << "Half thickness " << abs(d_y[2] - d_y[8]) / d_y[2] << endl;
  mg->Draw("A pmc plc");

}

// Main function for calibration process
void calibMethodClearCheck () {

  // gSystem->Load("/home/komyour/soft/go4-5.3.0/lib/libGo4Analysis.so");
  // gSystem->Load("/home/komyour/soft/go4egor/build/src/libUserAnalysis.so");

  Double_t maxDeadThickness = 0.0004; // maximal value of dead layer thickness from simulation
  // for (Int_t i = 0; i < 2; i++) {
  //   MonteCarloEdep(i, 10, maxDeadThickness, fMCDataFiles[i]);
  // 161.955 54.9278
  // 1.48967 1.09163
  // }
  // 162.426 55.8634 
  // 1.48109 1.08787

  fm[0] = 161.002, fm[1] = 58.6571;
  fn[0] = 1.48196, fn[1] = 1.08467;
  // 161.002 58.6571 4.6758e-310 3.21143e-322
  // N 1.47552 1.08467 1.58101e-322 7.1639/5e-322
  // d = 0.0003;

  //   161.002 58.6571 4.64714e-310 1.63042e-322
  // N 1.48196 1.08702 

  // fm[0] =  161.955; fm[1] = 54.9278;
  // fn[0] =  1.48967; fn[1] = 1.09163;
  fk[0] =  0.;      fk[1] = 0.;

  cout << "M " << fm[0] << " " << fm[1] << " " << fm[2] << " " << fm[3] << endl;
  cout << "N " << fn[0] << " " << fn[1] << " " << fn[2] << " " << fn[3] << endl;
  cout << "K " << fk[0] << " " << fk[1] << " " << fk[2] << " " << fk[3] << endl;
  for (int i = 0; i < 4; i++) {
    SearchChanels(fUpperSubAddress_X, fUpperSubAddress_Y, fExpRootFile[i], i);
  }
  // Int_t upperSubAddress_X, Int_t upperSubAddress_Y, TString rootFileName, Int_t fileInd
  cout << "Before Newton solver" << endl;
  fNumclCoefsFile.open("numclSolCoeffs.txt", ios::trunc);
  fNumclCoefsFile << setw(7) << "a" << setw(12) << "b" << setw(12) << "d1" << setw(12) << "d2"
                  << setw(14) << "| " << "N_1" << setw(8) << "N2" << setw(8) << "N1*" << setw(8) << "N2*"
                  << setw(8) << "N1-N2" << setw(8) << "N1*-N2*"
                  << setw(8) << "N1*-N1" << setw(8) << "N2*-N2" << endl; 
  for (int i = 0; i < fUpperSubAddress_X; i++) {
    // SystemSolverNewton(i, 1e-5);
    EquationSolverNewton(i, 1e-10, 10);
  }
  ofstream outFile;
  outFile.open("channels.txt", ios::trunc);
  for (int i = 0; i < fUpperSubAddress_X; i++) {
    outFile << "\t" << fChannels[0][i].first << " " << fChannels[0][i].second << " | " 
         << fChannels[1][i].first << " " << fChannels[1][i].second << endl; 
  }

  cout << 4.7844 - 1e3 * (161.002 * pow(0.0003 / cos(TMath::DegToRad()*44), 2) + 1.48196 * 0.0003 / cos(TMath::DegToRad()*44)) << endl; 
  // for (int i = 0; i < 2; i++) {
  //   cout << "File one channels " << endl;
  //   for (auto itChannels : fChannels[i]) {
  //     cout << "\t" << itChannels.first << " " << itChannels.second << endl; 
  //   }
  //   cout << endl;
  // }

}
