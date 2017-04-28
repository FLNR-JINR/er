void NeuRad_params(){
	Int_t pixels = 64;
	Int_t modules = 1;

	Float_t QuantumEfficiency = 0.2;
	Float_t Gain = 5.;
	Float_t Sigma = 5.;
	Float_t Crosstalk = 0.01;
	Float_t DiagCrosstalk = 0.005;

	TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
	NeuRadDetDigiFile += "/parameters/NeuRad.digi.v3.par";

	Int_t pixelsInRow = TMath::Sqrt(pixels);
	ofstream fpar;
	fpar.open(NeuRadDetDigiFile.Data());
	fpar << "//-------------------------------------------------------------" << endl;
	fpar << "//----------- Digitisation parameter for NeuRad    ------------" << endl;
	fpar << "//-------------------------------------------------------------" << endl;
	fpar << "[ERNeuRadDigiPar]" << endl;
	fpar << "//-------------------------------------------------------------" << endl;

	fpar << "ERNeuRadPMTQuantumEfficiency: Float_t \\" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
					fpar << QuantumEfficiency << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar << "ERNeuRadPMTGain: Float_t \\ //[mV/p.e]" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
				fpar << Gain << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar << "ERNeuRadPMTSigma: Float_t \\" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
				fpar << Sigma << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar << "ERNeuRadPMTCrosstalks: Float_t \\" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
				//Углы модуля
				if (iPixel == 0 && jPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< Crosstalk << " ";
					fpar << 0. << " " << Crosstalk << " "<< DiagCrosstalk << " ";
					continue;
				}
				if (iPixel == 0 && jPixel == pixelsInRow-1){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << Crosstalk << " " << 0. << " "<< 0. << " ";
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == 0){
					fpar << 0. << " " << Crosstalk << " "<< Crosstalk << " ";
					fpar << 0. << " " << 0. << " "<< Crosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == pixelsInRow-1){
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< 0. << " ";
					fpar << Crosstalk << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				//стороны
				if (iPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << Crosstalk << " " << 0. << " "<< Crosstalk << " ";
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< DiagCrosstalk << " ";
					continue;
				}
				if (jPixel == 0){
					fpar << 0. << " " << Crosstalk << " "<< DiagCrosstalk  << " ";
					fpar << 0. << " " << 0. << " "<< Crosstalk << " ";
					fpar << 0. << " " << Crosstalk << " "<< DiagCrosstalk << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1){
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< DiagCrosstalk << " ";
					fpar << Crosstalk << " " << 0. << " "<< Crosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (jPixel == pixelsInRow-1){
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< 0. << " ";
					fpar << Crosstalk << " " << 0. << " "<< 0. << " ";
					fpar << DiagCrosstalk << " " << Crosstalk << " "<< 0. << " ";
					continue;
				}
				fpar << DiagCrosstalk << " " << Crosstalk << " "<< DiagCrosstalk << " ";
				fpar << Crosstalk << " " << 0. << " "<< Crosstalk << " ";
				fpar << DiagCrosstalk << " " << Crosstalk << " "<< DiagCrosstalk << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar.close();
}