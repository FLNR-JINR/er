
#include "ERTektronixSource.h"

#include "FairRootManager.h"
#include "FairRun.h"

#include <iostream>
using namespace std;

Int_t ERTektronixSource::fEvent = 0;

ERTektronixSource::ERTektronixSource():
fNPoints(1000),
fNProcessedFiles(0),
fNForProcFiles(0),
fNChanels(4),
fRawEvents(NULL)
{
}

ERTektronixSource::ERTektronixSource(const ERTektronixSource& source){
}

ERTektronixSource::~ERTektronixSource(){

}

Bool_t ERTektronixSource::Init(){
	//input files opening
	ifstream* f = new ifstream();
	for (std::map<int,vector<TString> >::iterator it=fPaths.begin(); it!=fPaths.end(); ++it){
		Int_t chanel = it->first;
		vector<TString> chanelFiles = it->second;
		for (Int_t iFile = 0; iFile < chanelFiles.size(); iFile++ ){
			f->open(chanelFiles[iFile]);
			if (!f->is_open()){
				cerr << "Couldn`t open file " << chanelFiles[iFile] << endl;
				return kFALSE;
			}
			else{
				f->close();
			}
		}
	}
	delete f;
	
	Int_t eventNb  = fPaths.begin()->second.size();
	for (std::map<int,vector<TString> >::iterator it=fPaths.begin(); it!=fPaths.end(); ++it){
		if (it->second.size() != eventNb){
			cerr << "Differen event count in chanels" << endl;
			return kFALSE;
		}
	}
	
	//Register new objects in output file
	fRawEvents = new RawEvent*[fNChanels];
	FairRootManager* ioman = FairRootManager::Instance();
	for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
		fRawEvents[iChanel] = new RawEvent(fNPoints);
		TString bName;
		bName.Form("ch%d.",iChanel+1);
		ioman->Register(bName, "Tektronix", fRawEvents[iChanel], kTRUE);
	}
	
	return kTRUE;
}

Int_t ERTektronixSource::ReadEvent(UInt_t id){
	Reset();
	//Проверяем есть ли еще события для обработки
	if (fPaths.size()+1 == fEvent)
		return 1;
	if (fEvent == 0){
		fEvent ++;
		return 0;
	}

	ifstream* f = new ifstream();
	Double_t amplitude;
	for (std::map<int,vector<TString> >::iterator it=fPaths.begin(); it!=fPaths.end(); ++it){
		Int_t chanel = it->first;
		vector<TString> chanelFiles= it->second;
		f->open(chanelFiles[fEvent-1]);
		for (Int_t iPoint = 0; iPoint < fNPoints; iPoint++){
			if (f->eof()){
				f->close();
				cout << "Not enough points in file " << chanelFiles[fEvent-1] << " for " <<  chanel << " chanel!" <<endl;
				break;
			}
			(*f) >> amplitude;
			fRawEvents[chanel-1]->SetAmp(amplitude,iPoint);
			Double_t time = 5e-04+iPoint*1e-01;
			fRawEvents[chanel-1]->SetTime(time,iPoint);
		}
		cout << "File " << chanelFiles[fEvent-1] << " for " <<  chanel << " chanel finished" <<endl;
	}
	delete f;
	fEvent ++;
	return 0;
}

void ERTektronixSource::Close(){
}

void ERTektronixSource::Reset(){
	for (Int_t iChanel = 0; iChanel < fNChanels; iChanel++){
		fRawEvents[iChanel]->Reset();
	}
}

void ERTektronixSource::AddFile(Int_t chanel, TString path){
	fPaths[chanel].push_back(path);
	cout << "Input file " << path << " added to " << chanel << "chanel" << endl;
}