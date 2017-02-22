// -------------------------------------------------------------------------
// -----                      ERDecayer header file                    -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERDECAY_H
#define ERDECAY_H

#include <vector>

#include "ERDecay.h"


class ERDecayer{
private:
	std::vector<ERDecay*> fDecays;
public:
	ERDecayer();
	~ERDecayer();

	Bool_t Stepping();

	void AddDecay(ERDecay* decay);

	void BeginEvent();
	void FinishEvent();
	Bool_t Init();

	ClassDef(ERDecayer, 1)
};

#endif
