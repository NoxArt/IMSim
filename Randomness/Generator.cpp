/* 
 * File:   RandomGenerator.cpp
 * Author: Nox
 * 
 * Created on 3. prosinec 2011, 23:20
 */

#include "Generator.h"
#include <iostream>
#include <cmath>

namespace IMSim {
	namespace Randomness {
		
		void Generator::reset() {
			this->lastValue = this->seed;
		}
		
		// Implementations
		RANDOM BasicCongruentGenerator::random() {			
			// implicit modulo			
			this->lastValue = (this->lastValue * this->a + this->b);
			
			return (double)this->lastValue / (double)SEED_MAX;
		}
		
		RANDOM BasicCongruentGenerator::exponential(double mediumValue) {
			return -mediumValue * std::log(this->random());
		}
	
	}
}