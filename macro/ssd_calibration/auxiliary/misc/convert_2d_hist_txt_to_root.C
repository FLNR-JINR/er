/**  
** @brief Reads vector of floating data from a file stream.
** @param filePath path to input file
**/
std::vector<std::vector<Double_t>> Read2DDoubleVectorFromFile(const TString& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    Error("Read2DDoubleVectorFromFile", "Failed to open file: %s", filePath.Data());
  }
  std::vector<std::vector<Double_t>> vec2d;
  std::string line;
  while (std::getline(file, line)) {
    std::vector<Double_t> vec;
    std::istringstream iss(line);
    std::string str_val;
    while (iss >> str_val) {
      vec.push_back(std::stod(str_val));
    }
    vec2d.push_back(vec);
  }
  file.close();

  return vec2d;
}


void convert_2d_hist_txt_to_root () {
  const TString filepath = "map.txt";
  auto peaksPixelHighEMap = Read2DDoubleVectorFromFile(filepath);

  const int first_dim_len = peaksPixelHighEMap.size();
  const int second_dim_len = peaksPixelHighEMap[0].size();

  auto hist = new TH2D(filepath.Data(), filepath.Data(),
                       first_dim_len, 0, first_dim_len,
                       second_dim_len, 0, second_dim_len);

  for (Int_t iFirstDim = 0; iFirstDim < first_dim_len; iFirstDim++) {
    for (Int_t iSecondDim = 0; iSecondDim < first_dim_len; iSecondDim++) {
      hist->SetBinContent(iFirstDim + 1, iSecondDim + 1, peaksPixelHighEMap[iFirstDim][iSecondDim]);
    }
  }

  auto file = TFile::Open(filepath + ".root", "RECREATE");
  hist->Write();
  file->Close();
}
