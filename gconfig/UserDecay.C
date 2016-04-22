void UserDecayConfig() {
   cerr << "Loading User Decay Config from macro"<< endl;
   FairRunSim* run = FairRunSim::Instance();
   
  /* FairParticle* FirstIon = new FairParticle("FirstIon", 8, 26, 26+8, 8, kFALSE, 5.);
   run->AddNewParticle(FirstIon);

   FairParticle* SecondIon = new FairParticle("SecondIon", 8, 24, 24+8, 8, kFALSE, 5.);
   run->AddNewParticle(SecondIon);
 */
   TDatabasePDG *db= TDatabasePDG::Instance();
   TParticlePDG *p=0;

   p = TDatabasePDG::Instance()->GetParticle("FirstIon");

   cerr << p->PdgCode() << endl;

   Int_t mode[6][3];                  
   Float_t bratio[6];
   
   for (Int_t kz = 0; kz < 6; kz++) {
      bratio[kz] = 0.;
      mode[kz][0] = 0;
      mode[kz][1] = 0;
      mode[kz][2] = 0;
    //  cout << mode[kz][0] << " " << 	mode[kz][1] << " " << mode[kz][2] << endl;
   }
   bratio[0] = 100.;
   mode[0][0] =2212  ;
   mode[0][1] =2212  ;
   mode[0][2] =10080240;
   
   gMC->SetDecayMode(10080260,bratio,mode);
}