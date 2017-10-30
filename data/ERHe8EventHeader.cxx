#include <iostream>
using namespace std;

#include "ERHe8EventHeader.h"

ERHe8EventHeader::ERHe8EventHeader(){

}
//-----------------------------------------------------------------------------
Bool_t ERHe8EventHeader::Register(TTree* tree, TString branchName){
	tree->SetMakeClass(1);
	tree->SetBranchAddress(branchName + TString(".nevent"),&this->HE8Event_nevent);
	tree->SetBranchAddress(branchName + TString(".trigger"),&HE8Event_trigger);
	tree->SetBranchAddress(branchName + TString(".subevents"),&HE8Event_subevents);
	tree->SetBranchAddress(branchName + TString(".evsize"),&HE8Event_evsize);
	tree->SetBranchAddress(branchName + TString(".time"),&HE8Event_time);

	ReadInputFile();
  ReactionPreparation();
  
	return kTRUE;
}
//-----------------------------------------------------------------------------
void ERHe8EventHeader::ReadInputFile(){
  cout << "Filling header from input files" << endl;
  char Zeros[32];
  ReIN.Simulation = false;
  ReIN.Vertex = false;
  ReIN.DetectorTune = false;
  ReIN.ToFis = false;
  ReIN.TRACKINGis = false;
  ReIN.TrackCheck = false;
  ReIN.WriteRawData = false;
  ReIN.WriteCalData = false;
  ReIN.WriteToFData = false;
  ReIN.WriteTrackData = false;
  ReIN.WriteTelData = false;
  ReIN.WritePhysData = false;
  ReIN.WritePlayData = false;
  ReIN.WriteRawTrack = false;
  /******************** Readout ReactionInput.dat ************************/
  printf("************************************************************\n");
  TString filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/") + fReactionInputFile;
  FILE *F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) {
    printf("ERHe8EventHeader: File ReactionInput.dat was not found\n");
  } else {
    int res;
    res = fscanf(F1,"%s\n",ReIN.ReactionName);
    res = fscanf(F1,"%s %s\n",ReIN.AboutBeam,ReIN.EnergyUn);
    res = fscanf(F1,"%s %s\n",ReIN.AboutSlit,ReIN.SlitUn);
    res = fscanf(F1,"%s %s\n",Zeros,ReIN.Mechanism);
    res = fscanf(F1,"%s %s\n",Zeros,ReIN.Fname);
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.Simulation = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.Vertex = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.DetectorTune = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.ToFis = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.TRACKINGis = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.TrackCheck = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteRawData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteCalData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteToFData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteTrackData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteTelData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WritePhysData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WritePlayData = true;}
    res = fscanf(F1,"%s %s\n",Zeros,Zeros);
    if(!strcmp(Zeros,"yes")) {ReIN.WriteRawTrack = true;}

    for(int m=0;m<8;m++)
    {
      int res2;
      res2 = fscanf(F1,"%s %i\n",Zeros,&ReIN.ifill[m]);
    }
    printf("Main: File ReactionInput.dat has been read\n");
  }
  fclose(F1);

  /*********************** Readout MWPC parameters:***********************/
  if(ReIN.TRACKINGis)
  {
    filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/track.dat");
    FILE *F2 = fopen(filePath.Data(),"r");
    if(F2==NULL)
    {
      printf("Main: File track.dat was not found\n");
    } else {
      int res;
      res = fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWwinMatter,&UpMat.MWwinThick);
      res = fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWgasMatter,&UpMat.MWgasThick);
      res = fscanf(F1,"%s %s %lf\n",Zeros,UpMat.MWcathMatter,&UpMat.MWcathThick);
      res = fscanf(F1,"%s %i\n",Zeros,&UpMat.MWNcathodes);
      res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWXYdist);
      res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWstep);
      res = fscanf(F1,"%s %i\n",Zeros,&UpMat.MWNwires);
      res = fscanf(F1,"%s\n",Zeros);
      res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWfarDist);
      res = fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.MWfarXshift,&UpMat.MWfarYshift);
      res = fscanf(F1,"%s %i %i\n",Zeros,&UpMat.MWfarXNum,&UpMat.MWfarYNum);
      res = fscanf(F1,"%s\n",Zeros);
      res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.MWclosDist);
      res = fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.MWclosXshift,&UpMat.MWclosYshift);
      res = fscanf(F1,"%s %i %i\n",Zeros,&UpMat.MWclosXNum,&UpMat.MWclosYNum);
      printf("Main: File track.dat has been read\n");
    }
  fclose(F2);
  }
  UpMat.MWcathThick *= UpMat.MWNcathodes;
  /*********************** Readout ToF parameters:************************/
  if(ReIN.ToFis)
  {
    filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/tof.dat");
    FILE *F3 = fopen(filePath.Data(),"r");
    if(F3==NULL) {
      printf("Main: File tof.dat was not found\n");
    }
  else
  {
    int res;
    res = fscanf(F1,"%s %lf %lf\n",UpMat.PlasticMatter1,&UpMat.PlasticAngle1,&UpMat.PlasticThick1);
    res = fscanf(F1,"%s %lf %lf\n",UpMat.PlasticMatter2,&UpMat.PlasticAngle2,&UpMat.PlasticThick2);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.PlasticDist);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.ToFRes);
    res = fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF3l_rng,&UpMat.tF3r_rng);
    res = fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF4l_rng,&UpMat.tF4r_rng);
    res = fscanf(F1,"%s %lf %lf\n",Zeros,&UpMat.tF3_dlt,&UpMat.tF4_dlt);
    printf("Main: File tof.dat has been read\n");
  }
  fclose(F3);
  UpMat.PlasticThick1/=cos(UpMat.PlasticAngle1*rad);
  UpMat.PlasticThick2/=cos(UpMat.PlasticAngle2*rad);
  }
  /********************* Readout Target parameters:***********************/
  filePath = gSystem->Getenv("VMCWORKDIR") + TString("/input/target.dat");
  F1 = fopen(filePath.Data(),"r");
  if(F1==NULL) printf("Main: File target.dat was not found\n");
  else
  {
    int res;
    res = fscanf(F1,"%s %s\n",Zeros,UpMat.TarShape);
    res = fscanf(F1,"%s %lf %lf %lf\n",Zeros,&UpMat.TarXshift,&UpMat.TarYshift,&UpMat.TarZshift);
    res = fscanf(F1,"%s %s %lf\n",Zeros,UpMat.TarFoilMatter,&UpMat.FoilThick);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarRadius);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarHeight);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarWallThick);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarEntrHoleRad);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.MeniskSize);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarTemp);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarPress);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.TarAngle);
    res = fscanf(F1,"%s %s\n",Zeros,UpMat.HeatScreenAns);
    res = fscanf(F1,"%s %s %lf\n",Zeros,UpMat.HeatScreenMatter,&UpMat.HeatScreenThick);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.InHscrRad);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.HscrWallWidth);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.HscrHeight);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.EntrHRad);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.ExHX);
    res = fscanf(F1,"%s %lf\n",Zeros,&UpMat.ExHY);
    printf("Main: File target.dat has been read\n");
  }
  fclose(F1);
  if(UpMat.MeniskSize==0.) UpMat.MeniskSize=0.00001;
}
//-----------------------------------------------------------------------------
void ERHe8EventHeader::ReactionPreparation(){
  cout << " Separate Input and Output channels " << endl;
  char ReaNa[32];
  char InputChannel[32];
  char OutputChannel[32];
  char OutputChannelTemp[32];
  char Resonance[32];
  char ResonanceTemp[32];
  strcpy(ReaNa,ReIN.ReactionName);
  char* plett;
  plett = strchr(ReIN.ReactionName,'-');
  plett+=2;
  strcpy(OutputChannelTemp,plett);
  strcpy(ResonanceTemp,plett);
  plett = strtok(ReIN.ReactionName,"-");
  strcpy(InputChannel,plett);

  cout << "Define if there is any resonance in the Output channel" << endl;
  int NofPartRes = 0;
  plett = strchr(OutputChannelTemp,'[');
  if(plett!=NULL)
  {
    plett = strtok(OutputChannelTemp,"[");
    strcpy(OutputChannel,plett);
    plett = strtok(NULL,"]");
    strcat(OutputChannel,plett);
    plett = strchr(ResonanceTemp,']');plett++;
    if(plett!=NULL) strcat(OutputChannel,plett);
    plett = strtok(ResonanceTemp,"[");
    plett = strtok(NULL,"]");
    strcpy(ResonanceTemp,plett);
    plett = strtok(ResonanceTemp,"[");
    strcpy(ResonanceTemp,plett);
    plett = strchr(ResonanceTemp,')');
    if(plett!=NULL)
    {
      plett = strtok(ResonanceTemp,")");
      strcpy(Resonance,plett);
      plett = strtok(NULL,")");
      strcat(Resonance,plett);
    }
    else strcpy(Resonance,ResonanceTemp);
    NofPartRes = HowMuchParticles(Resonance);
    printf("Resonance is %s\n",Resonance);
  }
  else strcpy(OutputChannel,OutputChannelTemp);
  
  cout << "Separate Detected and Unobserved particles in the Output channel" << endl;
  char zero[]="";
  NofUnObsPart = 0;
  strcpy(DetectedPart,OutputChannel);
  plett = strtok(DetectedPart,"()");
  strcpy(DetectedPart,plett);
  plett = strchr(OutputChannel,')');
  plett++;
  if(strcmp(plett,zero)) 
  {
    plett++;strcpy(UnObservedPart,plett);
    NofUnObsPart = HowMuchParticles(UnObservedPart);
  }
  
  cout << "How much Input and Detected particles" << endl;
  NofInPart = HowMuchParticles(InputChannel);
  if(NofInPart<2||NofInPart>2) {printf("Wrong number of particles in the Input channel\n");}
  NofDetPart = HowMuchParticles(DetectedPart);
  if(NofDetPart==0) {printf("Wrong number of detected particles\n");}

  cout  << "Define particles in the input channel" << endl;

  plett = strtok(InputChannel,"+");
  strcpy(projname,plett);
  plett = strtok(NULL,"+");
  strcpy(tarname,plett);
}
//-----------------------------------------------------------------------------
int ERHe8EventHeader::HowMuchParticles(char* str){
  char xname[32];
  char* ptr;
  strcpy(xname,str);
  int N = 0;
  do
  {
    ptr = strchr(xname,'+');
    if(ptr!=NULL)
    {N++;ptr++;strcpy(xname,ptr);}
  } while(ptr!=NULL);
  N++;
  return N;
}

ClassImp(ERHe8EventHeader)