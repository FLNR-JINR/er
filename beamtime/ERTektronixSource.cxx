
#include "ERTektronixSource.h"

#include "FairRootManager.h"

#include <iostream>
using namespace std;

ERTektronixSource::ERTektronixSource():
fNPoints(1000),
fNProcessedFiles(0),
fNForProcFiles(0)
{
	fCh1 = new RawEvent(fNPoints);
	fCh2 = new RawEvent(fNPoints);
	fCh3 = new RawEvent(fNPoints);
	fCh4 = new RawEvent(fNPoints);
}

ERTektronixSource::ERTektronixSource(const ERTektronixSource& source){
}

ERTektronixSource::~ERTektronixSource(){

}

Bool_t ERTektronixSource::Init(){
	//input files opening
	for (std::map<int,vector<TString> >::iterator it=fPaths.begin(); it!=fPaths.end(); ++it){
		Int_t chanel = it->first;
		vector<TString> chanelFiles = it->second;
		for (Int_t iFile = 0; iFile < chanelFiles.size(); iFile++ ){
			fFiles[chanel].push_back(new ifstream());
			fFiles[chanel][iFile]->open(chanelFiles[iFile]);
			if (!fFiles[chanel][iFile]->is_open())
				cerr << "Couldn`t open file " << chanelFiles[iFile] << endl;
			else
				fNForProcFiles++;
		}
		fCurrentFiles[chanel] = 0;
	}

	//register data in output file
	FairRootManager* ioman = FairRootManager::Instance();
  	ioman->Register("ch1.", "Tektronix", fCh1, kTRUE);
  	ioman->Register("ch2.", "Tektronix", fCh2, kTRUE);
  	ioman->Register("ch3.", "Tektronix", fCh3, kTRUE);
  	ioman->Register("ch4.", "Tektronix", fCh4, kTRUE);
	return kTRUE;
}

Int_t ERTektronixSource::ReadEvent(UInt_t id){
	//Проверяем есть ли еще файлы для обработки
	if (fNForProcFiles == fNProcessedFiles)
		return 1;
	
	Double_t data;
	for (std::map<int,vector<ifstream*> >::iterator it=fFiles.begin(); it!=fFiles.end(); ++it){
		Int_t chanel = it->first;
		vector<ifstream*> chanelFiles= it->second;
		Int_t nPointsInEvent = 0;
		for (Int_t iPoint = 0; iPoint < fNPoints; iPoint++){
			ifstream* stream = chanelFiles[fCurrentFiles[chanel]];
			if (stream->eof()){
				stream->close();
				fCurrentFiles[chanel]++;
				cout << "File " << fCurrentFiles[chanel]-1 << " for " <<  chanel << " chanel finished" <<endl;
				fNProcessedFiles++;
				break;
			}
			(*stream) >> data;
			if (chanel== 1)
				fCh1->SetAmp(data,iPoint);
			if (chanel == 2)
				fCh2->SetAmp(data,iPoint);
			if (chanel == 3)
				fCh3->SetAmp(data,iPoint);
			if (chanel == 4)
				fCh4->SetAmp(data,iPoint);
		}
	}
	return 0;
}

void ERTektronixSource::Close(){
}

void ERTektronixSource::Reset(){
	fCh1->Reset();
	fCh2->Reset();
	fCh3->Reset();
	fCh4->Reset();
}

void ERTektronixSource::AddFile(Int_t chanel, TString path){
	fPaths[chanel].push_back(path);
	cout << "Input file " << path << " added to " << chanel << "chanel" << endl;
}