#include <iostream>
using namespace std;

#include "ERHe8EventHeader.h"

ERHe8EventHeader::ERHe8EventHeader(){

}

Bool_t ERHe8EventHeader::Register(TTree* tree, TString branchName){
	tree->SetMakeClass(1);
	tree->SetBranchAddress(branchName + TString(".nevent"),&this->HE8Event_nevent);
	tree->SetBranchAddress(branchName + TString(".trigger"),&HE8Event_trigger);
	tree->SetBranchAddress(branchName + TString(".subevents"),&HE8Event_subevents);
	tree->SetBranchAddress(branchName + TString(".evsize"),&HE8Event_evsize);
	tree->SetBranchAddress(branchName + TString(".time"),&HE8Event_time);

	ReadInputFile();

	return kTRUE;
}

void ERHe8EventHeader::ReadInputFile(){
  cout << "Filling header from input files" << endl;
  char Zeros[32];
  ReIN.Simulation = false;
  ReIN.Vertex = false;
  ReIN.DetectorTune = false;
  ReIN.TOFis = false;
  ReIN.TRACKINGis = false;
  ReIN.TrackCheck = false;
  ReIN.WriteRawData = false;
  ReIN.WriteCalData = false;
  ReIN.WriteTofData = false;
  ReIN.WriteTrackData = false;
  ReIN.WriteTelData = false;
  ReIN.WritePhysData = false;
  ReIN.WritePlayData = false;
  ReIN.WriteRawTrack = false;
  /******************** Readout ReactionInput.dat ************************/
  printf("************************************************************\n");
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/ReactionInput.dat");
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) {printf("ERHe8EventHeader: File ReactionInput.dat was not found\n");}
  else
  {
    fscanf(F1,"%s\n",ReIN.ReactionName);
    fscanf(F1,"%s %s\n",ReIN.AboutBeam,ReIN.EnergyUn);
    fscanf(F1,"%s %s\n",ReIN.AboutSlit,ReIN.SlitUn);
    fscanf(F1,"%s %s\n",Zeros,ReIN.Mechanism);
    fscanf(F1,"%s %s\n",Zeros,ReIN.Fname);
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.Simulation = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.Vertex = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.DetectorTune = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.TOFis = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.TRACKINGis = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.TrackCheck = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteRawData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteCalData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteTofData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteTrackData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteTelData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WritePhysData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WritePlayData = true;}
    fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteRawTrack = true;}

    for(int m=0;m<8;m++)
    {
      fscanf(F1,"%s %i\n",Zeros,&ReIN.ifill[m]);
    }
    printf("Main: File ReactionInput.dat has been read\n");
  }
  fclose(F1);

  /*********************** Readout MWPC parameters:***********************/
  if(ReIN.TRACKINGis)
  {
    filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/track.dat");
    F1 = fopen(filePath.Data(),"r");
    if(F1==NULL) {printf("Main: File track.dat was not found\n");}
    else  
    { 
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWwinMatter,&UpMat.MWwinThick);
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWgasMatter,&UpMat.MWgasThick);
      fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWcathMatter,&UpMat.MWcathThick);
      fscanf(F1,"%s %i\n",Zeros,&UpMat.MWNcathodes);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWXYdist);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWstep);
      fscanf(F1,"%s %i\n",Zeros,&UpMat.MWNwires);
      fscanf(F1,"%s\n",Zeros);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWfarDist);
      fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.MWfarXshift,&UpMat.MWfarYshift);
      fscanf(F1,"%s %i %i\n",Zeros,&UpMat.MWfarXNum,&UpMat.MWfarYNum);
      fscanf(F1,"%s\n",Zeros);
      fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWclosDist);
      fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.MWclosXshift,&UpMat.MWclosYshift);
      fscanf(F1,"%s %i %i\n",Zeros,&UpMat.MWclosXNum,&UpMat.MWclosYNum);
      printf("Main: File track.dat has been read\n");
    }
  fclose(F1);
  }
  UpMat.MWcathThick *= UpMat.MWNcathodes;
  /*********************** Readout TOF parameters:************************/
  if(ReIN.TOFis)
  {
  filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/tof.dat");
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) {printf("Main: File tof.dat was not found\n");}
  else
  { 
    fscanf(F1,"%s %lf %lf\n",UpMat.PlasticMatter1,&UpMat.PlasticAngle1,&UpMat.PlasticThick1);
    fscanf(F1,"%s %lf %lf\n",UpMat.PlasticMatter2,&UpMat.PlasticAngle2,&UpMat.PlasticThick2);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.PlasticDist);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TofRes);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF3l_rng,&UpMat.tF3r_rng);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF4l_rng,&UpMat.tF4r_rng);
    fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF3_dlt,&UpMat.tF4_dlt);
    printf("Main: File tof.dat has been read\n");
  }
  fclose(F1);
  UpMat.PlasticThick1/=cos(UpMat.PlasticAngle1*rad);
  UpMat.PlasticThick2/=cos(UpMat.PlasticAngle2*rad);
  }
  /********************* Readout Target parameters:***********************/
  filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/target.dat");
  F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) printf("Main: File target.dat was not found\n");
  else
  { 
    fscanf(F1,"%s %s\n",Zeros,UpMat.TarShape);
    fscanf(F1,"%s %lf %lf %lf\n",Zeros,&UpMat.TarXshift,&UpMat.TarYshift,&UpMat.TarZshift);
    fscanf(F1,"%s %s %lf\n",Zeros,UpMat.TarFoilMatter,&UpMat.FoilThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarRadius);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarHeight);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarWallThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarEntrHoleRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.MeniskSize);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarTemp);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarPress);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarAngle);
    fscanf(F1,"%s %s\n",Zeros,UpMat.HeatScreenAns);
    fscanf(F1,"%s %s %lf\n",Zeros,UpMat.HeatScreenMatter,&UpMat.HeatScreenThick);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.InHscrRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.HscrWallWidth);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.HscrHeight);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.EntrHRad);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.ExHX);
    fscanf(F1,"%s %lf\n",Zeros,&UpMat.ExHY);
    printf("Main: File target.dat has been read\n");
  }
  fclose(F1);
  if(UpMat.MeniskSize==0.) UpMat.MeniskSize=0.00001;
}

ClassImp(ERHe8EventHeader)