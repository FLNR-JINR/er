#include "ERBeamDetCalEvent.h"

ERBeamDetCalEvent::ERBeamDetCalEvent(){

}

ERBeamDetCalEvent::~ERBeamDetCalEvent(){

}

void ERBeamDetCalEvent::Reset(){

	tF3l=0.;tF3r=0.;tF4r=0.;tF4l=0.;
	aF3l=0.;aF3r=0.;aF4r=0.;aF4l=0.;
}

ClassImp(ERBeamDetCalEvent);