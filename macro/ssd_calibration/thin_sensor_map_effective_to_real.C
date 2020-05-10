// This script is intended to recalculate an effective thin sensor thickness to real thickness.
// Recalculating may be required when the sensor is significantly shifted from a position 
// where the assumption of direct exposure is applicable.
// The thickness of sensor is stored in a pixel map (TH2D histogram).
// Effective values are estimated by procedure over calibration run data where
// a sensor may be offset on some distance from a radioactive source.
//
//       ______
//  _____|    1|
// |4    |     |
// |     |_____|__                        
// |_____|__|    2|
//    |3    |     |
//    |     |_____|                        
//    |_____|      
//               Y^
//                |
//           <----| 
//           X 

enum StripDirection 
{
  X_P, // X positive
  Y_P, // Y positive
  X_N, // X negative
  Y_N  // Y negative
};
                         
TString mapFileName = "./maps/thicknessMap_calib_90_all_SSD_1m_3.root";
TString mapFileNameOut = "./maps/thicknessMap_calib_90_all_SSD_1m_3_real.root";
Int_t strNbThin = 16; // strips number (along X axis in 2d-histogram)
Int_t strNbThick = 16; // strips number (along Y axis in 2d-histogram)


// FIRST telescope
// Double_t sensSizeX = 5.; // [cm]
// Double_t sensSizeY = 6.; // [cm]
// Double_t sensOffsetX = -4.63;
// Double_t sensOffsetY = 2.35;
// Double_t sensOffsetZ = 30.;
// StripDirection mapStrDirThin = StripDirection::X_N;
// StripDirection mapStrDirThick = StripDirection::Y_N;


// // SECOND telescope
// Double_t sensSizeX = 6.; // [cm]
// Double_t sensSizeY = 5.; // [cm]
// Double_t sensOffsetX = -2.35;
// Double_t sensOffsetY = -4.65;
// Double_t sensOffsetZ = 30.;
// StripDirection mapStrDirThin = StripDirection::X_P;
// StripDirection mapStrDirThick = StripDirection::Y_N;

// // THIRD telescope
Double_t sensSizeX = 5.; // [cm]
Double_t sensSizeY = 6.; // [cm]
Double_t sensOffsetX = 4.63;
Double_t sensOffsetY = -2.35;
Double_t sensOffsetZ = 30.;
StripDirection mapStrDirThin = StripDirection::X_P;
StripDirection mapStrDirThick = StripDirection::Y_P;

// FOURTH telescope
// Double_t sensSizeX = 6.; // [cm]
// Double_t sensSizeY = 5.; // [cm]
// Double_t sensOffsetX = 2.35;
// Double_t sensOffsetY = 4.63;
// Double_t sensOffsetZ = 30.;
// StripDirection mapStrDirThin = StripDirection::X_N;
// StripDirection mapStrDirThick = StripDirection::Y_P;


// All input maps have ascending strip numbering along negative
// X-axis direction and positive Y-direction according to classical
// ROOT/Geant axes directions (as in the figure on the top of this file).
// In a thickness map Along X-axis thin sensor coordinates are put,  
// along Y - thick sensors ones.
// Thin strips in a map may detect Y-coordinate with asd
// in an experiment, so a map will be transposed.
void MapStripsRedirection(TH2D* mapIn, TH2D* mapInRealDir) {
  int xPixel;
  int yPixel;
  int srtNbFirstDir = mapIn->GetNbinsX();
  int srtNbSecondDir = mapIn->GetNbinsY();
  for (int i = 1; i <= srtNbFirstDir; i++) {   
    switch (mapStrDirThin) {
      case X_P: xPixel = srtNbFirstDir - i + 1; break;
      case Y_P: yPixel = i; break;
      case X_N: xPixel = i; break;
      case Y_N: yPixel = srtNbFirstDir - i + 1; break;
    }
    for (int j = 1; j <= srtNbSecondDir; j++) {
      switch (mapStrDirThick) {
        case X_P: xPixel = j; break;
        case Y_P: yPixel = j; break;
        case X_N: xPixel = srtNbSecondDir - j + 1; break;
        case Y_N: yPixel = srtNbSecondDir - j + 1; break;
      }
      Double_t inPixVal = mapIn->GetBinContent(i, j);
      mapInRealDir->SetBinContent(xPixel, yPixel, inPixVal);
    }
  }
}

TString getFileNameFromPath (TString const &path) {
  TString fileName(path(path.Last('/') + 1, path.Length())); // file name with extension
  // remove file extension. Extension is considered as part of the name after last "." symbol
  fileName.Remove(fileName.Last('.'), fileName.Length());    
  return fileName;
}

void thin_sensor_map_effective_to_real () {
  auto mapFile = TFile::Open(mapFileName);
  auto mapIn = (TH2D*)mapFile->Get(mapFile->GetListOfKeys()->At(0)->GetName());
  auto name = getFileNameFromPath(mapFile->GetName());
  // mapFile->Close();
  auto fileOut = TFile::Open(mapFileNameOut, "RECREATE");
  auto nameTmp = name + TString("_tmp");
  auto nameReal = name + TString("_realHist");
  auto nameOut = name + TString("_out");
  auto nameAngles = name + TString("_angles");

  TH2D* mapInRealDir;
  TH2D* mapOut;
  TH2D* mapOutInitDirection;
  TH2D* angles;

  if (mapStrDirThin == Y_N || mapStrDirThin == Y_P) {
    mapInRealDir = new TH2D(nameTmp, nameTmp, strNbThick, 0, strNbThick, strNbThin, 0, strNbThin);
    mapOut = new TH2D(name, name, strNbThick, 0, strNbThick, strNbThin, 0, strNbThin);
  } else {
    mapInRealDir = new TH2D(nameTmp, nameTmp, strNbThin, 0, strNbThin, strNbThick, 0, strNbThick);
    mapOut = new TH2D(name, name, strNbThin, 0, strNbThin, strNbThick, 0, strNbThick);
  }
  mapOutInitDirection = new TH2D(nameReal, nameReal, strNbThin, 0, strNbThin, strNbThick, 0, strNbThick);
  angles = new TH2D(nameAngles, nameAngles, strNbThin, 0, strNbThin, strNbThick, 0, strNbThick);
  MapStripsRedirection(mapIn, mapInRealDir);
  mapIn->Draw("col&text");
  Double_t strStepX = sensSizeX / ((Double_t) strNbThin);
  Double_t strStepY = sensSizeY / ((Double_t) strNbThick);

  Double_t outStrNbX = mapInRealDir->GetNbinsX();
  Double_t outStrNbY = mapInRealDir->GetNbinsY();
  auto offset = TVector3(sensOffsetX, sensOffsetY, sensOffsetZ);

  for (Int_t i = 1; i <= outStrNbX; i++) {
    Double_t pixX = sensSizeX / 2. 
                  - strStepX / 2. 
                  - (i - 1) * strStepX
                  + offset.X();
    Double_t pixZ = sensOffsetZ;
    for (Int_t j = 1; j <= outStrNbY; j++) {
      Double_t pixY = - sensSizeY / 2. 
                    + strStepY / 2. 
                    + (j - 1) * strStepY
                    + offset.Y();

      // Uncomment that if you believe that projection on thin sensor
      // will make your coordinate estimation significantly more accurate.
      // And check the logic before.
      // if (sensSizeX>sensSizeY) pixX = pixX*sensOffsetZ/(sensOffsetZ+1.5);
      // if (sensSizeX<sensSizeY) pixY = pixY*sensOffsetZ/(sensOffsetZ+1.5);          

      auto pixVec = TVector3(pixX, pixY, pixZ);
      Double_t d_eff = mapInRealDir->GetBinContent(i, j);
      Double_t d = d_eff * cos(pixVec.Theta());
      angles->SetBinContent(i,j,pixVec.Theta()*TMath::RadToDeg());
      mapOut->SetBinContent(i, j, d);
    }
  }
  auto canv = new TCanvas();
  mapOut->Draw("col&text");
  auto canvAngle = new TCanvas();
  angles->Draw("col&text");
  auto canv1 = new TCanvas();
  mapOutInitDirection->Draw("col&text");
  MapStripsRedirection(mapOut, mapOutInitDirection);  
  mapOutInitDirection->Write();
}
