// -------------------------------------------------------------------------
// -----                        ERHe8EventHeader header file                   -----
// -----                     Created 04/17  by V.Schetinin             -----
// -------------------------------------------------------------------------

#ifndef ERHe8EventHeader_H
#define ERHe8EventHeader_H

#include "TTree.h"
#include "TSystem.h"
#include "TString.h"

#include "FairEventHeader.h"

#define rad 0.01745329252 /* pi/180 */


class ReactionDataInput 
{
public:
  ReactionDataInput(){};
  ~ReactionDataInput(){}
  char ReactionName[32];
  char Mechanism[32];
  char AboutBeam[32];
  char AboutSlit[32];
  char EnergyUn[32];
  char SlitUn[32];
  char Fname[32];
  bool Simulation;
  bool Vertex;
  bool DetectorTune;
  bool TOFis;
  bool TRACKINGis;
  bool TrackCheck;
  bool WriteRawData;
  bool WriteCalData;
  bool WriteTofData;
  bool WriteTrackData;
  bool WriteTelData;
  bool WritePhysData;
  bool WritePlayData;
  bool WriteRawTrack;
  int ifill[8];
};

class UpstreamMatter 
{
public:
  UpstreamMatter(){};
  ~UpstreamMatter(){}
  char PlasticMatter1[16];
  char PlasticMatter2[16];
  char MWwinMatter[16];
  char MWcathMatter[16];
  char MWgasMatter[16];
  char HeatScreenAns[16];
  char HeatScreenMatter[16];
  char TarShape[16];
  char TarFoilMatter[16];
  int MWclosXNum;
  int MWclosYNum;
  double MWXYdist;
  double MWstep;
  int MWNcathodes;
  int MWNwires;
  double MWfarDist;
  double MWfarXshift;
  double MWfarYshift;
  int MWfarXNum;
  int MWfarYNum;
  double MWclosDist;
  double MWclosXshift;
  double MWclosYshift;
  double PlasticThick1;
  double PlasticThick2;
  double MWwinThick;
  double MWcathThick;
  double MWgasThick;
  double PlasticAngle1;
  double PlasticAngle2;
  double PlasticDist;
  double TofRes;
  double tF3l_rng;
  double tF3r_rng;
  double tF4l_rng;
  double tF4r_rng;
  double tF3_dlt;
  double tF4_dlt;
  double beam_TOF[10][105];
  double beam_MWwin[10][105];
  double beam_MWgas[10][105];
  double beam_MWcathod[10][105];
  double beam_TARwin[10][105];
  double beam_target[10][105];
  double beam_heatscreen[10][105];
  double TarRadius;
  double TarXshift;
  double TarYshift;
  double TarZshift;
  double TarFoilThick;
  double TarThick;
  double TarHeight;
  double TarWallThick;
  double TarEntrHoleThick;
  double TarTemp;
  double TarPress;
  double TarAngle;
  double FoilThick;
  double TarEntrHoleRad;
  double MeniskSize;
  double HeatScreenThick;
  double InHscrRad;
  double HscrWallWidth;
  double HscrHeight;
  double EntrHRad;
  double ExHX;
  double ExHY;
};


class ERHe8EventHeader : public FairEventHeader {
public:
	Int_t HE8Event_nevent;
	Int_t HE8Event_trigger;
	Int_t HE8Event_subevents;
	Int_t HE8Event_evsize;
	UInt_t HE8Event_time;
	int mbeam;
	int mtrack;
	int mp1,mp2;
public:
	ERHe8EventHeader();
	Bool_t Register(TTree* tree, TString branchName);
public:
	ReactionDataInput ReIN;		//!
	UpstreamMatter UpMat;		//!
	int NofUnObsPart; 			//!
	char projname[5];			//!
  	char tarname[5];  			//!
  	char DetectedPart[32];		//!
  	char UnObservedPart[32];	//!
  	int NofDetPart;				//!
    int NofInPart;				//!
protected:
	void ReadInputFile();
	void ReactionPreparation();
	int HowMuchParticles(char* str);
	ClassDef(ERHe8EventHeader, 1)

};

#endif