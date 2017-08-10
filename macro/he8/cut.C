{
    TString inFilename1="out.root";

    TFile* f2 = new TFile(inFilename1);
    cout << "File: " << f2->GetName() << endl;

    TTree* t = (TTree*)f2->Get("cbmsim");
    cout << "Tree: " << t->GetName() << endl;
    t->SetMarkerStyle(22);
 t->SetMarkerSize(1);

   TCutG *cutg = new TCutG("CUTG",8);
   cutg->SetVarY("TelescopeRecoEvent.dep21");
   cutg->SetVarX("TelescopeRecoEvent.dep2sum");

   cutg->SetPoint(0,87.9733,29.008);
   cutg->SetPoint(1,142.623,19.0977);
   cutg->SetPoint(2,160.635,15.9571);
   cutg->SetPoint(3,160.209,20.7029);
   cutg->SetPoint(4,87.6267,35.4985);
   cutg->SetPoint(5,87.8578,29.0778);
   cutg->SetPoint(6,87.8578,29.0778);
   cutg->SetPoint(7,87.9733,29.008);

   TCanvas* canvasmis = new TCanvas("mis tests"); 

   canvasmis->Divide(2,2);

   canvasmis->cd(1);  
  
   t->Draw("TelescopeRecoEvent.mis11n.Excitation","BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l>400 && BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l<1200 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r>45 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r<54 && BeamDetCalEvent.tF4l > 50 && BeamDetCalEvent.tF4l < 100 && BeamDetCalEvent.tF4r > 50 && BeamDetCalEvent.tF4r < 100 && CUTG && RTelescopeCalEvent.mC12==0 && TelescopeRecoEvent.dep11>2.0 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.15 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12 > -0.35 ");
   //&& CUTG  && TelescopeRecoEvent.dep11>2 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.1 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12 > -0.3
   
   canvasmis->cd(2);   t->Draw("BeamDetRecoEvent.tofb","BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l>400 && BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l<1200 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r>45 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r<54 && BeamDetCalEvent.tF4l > 50 && BeamDetCalEvent.tF4l < 100 && BeamDetCalEvent.tF4r > 50 && BeamDetCalEvent.tF4r < 100 && CUTG && RTelescopeCalEvent.mC12==0 && TelescopeRecoEvent.dep11>2.0 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.15 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12 > -0.35 ");
   canvasmis->cd(3);   t->Draw("BeamDetRecoEvent.Tb","BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l>400 && BeamDetCalEvent.aF3r+BeamDetCalEvent.aF3l<1200 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r>45 && BeamDetCalEvent.tF4r-BeamDetCalEvent.tF3r<54 && BeamDetCalEvent.tF4l > 50 && BeamDetCalEvent.tF4l < 100 && BeamDetCalEvent.tF4r > 50 && BeamDetCalEvent.tF4r < 100 && CUTG && RTelescopeCalEvent.mC12==0 && TelescopeRecoEvent.dep11>2.0 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12<0.15 && TelescopeRecoEvent.dep11-TelescopeRecoEvent.dep12 > -0.35 ");  
   
}
