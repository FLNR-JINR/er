  

#ifndef ERSupport_H
#define ERSupport_H

#define TempNorm 293.16 /* Normal temperature (K) */
#define AMU 931.494028 /* atomic mass unit (MeV) */

double EiEo(double tableER[][105],double Tp,double Rp);
int ReadRint(char* Fname,double Ranges[][105]);
int intrp4(double* x,double* y, double* c);

#endif
