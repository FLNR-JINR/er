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
          
TString   fTreeName = "AnalysisxTree";
TString   fBranchName = "NeEvent.";
TString   fLeafName_X = "SQX_L";
TString   fLeafName_Y = "SQY_L";
          
Int_t     fLowerChannel = 0;
Int_t     fUpperChannel = 0;
          
vector<TString> fExpRootFile = {
                                "si_1000_LR_02_0001.root",
                                "si_1000_LR_03_0001.root"
                               };
TString   fOutRootFileDir = "out_direct.root";
TString   fOutRootFileRot = "out_rot.root";
TString   fChannelFileNames = "channelsDirect.txt";

// Each file contains data for one energy value
std::vector<TString> fMCDataFiles = {
                                     "sim_digi_calib_direct1.root",
                                     "sim_digi_calib_direct4.root"
                                     // "sim_digi_calib_rot1.root",
                                     // "sim_digi_calib_rot4.root"
                                    };


vector<Double_t> fLowEnergyThresholdChannel; // algorithm described below finds minimal bin which is identified as min value righter than zero

TTree*  fTree;
TString fTmpHistParams;


// cout << "vect " << fLowEnergyThresholdChannel[5] << endl;
Double_t  fFitMinSigma = 6;       //pouziva se, private
Double_t  fFitPeakThreshold = 0.3;

ofstream fNumclCoefsFile;

// Временная мера, потом нужно будет убрать
void InitTree (TString rootFileName) {

  // if (fTree != NULL) {
  //   cout << "fTree had already initialized " << endl;
  //   return;
  // } else {
  //   cout << "Start initializing tree" << endl;
  // }

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

void FindThresholdBin (Int_t upperSubAddress, TString rootFileName) {
  // cout << "treee1 entries " << fTree->GetEntries() << endl;
  InitTree(rootFileName);
  fTmpHistParams.Form("(%d, 0, %d)", fSumChannelCount, fSumChannelCount - 1);
  // for each strip 
  fLowEnergyThresholdChannel.resize(upperSubAddress);
  for (Int_t subAddr = 0; subAddr < upperSubAddress; subAddr++) {
    TString varInd_X;
    varInd_X.Form("[%d]", subAddr);
    TString varExp = fLeafName_X + varInd_X + ">>tmpHistToGetEnergyThershold" + fTmpHistParams; 
    fTree->Draw(varExp,"","");
    TH1F *tmpHistToGetEnergyThershold = (TH1F*)gDirectory->Get("tmpHistToGetEnergyThershold");
    Int_t binsCount = tmpHistToGetEnergyThershold->GetXaxis()->GetNbins();
    
    Bool_t minBinObtained = false;
    Int_t  thresholdBinNb = 0;
    Int_t  maxBinNb = tmpHistToGetEnergyThershold->GetMaximumBin();
    Int_t  prevBinContent = tmpHistToGetEnergyThershold->GetBinContent(maxBinNb);

    for (Int_t binNb = maxBinNb + 1; binNb < binsCount && !minBinObtained; binNb++) {
      Int_t binContent = tmpHistToGetEnergyThershold->GetBinContent(binNb);
      if (prevBinContent <= binContent) {
        minBinObtained = true;
        thresholdBinNb = binNb;
      }
      prevBinContent = binContent;
    }
    fLowEnergyThresholdChannel[subAddr] = --thresholdBinNb;
  // cout << "LOG" << endl;
    cout << " " << fLowEnergyThresholdChannel[subAddr];
    // cout << "thresholdBinContent " 
    //      << tmpHistToGetEnergyThershold->GetBinContent(fLowEnergyThresholdChannel[subAddr]) << endl;
    cout << endl;
  }
  cout << "treee entries " << fTree->GetEntries() << endl;
}

/*Подсчет количества каналов АЦП для каждого стрипа/пикселя*/
void SearchChanels (Int_t upperSubAddress_X, Int_t upperSubAddress_Y, TString rootFileName, Int_t fileInd) {
  InitTree(rootFileName);
  TString outFileName = "tmp_";
  outFileName += rootFileName;
  TFile *outFile = new TFile(outFileName.Data(), "RECREATE");
  cout << "Created temporary file '" << outFileName << " with unity multiplicity" << endl;
  TTree *outTree = new TTree(fTreeName, fTreeName + " with single multiplicity");
  TNeEvent* neEvent = new TNeEvent(); // class from Go4Egor pack
  
  outTree->Branch(fBranchName, &neEvent);  
  fTree->SetBranchAddress(fBranchName, &neEvent);  
  // cout << "SetBranchAddress status " << fTree->SetBranchAddress(fBranchName, &neEvent) << endl;
  // цикл перебора событий и запись в выходной файл только тех, у которых множественность X и Y равна единице. -- вынести в отдельную функцию
  Long64_t entriesCount = fTree->GetEntries();
  cout << "entries " << entriesCount << endl;
  // int i = 0;
  for (Long64_t eventNb = 0; eventNb < entriesCount; eventNb++) {
    // cout << "In cyc" << i++ << endl;
    Int_t frontMultyplicity = 0;
    Int_t backMultyplicity = 0;
    fTree->GetEntry(eventNb);
    for (Int_t subAddr_X = 0; subAddr_X < upperSubAddress_X; subAddr_X++) {
      if (neEvent->SQX_L[subAddr_X] > fLowEnergyThresholdChannel[subAddr_X]) {
        frontMultyplicity++;
        // cout << "front mul " << frontMultyplicity << endl;
        if (frontMultyplicity > 1) {
          break;
        }
      }
    }
    if(frontMultyplicity != 1) {
      continue;
    }
    for (Int_t subAddr_Y = 0; subAddr_Y < upperSubAddress_Y; subAddr_Y++) {
      if (neEvent->SQY_L[subAddr_Y] > 130) {
        backMultyplicity++;
        // cout << "back mul " << backMultyplicity << endl;
        if (backMultyplicity > 1) {
          break;
        }
      }
    }
    if(frontMultyplicity == 1 && backMultyplicity == 1)  {
      // cout << "Filled " << eventNb << endl;
      outTree->Fill();
    }
  }
  TString  canvasName = "";
  
  cout << "out entries " << outTree->GetEntries() << endl;
  outFile->Write();
  ofstream calibFile;
  ofstream chFile;
  chFile.open("fChannelFileName");
  calibFile.open("calib.par");
  for (Int_t subAddr_X = 0; subAddr_X < upperSubAddress_X; subAddr_X++) {
    canvasName.Form("specrumInStrip_F%d_%d", fileInd, subAddr_X);
    TCanvas* spectrumCanvas = new TCanvas(canvasName);
    spectrumCanvas->cd(1);

    TString varInd_X;
    varInd_X.Form("[%d]", subAddr_X);
    TSpectrum sc; //by default for 100 peaks
    TString varExp = fLeafName_X + varInd_X + ">>tmpHistToGetPeaks" + fTmpHistParams; 
    TString cutExpTheshold = fLeafName_X + varInd_X + ">" + TString::Itoa(fLowEnergyThresholdChannel[subAddr_X],10); 
    cout << "Strip number " << subAddr_X << endl;
    // commented lines for an overpixels analyis noted with <--------
    // for (Int_t subAddr_Y = 0; subAddr_Y < upperSubAddress_Y; subAddr_Y++) { <--------
    // TString varInd_Y; <--------
    // varInd_Y.Form("[%d]", subAddr_Y); <--------
    TString cutExp = cutExpTheshold;// + "&&" + fLeafName_Y + varInd_Y + ">130";// ">130"; <--------
    outTree->Draw(varExp, cutExp, "");
    TH1F *tmpHistToGetPeaks = (TH1F*)gDirectory->Get("tmpHistToGetPeaks");
    sc.Search(tmpHistToGetPeaks, fFitMinSigma, "", fFitPeakThreshold);
    Int_t peaksCount = sc.GetNPeaks();
    Double_t* peaksPos = sc.GetPositionX();
    // initializing of a vector by specrum finding results
    vector<Double_t> *peaksChannels = new vector<Double_t>(peaksPos, peaksPos + peaksCount);
    sort(peaksChannels->begin(), peaksChannels->end());
    cout << "NPeaks " << peaksCount << endl;
    cout << "\t peakPos ";
    for (int i = 0; i < peaksCount; i++) {
      cout << peaksChannels->at(i) << " ";
    }
    cout <<  endl;
    if (!peaksCount) {
      chFile << 100 << "\t" << 100 << endl;
      fChannels[fileInd].push_back(pair<Double_t, Double_t>(100, 100));
      continue;
    }
    chFile << peaksChannels->front() << "\t" << peaksChannels->back() << endl;
    fChannels[fileInd].push_back(pair<Double_t, Double_t>(peaksChannels->front(), peaksChannels->back()));
    spectrumCanvas->SaveAs(canvasName + ".root");
    // }    
  }
}


// Newton's method to solve linear equation system
void SystemSolverNewton (Int_t stripNb, Double_t eps) {
  Int_t    systemSize = 4;
  TArrayD  jacobian(systemSize * systemSize);
  TVectorD ans(systemSize);
  std::vector<Double_t> prevValues(systemSize);
  // for (int i = 0; i < 4; i++) {
  //   prevValues.push_back(0.001);
  // }
  prevValues[0] = -0.07084;//0.01;-0.07084  0.006732
  prevValues[1] = 0.006732;//0.01;
  prevValues[2] = 0.0003;//0.0004;
  prevValues[3] = 0.00035;//0.0004;

  Bool_t   endOfIterations = kFALSE;
  int iterCount = 0;
  while (!endOfIterations) {
    iterCount++;
    if (iterCount > 1) {
      cout << "Too many iterations " << endl;
      break;
    }
/*    jacobian[0 + 0 * systemSize] = 2 * prevValues[0];
    jacobian[1 + 0 * systemSize] = 2 * prevValues[1];
    jacobian[2 + 0 * systemSize] = 2 * prevValues[2];
    jacobian[0 + 1 * systemSize] = 4 * prevValues[0];
    jacobian[1 + 1 * systemSize] = 2 * prevValues[1];
    jacobian[2 + 1 * systemSize] = - 4;
    jacobian[0 + 2 * systemSize] = 6 * prevValues[0];
    jacobian[1 + 2 * systemSize] = - 4;
    jacobian[2 + 2 * systemSize] = 2 * prevValues[2];*/


    jacobian[0 + 0 * 4] = fChannels[0][stripNb].first;
    jacobian[0 + 1 * 4] = fChannels[0][stripNb].second;
    jacobian[0 + 2 * 4] = fChannels[1][stripNb].first;
    jacobian[0 + 3 * 4] = fChannels[1][stripNb].second;
    for (Int_t i = 0; i < 4; i++) {
      jacobian[1 + i * 4] = 1;  
    }
    jacobian[2 + 0 * 4] = 2 * fm[0] * prevValues[2] + fn[0];
    jacobian[2 + 1 * 4] = 2 * fm[1] * prevValues[2] + fn[1];
    jacobian[3 + 2 * 4] = 2 * fm[0] * prevValues[3] + fn[0];
    jacobian[3 + 3 * 4] = 2 * fm[1] * prevValues[3] + fn[1];
    ans[0] = - (prevValues[0] * fChannels[0][stripNb].first  + prevValues[1] + fm[0] * pow(prevValues[2], 2) + fn[0] * prevValues[2] + fk[0] - fCalibEnergies[0]);
    ans[1] = - (prevValues[0] * fChannels[0][stripNb].second + prevValues[1] + fm[1] * pow(prevValues[2], 2) + fn[1] * prevValues[2] + fk[1] - fCalibEnergies[1]);
    ans[2] = - (prevValues[0] * fChannels[1][stripNb].first  + prevValues[1] + fm[0] * pow(prevValues[3], 2) + fn[0] * prevValues[3] + fk[0] - fCalibEnergies[0]);
    ans[3] = - (prevValues[0] * fChannels[1][stripNb].second + prevValues[1] + fm[1] * pow(prevValues[3], 2) + fn[1] * prevValues[3] + fk[1] - fCalibEnergies[1]);
 

 /*   ans[0] = - (pow(prevValues[0], 2) + pow(prevValues[1], 2) + pow(prevValues[2], 2) - 1);
    ans[1] = - (2 * pow(prevValues[0], 2) + pow(prevValues[1], 2) - 4 * prevValues[2]);
    ans[2] = - (3 * pow(prevValues[0], 2) - 4 * prevValues[1] + pow(prevValues[2], 2));
 */   
    TMatrixD eqMatrix(systemSize, systemSize);
    eqMatrix.Use(systemSize, systemSize, jacobian.GetArray());
    for (int i = 0; i < systemSize; i++) {
        cout << ans[i] << ",";
    }
    cout << ";\t\n";
    // if (subAddr_X == 1systemSize &&  subAddr_Y == 15) {
    for (int i = 0; i < systemSize; i++) {
      for (int j = 0; j < systemSize; j++) {
        cout << eqMatrix[i][j] << " ";
      }
      cout << endl;
    }
    cout  << endl; 
    // }
    TDecompLU lu(eqMatrix);
    lu.Solve(ans);

    // Double_t *tmpAns = &ans[0]; // попробовать работать напрямую &ans[], а не через временную переменную

    endOfIterations = kTRUE;
    for (int i = 0; i < systemSize; i++) {
      cout << ans[i] << " "; 
    }
    cout << endl;
    for (int i = 0; i < systemSize; i++) {
      cout << prevValues[i] << " "; 
    }
    cout << endl;
    cout << "_______________" << endl; 
    for (Int_t i = 0; i < systemSize; i++) {
      if (abs(ans[i]) > eps) {
        endOfIterations = kFALSE;
      }
      prevValues[i] += ans[i]; // убрать этот позор, поменять на налаженную работу с указателями, как на написано на пару строк ниже
    }
    // prevValues = ans.GetElements();
  }
  cout << "Strip " << stripNb << " : " << iterCount << " iterations" << endl;
  for (int i = 0; i < systemSize; i++) {
    cout << prevValues[i] << " "; 
  }
  cout << endl;
  // cout << "D1 = " << 
}

void EquationSolverNewton (Int_t stripNb, Double_t eps, Int_t iterLimit) {
  Double_t eta  = fn[1] - fn[0];
  Double_t mu   = fm[1] - fm[0];
  Double_t nu1  = fChannels[0][stripNb].first - fChannels[0][stripNb].second;
  Double_t nu2  = fChannels[1][stripNb].first - fChannels[1][stripNb].second;
  Double_t epsE = fCalibEnergies[0] - fCalibEnergies[1];
  cout << endl;
  cout << eta << " " << mu << " " << nu1 << " " << nu2 << " " << epsE << endl;
  
  Double_t a = fmax((epsE - eta*eta/4/mu) / nu2, (epsE - eta*eta/4/mu) / nu1);
  a += 1e-7;
  cout << "Начальное приближение " << a << endl;
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
    // cout << "preF " << fm[0]/2/pow(mu, 2) * (2*mu*(nu2-nu1)*a + eta*(sqrt(eta*eta-4*mu*(epsE-nu2*a)) - sqrt(eta*eta-4*mu*(epsE-nu1*a)))) << endl;
    F = fm[0]/2/pow(mu, 2) * (2*mu*(nu2-nu1)*a + eta*(sqrt(eta*eta-4*mu*(epsE-nu2*a)) - sqrt(eta*eta-4*mu*(epsE-nu1*a))))
      + fn[0]/2/mu * (sqrt(eta*eta-4*mu*(epsE-nu1*a)) - sqrt(eta*eta-4*mu*(epsE-nu2*a)))
      - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * a;
    // cout << "F1 = " << F << "; f = " << f << endl;
    // a += 1e-5;
    // F = fm[0]/2/pow(mu, 2) * (2*mu*(nu2-nu1)*a + eta*(sqrt(eta*eta-4*mu*(epsE-nu2*a)) - sqrt(eta*eta-4*mu*(epsE-nu1*a))))
    //   + fn[0]/2/mu * (sqrt(eta*eta-4*mu*(epsE-nu1*a)) - sqrt(eta*eta-4*mu*(epsE-nu2*a)))
    //   - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * a;
    cout << "Fdf = " << F << "; f = " << f << endl;
    f = fm[0]/mu*(nu2-nu1 + eta * (nu2/sqrt(eta*eta-4*mu*(epsE-nu2*a)) - nu1/sqrt(eta*eta-4*mu*(epsE-nu1*a))))
      + fn[0] * (nu1/sqrt(eta*eta-4*mu*(epsE-nu1*a)) - nu2/sqrt(eta*eta-4*mu*(epsE-nu2*a))) 
      - (fChannels[0][stripNb].first - fChannels[1][stripNb].first);
    a -= F / f;
    cout << "a = " << a << endl;
  }
  Double_t d1 = (-eta - sqrt(pow(eta, 2) - 4*mu*(epsE - nu1*a))) / 2 / (mu);
  Double_t d2 = (-eta - sqrt(pow(eta, 2) - 4*mu*(epsE - nu2*a))) / 2 / (mu);
  Double_t b = fCalibEnergies[0] - fm[0] * pow(d1, 2) - fn[0] * d1 - a * fChannels[0][stripNb].first;

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
  if (stripNb == 0) {
    Double_t grA = 1.89e-05, grB = 0.00246391, grD1 = -0.00128995, grD2 = -0.00114981;
    Double_t numclA = 1.92371e-05, numclB = 0.0020608, numclD1 = -0.000988766, numclD2 = -0.000890227;
    cout << "Graph disparity " << "\t {";
    cout << fm[0]/2/pow(mu, 2) * (2*mu*(nu2-nu1)*grA + eta*(sqrt(eta*eta-4*mu*(epsE-nu2*grA)) - sqrt(eta*eta-4*mu*(epsE-nu1*grA))))
          + fn[0]/2/mu * (sqrt(eta*eta-4*mu*(epsE-nu1*grA)) - sqrt(eta*eta-4*mu*(epsE-nu2*grA)))
          - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * grA << "}" << endl;
    // cout << (grA * fChannels[0][stripNb].first  + grB + fm[0] * pow(grD1, 2) + fn[0] * grD1 + fk[0] - fCalibEnergies[0]) << ", ";
    // cout << (grA * fChannels[0][stripNb].second + grB + fm[1] * pow(grD1, 2) + fn[1] * grD1 + fk[1] - fCalibEnergies[1]) << ", ";
    // cout << (grA * fChannels[1][stripNb].first  + grB + fm[0] * pow(grD2, 2) + fn[0] * grD2 + fk[0] - fCalibEnergies[0]) << ", ";
    // cout << (grA * fChannels[1][stripNb].second + grB + fm[1] * pow(grD2, 2) + fn[1] * grD2 + fk[1] - fCalibEnergies[1]) << "}" << endl;

    cout << "Numerical disparity " << "\t {";
    // cout << (numclA * fChannels[0][stripNb].first  + numclB + fm[0] * pow(numclD1, 2) + fn[0] * numclD1 + fk[0] - fCalibEnergies[0]) << ", ";
    // cout << (numclA * fChannels[0][stripNb].second + numclB + fm[1] * pow(numclD1, 2) + fn[1] * numclD1 + fk[1] - fCalibEnergies[1]) << ", ";
    // cout << (numclA * fChannels[1][stripNb].first  + numclB + fm[0] * pow(numclD2, 2) + fn[0] * numclD2 + fk[0] - fCalibEnergies[0]) << ", ";
    // cout << (numclA * fChannels[1][stripNb].second + numclB + fm[1] * pow(numclD2, 2) + fn[1] * numclD2 + fk[1] - fCalibEnergies[1]) << "}" << endl;
    cout << fm[0]/2/pow(mu, 2) * (2*mu*(nu2-nu1)*numclA + eta*(sqrt(eta*eta-4*mu*(epsE-nu2*numclA)) - sqrt(eta*eta-4*mu*(epsE-nu1*numclA))))
          + fn[0]/2/mu * (sqrt(eta*eta-4*mu*(epsE-nu1*numclA)) - sqrt(eta*eta-4*mu*(epsE-nu2*numclA)))
          - (fChannels[0][stripNb].first - fChannels[1][stripNb].first) * numclA << "}" << endl;
  }
}


// Function for analysing of an eloss distribution law in Si layers
void AnalyzeMonteCarloEdep () {
  // cout << "LOG" << endl;
  TCanvas* canvasMeanEdepByStrip = new TCanvas("MeanEdepByStrip");
  TCanvas* tmpCanvas = new TCanvas("tmpCanvas");
  tmpCanvas->cd(1);
  std::vector<TH1F*> histsMeanEdepByStrip;

  Int_t    sensDeadCount = 10;
  Int_t    deadLayersLikeSensCount = 7; // 5; 6;
  Int_t    thicknessDead = 24;
  Int_t    calibrateStripCount = 32;
  Int_t    stripNb = 16;
  std::vector<Double_t> x(sensDeadCount + 1);
  std::vector<Double_t> y(sensDeadCount + 1);
  std::vector<Double_t> d_x(sensDeadCount + 1);
  std::vector<Double_t> d_y(sensDeadCount + 1);
  for (auto itFile : fMCDataFiles) {
    TFile* inFile = new TFile(itFile);
    fTree = (TTree*)inFile->Get("er");

    histsMeanEdepByStrip.push_back(new TH1F("hMeanEdepByStrip",
                                            "MeanEdepByStrip",
                                            sensDeadCount, 0, sensDeadCount));

    TString varExp = "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep";
    TString cutToChooseStrip = "";


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
      Double_t maxBin = tmpHistToGetStripMeanEdep->GetMaximumBin();
      Double_t maxBinContent = tmpHistToGetStripMeanEdep->GetBinContent(maxBin);
      // cout << "varExp " << varExp << endl;
      cout << "__" << endl; 
      cout << "varCut " << cutToChooseStrip <<  endl;
      cout << "mean for " << stripNb << " strip is " << meanValue << endl;
      cout << "sensDead " << sensDead << endl << endl;
      x[sensDead] = 0.0004 / sensDeadCount * sensDead;
      y[sensDead] = meanValue;
      if (sensDead != 10) {
        d_x[sensDead + 1] = 0.0004 / sensDeadCount * (sensDead + 1);
        d_y[sensDead + 1] = y[sensDead + 1] - y[sensDead];
      }
      // cout << "maxBin for " << stripNb << " strip is " << maxBin << endl;
      // cout << "maxBinContent for " << stripNb << " strip is " << maxBinContent << endl;
      histsMeanEdepByStrip.back()->SetBinContent(sensDead, meanValue);
    }
  }
  d_x[1] = x[1];
  d_y[1] = y[1];
  // TGraph *gr = new TGraph(sensDeadCount, x, y);
  // TGraph* gr = new TGraph(sensDeadCount + 1, &x[0], &y[0]);
  // gr->SetName("name");
  // gr->Draw("AC*");

  // TGraph* gr_d = new TGraph(sensDeadCount + 1, &d_x[0], &d_y[0]);
  // gr_d->SetName("name");
  // gr_d->Draw("AC*");

  // cout << d_y[1] << endl;
  // cout << "Half thickness " << abs(d_y[2] - d_y[8]) / d_y[2] << endl;
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
      d_x[sensDead + 1] = maxDeadThickness / sensDeadCount * (sensDead + 1) - 0.00002;
      d_y[sensDead + 1] = (y[sensDead + 1] - y[sensDead]) / 4e-5;
    }
    histsMeanEdepByStrip.back()->SetBinContent(sensDead, meanValue);
  }
  d_x[1] = x[1] - 0.00002;
  d_y[1] = y[1] / 4e-5;

  // Quadratic approximation coeffs of an eloss function 
  fm[energyInd] = (d_y[sensDeadCount] - d_y[1]) / 2 / (d_x[sensDeadCount] - d_x[1]);
  fn[energyInd] = (d_y[1] * d_x[sensDeadCount] - d_y[sensDeadCount] * d_x[1]) / (d_x[sensDeadCount] - d_x[1]);
  fk[energyInd] = 0;
  // fk[energyInd] = d_y[1] - fm[energyInd] * d_x[1] * d_x[1] - fn[energyInd] * d_x[1];

  // TGraph *gr = new TGraph(sensDeadCount, x, y);
  TGraph* gr = new TGraph(sensDeadCount + 1, &x[0], &y[0]);
  gr->RemovePoint(0);
  gr->SetName("name");
  gr->Draw("AC*");
  // cout << "d_y " << d_y[1] << " " << d_y[sensDeadCount] << endl;
  // cout << "d_x " << d_x[1] << " " << d_x[sensDeadCount] << endl;
  TGraph* gr_d = new TGraph(sensDeadCount + 1, &d_x[0], &d_y[0]);
  gr_d->RemovePoint(0);
  gr_d->SetName("name");
  gr_d->Draw("AC*");

  // cout << d_y[1] << endl;
  // cout << "Half thickness " << abs(d_y[2] - d_y[8]) / d_y[2] << endl;
}

// Main function for calibration process
void calibrationHandler () {

  // gSystem->Load("/home/komyour/soft/go4-5.3.0/lib/libGo4Analysis.so");
  // gSystem->Load("/home/komyour/soft/go4egor/build/src/libUserAnalysis.so");

  Double_t maxDeadThickness = 0.0004; // maximal value of dead layer thickness from simulation
  for (Int_t i = 0; i < 2; i++) {
    MonteCarloEdep(i, 10, maxDeadThickness, fMCDataFiles[i]);
  }
  cout << "M " << fm[0] << " " << fm[1] << endl;
  cout << "N " << fn[0] << " " << fn[1] << endl;
  cout << "K " << fk[0] << " " << fk[1] << endl;
  for (int i = 0; i < 2; i++) {
    FindThresholdBin(fUpperSubAddress_X, fExpRootFile[i]);
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
  // for (int i = 0; i < 2; i++) {
  //   cout << "File one channels " << endl;
  //   for (auto itChannels : fChannels[i]) {
  //     cout << "\t" << itChannels.first << " " << itChannels.second << endl; 
  //   }
  //   cout << endl;
  // }
  for (int i = 0; i < fUpperSubAddress_X; i++) {
    outFile << "\t" << fChannels[0][i].first << " " << fChannels[0][i].second << " | " 
         << fChannels[1][i].first << " " << fChannels[1][i].second << endl; 
  }

}

