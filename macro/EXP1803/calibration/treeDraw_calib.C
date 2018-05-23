void treeDraw_calib() {
	TString inFilename1="sim_digidigi.root";

	TFile* f2 = new TFile(inFilename1);
	cout << "File: " << f2->GetName() << endl;

	TTree* t = (TTree*)f2->Get("er");
	cout << "Tree: " << t->GetName() << endl;

	TString condition;
	condition = //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fStripNb == 15 && ")+
              //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_1_X.fStripNb == 15 && ")+
              //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_2_X.fStripNb == 15 && ")+
              //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_3_X.fStripNb == 15 && ")+
              //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_4_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_5_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_6_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_7_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_8_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_9_X.fStripNb == 15 && ")+
              TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fStripNb == 15");
  TString variable;
										 // "ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep"
  variable = //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_0_X.fEdep + ")+
             //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_1_X.fEdep + ")+
             //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_2_X.fEdep + ")+
             //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_3_X.fEdep + ")+
             //TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_4_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_5_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_6_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_7_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_8_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_dead_XY_9_X.fEdep + ")+
             TString("ERQTelescopeSiDigi_SiDet_DoubleSi_SD2_calib_XY_10_X.fEdep");
	t->StartViewer();
	t->Draw( variable.Data(),
           condition.Data(), "", 10000, 0);
	
	// TString inFilename1="sim_digi.root";

	// TFile* f2 = new TFile(inFilename1);
	// cout << "File: " << f2->GetName() << endl;

	// TTree* t = (TTree*)f2->Get("er");
	// cout << "Tree: " << t->GetName() << endl;
	// t->StartViewer();
	// t->Draw("ERQTelescopeSiDigi_CalibSi1_SingleSi_calib_X_0.fEdep","","", 10000, 0);
	 
}
