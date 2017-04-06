//===================================================================
//== TLiEvent.cxx:	exp1210
//== Version: 2010-11-12
//===================================================================

#include "TLiEvent.h"
#include "Riostream.h"

using namespace std;

TLiEvent::TLiEvent() 
	:TGo4EventElement()
{
	cout << "**** TLiEvent: Create instance" << endl;
} //-------------------------------------------------------------------

TLiEvent::TLiEvent(const char* name) 
	:TGo4EventElement(name)
{
	cout << "**** TLiEvent: Create instance " << name << endl;
} //-------------------------------------------------------------------

TLiEvent::~TLiEvent()
{
	cout << "**** TLiEvent: Delete instance " << endl;
} //-------------------------------------------------------------------

void  TLiEvent::Clear(Option_t *t)
{
   //==================================================================
   //== all members should be cleared.
   //== memset(fRawData,0, sizeof(fRawData));
   //==================================================================
   
   memset(SQ11, 0, sizeof(SQ11)); 
   memset(SQ12, 0, sizeof(SQ12));
   memset(SQ13, 0, sizeof(SQ13));
   memset(SQ21, 0, sizeof(SQ21));
   memset(SQ22, 0, sizeof(SQ22));
   memset(SQ23, 0, sizeof(SQ23));
   memset(ANR, 0, sizeof(ANR));
   memset(ANS, 0, sizeof(ANS));
   memset(ANC, 0, sizeof(ANC));
   memset(TSQ11, 0, sizeof(TSQ11)); 
   memset(TSQ12, 0, sizeof(TSQ12));
   memset(TSQ21, 0, sizeof(TSQ21));
   memset(TSQ22, 0, sizeof(TSQ22));
   memset(TANR, 0, sizeof(TANR));
   memset(TANS, 0, sizeof(TANS));
   memset(unusedQDC, 0, sizeof(unusedQDC));
   memset(unusedTDC, 0, sizeof(unusedTDC));
   memset(SCR, 0, sizeof(SCR));
	memset(NeuTDC,0,sizeof(NeuTDC));
	memset(Amp_TDC,0,sizeof(Amp_TDC));

qdcF3L =0;
qdcF3R=0;
qdcF4L=0;
qdcF4R=0;
qdcF4U=0;
qdcF5L=0;
qdcF5R=0;
qdcF5U=0;
qdcF5D=0;
tdcF3L=0;
tdcF3R=0;
tdcF4L=0;
tdcF4R=0;
tdcF4U=0;
tdcF5L=0;
tdcF5R=0;
tdcF5U=0;
tdcF5D=0;
mainTRIGGER=0;
beamTRIGGER=0;
annTRIGGER=0;
   memset(x1,   0, sizeof(x1));
   memset(y1,   0, sizeof(y1));
   memset(x2,   0, sizeof(x2));
   memset(y2,   0, sizeof(y2));
   memset(adc1, 0, sizeof(adc1));
   memset(adc2, 0, sizeof(adc2));
   memset(adc3, 0, sizeof(adc3));
   memset(adc4, 0, sizeof(adc4));
//   memset(tdc1, 0, sizeof(tdc1));
//   memset(tdc2, 0, sizeof(tdc2));
	time = 0;
	nevent = 0;
	trigger = 0;
	subevents = 0;
	evsize = 0;	
} //-------------------------------------------------------------------
