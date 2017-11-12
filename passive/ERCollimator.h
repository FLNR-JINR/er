/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERCollimator_H
#define ERCollimator_H

#include "FairModule.h"

class TGeoCombiTrans;

class ERCollimator : public FairModule
{
public:

	/** Default constructor **/
	ERCollimator();

	/** Constructor with file name
	** @param fileName  Name of geometry file
	**
	** This constructor will force the target geometry to be
	** constructed from a geometry file (ROOT format only).
	**/
	ERCollimator(const char* fileName);

	/** Destructor **/
	virtual ~ERCollimator();

	/** Built the geometry **/
	virtual void ConstructGeometry();

private:
	TGeoCombiTrans* fPositionRotation;

	ClassDef(ERCollimator,1)
};

#endif // ERCollimator_H
