void fieldDataToERFormat() {

  TString filePath = getenv("VMCWORKDIR");
  filePath += "/input/testField.dat";
	ofstream outFile(filePath);
  ifstream  inFile;
  inFile.open("field map2.table");

  outFile << "nosym" << endl;
  Double_t startCoordX = -28.; 
  Double_t startCoordY = -6.; 
  Double_t startCoordZ = -150.;
  Double_t finishCoordX = 28.; 
  Double_t finishCoordY = 6.; 
  Double_t finishCoordZ = 100.;
  Double_t coordX = startCoordX;
  Double_t coordY = startCoordY;
  Double_t coordZ = startCoordZ;
  Double_t gridStep = 2.;
  outFile << startCoordX << "\t" << finishCoordX << "\t" << abs(startCoordX - finishCoordX) / gridStep + 1 << endl;
  outFile << startCoordY << "\t" << finishCoordY << "\t" << abs(startCoordY - finishCoordY) / gridStep + 1 << endl;
  outFile << startCoordZ << "\t" << finishCoordZ << "\t" << abs(startCoordZ - finishCoordZ) / gridStep + 1 << endl;
  Double_t number;
  Int_t    iterCount = 0;

  while (inFile >> number) {
    if (iterCount > 2) {
      outFile << number << "\t";
    }
    iterCount++;
    if (iterCount == 6) {
        outFile << endl;
        iterCount = 0;
    }
  }
  /*while (coordX <= finishCoordX) {
    while (coordY <= finishCoordY) {
      while (coordZ <= finishCoordZ) {
        outFile << coordX << "\t" << coordY << "\t" << coordZ << "\t";
        outFile << 1.36552379466 << "\t" << 0 << "\t" << 0 << endl;
        coordZ += gridStep;
      }
      coordZ  = startCoordZ;
      coordY += gridStep;
    }
    coordY = startCoordY;
    coordX += gridStep;
  }*/
}