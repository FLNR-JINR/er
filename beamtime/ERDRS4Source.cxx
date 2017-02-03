
#include "ERDRS4Source.h"

#include "RawEvent.h"

#include <iostream>
using namespace std;

ERDRS4Source::ERDRS4Source(TString path):
fPath(path)
{
}
ERDRS4Source::ERDRS4Source(const ERDRS4Source& source){

}

ERDRS4Source::~ERDRS4Source(){

}

Bool_t ERDRS4Source::Init(){
	cerr << "Init" << endl;
	fFile = fopen(fPath.Data(), "r");
	if (fFile == NULL) {
		printf("Cannot find file \'%s\'\n", fPath.Data());
		return kFALSE;
	}
	
	return kTRUE;
}

Int_t ERDRS4Source::ReadEvent(UInt_t id){
	cerr << "ReadEvent" << endl;
	FILE* f = fFile;
	const char* filename = fPath.Data();
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

	int ndt;
	double threshold, sumdt, sumdt2;
	RawEvent* event[4];

	TString bName;
	// read file header
	fread(&fh, sizeof(fh), 1, f);
	if (fh.tag[0] != 'D' || fh.tag[1] != 'R' || fh.tag[2] != 'S') {
		printf("Found invalid file header in file \'%s\', aborting.\n", filename);
		return 0;
	}

	if (fh.version != '2') {
		printf("Found invalid file version \'%c\' in file \'%s\', should be \'2\', aborting.\n", fh.version, filename);
		return 0;
	}

	// read time header
	fread(&th, sizeof(th), 1, f);
	if (memcmp(th.time_header, "TIME", 4) != 0) {
		printf("Invalid time header in file \'%s\', aborting.\n", filename);
		return 0;
	}

	for (b = 0 ; ; b++) {
		// read board header
		fread(&bh, sizeof(bh), 1, f);
		if (memcmp(bh.bn, "B#", 2) != 0) {
			// probably event header found
			fseek(f, -4, SEEK_CUR);
			break;
		}

		printf("Found data for board #%d\n", bh.board_serial_number);

		// read time bin widths
		memset(bin_width[b], sizeof(bin_width[0]), 0);
		for (chn=0 ; chn<5 ; chn++) {
			fread(&ch, sizeof(ch), 1, f);
			if (ch.c[0] != 'C') {
				// event header found
				fseek(f, -4, SEEK_CUR);
				break;
			}
			i = ch.cn[2] - '0' - 1;
			printf("Found timing calibration for channel #%d\n", i+1);
			fread(&bin_width[b][i][0], sizeof(float), 1024, f);
			/*my printf
		printf("bin width %d \n", bin_width[b][i][10]); */
			// fix for 2048 bin mode: double channel
			if (bin_width[b][i][1023] > 10 || bin_width[b][i][1023] < 0.01) {
				for (j=0 ; j<512 ; j++)
					bin_width[b][i][j+512] = bin_width[b][i][j];
				/*my printf
		printf("bin width %d \n", bin_width[b][i][j+512]); */
			}
		}
	}
	n_boards = b;

	// initialize statistics
	ndt = 0;
	sumdt = sumdt2 = 0;

	// loop over all events in the data file
	for (n=0 ; ; n++) {

		//reset Events
		for (Int_t noChannel = 0; noChannel < 4; noChannel++) { event[noChannel]->Reset(); }
		// read event header
		i = (int)fread(&eh, sizeof(eh), 1, f);
		if (i < 1)
			break;

		if ( !(eh.event_serial_number%100) ) {
			printf("Found event #%d\n", eh.event_serial_number);
		}

		// loop over all boards in data file
		for (b=0 ; b<n_boards ; b++) {

			// read board header
			fread(&bh, sizeof(bh), 1, f);
			if (memcmp(bh.bn, "B#", 2) != 0) {
				printf("Invalid board header in file \'%s\', aborting.\n", filename);
				return 0;
			}

			// read trigger cell
			fread(&tch, sizeof(tch), 1, f);
			if (memcmp(tch.tc, "T#", 2) != 0) {
				printf("Invalid trigger cell header in file \'%s\', aborting.\n", filename);
				return 0;
			}

			if (n_boards > 1)
				printf("Found data for board #%d\n", bh.board_serial_number);

			// reach channel data
			for (chn=0 ; chn<4 ; chn++) {

				// read channel header
				fread(&ch, sizeof(ch), 1, f);
				if (ch.c[0] != 'C') {
					// event header found
					fseek(f, -4, SEEK_CUR);
					break;
				}
				chn_index = ch.cn[2] - '0' - 1;
				//	printf("print channel %d \n",chn_index);
				fread(&scaler, sizeof(int), 1, f);
				fread(voltage, sizeof(short), 1024, f);

				for (i=0 ; i<1024 ; i++) {
					// convert data to volts
					waveform[b][chn_index][i] = (voltage[i] / 65536. + eh.range/1000.0 - 0.5); //calculation of amplitudes values for each cell

					//for ROOT
					//ncell = i;
//					if(chn_index == 0) {
//						event[0]->SetAmp(waveform[b][chn_index][i], i);
//					}
					event[chn_index]->SetAmp(waveform[b][chn_index][i], i);
					//  if(chn_index == 1) {amp_ch2[i] = waveform[b][chn_index][i];}

					// calculate time for this cell
					for (j=0,time[b][chn_index][i]=0 ; j<i ; j++){
						time[b][chn_index][i] += bin_width[b][chn_index][(j+tch.trigger_cell) % 1024];
					}
				}
			} // end of the channel loop (chn)

			// align cell #0 of all channels
			t1 = time[b][0][(1024-tch.trigger_cell) % 1024];
//			event[0]->SetTime(time[b][chn][i],i);
			//my print;
			// printf("t1 %1.6lf \n",time[b][0][(1024-tch.trigger_cell) % 1024]);
			for (chn=1 ; chn<4 ; chn++) {
				t2 = time[b][chn][(1024-tch.trigger_cell) % 1024];
				//adding channels 3 and 4
				t3 = time[b][chn][(1024-tch.trigger_cell) % 1024];
				t4 = time[b][chn][(1024-tch.trigger_cell) % 1024];
				//my prinf
				//printf("t2 %f for %d %d %d \n",time[b][chn][(1024-tch.trigger_cell) % 1024], b, chn, i);
				dt = t1 - t2;
				dt34 = t3 - t4;
				for (i=0 ; i<1024 ; i++) {
					time[b][chn][i] += dt;  //each element of time gets dt correction
					//my print;
					// printf("time %1.6lf for %d %d %d \n",time[b][chn][i], b, chn, i);
//					event[chn]->SetTime(time[b][chn][i],i);
				}

			}

			t1 = t2 = t3 = t4 = 0;
			threshold = -0.045; //my threshold, used to be 0.3


			//for ROOT
			for(i=0 ; i<1024 ; i++) {
				for (Int_t chNum = 0; chNum < 4; chNum++) {	event[chNum]->SetTime(time[b][chNum][i],i); }
			}
			//event->Print();
			// find peak in channel 1 above threshold
			for (i=0 ; i<1022 ; i++) {

				if (waveform[b][0][i] < threshold && waveform[b][0][i+1] >= threshold) {
					t1 = (threshold-waveform[b][0][i])/(waveform[b][0][i+1]-waveform[b][0][i])*(time[b][0][i+1]-time[b][0][i])+time[b][0][i];
					//my prinf
					//printf("t1 recalc %1.6lf %d \n",t1, i);
					break;
				}

			}

			// find peak in channel 2 above threshold
			for (i=0 ; i<1022 ; i++) {
				if (waveform[b][1][i] < threshold && waveform[b][1][i+1] >= threshold) {
					t2 = (threshold-waveform[b][1][i])/(waveform[b][1][i+1]-waveform[b][1][i])*(time[b][1][i+1]-time[b][1][i])+time[b][1][i];
					//my prinf
					//printf("t2 recalc %1.6lf \n",t2);
					break;
				}
			}

			// calculate distance of peaks with statistics
			if (t1 > 0 && t2 > 0) {
				ndt++;
				dt = t2 - t1;
				sumdt += dt;
				sumdt2 += dt*dt;
			}
		} //end of the boards loop
	} // end of the events loop

	// print statistics
	return 1;
}

void ERDRS4Source::Close(){
	cerr << "Close"<< endl;
}

void ERDRS4Source::Reset(){
	cerr << "Reset" << endl;
}