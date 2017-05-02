void NeuRad_params(){
	/* wbt
	Int_t pixels = 64;
	Int_t modules = 1;
	Float_t QuantumEfficiency = 0.2;
	Float_t Gain = 5.;
	Float_t Sigma = 5.;
	Float_t PMTCrosstalk = 0.005;
	Float_t PMTDiagCrosstalk = 0.001;
	Float_t FiberCrosstalk = 0.005;
	Float_t FiberDiagCrosstalk = 0.001;


	TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
	NeuRadDetDigiFile += "/parameters/NeuRad.digi.v3.par";

    */

    /* oscill */
    Int_t pixels = 256;
	Int_t modules = 1;
	Float_t QuantumEfficiency = 0.2;
	Float_t Gain = 5.;
	Float_t Sigma = 5.;
	Float_t PMTCrosstalk = 0.005;
	Float_t PMTDiagCrosstalk = 0.001;
	Float_t FiberCrosstalk = 0.005;
	Float_t FiberDiagCrosstalk = 0.001;

	TString NeuRadDetDigiFile = gSystem->Getenv("VMCWORKDIR");
	NeuRadDetDigiFile += "/parameters/NeuRad.digi.v4.par";

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
					fpar << 0. << " " << 0. << " "<< PMTCrosstalk << " ";
					fpar << 0. << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == 0 && jPixel == pixelsInRow-1){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << PMTCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == 0){
					fpar << 0. << " " << PMTCrosstalk << " "<< PMTCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< PMTCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1 && jPixel == pixelsInRow-1){
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< 0. << " ";
					fpar << PMTCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				//стороны
				if (iPixel == 0){
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					fpar << PMTCrosstalk << " " << 0. << " "<< PMTCrosstalk << " ";
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
					continue;
				}
				if (jPixel == 0){
					fpar << 0. << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk  << " ";
					fpar << 0. << " " << 0. << " "<< PMTCrosstalk << " ";
					fpar << 0. << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
					continue;
				}
				if (iPixel == pixelsInRow-1){
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
					fpar << PMTCrosstalk << " " << 0. << " "<< PMTCrosstalk << " ";
					fpar << 0. << " " << 0. << " "<< 0. << " ";
					continue;
				}
				if (jPixel == pixelsInRow-1){
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< 0. << " ";
					fpar << PMTCrosstalk << " " << 0. << " "<< 0. << " ";
					fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< 0. << " ";
					continue;
				}
				fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
				fpar << PMTCrosstalk << " " << 0. << " "<< PMTCrosstalk << " ";
				fpar << PMTDiagCrosstalk << " " << PMTCrosstalk << " "<< PMTDiagCrosstalk << " ";
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