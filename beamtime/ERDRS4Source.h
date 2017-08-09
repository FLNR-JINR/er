/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERDRS4Source_H
#define ERDRS4Source_H

#include <stdio.h>

#include "TString.h"

#include "FairSource.h"
#include "FairRootManager.h"

#include "ERNeuRadRawEvent.h"

typedef struct {
    char           tag[3];
    char           version;
} FHEADER;

typedef struct {
    char           time_header[4];
} THEADER;

typedef struct {
    char           bn[2];
    unsigned short board_serial_number;
} BHEADER;

typedef struct {
    char           event_header[4];
    unsigned int   event_serial_number;
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short millisecond;
    unsigned short range;   // range center in mV
} EHEADER;

typedef struct {
    char           tc[2];
    unsigned short trigger_cell;
} TCHEADER;

typedef struct {
    char           c[1];
    char           cn[3];
} CHEADER;

class ERDRS4Source : public FairSource
{
  public:
    ERDRS4Source();
    ERDRS4Source(const ERDRS4Source& source);
    virtual ~ERDRS4Source();

    virtual Bool_t Init();

    virtual Int_t ReadEvent(UInt_t=0);

    virtual void Close();

    virtual void Reset();

    virtual Source_Type GetSourceType(){return kFILE;}

    virtual void SetParUnpackers(){}

    virtual Bool_t InitUnpackers(){return kTRUE;}

    virtual Bool_t ReInitUnpackers(){return kTRUE;}

    void SetFile(TString path){fPath = path;}
    void SetNChanels(Int_t chanels){fNChanels = chanels;}
    void SetNPoints(Int_t points){fNPoints = points;}
  private:
    FILE* f;
    TString fPath;
    FHEADER  fh;
    THEADER  th;
    BHEADER  bh;
    EHEADER  eh;
    TCHEADER tch;
    CHEADER  ch;

    unsigned int scaler;
    unsigned short voltage[1024];
    double waveform[16][4][1024], time[16][4][1024];
    float bin_width[16][4][1024];
    int i, j, b, chn, n, chn_index, n_boards;
    double t1, t2, t3, t4, dt, dt34;
    char filename[256]; //for input binary file
    char outroot[256];  //for output root file

    int ndt;
    double threshold, sumdt, sumdt2;

    ERNeuRadRawEvent** fRawEvents;
    Int_t fNChanels;
    Int_t fNPoints;

  protected:
    Int_t ReadHeader();
  public:
    ClassDef(ERDRS4Source, 1)
};


#endif
