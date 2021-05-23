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
   cutF3->SetPoint(0,66.5525,1349.65);
   cutF3->SetPoint(1,66.5931,1550.89);
   cutF3->SetPoint(2,66.9172,1709.56);
   cutF3->SetPoint(3,68.2341,1829.53);
   cutF3->SetPoint(4,69.6049,1848.88);
   cutF3->SetPoint(5,70.1992,1852.75);
   cutF3->SetPoint(6,70.5436,1798.57);
   cutF3->SetPoint(7,70.753,1696.01);
   cutF3->SetPoint(8,70.888,1593.46);
   cutF3->SetPoint(9,70.9623,1454.14);
   cutF3->SetPoint(10,70.9623,1305.15);
   cutF3->SetPoint(11,70.861,1206.46);
   cutF3->SetPoint(12,70.5369,1111.65);
   cutF3->SetPoint(13,69.3483,1049.73);
   cutF3->SetPoint(14,68.2138,1026.51);
   cutF3->SetPoint(15,67.3359,1041.99);
   cutF3->SetPoint(16,66.7889,1109.71);
   cutF3->SetPoint(17,66.5863,1301.28);
   cutF3->SetPoint(18,66.5525,1349.65);
   // cutF3->Draw("");
}

void create_Cut_F5() { 
   cout << "create_Cut_F5 RUN " << endl;
   cutF5 = new TCutG("F5_Beam",17);
   cutF5->SetVarX("0.0625*0.25*(Beam_detector_tF5.fData.fValue[0] + Beam_detector_tF5.fData.fValue[1] + Beam_detector_tF5.fData.fValue[2] + Beam_detector_tF5.fData.fValue[3] - Beam_detector_tF3.fData.fValue[0] - Beam_detector_tF3.fData.fValue[1] - Beam_detector_tF3.fData.fValue[2] - Beam_detector_tF3.fData.fValue[3])");
   cutF5->SetVarY("(Beam_detector_F5.fData.fValue[0] + Beam_detector_F5.fData.fValue[1] + Beam_detector_F5.fData.fValue[2] + Beam_detector_F5.fData.fValue[3])/4.");
   cutF5->SetTitle("F5_Beam_title");
   cutF5->SetFillColor(1);
   cutF5->SetPoint(0,66.5998,1392.22);
   cutF5->SetPoint(1,66.5998,1597.33);
   cutF5->SetPoint(2,66.9105,1736.65);
   cutF5->SetPoint(3,67.4372,1833.4);
   cutF5->SetPoint(4,68.6122,1901.12);
   cutF5->SetPoint(5,69.4766,1922.41);
   cutF5->SetPoint(6,70.2127,1885.64);
   cutF5->SetPoint(7,70.6044,1804.37);
   cutF5->SetPoint(8,70.8745,1632.16);
   cutF5->SetPoint(9,70.9286,1458.01);
   cutF5->SetPoint(10,70.8475,1297.41);
   cutF5->SetPoint(11,70.3343,1163.89);
   cutF5->SetPoint(12,69.2673,1123.26);
   cutF5->SetPoint(13,68.4907,1082.62);
   cutF5->SetPoint(14,67.8019,1082.62);
   cutF5->SetPoint(15,67.1873,1096.17);
   cutF5->SetPoint(16,66.8227,1225.81);
   cutF5->SetPoint(17,66.6201,1324.5);
   cutF5->SetPoint(18,66.5998,1392.22);
   // cutF5->Draw("");
}

void be10Cut() {
   cout << "beamCut run" << endl;
   create_Cut_F3();
   create_Cut_F5();
}
