/* 
 * File:   RandomGenerator.h
 * Author: Nox
 *
 * Created on 3. prosinec 2011, 23:20
 */

#ifndef RANDOMNESSGENERATOR_H
	#define	RANDOMNESSGENERATOR_H

#include <time.h>
#include <iostream>
#include "../IMSimInternal.h"


namespace IMSim {
	namespace Randomness {
	
		class Generator {

			public:
				Generator() {
					this->lastValue = this->seed = (SEED)time(NULL);	// ala recommended srand(time(NULL))
				}
				explicit Generator(SEED randomSeed) {
					this->lastValue = this->seed = randomSeed;
				}
				
				virtual RANDOM random() = 0;
				virtual RANDOM exponential(double mediumValue) = 0;

				void reset();
				
			protected:
				SEED getSeed();
				SEED lastValue;
				SEED seed;

		};
		
		// Implementations
		class BasicCongruentGenerator: public Generator {
			public:
				BasicCongruentGenerator(): Generator() {
					this->a = 1220703125L;
					this->b = 200;
				}
				explicit BasicCongruentGenerator(SEED randomSeed): Generator(randomSeed) {
					this->a = 1220703125L;
					this->b = 200;
				}

				BasicCongruentGenerator* setParams(unsigned int a, unsigned int b) {
					this->a = a;
					this->b = b;

					return this;
				}
				
				RANDOM random();
				RANDOM exponential(double mediumValue);

			private:
				SEED a;
				SEED b;

		};

	}
}

#endif	/* RANDOMNESSGENERATOR_H */

