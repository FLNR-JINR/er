#if !defined(__CLING__)

#include "TCutG.h"

#endif

TCutG *cutF3,*cutF5;

void create_Cut_F3() { 
   cutF3 = new TCutG("F3_Beam",16);
   cutF3->SetVarX("0.0625*0.25*(Beam_detector_tF5.fData.fValue[0] + Beam_detector_tF5.fData.fValue[1] + Beam_detector_tF5.fData.fValue[2] + Beam_detector_tF5.fData.fValue[3] - Beam_detector_tF3.fData.fValue[0] - Beam_detector_tF3.fData.fValue[1] - Beam_detector_tF3.fData.fValue[2] - Beam_detector_tF3.fData.fValue[3])");
   cutF3->SetVarY("(Beam_detector_F3.fData.fValue[0] + Beam_detector_F3.fData.fValue[1] + Beam_detector_F3.fData.fValue[2] + Beam_detector_F3.fData.fValue[3])/4.");
   cutF3->SetTitle("F3_ToF_title");
   cutF3->SetFillColor(1);
   cutF3->SetPoint(0,103.375,907.843);
   cutF3->SetPoint(1,103.888,944.995);
   cutF3->SetPoint(2,108.089,988.338);
   cutF3->SetPoint(3,115.328,1006.91);
   cutF3->SetPoint(4,115.787,962.539);
   cutF3->SetPoint(5,116.193,837.668);
   cutF3->SetPoint(6,115.99,658.101);
   cutF3->SetPoint(7,115.45,604.438);
   cutF3->SetPoint(8,111.452,585.862);
   cutF3->SetPoint(9,107.211,584.83);
   cutF3->SetPoint(10,105.145,579.67);
   cutF3->SetPoint(11,103.672,582.766);
   cutF3->SetPoint(12,103.186,682.869);
   cutF3->SetPoint(13,103.173,764.396);
   cutF3->SetPoint(14,103.2,845.924);
   cutF3->SetPoint(15,103.375,907.843);
   // cutF3->Draw("");
}

void create_Cut_F5() { 
   cout << "create_Cut_F5 RUN " << endl;
   cutF5 = new TCutG("F5_Beam",17);
   cutF5->SetVarX("0.0625*0.25*(Beam_detector_tF5.fData.fValue[0] + Beam_detector_tF5.fData.fValue[1] + Beam_detector_tF5.fData.fValue[2] + Beam_detector_tF5.fData.fValue[3] - Beam_detector_tF3.fData.fValue[0] - Beam_detector_tF3.fData.fValue[1] - Beam_detector_tF3.fData.fValue[2] - Beam_detector_tF3.fData.fValue[3])");
   cutF5->SetVarY("(Beam_detector_F5.fData.fValue[0] + Beam_detector_F5.fData.fValue[1] + Beam_detector_F5.fData.fValue[2] + Beam_detector_F5.fData.fValue[3])/4.");
   cutF5->SetTitle("F5_Beam_title");
   cutF5->SetFillColor(1);
   cutF5->SetPoint(0,103.348,819.092);
   cutF5->SetPoint(1,103.524,923.323);
   cutF5->SetPoint(2,106.063,971.827);
   cutF5->SetPoint(3,109.602,998.658);
   cutF5->SetPoint(4,112.64,1007.95);
   cutF5->SetPoint(5,115.423,990.402);
   cutF5->SetPoint(6,116.125,875.851);
   cutF5->SetPoint(7,116.112,681.837);
   cutF5->SetPoint(8,115.747,634.365);
   cutF5->SetPoint(9,112.073,601.342);
   cutF5->SetPoint(10,108.953,596.182);
   cutF5->SetPoint(11,106.198,585.862);
   cutF5->SetPoint(12,104.348,583.798);
   cutF5->SetPoint(13,103.416,595.15);
   cutF5->SetPoint(14,103.186,669.453);
   cutF5->SetPoint(15,103.132,739.628);
   cutF5->SetPoint(16,103.348,819.092);
   // cutF5->Draw("");
}

void beamCut() {
   cout << "beamCut run" << endl;
   create_Cut_F3();
   create_Cut_F5();
}
