#include "ERN15B11Detector.h"

// ROOT
#include <TClonesArray.h>
#include <TGeoMatrix.h>
#include <TVirtualMC.h>
#include <TGeoManager.h>
#include <TRefArray.h>

// FairRoot
//#include <FairRun.h>
//#include <FairRuntimeDb.h>
#include <FairRootManager.h>
//#include <FairLogger.h>
#include <FairVolume.h>

// Project
#include "cppTokenizer.h"
#include "ERPoint.h"

/*static*/
struct stcNodeSig ERN15B11Detector::mCurNodeSig;

/*static*/
Int_t ERN15B11Detector::mCurNodeUID = -1;

ERN15B11Detector::ERN15B11Detector() :
	FairDetector()
{
	LOG(DEBUG) << "ERN15B11Detector::ERN15B11Detector()" << FairLogger::endl;
}

ERN15B11Detector::ERN15B11Detector(const char* Name, Bool_t Active, Int_t DetId) :
	FairDetector(Name, Active, DetId)
{
	LOG(DEBUG) << "ERN15B11Detector::ERN15B11Detector(" << Name << ")" << FairLogger::endl;
}

ERN15B11Detector::~ERN15B11Detector()
{
}

void ERN15B11Detector::ConstructGeometry()
{
	TString fileName = GetGeometryFileName();
	if(fileName.EndsWith(".root")) {
		LOG(INFO) << "Constructing geometry from ROOT file '" << fileName.Data() << "'" << FairLogger::endl;
		this->ConstructRootGeometry();
	}
	else if (fileName.EndsWith(".gdml")) {
		LOG(INFO) << "Constructing geometry from GDML file '" << fileName.Data() << "'"  << FairLogger::endl;
		TGeoRotation* zeroRotation = new TGeoRotation("zeroRotation", 0., 0., 0.);
		this->ConstructGDMLGeometry(new TGeoCombiTrans(0., 0., 0., zeroRotation));
	}
	else {
		LOG(FATAL) << "Unknown geometry file format." << FairLogger::endl;
	}
}

Bool_t ERN15B11Detector::CheckIfSensitive(std::string name)
{
	LOG(DEBUG) << "ERN15B11Detector::CheckIfSensitive(" << name << ")" << FairLogger::endl;

/*	FairVolume* v_vol;
	std::cout << svList->GetEntries() << std::endl;

	for (Int_t i=0; i<svList->GetEntries(); i++) {
		v_vol = (FairVolume*)svList->At(i);
		std::cout << v_vol->GetName() << std::endl;
	}
*/

	if (name == "vDetGasPart") {
		return kTRUE;
	}
	if (name == "vSemi") {
		return kTRUE;
	}

	return kFALSE;
}

Bool_t ERN15B11Detector::ProcessHits(FairVolume* v)
{
	const char* curNodePath = gMC->CurrentVolPath();
	Int_t curNodeId = this->GetCurNodeSig(curNodePath);
	// The mCurNodeSig and mCurNodeUID are filled by the previous GetCurNodeSig() call

	LOG(DEBUG2) << "ERN15B11Detector::ProcessHits(" << v->GetName() << ") "
	           << "particle charge: " << gMC->TrackCharge() << " "
	           << curNodePath << ": " << mCurNodeUID
	           << FairLogger::endl;


	std::map<Int_t, TClonesArray*>::iterator iter = mOutputPoints.find(mCurNodeUID);

	TClonesArray* v_outputCollection = NULL;

	if (iter != mOutputPoints.end()) {
		// Collection exists
		v_outputCollection = iter->second;
		LOG(DEBUG2) << "Collection found." << FairLogger::endl;

	} else {
		LOG(FATAL) << "Collection not found." << FairLogger::endl;
		return kFALSE;
	}

	fELoss += gMC->Edep();


	if (gMC->IsTrackEntering()) {
		StartNewPoint();
	}

	if (gMC->IsTrackExiting() || gMC->IsTrackStop() || gMC->IsTrackDisappeared()) {
		FinishNewPoint(v_outputCollection);
	}

	return kTRUE;
}

void ERN15B11Detector::EndOfEvent()
{
	LOG(DEBUG) << "ERN15B11Detector::EndOfEvent" << FairLogger::endl;
	this->Reset();
}

void ERN15B11Detector::Register()
{
	LOG(DEBUG) << "ERN15B11Detector::Register" << FairLogger::endl;

	FairRootManager* ioman = FairRootManager::Instance();
	if (!ioman) {
		LOG(FATAL) << "IO manager is not set" << FairLogger::endl;
	}

	TGeoIterator next(gGeoManager->GetTopVolume());

	TGeoNode *node;
	while ((node=next())) {
		TString curPath;
		next.GetPath(curPath);

		Bool_t curSig = this->GetCurNodeSig(curPath);
		if (curSig) {
			LOG(DEBUG) << curPath << ": " << mCurNodeUID << FairLogger::endl;


			std::map<Int_t, TClonesArray*>::iterator iter = mOutputPoints.find(mCurNodeUID);

			if (iter != mOutputPoints.end()) {
				// Collection exists
				TClonesArray* v_collection = iter->second;
				LOG(DEBUG) << "Collection found." << FairLogger::endl;

			} else {
				//////LOG(DEBUG) << "Collection not found." << FairLogger::endl;

				// Collection does not exist - create it
				TClonesArray* v_newCollection = new TClonesArray("ERPoint");
				mOutputPoints.insert(std::pair<Int_t, TClonesArray*>(mCurNodeUID, v_newCollection));

				TString v_name;

				if ((mCurNodeSig.iSemi == -1) && (mCurNodeSig.iDetGasPart != -1)) {
					// DetGasPart
					v_name.Form("Gas_Layer_%d_Det_%d", mCurNodeSig.iLayer, mCurNodeSig.iDetContainer);
				} else if ((mCurNodeSig.iSemi != -1) && (mCurNodeSig.iDetGasPart == -1)) {
					// Semi
					v_name.Form("Semi_Layer_%d_Det_%d", mCurNodeSig.iLayer, mCurNodeSig.iDetContainer);
				}
				ioman->Register(v_name + TString("_Point"), v_name, v_newCollection, kTRUE);

				LOG(DEBUG) << "Collection not found - create it: " << v_name << FairLogger::endl;
			}

		}
	}
}

TClonesArray* ERN15B11Detector::GetCollection(Int_t iColl) const
{
	LOG(DEBUG) << "ERN15B11Detector::GetCollection" << FairLogger::endl;

	/*if (fSenVolumes.size() > iColl) {
		auto it = fSenVolumes.begin();
		for (Int_t i = 0; i<iColl; i++) {
			it++;
		}
		return it->second;
	} else {
		return NULL;
	}*/

	return NULL;
}

void ERN15B11Detector::Reset()
{
	LOG(DEBUG) << "ERN15B11Detector::Reset" << FairLogger::endl;

	std::map<Int_t, TClonesArray*>::iterator iter;

	for (iter = mOutputPoints.begin(); iter != mOutputPoints.end(); ++iter) {
		TClonesArray* curCollection = iter->second;
		curCollection->Clear();
	}
}

Bool_t ERN15B11Detector::GetCurNodeSig(const char* nodePath) const
{
	//// Reset current node signature
	mCurNodeSig.iLayer = -1;
	mCurNodeSig.iDetContainer = -1;
	mCurNodeSig.iDetGasPart = -1;
	mCurNodeSig.iSemi = -1;

	std::vector<std::string> mVolumesList;

	//// Keep the underscore sing at the end of these strings
	mVolumesList.push_back(std::string("vOneLayer_")); // 0-th item
	mVolumesList.push_back(std::string("vDetContainer_")); // 1-st item
	mVolumesList.push_back(std::string("vDetGasPart_")); // 2-nd item
	mVolumesList.push_back(std::string("vSemi_")); // 3-d item

	std::string fullPath(nodePath);
	std::string delimiter("/");
	std::vector<std::string> tokens = tokenize(fullPath, delimiter);

	std::vector<std::string>::const_iterator iter;
	for (iter = tokens.begin(); iter != tokens.end(); ++iter) {

		////std::cout << (*iter) << ": " << (*iter).compare(0, 6, "vSemi_") << ": ";

		UChar_t iCounter = 0;
		std::vector<std::string>::const_iterator iter2;
		for (iter2 = mVolumesList.begin(); iter2 != mVolumesList.end(); ++iter2) {

			size_t curStrLen = (*iter).size();
			size_t curSearchedStrLen = (*iter2).size();

			//std::cout << (*iter) << " (" << (*iter2) << "): len=" << curStrLen
			//          << " (len=" << curSearchedStrLen << ")";

			if ((*iter).compare(0, curSearchedStrLen, (*iter2)) == 0) {
				Short_t res = std::stoi((*iter).substr(curSearchedStrLen));
				//std::cout << " index=" << res << std::endl;

				switch (iCounter) {
				case 0: mCurNodeSig.iLayer = res; break;
				case 1: mCurNodeSig.iDetContainer = res; break;
				case 2: mCurNodeSig.iDetGasPart = res; break;
				case 3: mCurNodeSig.iSemi = res; break;
				default: break;
				}
			} else {
				//std::cout << std::endl;
			}

			iCounter++;
		}

	}

	LOG(DEBUG3) << nodePath
	           << " SIGNATURE:"
	           << " iLayer=" << mCurNodeSig.iLayer
	           << " iDetContainer=" << mCurNodeSig.iDetContainer
	           << " iDetGasPart=" << mCurNodeSig.iDetGasPart
	           << " iSemi=" << mCurNodeSig.iSemi
	           << FairLogger::endl;

	if ((mCurNodeSig.iDetContainer != -1) && (mCurNodeSig.iLayer != -1))
	{
		if ((mCurNodeSig.iSemi == -1) && (mCurNodeSig.iDetGasPart != -1)) {
			// DetGasPart
			mCurNodeUID = mCurNodeSig.iLayer * 100 + mCurNodeSig.iDetContainer;
			////std::cout << "-------------------case 1" << std::endl;
			return kTRUE;
		} else if ((mCurNodeSig.iSemi != -1) && (mCurNodeSig.iDetGasPart == -1)) {
			// Semi
			mCurNodeUID = 10000 + mCurNodeSig.iLayer * 100 + mCurNodeSig.iDetContainer;
			////std::cout << "-------------------case 2" << std::endl;
			return kTRUE;
		} else {
			////std::cout << "-------------------case 3" << std::endl;
			return kFALSE;
		}
	}
	////std::cout << "-------------------case 4" << std::endl;
	return kFALSE;
}

void ERN15B11Detector::StartNewPoint(void)
{
	fELoss = 0.;
	fEventID = gMC->CurrentEvent();
	gMC->TrackPosition(fPosIn);
	gMC->TrackMomentum(fMomIn);
	fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
	fTimeIn = gMC->TrackTime() * 1.0e09; //current time of flight of the track being transported
	fTrackLength = gMC->TrackLength(); //length of the current track from its origin (in cm)
	fMot0TrackID = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
	fMass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
	Int_t curVolId, corOffVolId;
	curVolId = gMC->CurrentVolID(fVolNb);
	TGeoHMatrix matrix;
	gMC->GetTransformation(gMC->CurrentVolPath(), matrix);
	Double_t globalPos[3], localPos[3];
	fPosIn.Vect().GetXYZ(globalPos);
	matrix.MasterToLocal(globalPos, localPos);
	fPosInLocal.SetXYZ(localPos[0], localPos[1], localPos[2]);
}

void ERN15B11Detector::FinishNewPoint(TClonesArray* p_collection)
{
	gMC->TrackPosition(fPosOut);
	gMC->TrackMomentum(fMomOut);
	fTimeOut = gMC->TrackTime() * 1.0e09;

	if (fELoss > 0.) {
		AddPoint(p_collection);
	}
}

ERPoint* ERN15B11Detector::AddPoint(TClonesArray* p_collection)
{
	if (p_collection == NULL) {
		LOG(WARNING) << "Trying to add an output point into a NULL collection." << FairLogger::endl;
		return NULL;
	}

	TClonesArray& clref = *p_collection;
	Int_t size = clref.GetEntriesFast();
	return new(clref[size]) ERPoint(fEventID, fTrackID, fMot0TrackID, fVolNb,
		fMass, fPosIn.Vect(),fPosInLocal,fPosOut.Vect(),fMomIn.Vect(),fMomOut.Vect(),fTimeIn,
		fTimeOut, fTrackLength, fELoss, 0., gMC->TrackPid(), gMC->TrackCharge());
}

ClassImp(ERN15B11Detector)
