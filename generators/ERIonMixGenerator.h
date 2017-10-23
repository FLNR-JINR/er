/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERIonMixGenerator_H
#define ERIonMixGenerator_H

#include <map>

#include "Rtypes.h"                     // for Double_t, Int_t, etc

#include "FairGenerator.h"              // for FairGenerator

#include "ERIonGenerator.h"

class FairIon;
class FairPrimaryGenerator;

/** @class ERIonMixGenerator
 ** @brief Class for the generation different ions during one simulation.
 ** @author M.Kozlov <kozlov.m.your@yandex.ru>
 ** @version 1.0
 **
 ** The ERIonGenerator defines the parameters of primary ion and background ions 
 ** which participates in simulation. It provides interfaces for determining  
 ** different types of dimentional, angular and energy distributions 
 ** at beam start position.
 ** Also it allows to add background ions with certain probability of appearence.
 ** Probability of primary ion is 1. For background ions it normalized relative
 ** to the main ion.
**/
class ERIonMixGenerator : public ERIonGenerator
{

	public:

		/** Default constructor **/
		ERIonMixGenerator();

		/** @brief Constructor with ion name
		 ** For the generation of ions with pre-defined FairIon
		 ** By default, the  excitation energy is zero. This can be changed with the
		 ** respective modifiers inherited from ERIonGenerator.
		 ** @param ionName  Ion name
		 ** @param mult      Number of ions per event
		**/
		ERIonMixGenerator(const Char_t* ionName, Int_t mult);


		/** @brief Constructor
		 ** For the generation of primary ions with atomic number z and mass number a.
		 ** By default, the mass equals a times the proton mass and the
		 ** excitation energy is zero. This can be changed with the
		 ** respective modifiers inherited from ERIonGenerator.
		 ** @param z         Atomic number
		 ** @param a         Atomic mass
		 ** @param q         Electric charge [e]
		 ** @param mult      Number of ions per event
		**/
		ERIonMixGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult);

		/** @brief Destructor **/
		virtual ~ERIonMixGenerator();

		/** Modifiers **/
		
		/** @brief Adds background ions with probability of appearance in simulation.
		 ** Probability of primary ion is 1. For background ions it normalized relative
		 ** to the main ion. For example, if we have primary ion with probability 0.5 and 
 		 ** two background ions with probabilities 0.2 and 0.3 we should pass argument
 		 ** newIonProb with \f$\frac{0.2}{0.5}\f$ and \f$\frac{0.3}{0.5}\f$ values respectively.
		 ** @param z           Atomic number
		 ** @param a           Atomic mass
		 ** @param q           Electric charge [e]
		 ** @param newIonProb  Probability of appearence relative to main ion
		**/
		void AddBackgroundIon(TString name, Int_t z, Int_t a, Int_t q, Double_t newIonProb);

		/** @brief Method ReadEvent
		 ** Generates <mult> of the specified ions and hands hem to the
		 ** FairPrimaryGenerator.
		**/
		Bool_t ReadEvent(FairPrimaryGenerator* primGen);

	private:
		std::map<Double_t, TString>  fBgIons;          ///< Background ion names with emegence probability
		Double_t                     fSumProbability;  ///< Background ions summary probability
		
		ERIonMixGenerator(const ERIonMixGenerator&);
		ERIonMixGenerator& operator=(const ERIonMixGenerator&);

		ClassDef(ERIonMixGenerator,1)
};


#endif

