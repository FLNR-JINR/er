#include "ERGeoCave.h"

#include "FairGeoMedia.h"

#include "FairGeoNode.h"
#include "FairGeoShapes.h"
#include "FairGeoBasicShape.h"

ERGeoCave::ERGeoCave()
	: FairGeoSet(),
	name("cave")
{
	fName="cave";
	maxModules=1;
}

// Read geometry from the file
Bool_t ERGeoCave::read(std::fstream& fin, FairGeoMedia* media)
{
	if (!media) return kFALSE;
	const Int_t maxbuf=256;
	char buf[maxbuf];
	FairGeoNode* volu=0;
	FairGeoMedium* medium;
	Bool_t rc=kTRUE;
	do {
		fin.getline(buf,maxbuf);
		if (buf[0]!='\0' && buf[0]!='/' && !fin.eof()) {
			if (strcmp(buf,name)==0) {
				volu=new FairGeoNode;
				volu->SetName(buf);
				volu->setVolumeType(kFairGeoTopNode);
				volu->setActive();
				fin.getline(buf,maxbuf);
				TString shape(buf);
				FairGeoBasicShape* sh=pShapes->selectShape(shape);
				if (sh) volu->setShape(sh);
				else rc=kFALSE;
				fin.getline(buf,maxbuf);
				medium=media->getMedium(buf);
				if (!medium) {
					medium=new FairGeoMedium();
					media->addMedium(medium);
				}
				volu->setMedium(medium);
				Int_t n=0;
				if (sh) n=sh->readPoints(&fin,volu);
				if (n<=0) rc=kFALSE;
			} else rc=kFALSE;
		}
	} while (rc && !volu && !fin.eof()); 

	if (volu && rc) {
		volumes->Add(volu);
		masterNodes->Add(new FairGeoNode(*volu));
	} else {
		delete volu;
		volu=0;
		rc=kFALSE;
	}
	return rc;
}

// Add the reference node
void ERGeoCave::addRefNodes()
{
	FairGeoNode* volu = getVolume(name);
	if (volu) masterNodes->Add(new FairGeoNode(*volu));
}

// Write the geometry to file
void ERGeoCave::write(std::fstream& fout)
{
	fout.setf(std::ios::fixed, std::ios::floatfield);
	FairGeoNode* volu = getVolume(name);
	if (volu) {
		FairGeoBasicShape* sh = volu->getShapePointer();
		FairGeoMedium* med = volu->getMedium();
		if (sh && med) {
			fout << volu->GetName() << std::endl;
			fout << sh->GetName() << std::endl;
			fout << med->GetName() << std::endl;
			sh->writePoints(&fout, volu);
		}
	}
}

// Print the geometry
void ERGeoCave::print()
{
	FairGeoNode* volu = getVolume(name);
	if (volu) {
		FairGeoBasicShape* sh = volu->getShapePointer();
		FairGeoMedium* med = volu->getMedium();
		if (sh && med) {
			std::cout << volu->GetName() << std::endl;
			std::cout << sh->GetName() << std::endl;
			std::cout << med->GetName() << std::endl;
			sh->printPoints(volu);
		}
	}
}

ClassImp(ERGeoCave)
