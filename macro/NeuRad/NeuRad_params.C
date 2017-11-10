void NeuRad_params(){
	// wbt 
	Int_t pixels = 64;
	Int_t modules = 1;
	Float_t QuantumEfficiency = 0.2;
	Float_t Gain = 5.;
	Float_t Sigma = 5.;
	Float_t PixelCrosstalk = 0.005;
	Float_t PixelDiagCrosstalk = 0.001;
	Float_t FiberCrosstalk = 0.005;
	Float_t FiberDiagCrosstalk = 0.001;


	TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
	NeuRadDetDigiFile += "/parameters/NeuRad.digi.v3.par";
	
    /**/

    /* oscill 
    Int_t pixels = 256;
	Int_t modules = 1;
	Float_t QuantumEfficiency = 0.2;
	Float_t Gain = 5.;
	Float_t Sigma = 5.;
	Float_t PixelCrosstalk = 0.005;
	Float_t PixelDiagCrosstalk = 0.001;
	Float_t FiberCrosstalk = 0.005;
	Float_t FiberDiagCrosstalk = 0.001;

	TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
	NeuRadDetDigiFile += "/parameters/NeuRad.digi.v4.par";*/
	/**/
	Int_t pixelsInRow = TMath::Sqrt(pixels);
	ofstream fpar;
	fpar.open(NeuRadDetDigiFile.Data());
	fpar << "//-------------------------------------------------------------" << endl;
	fpar << "//----------- Digitisation parameter for NeuRad    ------------" << endl;
	fpar << "//-------------------------------------------------------------" << endl;
	fpar << "[ERNeuRadDigiPar]" << endl;
	fpar << "//-------------------------------------------------------------" << endl;

	fpar << "ERNeuRadPixelQuantumEfficiency: Float_t \\" << endl;
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
	fpar << "ERNeuRadPixelGain: Float_t \\ //[mV/p.e]" << endl;
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
	fpar << "ERNeuRadPixelSigma: Float_t \\" << endl;
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
	fpar << "ERNeuRadPixelCrosstalks: Float_t \\" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
				//Углы модуля
				if (iPixel == 0 && jPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< PixelCrosstalk << " ";
					fpar << 0. << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == 0 && jPixel == pixelsInRow-1){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << PixelCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == 0){
					fpar << 0. << " " << PixelCrosstalk << " "<< PixelCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< PixelCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == pixelsInRow-1){
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< 0. << " ";
					fpar << PixelCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				//стороны
				if (iPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << PixelCrosstalk << " " << 0. << " "<< PixelCrosstalk << " ";
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
					continue;
				}
				if (jPixel == 0){
					fpar << 0. << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk  << " ";
					fpar << 0. << " " << 0. << " "<< PixelCrosstalk << " ";
					fpar << 0. << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1){
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
					fpar << PixelCrosstalk << " " << 0. << " "<< PixelCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (jPixel == pixelsInRow-1){
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< 0. << " ";
					fpar << PixelCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< 0. << " ";
					continue;
				}
				fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
				fpar << PixelCrosstalk << " " << 0. << " "<< PixelCrosstalk << " ";
				fpar << PixelDiagCrosstalk << " " << PixelCrosstalk << " "<< PixelDiagCrosstalk << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar << "ERNeuRadFiberCrosstalks: Float_t \\" << endl;
	for (Int_t iModule = 0; iModule < modules; iModule++){
		for(Int_t iPixel = 0; iPixel < pixelsInRow; iPixel++){
			for(Int_t jPixel = 0; jPixel < pixelsInRow; jPixel++){
				//Углы модуля
				if (iPixel == 0 && jPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< FiberCrosstalk << " ";
					fpar << 0. << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == 0 && jPixel == pixelsInRow-1){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << FiberCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == 0){
					fpar << 0. << " " << FiberCrosstalk << " "<< FiberCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< FiberCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == pixelsInRow-1){
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< 0. << " ";
					fpar << FiberCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				//стороны
				if (iPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << FiberCrosstalk << " " << 0. << " "<< FiberCrosstalk << " ";
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
					continue;
				}
				if (jPixel == 0){
					fpar << 0. << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk  << " ";
					fpar << 0. << " " << 0. << " "<< FiberCrosstalk << " ";
					fpar << 0. << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1){
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
					fpar << FiberCrosstalk << " " << 0. << " "<< FiberCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (jPixel == pixelsInRow-1){
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< 0. << " ";
					fpar << FiberCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< 0. << " ";
					continue;
				}
				fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
				fpar << FiberCrosstalk << " " << 0. << " "<< FiberCrosstalk << " ";
				fpar << FiberDiagCrosstalk << " " << FiberCrosstalk << " "<< FiberDiagCrosstalk << " ";
			}
			if (iPixel == pixelsInRow-1)
				fpar << endl;
			else
				fpar << " \\" <<endl;
		}
	}
	fpar.close();
}
