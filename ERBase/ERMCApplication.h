// -------------------------------------------------------------------------
// -----                      ERMCApplication header file              -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERMCAPPLICATION_H
#define ERMCAPPLICATION_H

#include "FairMCApplication.h"
#include "ERDecayer.h"

class ERMCApplication : public FairMCApplication{


public:
	ERMCApplication(const char* name, const char* title, TObjArray* ModList, const char* MatName);
    /** default constructor
    */
    ERMCApplication();
    /** default destructor
    */
    virtual ~ERMCApplication();
    /** Singelton instance
    */
    static ERMCApplication* Instance();
    
    /** Define action at each step, dispatch the action to the corresponding detectors */
    virtual void          Stepping();                                       // MC Application

    void SetDecayer(ERDecayer* decayer){fDecayer = decayer;}
  private:
  	ERDecayer* fDecayer;
  	
    ClassDef(ERMCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline ERMCApplication* ERMCApplication::Instance()
{ return (ERMCApplication*)(TVirtualMCApplication::Instance());}

#endif
