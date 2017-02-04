
#include "ERTektronixSource.h"

#include "FairRootManager.h"

#include <iostream>
using namespace std;

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
	//Проверяем есть ли еще файлы для обработки
	if (fNForProcFiles == fNProcessedFiles)
		return 1;
	
	Double_t amplitude;
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
			(*stream) >> amplitude;
			fRawEvents[chanel-1]->SetAmp(amplitude,iPoint);
			Double_t time = 5e-04+iPoint*1e-01;
			fRawEvents[chanel-1]->SetTime(time,iPoint);
		}
	}
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