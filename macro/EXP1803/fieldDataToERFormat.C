Double_t fieldTransZ = 0.; // this parameter should coincide with "magnetCenterZ" from /macro/geo/create_GadastEXP1803_geo.C

void fieldDataToERFormat() {

  TString filePath = getenv("VMCWORKDIR");
  filePath += "/input/exp1803Field.dat";
	ofstream outFile(filePath);
  ifstream  inFile;
  inFile.open("field_map2.table");

  outFile << "nosym" << endl;
  Double_t startCoordX = -28.; 
  Double_t startCoordY = -6.; 
  Double_t startCoordZ = -150. + fieldTransZ;
  Double_t finishCoordX = 28.; 
  Double_t finishCoordY = 6.; 
  Double_t finishCoordZ = 100. + fieldTransZ;
  Double_t coordX = startCoordX;
  Double_t coordY = startCoordY;
  Double_t coordZ = startCoordZ;
  Double_t gridStep = 2.;
  outFile << startCoordX << "\t" << finishCoordX << "\t" << abs(startCoordX - finishCoordX) / gridStep + 1 << endl;
  outFile << startCoordY << "\t" << finishCoordY << "\t" << abs(startCoordY - finishCoordY) / gridStep + 1 << endl;
  outFile << startCoordZ << "\t" << finishCoordZ << "\t" << abs(startCoordZ - finishCoordZ) / gridStep + 1 << endl;
  Double_t number;
  Int_t    iterCount = 0;
  Double_t x, y, z;
  Double_t integralB = 0;
  while (inFile >> number) {
    switch (iterCount) {
      case 0: x = number; break;
      case 1: y = number; break;
      case 2: z = number; break;
    }
    if (iterCount > 2) {
      outFile << -number << "\t";
    }
    if (iterCount == 4 && x == 0 && y == 0 && z >= -1500) {
      static int cou = 0;
      cout << "x = " << x << "; y = " << y << "; z = " << z << "; By = " << number << "; - " << ++cou << endl;
      integralB += number * gridStep * 1e-2;
    }
    iterCount++;
    if (iterCount == 6) {
        outFile << endl;
        iterCount = 0;
    }
  }
  cout << "Field integral along the trajectory (x = 0, y = 0) is " << integralB << endl;
}
