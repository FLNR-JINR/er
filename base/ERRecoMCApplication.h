// -------------------------------------------------------------------------
// -----                      ERRecoMCApplication header file              -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRecoMCApplication_H
#define ERRecoMCApplication_H

#include "FairMCApplication.h"

class ERRecoMCApplication : public FairMCApplication{

public:
	
	ERRecoMCApplication(const char* name, const char* title, TObjArray* ModList, const char* MatName);
    /** default constructor
    */
    ERRecoMCApplication();
    /** default destructor
    */
    virtual ~ERRecoMCApplication();

    ERRecoMCApplication* Instance();

    //Заглушка. FairMCApplication::AddParticles дергает FairRunSim.
    virtual void AddParticles(){}
    virtual void InitGeometry(){}

    ClassDef(ERRecoMCApplication,1)  //Interface to MonteCarlo application
};

// inline functions

inline ERRecoMCApplication* ERRecoMCApplication::Instance()
{ return (ERRecoMCApplication*)(TVirtualMCApplication::Instance());}

#endif
