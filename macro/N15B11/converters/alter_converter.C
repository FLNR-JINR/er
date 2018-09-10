#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "Riostream.h"
#include <TBenchmark.h>

unsigned short swapbw(unsigned short data)
{
    unsigned char c0,*c;
    unsigned short *us;

    c = (unsigned char *)&data;
    c0 = *c;
    *c = *(c+1);
    *(c+1) = c0;
    us = (unsigned short *)c;

    return (*us);
}

struct data_t
{
    Double_t    m16;
    Double_t    m15;
    Double_t    m24;
    Double_t    t104gas;
    Double_t    t104si;
    Double_t    t108gas;
    Double_t    t108si;
    Double_t    t109gas;
    Double_t    t109si;
    Double_t    t110gas;
    Double_t    t110si;
    Double_t    beamint;
    Double_t    cyctime;
    UInt_t      evType;
};
data_t* data = new data_t;

TFile *hfile=0;
TTree *tree=0;

int read(TString fname)
{
    cerr << "Read begin\n";
    ifstream in;
    int idet[13]; // local var

    int h,i,j,x,y;
    unsigned char inbuf[0x10000];
    unsigned char *buf; // begin of buffer
    unsigned char *bufe; // end of buffer
    unsigned short *ev, evlen;

    union {unsigned short d; unsigned char id[2];} idu;

    int count=0, nbuf=0;
    in.open(fname, ios::in|ios::binary);
    if(!in) {cerr<<"Open error\n"; cerr<<"fname="<<fname<<endl;}
    else {cerr<<"Open OK\n";}

    j = 0;
    int while_counter = 0;
    while(1)
    {
        cerr << "while(" << while_counter++ << ")\n";
        buf = &inbuf[0];
        memset(buf, 0, 24);
        in.read((char *) &inbuf[0], (int)0x10000);
        if(in)
        {
            nbuf++;
            cerr << "buffer number: " << nbuf << endl;
        }
        else
        {
            j = 3;
            cerr << "j = 3, in file reads error\n";
            break;
        }

        if( strncmp((const char *)buf,"EBYEDATA",8) != 0 )
        {
            cerr << "j = 1, first 8 bayts is not EBYEDATA\n";
            j=1;
            break;
        }

        buf = &inbuf[0];
        bufe = buf + 0x10000;
        buf += 24;
        while(buf < bufe)
        {
            ev = (unsigned short *)buf;
            if(*ev == 0xffff)
            {
                ev++;
                evlen = *ev;
                ev++;
            }
            else
            {
                cerr << "j = 1, bayts number: " << ev << endl;
                j=1;
                break;
            }

            evlen = swapbw(evlen);
            if(evlen == 0)
            {
                cerr << "j = 0, evlen = 0\n";
                j = 0;
                break;
            }

            if( (buf+evlen) > bufe )
            {
                printf("?");
                cerr << "j = 1, buf+evlen > bufe\n";
                j = 1;
                break;
            }

            if( (*ev==0xF81F)&&(*(ev+2)==0xF82F) && (evlen>0xC) )
            {
                    unsigned short n, d, ch;
                    unsigned short *ip;
                    int i, nch,tmpid;

                    n = (evlen - 0xc)/4; ip = ev + 4; // n-number of inputs

                    tmpid = nch = 0;
                    for(i=0; i < n; i++)
                    {
                        idu.d = *ip++;
                        ch = 0x1fff&swapbw(*ip++);
                        d = 4*(idu.id[1] - 0xb) + idu.id[0] - 1;
                        if((idu.id[1]&0xff) != tmpid)
                        {
                            tmpid=idu.id[1]&0xff;
                            nch++;
                        }

                        if((idu.id[1]==0x0c)&&(idu.id[0]==2)) data->m16=ch; //idet[0]=ch; m16
                        if((idu.id[1]==0x0d)&&(idu.id[0]==2)) data->m15=ch; //idet[1]=ch; m15
                        if((idu.id[1]==0x2f)&&(idu.id[0]==2)) data->m24=ch; //idet[2]=ch; m24
                        if((idu.id[1]==0x11)&&(idu.id[0]==4)) data->t104gas=ch; //idet[3]=ch; t104gas
                        if((idu.id[1]==0x11)&&(idu.id[0]==2)) data->t104si=ch; //idet[4]=ch; t104si
                        if((idu.id[1]==0x12)&&(idu.id[0]==4)) data->t108gas=ch; //idet[5]=ch; t108gas
                        if((idu.id[1]==0x12)&&(idu.id[0]==2)) data->t108si=ch; //idet[6]=ch; t108si
                        if((idu.id[1]==0x17)&&(idu.id[0]==4)) data->t109gas=ch; //idet[7]=ch; t109gas
                        if((idu.id[1]==0x17)&&(idu.id[0]==2)) data->t109si=ch; //idet[8]=ch; t109si
                        if((idu.id[1]==0x18)&&(idu.id[0]==4)) data->t110gas=ch; //idet[9]=ch; t110gas
                        if((idu.id[1]==0x18)&&(idu.id[0]==2)) data->t110si=ch; //idet[10]=ch; t110si
                        if((idu.id[1]==0x1d)&&(idu.id[0]==2)) data->beamint=ch; //idet[11]=ch; beamint
                        if((idu.id[1]==0x2f)&&(idu.id[0]==2)) data->cyctime=ch; //idet[12]=ch; cyctime
                    }

                    data->evType=nch; // Event type
                    tree->Fill();
                }

                count++;
                buf += evlen;
            }

        if(j)
        {
            cerr << "j = -j\n";
            printf("nb: %d count %d\n", nbuf, count);
            printf("%p %p\n", buf, bufe);
            return -j;
        }
    }

    printf("nb: %d count %d\n", nbuf, count);
    return 0;
}

void alter_converter(TString inFile = "kz43_0", TString inputDir = "files_to_convert", TString outDir = "output")
{
    TString ifilename;
    ifilename.Form("%s/%s", inputDir.Data(), inFile.Data());
    TString ofilename;
    ofilename.Form("%s/data_tree.root", outDir.Data());
    hfile = new TFile(ofilename,"RECREATE");
    tree = new TTree("run", " ");

    tree->Branch("t104gas",&data->t104gas,"T104gas/D");
    tree->Branch("t104si",&data->t104si,"T104si/D");
    tree->Branch("t108gas",&data->t108gas,"T108gas/D");
    tree->Branch("t108si",&data->t108si,"t108si/D");
    tree->Branch("t109gas",&data->t109gas,"T109gas/D");
    tree->Branch("t109si",&data->t109si,"T109si/D");
    tree->Branch("t110gas",&data->t110gas,"T110gas/D");
    tree->Branch("t110si",&data->t110si,"T110si/D");

    gBenchmark->Start("beamdata");

    read(ifilename);

    gBenchmark->Show("beamdata");

    tree->Print();
    tree->Write();
    hfile->Close();
}


