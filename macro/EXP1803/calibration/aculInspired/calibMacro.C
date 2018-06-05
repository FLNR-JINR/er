void calibMacro() {

  std::vector<Double_t> fCalibEnergies = {4.7844, 5.4895, 6.0024, 7.6869};
  Double_t              fDeadLayer;
  Int_t                 fLowerSubAddress_X = 0;
  Int_t                 fUpperSubAddress_X = 32;
  Int_t                 fLowerSubAddress_Y = 0;
  Int_t                 fUpperSubAddress_Y = 16;
  Int_t                 fSumChannelCount = 2048;

  TString               fTreeName = "AnalysisxTree";
  TString               fBranchName = "NeEvent.";
  TString               fLeafNameX = "SQX_L";
  TString               fLeafNameY = "SQY_L";

  Int_t                 fLowerChannel = 0;
  Int_t                 fUpperChannel = 0;

  TString               fInputRootFile = "/home/kmy/expertroot/er/macro/EXP1803/calibration/si_1000_LR_02_0001.root";

  std::vector<Double_t> fLowEnergyThresholdChannel;

  Double_t  fFitMinSigma = 6;       //pouziva se, private
  Double_t  fFitPeakThreshold = 0.5;

  TFile *inputFile = new TFile(fInputRootFile.Data());
  if ( !inputFile->IsOpen() ) {
    Error("calibMacro", "File %s was not opened and won't be processed", fInputRootFile.Data());
    return;
  }

  TTree *tree = (TTree*)inputFile->Get(fTreeName);
  if (!tree) {
    Error("calibMacro", "Tree %s was not found in file %s", fTreeName.Data(), fInputRootFile.Data());
    return;
  }
  // cout << "treee1 entries " << tree->GetEntries() << endl;
  for (Int_t subAddr = fLowerChannel; subAddr < fUpperSubAddress; subAddr++) {
    TString varInd;
    TString histParams;
    varInd.Form("[%d]", subAddr);
    histParams.Form("(%d, 0, %d)", fSumChannelCount, fSumChannelCount - 1);
    TString varExp = fLeafName + varInd + ">>tmpHistToGetEnergyThershold" + histParams; 
    tree->Draw(varExp,"","");
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
    fLowEnergyThresholdChannel.push_back(--thresholdBinNb);
    // cout << "subAddr " << subAddr << endl;
    // cout << "thresholdBin " << fLowEnergyThresholdChannel[subAddr] << endl;
    // cout << "thresholdBinContent " 
    //      << tmpHistToGetEnergyThershold->GetBinContent(fLowEnergyThresholdChannel[subAddr]) << endl;
  }
  // cout << "treee entries " << tree->GetEntries() * 32 << endl;






  TFile *out = new TFile("output.root", "RECREATE");
  TTree *outTree = new TTree(fTreeName, fTreeName + " with single multiplicity");
  TNeEvent* neEvent = new TNeEvent(); // class from Go4Egor pack
  // TNeEvent* neEvent1 = new TNeEvent(); // class from Go4Egor pack
  // UShort_t* sqx_l;
  // UShort_t* sqy_l;
  // tree->SetBranchAddress(fBranchName + fLeafName + "[32]", &sqx_l);  
  // tree->SetBranchAddress(fBranchName + fLeafNameY + "[16]", &sqy_l);  
  outTree->Branch(fBranchName, &neEvent);  
  tree->SetBranchAddress(fBranchName, &neEvent);  
  // цикл перебора событий и запись в выходной файл только тех, у которых множественность X и Y равна единице.
  Long64_t entriesCount = tree->GetEntries();
  for (Long64_t eventNb = 0; eventNb < entriesCount; eventNb++) {
    Int_t frontMultyplicity = 0;
    Int_t backMultyplicity = 0;
    tree->GetEntry(eventNb);
    for (Int_t subAddr = fLowerChannel; subAddr < fUpperSubAddress; subAddr++) {
      if (neEvent->SQX_L[subAddr] > fLowEnergyThresholdChannel[subAddr]) {
        frontMultyplicity++;
        if (frontMultyplicity > 1) {
          break;
        }
      }
      // if (neEvent->SQY_L[subAddr] > fLowEnergyThresholdChannel[subAddr]) {
      //   backMultyplicity++;
      //   if (backMultyplicity > 1) {
      //     break;
      //   }
      // }
    }
    if(frontMultyplicity == 1 /*&& backMultyplicity == 1*/)  {
      // cout << "Filled " << eventNb << endl;
      outTree->Fill();
    }
  }
  cout << "out entries " << outTree->GetEntries() << endl;

  ofstream calibFile;
  calibFile.open("calib.par");
  for (Int_t subAddr = fLowerChannel; subAddr < fUpperSubAddress; subAddr++) {
    TString varInd;
    TString histParams;
    TSpectrum sc; //by default for 100 peaks
    varInd.Form("[%d]", subAddr);
    histParams.Form("(%d, 0, %d)", fSumChannelCount, fSumChannelCount - 1);
    TString varExp = fLeafName + varInd + ">>tmpHistToGetPeaks" + histParams; 
    TString cutExp = fLeafName + varInd + ">" + TString::Itoa(fLowEnergyThresholdChannel[subAddr],10); 
    
    outTree->Draw(varExp, cutExp, "");
    TH1F *tmpHistToGetPeaks = (TH1F*)gDirectory->Get("tmpHistToGetPeaks");

    sc.Search(tmpHistToGetPeaks, fFitMinSigma, "goff", fFitPeakThreshold);
    Int_t peaksCount = sc.GetNPeaks();

    Double_t* peaksPos = sc.GetPositionX();
    vector<Double_t> peaksChannels(peaksPos, peaksPos + peaksCount);
    sort(peaksChannels.begin(), peaksChannels.end());
    // TMath::Sort(peaksCount, peaksPos, j, kFALSE);
    cout << "NPeaks " << peaksCount << endl;
    cout << "\t peakPos ";
    for (int i = 0; i < peaksCount; i++) {
      cout << peaksChannels[i] << " ";
    }
    cout << endl;
    // E = k*A + b;
    // определение позиции средних значений с помощью TSpectrum
    // поределениt калибровоных значений только по крайним пикам
    Double_t k = (fCalibEnergies.front() - fCalibEnergies.back())
               / (peaksChannels[0] - peaksChannels[peaksCount-1]);
    Double_t b = (peaksChannels[0] * fCalibEnergies.back() - peaksChannels[peaksCount-1] * fCalibEnergies.front()) 
               / (peaksChannels[0] - peaksChannels[peaksCount-1]);
    calibFile << k << "\t" << b;  
    if (peaksCount < 4) {
      calibFile << " ; failed - too few peaks ";
    }
    calibFile << endl;
  }  
}
