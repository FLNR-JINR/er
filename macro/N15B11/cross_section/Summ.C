void Summ()
{
	ifstream fin("cases/case_1_N15_cross_and_theta.txt");
	if (!fin.is_open()) {
		cerr << "Can not open file .txt" << endl;
		return; 
	}

	Int_t it = 0;
	Double_t Integral = 0.;
	Double_t prevTheta = 0.;
	Double_t prevSigma = 0.;
	while (!fin.eof()) {
		Double_t curTheta;
		Double_t curSigma;
		fin >> curTheta >> curSigma;
		if (curTheta >= 86.) break;
		Integral += curSigma;
		//cout << curTheta << "\t" << curSigma << endl;
	}
	cout << "Integral: " << Integral << endl;
	fin.close();
	fin.open("input/nrv.txt");
	if (!fin.is_open()) {
	  cerr << "Can not open file .txt" << endl;
	  return; 
	}
	Double_t Integral2 = 0.;
	Double_t cTheta = 11.;
	while (!fin.eof()) {
		Double_t curTheta;
		Double_t curSigma;
		fin >> curTheta >> curSigma;
		if (curTheta < 11.) continue;
		if (curTheta >= 86.) break;
		
		if (curTheta == cTheta) {
		  Integral2 += curSigma;
		  cTheta += 1.;
		}
		
		//Integral2 += curSigma;
		//cout << curTheta << "\t" << curSigma << endl;
	}
	//Integral2 *= 0.1;
	cout << "Integral2: " << Integral2 << endl;

	cout << "Integral2/Integral: " << Integral2/Integral << endl;
	cout << "Integral/Integral2: " << Integral/Integral2 << endl;
}