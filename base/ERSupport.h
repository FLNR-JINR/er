  

#ifndef ERSupport_H
#define ERSupport_H

#include <map>

#include "TMatrixD.h"
#include "TString.h"

#include "FairLogger.h"

enum ERDataObjectType {Point, Digi, Track, Particle};
inline TString ERDataObjectTypeStr(ERDataObjectType object) {
    switch(object) {
        case Point : return "Point";
        case Digi  : return "Digi";
        case Track : return "Track";
        case Particle : return "Particle";
        default : LOG(FATAL) << "Unknown ERDataObject type\n";
    }
    return "";
}
enum ChannelSide {First, Second, None};
enum SensetiveType {Si, CsI, LaBr};
enum OrientationAroundZ {X, Y, Default};
inline TString SensetiveTypeStr(SensetiveType type) {
  switch(type) {
    case Si : return "Si";
    case CsI : return "CsI";
    case LaBr : return "LaBr";
    default : LOG(FATAL) << "Unknown telescope sensetive type\n"; 
  }
  return "";
}
inline TString ChannelSideStr(ChannelSide side) {
  switch(side) {
    case First : return "1";
    case Second : return "2";
    case None : LOG(FATAL) 
        << "String representation for None channel side is not available\n"; 
    default : LOG(FATAL) << "Unknown telescope channel side\n"; 
  }
  return "";
}
inline TString OrientationAroundZStr(OrientationAroundZ type) {
  switch(type) {
    case X : return "X";
    case Y : return "Y";
    case Default : LOG(FATAL) 
        << "String representation for Default orientation around Z is not available\n"; 
    default : LOG(FATAL) << "Unknown telescope orientation around z\n"; 
  }
  return "";
}

#define TempNorm 293.16 /* Normal temperature (K) */
#define AMU 931.494028 /* atomic mass unit (MeV) */

TMatrixD* ReadCalFile(TString fileName);
Int_t GetChannelNumber(const Int_t rawChannel, const std::map<Int_t, Int_t>* channelsMapping);
double EiEo(double tableER[][105],double Tp,double Rp);
int ReadRint(char* Fname,double Ranges[][105]);
int intrp4(double* x,double* y, double* c);

#endif
