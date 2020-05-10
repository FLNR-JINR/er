// read numerical data from a file to a vector 
template<typename T>
void readVector(std::vector<T> &vec, TString fileName) {
  ifstream file;
  file.open(fileName.Data());
  if(!file.good()) {
    cout << "Can't open the " << fileName << " file" << endl;
    exit(-1);
  }
  Int_t threshold;
  // file >> threshold;
  while (file >> threshold) {
    vec.push_back(threshold);
    cout << threshold << " ";
  }
  cout << endl;
  file.close();
}

// dump numerical data to a file 
template<typename T>
void saveVector(std::vector<T> &vec, TString fileName) {
  ofstream file;
  file.open(fileName.Data(), ios::trunc);
  for (auto itVec : vec) {
    file << itVec << " ";
    cout << itVec << " ";
  }
  cout << endl;
  cout << "Saved file " << fileName << endl;
  file.close();
}

TString getFileNameFromPath (TString const &path) {
  TString fileName(path(path.Last('/') + 1, path.Length())); // file name with extension
  // remove file extension. Extension is considered as part of the name after last "." symbol
  fileName.Remove(fileName.Last('.'), fileName.Length());    
  return fileName;
}

TString createThresholdFileName (TString expFileName, TString stationName) {
  TString fileName;
  fileName.Form("tresholds_%s_%s.txt", expFileName.Data(), stationName.Data());
  return fileName;
}

TString makeMulSelectFileName (TString fileName, vector<TString> const &stationNames) {
  TString mulFileName("mulSelect_");
  mulFileName += fileName;
  for (auto itStations : stationNames) {
    mulFileName += "_" + itStations;
  }
  mulFileName += ".root";
  return mulFileName;
}

struct AnalysisSetup {
  // Part of a future calibration manager class 
  TString fRawDataPath;
  TString fTreeName;
  TString fBrachName;
  TString fStationName;
  TString fSaveDir;
  Int_t fStripsAmount;
  Int_t fBinsAmount; 
  Int_t fStartBin; 
  TString fFileBaseName;
  TString fTherholdFileName;
  TString fMultSelectFileName;
  TString fTherholdFilePath;
  TString fMultSelectFilePath;
  mutable vector<Double_t> fThresolds;
  Int_t fThresholdOffset;

  Double_t fEpsPeak;
  Double_t fFitMinSigma;
  Double_t fFitPeakThreshold;
  Int_t fIdentifyWindWidth;

  AnalysisSetup(TString _RawDataPath, TString _TreeName, TString _BrachName,
                TString _StationName, Int_t _StripsAmount, 
                Int_t _BinsAmount, Int_t _StartBin,
                Int_t _ThresholdOffset = 0);
  void DeployFolderStructure();
  void ReadThresholds() const;
  
  void SetEpsPeak(Double_t _EpsPeak) {fEpsPeak = _EpsPeak;}
  void SetFitMinSigma(Double_t _FitMinSigma) {fFitMinSigma = _FitMinSigma;}
  void SetFitPeakThreshold(Double_t _FitPeakThreshold) {fFitPeakThreshold = _FitPeakThreshold;}
  void SetIdentifyWindWidth(Int_t _IdentifyWindWidth) {fIdentifyWindWidth = _IdentifyWindWidth;}
};

// read threshold data from a file to a vector 
void AnalysisSetup::ReadThresholds() const {
  readVector(fThresolds, fTherholdFilePath);
}

void AnalysisSetup::DeployFolderStructure () {
  TString command = "mkdir -p ";
  TString dirToCreate;
  dirToCreate.Form("./results/%s/%s/%s", fFileBaseName.Data(), 
                                       fStationName.Data(),
                                       "canvases");
  gSystem->Exec(command + dirToCreate);

  // dirToCreate.Form("./results/%s/%s/%s", fFileBaseName.Data(), 
  //                                      fStationName.Data(),
  //                                      "calibratedCanvases");
  gSystem->Exec(command + dirToCreate);

  dirToCreate.Form("./results/%s/%s/%s", fFileBaseName.Data(), 
                                       fStationName.Data(),
                                       "tmpHists");
  gSystem->Exec(command + dirToCreate);
}

AnalysisSetup::AnalysisSetup(TString _RawDataPath, TString _TreeName, TString _BrachName,
                             TString _StationName, Int_t _StripsAmount,
                             Int_t _BinsAmount, Int_t _StartBin,
                             Int_t _ThresholdOffset = 0)
: fRawDataPath(_RawDataPath),
  fTreeName(_TreeName),
  fBrachName(_BrachName),
  fStationName(_StationName),
  fStripsAmount(_StripsAmount),
  fBinsAmount(_BinsAmount),
  fStartBin(_StartBin),
  fThresholdOffset(_ThresholdOffset)
{
  fFileBaseName = getFileNameFromPath(_RawDataPath);
  fTherholdFileName = createThresholdFileName(fFileBaseName, _StationName);
  fMultSelectFileName = makeMulSelectFileName(fFileBaseName, {_StationName});
  DeployFolderStructure();
  fTherholdFilePath.Form("./results/%s/%s/%s", fFileBaseName.Data(), 
                                            fStationName.Data(), 
                                            fTherholdFileName.Data());
  fMultSelectFilePath.Form("./results/%s/%s/%s", fFileBaseName.Data(), 
                                              fStationName.Data(), 
                                              fMultSelectFileName.Data());
}

void PreviewRawData(AnalysisSetup *setup, Int_t binsAmount = 2048) {
  cout << "[PreviewRawData] Begin preview buillder" << endl;
  gStyle->SetOptStat(1001101);  
  TFile *inputFile = new TFile(setup->fRawDataPath);  
  auto tree = (TTree*)inputFile->Get(setup->fTreeName);

  UShort_t stationData[setup->fStripsAmount];
  TString leafName;
  leafName.Form("%s[%d]", setup->fStationName.Data(), setup->fStripsAmount);
  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup->fBrachName + "." + leafName, stationData);

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("%s_%d", setup->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, histName, binsAmount, 0, binsAmount));
  }
  cout << tree->GetEntries() << endl;
  for (Long64_t eventNb = 0; eventNb < tree->GetEntries(); eventNb++) {
    tree->GetEntry(eventNb);
    if (!(eventNb % 100000)) {
      cout << "events were handled: " << eventNb << endl;
    }
    for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {   
      hists[iStrip]->AddBinContent(stationData[iStrip]);
    }
  }

  TString saveFileName;
  auto canvas = new TCanvas();
  int canvFirtsDim = round(sqrt(setup->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
  }
  saveFileName.Form("./results/%s/%s/canvases/preview_%s_%s.root", 
                     setup->fFileBaseName.Data(), 
                     setup->fStationName.Data(),
                     setup->fFileBaseName.Data(),
                     setup->fStationName.Data());
  canvas->SaveAs(saveFileName);
  // exit(1);
}

void FindThresholds(AnalysisSetup* setup) {
  TFile *file = new TFile(setup->fRawDataPath);
  auto tree = (TTree*)file->Get(setup->fTreeName.Data());
  // if (tree == nullptr) {

  // }
  cout << tree->GetEntries() << endl;  
  vector<Double_t> thersholdArray;
  thersholdArray.resize(setup->fStripsAmount);
  TString histParams;
  histParams.Form("(%d,%d,%d)", setup->fBinsAmount, setup->fStartBin, setup->fBinsAmount);
  for (Int_t subAddr = 0; subAddr < setup->fStripsAmount; subAddr++) {
    TString varInd;
    varInd.Form("[%d]", subAddr);
    TString varExp = setup->fBrachName + "." + setup->fStationName + varInd 
                   + ">>histFindNoiseThreshold" + histParams; 
    tree->Draw(varExp,"","");
    TH1F *histFindNoiseThreshold = (TH1F*)gDirectory->Get("histFindNoiseThreshold");
    Int_t binsAmount = histFindNoiseThreshold->GetXaxis()->GetNbins();
    
    Bool_t minBinObtained = false;
    Int_t  thresholdBinNb = 0;
    Int_t  maxBinNb = histFindNoiseThreshold->GetMaximumBin();
    Int_t  prevBinContent = histFindNoiseThreshold->GetBinContent(maxBinNb);

    for (Int_t binNb = maxBinNb + 1; binNb < binsAmount && !minBinObtained; binNb++) {
      Int_t binContent = histFindNoiseThreshold->GetBinContent(binNb);
      if (prevBinContent <= binContent) {
        minBinObtained = true;
        thresholdBinNb = binNb;
      }
      prevBinContent = binContent;
    }
    thersholdArray[subAddr] = --thresholdBinNb;// + 5;
  }
  saveVector(thersholdArray, setup->fTherholdFilePath);
}

int CheckMultiplicityOfEvent(AnalysisSetup *setup, UShort_t *data) {
  int mult = 0;
  for (size_t i = 0; i < setup->fStripsAmount; i++) {
    if (data[i] > setup->fThresolds[i]) {
      mult++;
      if (mult > 1) {
        break;
      }
    }
  }
  return mult;
}

int CheckMultiplicityOfEvent(AnalysisSetup const *setup, UShort_t const *data, 
		             vector<AnalysisSetup*> setups, 
			     vector<vector<UShort_t>> const *dataVec) 
{
  int mult = 0;
  for (size_t i = 0; i < setup->fStripsAmount; i++) {
    if (data[i] > setup->fThresolds[i]) {
      mult++;
      if (mult > 1) {
        break;
      }
    }
  }
  int ind = 0;
  for (auto const &itSetup: setups) {
    mult = 0;
    for (size_t i = 0; i < itSetup->fStripsAmount; i++) {
      if (dataVec->at(ind).at(i) > itSetup->fThresolds[i]) {
        mult++;
        if (mult > 1) {
          break;
        }
      }
    }
    if (mult > 1) {
      break;
    }
    ind++;
  }
  return mult;
}

// Transformation of the input raw data to the output file with several leaf 
// for the objective silicon station with multiplicity equals one
// in each station
// All stations are from one file.
void MultiplicitySelection(AnalysisSetup* setup, vector<AnalysisSetup*> setups) {
  setup->ReadThresholds();

  TFile *inputFile = new TFile(setup->fRawDataPath);
  auto tree = (TTree*)inputFile->Get(setup->fTreeName);
  // create output file
  TFile *outFile = new TFile(setup->fMultSelectFilePath.Data(), "RECREATE");
  cout << "[MultiplicitySelection] File " << setup->fMultSelectFilePath << " is created" << endl;
  TTree *outTree = new TTree(setup->fTreeName, setup->fTreeName + " with single multiplicity");

  // variables for the work with trees
  UShort_t stationData[setup->fStripsAmount];
  TString outDataName;
  outDataName.Form("%s[%d]", setup->fStationName.Data(), setup->fStripsAmount);

  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup->fBrachName + "." + outDataName, stationData);

  // assign data structure to the output tree
  outTree->Branch(setup->fStationName, &stationData, outDataName + "/s");
  vector<vector<UShort_t>> *saveBrData = new vector<vector<UShort_t>>();
  Int_t ind = 0;
  for (auto const &itSetup: setups) {
    itSetup->ReadThresholds();
    saveBrData->push_back(vector<UShort_t>(itSetup->fStripsAmount));
    TString strIn = itSetup->fBrachName + "." + itSetup->fStationName 
		  + "[" + TString::Itoa(itSetup->fStripsAmount, 10) + "]"; 
    TString strOut = itSetup->fStationName 
		   + "[" + TString::Itoa(itSetup->fStripsAmount, 10) + "]"; 
    cout << strIn << " " << strOut << endl;
    tree->SetBranchAddress(strIn, &(saveBrData->at(ind)[0]));
    outTree->Branch(itSetup->fStationName, &(saveBrData->at(ind)[0]), strOut + "/s");
    ind++;
  }

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("%s_%s_%d", setup->fMultSelectFileName.Data(), setup->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, histName, setup->fBinsAmount, 0, setup->fBinsAmount));
  }

  Long64_t entriesAmount = tree->GetEntries();
  cout << "[MultiplicitySelection] Input tree entries amount: " << entriesAmount << endl;
  for (Long64_t eventNb = 0; eventNb < entriesAmount; eventNb++) {
    tree->GetEntry(eventNb);
    bool flagWriteEvent = true;
    int mult = CheckMultiplicityOfEvent(setup, stationData, setups, saveBrData);
    if (mult != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (flagWriteEvent) {
      for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
        hists[iStrip]->Fill(stationData[iStrip]);
      }
      outTree->Fill();
    }
  }
  cout << "[MultiplicitySelection] Output tree entries amount: " << outTree->GetEntries() << endl;
  outFile->Write();

  TString saveFileName;
  auto canvas = new TCanvas("second_spectraPreview_selected");
  int canvFirtsDim = round(sqrt(setup->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
  }
  saveFileName.Form("./results/%s/%s/canvases/multSelected_%s_%s.root", 
                     setup->fFileBaseName.Data(), 
                     setup->fStationName.Data(),
                     setup->fFileBaseName.Data(),
                     setup->fStationName.Data());
  canvas->SaveAs(saveFileName);

  cout << "[MultiplicitySelection] Output tree entries amount: " << outTree->GetEntries() << endl;


}

// Transformation of the input raw data to the output file with one leaf 
//
// for the objective silicon station with multiplicity equals one
void MultiplicitySelection(AnalysisSetup *setup) {
  setup->ReadThresholds();

  TFile *inputFile = new TFile(setup->fRawDataPath);
  auto tree = (TTree*)inputFile->Get(setup->fTreeName);
  // create output file
  TFile *outFile = new TFile(setup->fMultSelectFilePath.Data(), "RECREATE");
  cout << "[MultiplicitySelection] File " << setup->fMultSelectFilePath << " is created" << endl;
  TTree *outTree = new TTree(setup->fTreeName, setup->fTreeName + " with single multiplicity");

  // variables for the work with trees
  UShort_t stationData[setup->fStripsAmount];
  TString outDataName;
  outDataName.Form("%s[%d]", setup->fStationName.Data(), setup->fStripsAmount);

  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup->fBrachName + "." + outDataName, stationData);

  // assign data structure to the output tree
  outTree->Branch(setup->fStationName, &stationData, outDataName + "/s");

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("%s_%s_%d", setup->fMultSelectFileName.Data(), setup->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, histName, setup->fBinsAmount, 0, setup->fBinsAmount));
  }

  Long64_t entriesAmount = tree->GetEntries();
  cout << "[MultiplicitySelection] Input tree entries amount: " << entriesAmount << endl;
  for (Long64_t eventNb = 0; eventNb <  entriesAmount; eventNb++) {
    tree->GetEntry(eventNb);
    bool flagWriteEvent = true;
    int mult = CheckMultiplicityOfEvent(setup, stationData);
    if (mult != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (flagWriteEvent) {
      for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
        hists[iStrip]->Fill(stationData[iStrip]);
      }
      outTree->Fill();
    }
  }
  cout << "[MultiplicitySelection] Output tree entries amount: " << outTree->GetEntries() << endl;

  TString saveFileName;
  auto canvas = new TCanvas("second_spectraPreview_selected");
  int canvFirtsDim = round(sqrt(setup->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
    hists[iStrip]->Write();
  }
  saveFileName.Form("./results/%s/%s/canvases/multSelected_%s_%s.root", 
                     setup->fFileBaseName.Data(), 
                     setup->fStationName.Data(),
                     setup->fFileBaseName.Data(),
                     setup->fStationName.Data());
  canvas->SaveAs(saveFileName);

  cout << "[MultiplicitySelection] Output tree entries amount: " << outTree->GetEntries() << endl;
  outFile->Write();
}

// returns amplitudes are sorted in ascending order 
vector<Double_t> GetPeaksFromSpectrum(AnalysisSetup const *setup, 
                                      TH1F const *hist) 
{
  TSpectrum sc;
  // hist->SaveAs(TString(hist->GetName()) + ".root");
  sc.Search(hist, setup->fFitMinSigma, "", setup->fFitPeakThreshold);
  Int_t peaksCnt = sc.GetNPeaks();

  cout << "[GetPeaksFromSpectrum] Info: occured peaks amount is " 
       << peaksCnt << endl;
  Double_t* peaksPos = sc.GetPositionX();
  vector<Double_t> peaksChannels(peaksPos, peaksPos + peaksCnt);
  std::sort(peaksChannels.begin(), peaksChannels.end());
  return peaksChannels;    
}

void CreateTmpHistInDir(AnalysisSetup *setup, Int_t stripNb) {
  TFile *inputFile = new TFile(setup->fMultSelectFilePath);  
  auto tree = (TTree*)inputFile->Get(setup->fTreeName);

  TString varInd;
  TString histName;
  histName.Form("tmpHist_%s_%d", setup->fFileBaseName.Data(), stripNb);
  varInd.Form("[%d]", stripNb);
  TString histParams;
  histParams.Form("(%d,%d,%d)", setup->fBinsAmount, 0, setup->fBinsAmount);  
  TString varExp = setup->fStationName + varInd + ">>" + histName + histParams; 
  TString cutExp = setup->fStationName + varInd + ">" 
                 + TString::Itoa(setup->fThresolds[stripNb] + setup->fThresholdOffset,10); 
  cout << varExp << " " << cutExp << endl;
  auto tmpHist = new TH1F(histName, histName, setup->fBinsAmount, 0, setup->fBinsAmount);
  tree->Draw(varExp, "", "");
  tree->Draw(varExp, cutExp, "");
  return;
}

// maps will be members of the EROneSpectrumCalibration
// map<TString, vector<Double_t>> fPeaks = {{"low", vector<Double_t>()}, 
//                                          {"middle", vector<Double_t>()}, 
//                                          {"high", vector<Double_t>()}};
// map<TString, vector<Double_t>> fPeaksRMS = {{"low", vector<Double_t>()}, 
//                                             {"middle", vector<Double_t>()}, 
//                                             {"high", vector<Double_t>()}};
vector<vector<Double_t>> fPeaks = {vector<Double_t>(), vector<Double_t>(), vector<Double_t>()};
vector<vector<Double_t>> fPeaksRMS = {vector<Double_t>(), vector<Double_t>(), vector<Double_t>()};
void SlidingWindowPeakSearch(AnalysisSetup *setup, Int_t stripNb) {
  CreateTmpHistInDir(setup, stripNb);
  TString  histName;
  histName.Form("tmpHist_%s_%d", setup->fFileBaseName.Data(), stripNb);
  TH1F* histSpectrum = (TH1F*)gDirectory->Get(histName);
  vector<Double_t> peaksReq;
  vector<Double_t> peaks = GetPeaksFromSpectrum(setup, histSpectrum);
  if (peaks.size() == 4) {
    // Only three peaks os four are required for the calibration algorithm
    peaksReq.push_back(peaks[0]);
    peaksReq.push_back(peaks[2]);
    peaksReq.push_back(peaks[3]);
  } else {
    peaksReq.push_back(100);
    peaksReq.push_back(200);
    peaksReq.push_back(300);
  }
  //exit(0);
  for (Int_t peakNb = 0; peakNb < peaksReq.size(); peakNb++) {
    cout << histSpectrum->GetName() << "name " << endl;
    histSpectrum->GetXaxis()->SetRange((int)(peaksReq[peakNb] - setup->fEpsPeak), 
                                       (int)(peaksReq[peakNb] + setup->fEpsPeak));
    int maxBin = histSpectrum->GetMaximumBin();
    // cout << "Max bin " << maxBin << endl;
    // cout << (int)(peaksReq[peakNb]) << endl;
    gStyle->SetStatFormat("6.8g");
    unsigned maxIntegral = numeric_limits<unsigned>::min();
    Double_t peakMean;
    Double_t peakRMS;
    unsigned peakWindLeftBound;
    for (int i = maxBin - setup->fEpsPeak; i < maxBin; i++) {
      unsigned integral;
      histSpectrum->GetXaxis()->SetRange(i, i+setup->fIdentifyWindWidth - 1);
      integral = histSpectrum->Integral();
      if (maxIntegral < integral) {
        maxIntegral = integral;
        peakWindLeftBound = i;
        peakMean = histSpectrum->GetMean();
        peakRMS  = histSpectrum->GetStdDev();
        // cout.precision(7);
        // cout << "Integral " << maxIntegral << "; Mean " << peakMean << endl;  
      }
      // cout << "Integral " << maxIntegral << endl;  
    }
    // cout << endl;
    histSpectrum->GetXaxis()->SetRange(peakWindLeftBound, 
                                       peakWindLeftBound+setup->fIdentifyWindWidth - 1);
    // cout << "peakMean " << peakMean << " peakRMS " << peakRMS << endl;
    fPeaks[peakNb].push_back(peakMean);
    fPeaksRMS[peakNb].push_back(peakRMS);
    // cout << fPeaks[peakNb].back() << " " << fPeaksRMS[peakNb].back() << endl;
  }
  histSpectrum->GetXaxis()->SetRange(0, setup->fBinsAmount);
  TString saveDir;
  saveDir.Form("./results/%s/%s/tmpHists/", setup->fFileBaseName.Data(), 
                                           setup->fStationName.Data());
  histSpectrum->SaveAs(saveDir + TString(histSpectrum->GetName()) + ".root");
}

// d(eta) qudratic approximation coefficients
// in format {p0, p1, p2} where p0, p1, p2 --- coefficients: eta = p0 + p1*d + p2*d^2 
// p0, p1, p2

// -5254.06, 18945.8, -17045.7
// -5254.0568, 18945.7708, -17045.7202
// 0.580352, -0.00113146, -5.09851e-05
Double_t GetDeadByQuadThicknessVsEtaFit (Double_t eta) {
  // return (-5254.0568 + 18945.7708 * eta - 17045.7202 * eta * eta); // 
  return (-5272.9763 + 19011.5041 * eta - 17102.8129 * eta * eta); // quad fit for elosses caliculated with 1e-5um step
}

// Spectrum characteristics like dead layer estimation, 
// ADC-channels peak values, etc which intended  to get 
// calibration coefficients on the next step
void SaveSpectrumCharacteristics (AnalysisSetup *setup) {
  TString estimateDeadHistName;
  TString etaHistName;

  TString channelsHist_low_Name;
  TString channelsHist_middle_Name;
  TString channelsHist_high_Name;

  TH2D* estimateDeadHist;
  TH2D* etaHist;

  TH2D* channelsHist_low;
  TH2D* channelsHist_middle;
  TH2D* channelsHist_high;

  TString rmsHist_lowName;
  TString rmsHist_middleName;
  TString rmsHist_highName;

  TH2D* rmsHist_low;
  TH2D* rmsHist_middle;
  TH2D* rmsHist_high;
 
  TString rmsDivDeltaPeaks_Name;
  TH2D* rmsDivDeltaPeaks;

  TString expFileName = setup->fFileBaseName;
  TString stName = setup->fStationName;
  estimateDeadHistName.Form("estimateDeadHist_%s_%s", expFileName.Data(), stName.Data());
  etaHistName.Form("etaHist_%s_%s", expFileName.Data(), stName.Data());

  channelsHist_low_Name.Form("channelsHist_low_%s_%s", expFileName.Data(), stName.Data());
  channelsHist_middle_Name.Form("channelsHist_middle_%s_%s", expFileName.Data(), stName.Data());
  channelsHist_high_Name.Form("channelsHist_high_%s_%s", expFileName.Data(), stName.Data());

  rmsHist_lowName.Form("rmsHist_low_%s_%s", expFileName.Data(), stName.Data());
  rmsHist_middleName.Form("rmsHist_middle%s_%s", expFileName.Data(), stName.Data());
  rmsHist_highName.Form("rmsHist_high%s_%s", expFileName.Data(), stName.Data());

  rmsDivDeltaPeaks_Name.Form("rmsDivDeltaPeaks%s_%s", expFileName.Data(), stName.Data());

  estimateDeadHist = new TH2D (estimateDeadHistName, estimateDeadHistName, 1, 0, 1,
                               setup->fStripsAmount, 0, setup->fStripsAmount);
  etaHist = new TH2D (etaHistName, etaHistName, 1, 0, 1, 
                      setup->fStripsAmount, 0, setup->fStripsAmount);
  channelsHist_low = new TH2D (channelsHist_low_Name, channelsHist_low_Name, 1, 0, 1,
                               setup->fStripsAmount, 0, setup->fStripsAmount);
  channelsHist_middle = new TH2D (channelsHist_middle_Name, channelsHist_middle_Name, 1, 0, 1,
                                  setup->fStripsAmount, 0, setup->fStripsAmount);
  channelsHist_high = new TH2D (channelsHist_high_Name, channelsHist_high_Name, 1, 0, 1,
                                setup->fStripsAmount, 0, setup->fStripsAmount);
  rmsHist_low = new TH2D (rmsHist_lowName, rmsHist_lowName, 1, 0, 1,
                          setup->fStripsAmount, 0, setup->fStripsAmount);
  rmsHist_middle = new TH2D (rmsHist_middleName, rmsHist_middleName, 1, 0, 1,
                             setup->fStripsAmount, 0, setup->fStripsAmount);
  rmsHist_high = new TH2D (rmsHist_highName, rmsHist_highName, 1, 0, 1,
                           setup->fStripsAmount, 0, setup->fStripsAmount);
  rmsDivDeltaPeaks = new TH2D (rmsDivDeltaPeaks_Name, rmsDivDeltaPeaks_Name, 1, 0, 1,
                               setup->fStripsAmount, 0, setup->fStripsAmount);    
  // }
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {
    Double_t eta;
    Double_t nu;
    // cout << fPeaks[2][stripNb] << " " << fPeaks[1][stripNb] << " " << fPeaks[0][stripNb] << endl;
    Double_t N3 = fPeaks[2][stripNb];
    Double_t N2 = fPeaks[1][stripNb];
    Double_t N1 = fPeaks[0][stripNb];
    // eta = (fPeaks[2][stripNb] - fPeaks[1][stripNb])
    //     / (fPeaks[2][stripNb] - fPeaks[0][stripNb]);
    eta = (N3 - N2)
        / (N3 - N1);
    nu = fPeaks[2][stripNb] - fPeaks[0][stripNb];
    // cout << eta << endl;
    // dead layer estimation by "eta" parameter approximation
    // Double_t estimDead = (eta - 0.580490) / (-0.00144091); // standart fit for thickness 2-3 um dead layer * old
    // Double_t estimDead = (eta - 0.580494) / (-0.00137519); // G4EMCalculator::Compute approximation
    // Double_t estimDead = (eta - 0.580475) / (-0.00140309); // Geant4 eloss direct propagation 
    // Double_t estimDead = (eta - 0.580467) / (-0.00139804); // Geant4 eloss with angular distribution 
    // Double_t estimDead = (eta - 0.580607) / (-0.00143713);    // G4EMCalculator::Compute approximation with regular 6 approx points: 1 to 6 *default
    // Double_t estimDead = (eta - 0.580474) / (-0.00140655); // Geant4 edep direct propagation 
    //Double_t estimDead = (eta - 0.580515) / (-0.00141868); // Geant4 edep with angular distribution 
    // Double_t estimDead = (eta - 0.580610) / (-0.00143751); // G4EmCalculator 1-6 um 6 points with integration step equals 7nm  0.580609655938, -0.00143750879398
    
    //Double_t estimDead = (eta - 0.580607) / (-0.00143726); // G4EmCalculator 1-6 um 6 points with integration step equals 1e-5 um  0.580609655938, -0.00143750879398
    Double_t estimDead = GetDeadByQuadThicknessVsEtaFit(eta); // G4EmCalculator 
    Int_t firstBin = 1;
    Int_t secondBin = stripNb + 1;      
    estimateDeadHist->SetBinContent(firstBin, secondBin, estimDead);
    etaHist->SetBinContent(firstBin, secondBin, eta);
    channelsHist_low->SetBinContent(firstBin, secondBin, fPeaks[0][stripNb]);
    channelsHist_middle->SetBinContent(firstBin, secondBin, fPeaks[1][stripNb]);
    channelsHist_high->SetBinContent(firstBin, secondBin, fPeaks[2][stripNb]);
    
    rmsHist_low->SetBinContent(firstBin, secondBin, fPeaksRMS[0][stripNb]);
    rmsHist_middle->SetBinContent(firstBin, secondBin, fPeaksRMS[1][stripNb]);
    rmsHist_high->SetBinContent(firstBin, secondBin, fPeaksRMS[2][stripNb]);

    double rmsDivDeltaPeaksVal = (int)(1e5*fPeaksRMS[2][stripNb] 
                                   / (fPeaks[2][stripNb] - fPeaks[0][stripNb]));
    rmsDivDeltaPeaks->SetBinContent(firstBin, secondBin, rmsDivDeltaPeaksVal / 1e5);
  }
  TString saveDir;
  saveDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), setup->fStationName.Data());
  // save as colorful canvases instead of histos saving
  estimateDeadHist->SaveAs(saveDir + estimateDeadHist->GetName() + ".root");
  etaHist->SaveAs(saveDir + etaHist->GetName() + ".root");
  channelsHist_low->SaveAs(saveDir + channelsHist_low->GetName() + ".root");
  channelsHist_middle->SaveAs(saveDir + channelsHist_middle->GetName() + ".root");
  channelsHist_high->SaveAs(saveDir + channelsHist_high->GetName() + ".root");
  rmsHist_low->SaveAs(saveDir + rmsHist_low->GetName() + ".root");
  rmsHist_middle->SaveAs(saveDir + rmsHist_middle->GetName() + ".root");
  rmsHist_high->SaveAs(saveDir + rmsHist_high->GetName() + ".root");
  rmsDivDeltaPeaks->SaveAs(saveDir + rmsDivDeltaPeaks->GetName() + ".root");
}

vector<Double_t> fEnergiesStart = {4.7844, 6.0024, 7.6869}; 
vector<Double_t> fEnergies;
// dE(E, d) qudratic approximation coefficients
// in format {p0, p1, p2} where p0, p1, p2 --- coefficients: dE = p0 + p1*d + p2*d^2 
// vector<vector<Double_t>> fApproxEcoeffs = { // legacy coefficients
//                                           {0.0113729, 0.141487, 0.00241099}, 
//                                           {0.00436517, 0.125545, 0.00131201},
//                                           {0.00178522, 0.107627, 0.000672926}
//                                          };
//vector<vector<Double_t>> fApproxEcoeffs = {
//                                          {0.146929, 0.00263134, 0.000125911}, // low energy approximation throught 6 regular points in 1 to 6 via G4EMCalculator::Compute 
//                                          {0.127388, 0.00171056, 5.16786e-05}, // middle energy approximation throught 6 regular points in 1 to 6 via G4EMCalculator::Compute
//                                          {0.108156, 0.00102346, 1.94286e-05}  // high energy approximation throught 6 regular points in 1 to 6 via G4EMCalculator::Compute
//                                         };

// Digi angle distrubuted coefficients
//vector<vector<Double_t>> fApproxEcoeffs = {{0.266604, 0.0427357, 0.00871288},
//					 {0.0192676, 0.119439, 0.0017361},
//					 {0.00551703, 0.105927, 0.000800737}};
// Corrected EMCalculator values
vector<vector<Double_t>> fApproxEcoeffs = {{0.0010319, 0.146954, 0.00181655},
					                                {0.0004273, 0.127711, 0.00106127},
				                                  {0.0001624, 0.108467, 0.000589357}};

void CalcEnergiesAfterDeadLayer (Double_t deadThickness =  2.56) {
  fEnergies.clear();
  int i = 0;
  for (auto itCoeffs : fApproxEcoeffs) {
    Double_t energy;
    energy = fEnergiesStart[i] 
           - itCoeffs[2] * pow(deadThickness, 2) 
           - itCoeffs[1] * deadThickness 
           - itCoeffs[0];
    fEnergies.push_back(energy);
    i++;
  }
}

void CalcCalibCoeffs(AnalysisSetup *setup) {
  TString fileBase = setup->fFileBaseName;
  TString channelName = setup->fStationName;
  TString deadFileName;
  TString meanFileName_low, meanFileName_middle, meanFileName_high;
  TString readDir;
  readDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), setup->fStationName.Data());
  meanFileName_low.Form("channelsHist_low_%s_%s.root", fileBase.Data(), channelName.Data());
  meanFileName_middle.Form("channelsHist_middle_%s_%s.root", fileBase.Data(), channelName.Data());
  meanFileName_high.Form("channelsHist_high_%s_%s.root", fileBase.Data(), channelName.Data());
  deadFileName.Form("estimateDeadHist_%s_%s.root", fileBase.Data(), channelName.Data());
  deadFileName = readDir + deadFileName;
  vector<TString> meanFiles = {readDir + meanFileName_low, 
                               readDir + meanFileName_middle, 
                               readDir + meanFileName_high};
  ofstream outFile, outFile_rev;
  ofstream calibPeaksFile;
  TString calibFileName, calibFileName_rev;
  TString calibPeaksName;

  calibFileName.Form("calibCoeff_%s_%s.txt", fileBase.Data(),  channelName.Data());
  calibFileName_rev.Form("calibCoeff_%s_%s_rev.txt", fileBase.Data(),  channelName.Data());
  calibPeaksName.Form("calibPeaks_%s_%s.txt", fileBase.Data(),  channelName.Data());

  auto file = TFile::Open(deadFileName.Data());
  auto histDead = (TH2D*)file->Get(file->GetListOfKeys()->At(0)->GetName());
  Double_t sumDeadThickness = 0;
  int normalStripsAmount = 0;
  for (int stripInd = 1; stripInd <= setup->fStripsAmount; stripInd++) {
    if (!isnan(histDead->GetBinContent(1, stripInd))) {
      sumDeadThickness += histDead->GetBinContent(1, stripInd); 
      normalStripsAmount++;
    }     
  }
  vector<TH2D*> meanValues;
  for (auto itFiles : meanFiles) {
    auto fileMean = TFile::Open(itFiles);
    meanValues.push_back((TH2D*)fileMean->Get(fileMean->GetListOfKeys()->At(0)->GetName()));
  }
  TString saveDir;
  saveDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), 
                                   setup->fStationName.Data());
  outFile.open(saveDir + calibFileName, ios::trunc);
  outFile_rev.open(saveDir + calibFileName_rev, ios::trunc);
  calibPeaksFile.open(saveDir + calibPeaksName, ios::trunc);

  vector<Double_t> coeffA;
  vector<Double_t> coeffB;
  vector<vector<Double_t>> calibratedPeaks;
  for (int stripInd = 0; stripInd < setup->fStripsAmount; stripInd++) {
    // cout << "\n stripInd " << stripInd << endl;
    auto canv = new TCanvas();
    vector<double> channels;
    vector<double> energies;
    // CalcEnergiesAfterDeadLayer(histDead->GetBinContent(1, stripInd + 1));
    CalcEnergiesAfterDeadLayer(sumDeadThickness / normalStripsAmount);
    for (auto itMeans : meanValues) {
      channels.push_back(itMeans->GetBinContent(1, stripInd + 1));
    }
    auto gr = new TGraph(fEnergies.size(), &channels[0], &fEnergies[0]);
    calibratedPeaks.push_back(fEnergies);
    
    TString grTitle;
    grTitle.Form("Linear calibration, %d strip", stripInd);
    gr->SetTitle(grTitle);
    gr->SetLineWidth(2);
    gr->SetMarkerSize(1.1);
    gr->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle("N, [ADC channels]");
    gr->GetYaxis()->SetTitle("E, [MeV]");
    TFitResultPtr fitRes = gr->Fit("pol1","S");
    // cout << "Fit params " << fitRes->Parameter(0) << ", " << fitRes->Parameter(1) / 2. << endl;
    Double_t a = fitRes->Parameter(1);
    Double_t b = fitRes->Parameter(0);
    if (int(fitRes)) {
      a = 1;
      b = 0;      
    }
    coeffA.push_back(a);
    coeffB.push_back(b);
    outFile << a << " " << b << endl;      
    outFile_rev << b << " " << a << endl;      
    cout << b << " " << a << " " << - b / a << endl;      
    for (auto itChannels : channels) {
      calibPeaksFile << a * itChannels + b << " ";
    }
    calibPeaksFile << endl;
  }
  cout << "Strips amount " << normalStripsAmount << endl;
  cout << "Average dead layer thickness " << sumDeadThickness / normalStripsAmount << endl;
  cout << "Calibration file is saved in " << saveDir + calibFileName << endl;

  // Form report
  TString reportFileName;
  reportFileName.Form("%sreport_%s_%s.txt", readDir.Data(),
                                             setup->fFileBaseName.Data(), 
                                             setup->fStationName.Data());
  cout << "reportFileName " << reportFileName << endl;
  ofstream reportFile;
  reportFile.open(reportFileName);
  reportFile << "Calibration results. " << endl << endl;
  reportFile << "Sliding window algorithm parameters: " <<  endl;
  reportFile << "  window width: " << setup->fIdentifyWindWidth <<  endl;
  reportFile << "  search region around peak: +-" << setup->fEpsPeak <<  endl;
  reportFile << "  peak RMS: " << setup->fFitMinSigma <<  endl;
  reportFile << "  peak amplitude threshold (with respect to maximal): " 
             << setup->fFitPeakThreshold <<  endl;

  reportFile << "ADC-channels counts are stored in files: " << endl;
  reportFile << " * " << meanFileName_low << " --- for low energy peak;" << endl;

  auto fileMean_low = TFile::Open(readDir + meanFileName_low);
  auto mean_low = (TH2D*)fileMean_low->Get(fileMean_low->GetListOfKeys()->At(0)->GetName());
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {
    reportFile << "\t" << "Strip " << stripNb << ": " << mean_low->GetBinContent(1, stripNb + 1) << endl;
    cout << calibratedPeaks[stripNb][0] 
	    - (mean_low->GetBinContent(1, stripNb + 1) * coeffA[stripNb] + coeffB[stripNb]) << endl;
  }
  cout << endl;
  reportFile << " * " << meanFileName_middle << " --- for middle energy peak;" << endl;
  auto fileMean_middle = TFile::Open(readDir + meanFileName_middle);
  auto mean_middle = (TH2D*)fileMean_middle->Get(fileMean_middle->GetListOfKeys()->At(0)->GetName());
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {
    reportFile << "\t" << "Strip " << stripNb << ": " << mean_middle->GetBinContent(1, stripNb + 1) << endl;
    cout << calibratedPeaks[stripNb][1] 
	    - (mean_middle->GetBinContent(1, stripNb + 1) * coeffA[stripNb] + coeffB[stripNb]) << endl;
  }
  cout << endl;

  reportFile << " * " << meanFileName_high << " --- for high energy peak;" << endl << endl;
  auto fileMean_high = TFile::Open(readDir + meanFileName_high);
  auto mean_high = (TH2D*)fileMean_high->Get(fileMean_high->GetListOfKeys()->At(0)->GetName());
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {
    reportFile << "\t" << "Strip " << stripNb << ": " << mean_high->GetBinContent(1, stripNb + 1) << endl;
    cout << calibratedPeaks[stripNb][2] 
	    - (mean_high->GetBinContent(1, stripNb + 1) * coeffA[stripNb] + coeffB[stripNb]) << endl;
  }
  cout << endl;
  reportFile << endl;

  reportFile << "Dead layer estimation [um] by strips are stored in file: "  << deadFileName << ":" << endl;
  for (int stripNb = 1; stripNb <= setup->fStripsAmount; stripNb++) {
    reportFile << "\t" << "Strip " << stripNb - 1 << ": " << histDead->GetBinContent(1, stripNb) << endl;
  }
  reportFile << endl;
  reportFile << "Effective dead layer thickness is: " << sumDeadThickness / normalStripsAmount << " [um]."  << endl;

  reportFile << "Calibration coefficients are stored in files: " << endl;  
  reportFile << " * " << calibFileName 
             << " --- linear coefficient (a) in the first column,"
             << " free coefficient (b) in the second" << endl;
  reportFile << " * " << calibFileName_rev << " --- reversed columns" << endl;

  reportFile << setw(20) << " b " << setw(10) << " a " << endl;
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {
    reportFile << "\t" << "Strip " << stripNb << ": " 
               << coeffB[stripNb] << " " 
               << coeffA[stripNb] 
               << endl;
  }
  reportFile.close();
  gSystem->Exec(TString("cat ") + reportFileName);
}

Bool_t readCalibCoeffs (AnalysisSetup const *setup, 
                      std::vector<pair<double, double>> &calibCoeffs) 
{
  TString readDir;
  readDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), 
                                   setup->fStationName.Data());
  TString calibFileName;
  calibFileName.Form("calibCoeff_%s_%s.txt", setup->fFileBaseName.Data(), 
                                             setup->fStationName.Data());  
  ifstream inFile;
  inFile.open(readDir + calibFileName);
  if(!inFile.good()) {
    cout << "Can't open the " << readDir + calibFileName << " file" << endl;
    cout << "Calibration coefficients were not read, calibrated canvas won't be built" << endl;
    return kFALSE;
  }

  Double_t a, b;
  cout << "Read coefficients " << endl;
  cout << "a " << "b " << endl;
  for (int i = 0; i < setup->fStripsAmount; ++i) {
    inFile >> a;
    inFile >> b;
    calibCoeffs.emplace_back(a, b);
    cout << a << " " << b << endl;
  }
  return kTRUE; 
}

// Builds calibrated canvas for the multiplicity selected file
void BuildCalibratedSpectra(AnalysisSetup *setup) {
  cout << "[Build calibrated canvases] Begin" << endl;
  vector<pair<double, double>> calibCoeffs;
  if (!readCalibCoeffs(setup, calibCoeffs)) {
    return;
  }
  gStyle->SetOptStat(1001101);
  TFile *inputFile = new TFile(setup->fMultSelectFilePath);  
  auto tree = (TTree*)inputFile->Get(setup->fTreeName);

  UShort_t stationData[setup->fStripsAmount];
  TString leafName;
  leafName.Form("%s", setup->fStationName.Data());
  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(leafName, stationData);

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("%s_%d", setup->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, 
                             histName, 
                             setup->fBinsAmount, 
                             calibCoeffs[iStrip].second, 
                             calibCoeffs[iStrip].second 
                             + setup->fBinsAmount*calibCoeffs[iStrip].first));
  }
  cout << tree->GetEntries() << endl;
  for (Long64_t eventNb = 0; eventNb < tree->GetEntries(); eventNb++) {
    tree->GetEntry(eventNb);
    if (!(eventNb % 100000)) {
      cout << "events were handled: " << eventNb << endl;
    }
    for (int iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) { 
      hists[iStrip]->Fill(calibCoeffs[iStrip].second + stationData[iStrip]*calibCoeffs[iStrip].first
                          + 0.5*(calibCoeffs[iStrip].second 
                                + setup->fBinsAmount*calibCoeffs[iStrip].first) / setup->fBinsAmount);
    }
  }
  TString saveFileName;
  auto canvas = new TCanvas("second_spectraPreview_selected");
  int canvFirtsDim = round(sqrt(setup->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
  }
  saveFileName.Form("./results/%s/%s/canvases/calibrated_%s_%s.root", 
                     setup->fFileBaseName.Data(), 
                     setup->fStationName.Data(),
                     setup->fFileBaseName.Data(), 
                     setup->fStationName.Data());
  canvas->SaveAs(saveFileName);
  // exit(1);
}

// Calibration over prepared files with noise thresholds and multiplicity selection
void Calibration(AnalysisSetup *setup) {
  setup->ReadThresholds();
  for (Int_t stripNb = 0; stripNb < setup->fStripsAmount; stripNb++) {  
    SlidingWindowPeakSearch(setup, stripNb);
    // here one can insert other peak searching algorithms
  }
  SaveSpectrumCharacteristics(setup);
  CalcCalibCoeffs(setup);
}

// builds output histogram for detector 1 (setup1) with demand to have no signal 
// in the detector 2 (setup2)
// Both stations data must be stored in common data file
void BuildOneDetectorSpectra (TString expFilePath,
                              TString treeName, TString branchName, 
                              TString stName1, TString stName2,
                              Int_t stripsAmount1, Int_t stripsAmount2, 
                              Int_t adcChannelsAmount)
{
  auto setup1 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName1, stripsAmount1, adcChannelsAmount, 1);
  auto setup2 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName2, stripsAmount2, adcChannelsAmount, 1);
  FindThresholds(setup1);
  // FindThresholds(setup2);
  setup1->ReadThresholds();
  setup2->ReadThresholds();

  TFile *inputFile = new TFile(expFilePath);
  auto tree = (TTree*)inputFile->Get(treeName);
  // create output file
  TFile *outFile = new TFile(setup1->fMultSelectFilePath.Data(), "RECREATE");
  TTree *outTree = new TTree(setup1->fTreeName, setup1->fTreeName + " with single multiplicity");
  // variables for the work with trees
  UShort_t stationData1[setup1->fStripsAmount];
  UShort_t stationData2[setup2->fStripsAmount];
  TString outDataName1;
  TString outDataName2;
  outDataName1.Form("%s[%d]", setup1->fStationName.Data(), setup1->fStripsAmount);
  outDataName2.Form("%s[%d]", setup2->fStationName.Data(), setup2->fStripsAmount);

  outTree->Branch(setup1->fStationName, &stationData1, outDataName1 + "/s");
  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup1->fBrachName + "." + outDataName1, stationData1);
  tree->SetBranchAddress(setup2->fBrachName + "." + outDataName2, stationData2);

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup1->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("thin_%s_%s_%d", setup1->fFileBaseName.Data(), 
                                   setup1->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, histName, 
                             setup1->fBinsAmount, 0, setup1->fBinsAmount));
  }

  Long64_t entriesAmount = tree->GetEntries();
  cout << "[MultiplicitySelection] Input tree entries amount: " 
       << entriesAmount << endl;
  for (Long64_t eventNb = 0; eventNb <  entriesAmount; eventNb++) {
    tree->GetEntry(eventNb);
    bool flagWriteEvent = true;
    int mult1 = CheckMultiplicityOfEvent(setup1, stationData1);
    int mult2 = CheckMultiplicityOfEvent(setup2, stationData2);
    if (mult1 != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (mult2 != 0) {
      flagWriteEvent = false;
      continue;
    }
    if (flagWriteEvent) {
      for (int iStrip = 0; iStrip < setup1->fStripsAmount; iStrip++) {
        hists[iStrip]->Fill(stationData1[iStrip]);
      }
      outTree->Fill();
    }
  }
  outTree->Write();
  TString saveFileName;
  saveFileName.Form("./results/%s/%s/canvases/only_%s_%s.root", 
                     setup1->fFileBaseName.Data(),
                     setup1->fStationName.Data(),
                     setup1->fFileBaseName.Data(), 
                     setup1->fStationName.Data());
  auto fileSaveHists = TFile::Open(saveFileName, "RECREATE");

  auto canvas = new TCanvas("");
  int canvFirtsDim = round(sqrt(setup1->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup1->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup1->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
    hists[iStrip]->Write();
  }
  canvas->Write();
}

// builds output histogram for detector 2 (setup2) with demand to have single 
// multiplicity in both stations (setup 1 too)
// Both stations data must be stored in common data file
void BuildOneDetectorFromPair (TString expFilePath,
                              TString treeName, TString branchName, 
                              TString stName1, TString stName2,
                              Int_t stripsAmount1, Int_t stripsAmount2, 
                              Int_t adcChannelsAmount)
{
  auto setup1 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName1, stripsAmount1, adcChannelsAmount, 1);
  auto setup2 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName2, stripsAmount2, adcChannelsAmount, 1);
  FindThresholds(setup1);
  FindThresholds(setup2);
  setup1->ReadThresholds();
  setup2->ReadThresholds();

  TFile *inputFile = new TFile(expFilePath);
  auto tree = (TTree*)inputFile->Get(treeName);
  // create output file
  // variables for the work with trees
  UShort_t stationData1[setup1->fStripsAmount];
  UShort_t stationData2[setup2->fStripsAmount];
  TString outDataName1;
  TString outDataName2;
  outDataName1.Form("%s[%d]", setup1->fStationName.Data(), setup1->fStripsAmount);
  outDataName2.Form("%s[%d]", setup2->fStationName.Data(), setup2->fStripsAmount);

  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup1->fBrachName + "." + outDataName1, stationData1);
  tree->SetBranchAddress(setup2->fBrachName + "." + outDataName2, stationData2);

  std::vector<TH1D*> hists;
  for (int iStrip = 0; iStrip < setup2->fStripsAmount; iStrip++) {
    TString histName;
    histName.Form("thin_%s_%s_%d", setup2->fFileBaseName.Data(), 
                                   setup2->fStationName.Data(), iStrip);
 
    hists.push_back(new TH1D(histName, histName, setup1->fBinsAmount, 0, setup1->fBinsAmount));
  }

  Long64_t entriesAmount = tree->GetEntries();
  cout << "[MultiplicitySelection] Input tree entries amount: " << entriesAmount << endl;
  for (Long64_t eventNb = 0; eventNb <  entriesAmount; eventNb++) {
    tree->GetEntry(eventNb);
    bool flagWriteEvent = true;
    int mult1 = CheckMultiplicityOfEvent(setup1, stationData1);
    int mult2 = CheckMultiplicityOfEvent(setup2, stationData2);
    if (mult1 != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (mult2 != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (flagWriteEvent) {
      for (int iStrip = 0; iStrip < setup2->fStripsAmount; iStrip++) {
        hists[iStrip]->Fill(stationData2[iStrip]);
      }
    }
  }
  TString saveFileName;
  saveFileName.Form("./results/%s/%s/canvases/twoSensors_%s_%s.root",
                     setup2->fFileBaseName.Data(), 
                     setup2->fStationName.Data(),
                     setup2->fFileBaseName.Data(),
                     setup2->fStationName.Data());
  auto fileToSave = TFile::Open(saveFileName.Data(), "RECREATE");

  auto canvas = new TCanvas("canvas");
  int canvFirtsDim = round(sqrt(setup1->fStripsAmount));
  int canvSecondDim = 1;
  for (; canvSecondDim*canvFirtsDim < setup1->fStripsAmount; canvSecondDim++) {}
  canvas->Divide(canvFirtsDim, canvSecondDim);
  for (Int_t iStrip = 0; iStrip < setup1->fStripsAmount; iStrip++) {
    canvas->cd(iStrip + 1);
    gPad->SetLogy();
    hists[iStrip]->Draw();
    hists[iStrip]->Write();
  }
  canvas->Write(saveFileName);
}

TString GetMeanFilePath (AnalysisSetup const *setup) {
  TString fileBase = setup->fFileBaseName;
  TString channelName = setup->fStationName;
  TString meanFileName_high;
  TString readDir;
  readDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), 
                                   setup->fStationName.Data());
  meanFileName_high.Form("channelsHist_high_%s_%s.root", fileBase.Data(), 
                                                         channelName.Data());
  return readDir + meanFileName_high;
}

TString GetMeanFilePath (AnalysisSetup const *setup, TString fileName) {
  TString fileBase = setup->fFileBaseName;
  TString channelName = setup->fStationName;
  TString meanFileName_high;
  TString readDir;
  readDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), 
                                   setup->fStationName.Data());
  meanFileName_high.Form("%s_%s_%s.root", fileName.Data(),
                                          fileBase.Data(), 
                                          channelName.Data());
  return readDir + meanFileName_high;
}

TString GetMeanFilePathThinMap (AnalysisSetup const *setup) {
  TString fileBase = setup->fFileBaseName;
  TString channelName = setup->fStationName;
  TString meanFileName_high;
  TString readDir;
  readDir.Form("./results/%s/%s/", setup->fFileBaseName.Data(), 
                                   setup->fStationName.Data());
  meanFileName_high.Form("channelsHist_high_map_%s_%s.root", fileBase.Data(), 
                                                             channelName.Data());
  return readDir + meanFileName_high;
}

TObject* GetFirstKey (TString const filePath) {
  auto fileMean_high = TFile::Open(filePath.Data());
  return fileMean_high->Get(fileMean_high->GetListOfKeys()->At(0)->GetName());
}

void CreateTmpHistInDir(AnalysisSetup const *setupThin,
                        AnalysisSetup const *setupThick,
                        Int_t const strNbThin,
                        Int_t const strNbThick) 
{
  TFile *inputFile = new TFile(setupThick->fMultSelectFilePath);  
  auto tree = (TTree*)inputFile->Get(setupThick->fTreeName);

  TString varIndThin, varIndThick;
  TString histName;
  histName.Form("tmpHistPix_%s_%d_%d", setupThick->fFileBaseName.Data(), 
                                    strNbThin,
                                    strNbThick);
  cout << histName << " histName " << endl;
  varIndThin.Form("[%d]", strNbThin);
  varIndThick.Form("[%d]", strNbThick);
  TString histParams;
  histParams.Form("(%d,%d,%d)", setupThick->fBinsAmount, 0, setupThick->fBinsAmount);  
  TString varExp = setupThick->fStationName + varIndThick + ">>" + histName + histParams; 
  //TString varExp = setupThin->fBrachName + "." + setupThin->fStationName + varIndThin + ">>" + histName + histParams; 
  //TString cutExp = setupThin->fBrachName + "." + setupThin->fStationName + varIndThin + ">" 
  //               + TString::Itoa(setupThin->fThresolds[strNbThin] + setupThin->fThresholdOffset,10); 
  TString cutExp = setupThin->fStationName + varIndThin + ">" 
                 + TString::Itoa(setupThin->fThresolds[strNbThin] + setupThin->fThresholdOffset,10); 
  cutExp += "&&";
  cutExp += (setupThick->fStationName + varIndThick + ">" 
          + TString::Itoa(setupThick->fThresolds[strNbThick] + setupThick->fThresholdOffset,10));
  cout << varExp << " " << cutExp << endl;
  auto tmpHist = new TH1F(histName, histName, setupThick->fBinsAmount, 0, setupThick->fBinsAmount);
  // @TODO look at this
  tree->GetListOfBranches()->Print();
  tree->Draw(varExp, "", "");
  tree->Draw(varExp, cutExp, "");
  return;
}

Double_t SlidingWindowHighPeakPixel(AnalysisSetup const *setupThin,
                                    AnalysisSetup const *setupThick, 
                                    Int_t const strNbThin, 
                                    Int_t const strNbThick) 
{
  CreateTmpHistInDir(setupThin, setupThick, strNbThin, strNbThick);
  TString  histName;
  histName.Form("tmpHistPix_%s_%d_%d", setupThick->fFileBaseName.Data(), 
                                 strNbThin,
                                 strNbThick);
  TH1F* histSpectrum = (TH1F*)gDirectory->Get(histName);
  Double_t peaksReq;
  vector<Double_t> peaks = GetPeaksFromSpectrum(setupThick, histSpectrum);
  if (peaks.size() < 1) {
    peaksReq = 0;
  } else {
    peaksReq = peaks.back();
  }
  cout << histSpectrum->GetName() << " name " << endl;
  histSpectrum->GetXaxis()->SetRange((int)(peaksReq - setupThick->fEpsPeak), 
                                     (int)(peaksReq + setupThick->fEpsPeak));
  int maxBin = histSpectrum->GetMaximumBin();
  // cout << "Max bin " << maxBin << endl;
  // cout << (int)(peaksReq) << endl;
  gStyle->SetStatFormat("6.8g");
  unsigned maxIntegral = numeric_limits<unsigned>::min();
  Double_t peakMean;
  Double_t peakRMS;
  unsigned peakWindLeftBound;
  for (int i = maxBin - setupThick->fEpsPeak; i < maxBin; i++) {
    unsigned integral;
    histSpectrum->GetXaxis()->SetRange(i, i+setupThick->fIdentifyWindWidth - 1);
    integral = histSpectrum->Integral();
    if (maxIntegral < integral) {
      maxIntegral = integral;
      peakWindLeftBound = i;
      peakMean = histSpectrum->GetMean();
      peakRMS  = histSpectrum->GetStdDev();
    }
  }
  histSpectrum->GetXaxis()->SetRange(peakWindLeftBound, 
                                     peakWindLeftBound+setupThick->fIdentifyWindWidth - 1);
  histSpectrum->GetXaxis()->SetRange(0, setupThick->fBinsAmount);
  TString saveDir;
  saveDir.Form("./results/%s/%s/tmpHists/", setupThick->fFileBaseName.Data(), 
                                            setupThick->fStationName.Data());
  histSpectrum->SaveAs(saveDir + TString(histSpectrum->GetName()) + ".root");
  return peakMean;
}

void MakeAmpMapThin(AnalysisSetup const *setupThin,
                    AnalysisSetup const *setupThick)
{
  TString filePath = GetMeanFilePathThinMap(setupThick);
  auto hist = new TH2D(getFileNameFromPath(filePath), getFileNameFromPath(filePath),
                       setupThin->fStripsAmount, 0, setupThin->fStripsAmount,
                       setupThick->fStripsAmount, 0, setupThick->fStripsAmount);
  for (int strIndThin = 1; strIndThin <= setupThin->fStripsAmount; strIndThin++) {
    for (int strIndThick = 1; strIndThick <= setupThick->fStripsAmount; strIndThick++) {
      Double_t amp = SlidingWindowHighPeakPixel(setupThin, setupThick, 
                                                strIndThin-1, strIndThick-1);
      cout << "amp " << amp << endl;
      hist->SetBinContent(strIndThin, strIndThick, amp);
    }
  }
  hist->SaveAs(filePath.Data());
}

// quadratic approximation coefficients 
// by dead layer points 8, 12, 16, 20, 24, 28, 32, 36 um
// vector<double> quad_coefs = {0.159353, 0.0838116, 0.00149019}; // highE
vector<double> quad_coefs = {0.159428, 0.0837999, 0.0014907}; // highE 

// by dead layer points 2, 4, 6, 8, 12, 16, 20, 24, 28, 32, 36, 40 um
// vector<double> quad_coefs = {0.0930085, 0.0857144, 0.00153956};
double GetDeadThicknessQuad(double dE) {
  for (auto itCoeffs : quad_coefs) {
    cout << " " << itCoeffs;
  }
  cout << endl;
  return (-quad_coefs[1] + sqrt(pow(quad_coefs[1], 2) 
          - 4*quad_coefs[2]*(quad_coefs[0] - dE))) 
         / (2 * quad_coefs[2]);
}

// Cubic approximation of the d(dE)  p0, p1, p2, p3 by dead layer points 8, 12, 16, 20, 24, 28, 32, 36 um
// for the 7.6869 MeV alpha-particle
// d(dE) = p0 + p1*dE + p2*dE^2 + p3*dE^3
vector<double> triag_coefs = {0.00805579, 9.18781, -0.401229, -0.0044059};
 
double GetDeadThicknessTri (double dE) {
  for (auto itCoeffs : triag_coefs) {
    cout << " " << itCoeffs;
  }
  cout << endl;
  return triag_coefs[0] + triag_coefs[1]*dE
         + triag_coefs[2] * pow(dE, 2)
         + triag_coefs[3] * pow(dE, 3);
}

void BuildInhomogenuityMap(Double_t deadThicknessMean,
                           AnalysisSetup const *setupThin, 
                           AnalysisSetup const *setupThick,
                           AnalysisSetup const *setupThickOnly) 
{
  setupThin->ReadThresholds();
  setupThick->ReadThresholds();
  setupThickOnly->ReadThresholds();
  MakeAmpMapThin(setupThin, setupThick);  
  auto meanHighThin = (TH2D*)GetFirstKey(GetMeanFilePathThinMap(setupThick));
  auto meanHighThick = (TH2D*)GetFirstKey(GetMeanFilePath(setupThickOnly));
  cout << meanHighThick << " " << meanHighThin << endl;
  vector<pair<double, double>> calibCoeffs;
  if (!readCalibCoeffs(setupThickOnly, calibCoeffs)) {
    return;
  }

  TString filePath = GetMeanFilePath(setupThick, "thicknessMap");
  auto hist = new TH2D(getFileNameFromPath(filePath), getFileNameFromPath(filePath),
                       setupThin->fStripsAmount, 0, setupThin->fStripsAmount,
                       setupThickOnly->fStripsAmount, 0, setupThickOnly->fStripsAmount);

  Double_t N1, N2, deltaE;
  for (int strIndThin = 1; strIndThin <= setupThin->fStripsAmount; strIndThin++) {
    for (int strIndThick = 1; strIndThick <= setupThick->fStripsAmount; strIndThick++) {
      N2 = meanHighThick->GetBinContent(1, strIndThick);
      N1 = meanHighThin->GetBinContent(strIndThin, strIndThick);
      deltaE = (N2 - N1) * calibCoeffs[strIndThick-1].first;
      hist->SetBinContent(strIndThin, strIndThick, 
                          GetDeadThicknessTri(deltaE) - deadThicknessMean);

    }
  }
  hist->SaveAs(filePath);
  hist->SaveAs(filePath + ".png");
}

// builds output histogram for detector 1 (setup1) with demand to have no signal 
// in the detector 2 (setup2)
// Both stations data must be stored in common data file
void BuildPixelSpectra (TString expFilePath,
                        TString treeName, TString branchName, 
                        TString stName1, TString stName2,
                        Int_t stripsAmount1, Int_t stripsAmount2, 
                        Int_t adcChannelsAmount)
{
  auto setup1 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName1, stripsAmount1, adcChannelsAmount, 1);
  auto setup2 = new AnalysisSetup(expFilePath, treeName, branchName, 
                                  stName2, stripsAmount2, adcChannelsAmount, 1);
  // FindThresholds(setup1);
  FindThresholds(setup2);
  setup1->ReadThresholds();
  setup2->ReadThresholds();

  TFile *inputFile = new TFile(expFilePath);
  auto tree = (TTree*)inputFile->Get(treeName);
  // variables for the work with trees
  UShort_t stationData1[setup1->fStripsAmount];
  UShort_t stationData2[setup2->fStripsAmount];
  TString outDataName1;
  TString outDataName2;
  outDataName1.Form("%s[%d]", setup1->fStationName.Data(), setup1->fStripsAmount);
  outDataName2.Form("%s[%d]", setup2->fStationName.Data(), setup2->fStripsAmount);
  // assign data structure to the input tree
  tree->SetMakeClass(1); // is needed for the correct interpretaion ot tree inner structure
  tree->SetBranchAddress(setup1->fBrachName + "." + outDataName1, stationData1);
  tree->SetBranchAddress(setup2->fBrachName + "." + outDataName2, stationData2);

  std::vector<TH1D*> hists;
  for (int iStripThin = 0; iStripThin < setup1->fStripsAmount; iStripThin++) {
    for (int iStripThick = 0; iStripThick < setup2->fStripsAmount; iStripThick++) {
      TString histName;
      histName.Form("%s_%s_thin-%d_thick-%d", setup1->fFileBaseName.Data(), 
                                              setup1->fStationName.Data(), 
                                              iStripThin,
                                              iStripThick);
   
      hists.push_back(new TH1D(histName, histName, 
                               setup1->fBinsAmount, 0, setup1->fBinsAmount));
    }
  }
  Long64_t entriesAmount = tree->GetEntries();
  cout << "[MultiplicitySelection] Input tree entries amount: " 
       << entriesAmount << endl;
  for (Long64_t eventNb = 0; eventNb <  entriesAmount; eventNb++) {
    tree->GetEntry(eventNb);
    bool flagWriteEvent = true;
    int mult1 = CheckMultiplicityOfEvent(setup1, stationData1);
    int mult2 = CheckMultiplicityOfEvent(setup2, stationData2);
    if (mult1 != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (mult2 != 1) {
      flagWriteEvent = false;
      continue;
    }
    if (flagWriteEvent) {
      for (int iStripThin = 0; iStripThin < setup1->fStripsAmount; iStripThin++) {
        for (int iStripThick = 0; iStripThick < setup2->fStripsAmount; iStripThick++) {
          int histInd = iStripThin*setup1->fStripsAmount + iStripThick; 
          hists[histInd]->Fill(stationData1[iStripThin]);
        }
      }
    }
  }
  TString saveFileName;
  saveFileName.Form("./results/%s/%s/canvases/pixelSpectra_%s_%s.root", 
                     setup1->fFileBaseName.Data(),
                     setup1->fStationName.Data(),
                     setup1->fFileBaseName.Data(), 
                     setup1->fStationName.Data());
  auto fileSaveHists = TFile::Open(saveFileName, "RECREATE");
  for (int iStripThin = 0; iStripThin < setup1->fStripsAmount; iStripThin++) {
    for (int iStripThick = 0; iStripThick < setup2->fStripsAmount; iStripThick++) {
      int histInd = iStripThin*setup1->fStripsAmount + iStripThick; 
      hists[histInd]->Draw();
      hists[histInd]->Write();
    }
  }
}

void calibrationByOneSpectrum (TString fileName = "", TString sensorName = "", Double_t w = 0) 
{
  TString treeName = "AnalysisxTree";
  TString className = "NeEvent";
  Int_t stripsAmount = 16; // stips amount in the analysed station 
  Int_t adcChannelRange = 2048; // amount of bins in 
  Int_t startBinToFindThresholds = 1;
  auto setup = new AnalysisSetup(fileName, treeName, className, 
                                 sensorName,
                                 stripsAmount, adcChannelRange,
                                 startBinToFindThresholds, 1);

  // // build preview of raw data and store canvas in ./result/canvases sub-directory
  // // PreviewRawData(setup1, 2048);

  // // Find noise thresholds. The file with thresholds thresholds_[EXP_FILE_NAME]_[SENSOR_NAME].txt is stored in ./result directory
  FindThresholds(setup);

  // // Multiplicity selection. The root file is used for the further handling is saved under 
  // // multSelected_[RAW_FILE_NAME]_[SENSOR_NAME_1].root
  // // A canvas with calibrated spectra is saved in ./canvases sub-directory
  MultiplicitySelection(setup);
  
  setup->SetEpsPeak(w / 2.); // set search region for the "sliding window" around the peak position X obtained by TSpectrum: [X-eps; X+eps]
  setup->SetIdentifyWindWidth(w); // the "sliding window" width
  setup->SetFitMinSigma(6.); // peaks with the set sigma will be obtained by TSpectrum algorithm 
  setup->SetFitPeakThreshold(0.7); // peaks height relative to the maximal peak amplitude

  Calibration(setup);
  // BuildCalibratedSpectra(setup);
}
