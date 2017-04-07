/*
 * ERNXyterAnalyzer.h
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#ifndef BEAMTIME_ERNXYTERANALYZER_H_
#define BEAMTIME_ERNXYTERANALYZER_H_

#include <FairTask.h>

class ERNXyterAnalyzer: public FairTask {
public:
	ERNXyterAnalyzer();
	virtual ~ERNXyterAnalyzer();
};

#endif /* BEAMTIME_ERNXYTERANALYZER_H_ */
