#ifndef INTEGRATOR_H
	#define	INTEGRATOR_H

#include "../IMSimInternal.h"
#include "Block.h"

namespace IMSim {

	class Integrator;
	class StateStrategy;
	
	// Strategies
	class IntegratorStrategy {
		public:
			virtual BLOCK_VALUE step(Integrator *integrator, Block* block, TIME time) = 0;
	};
	
	class EulerIntegratorStrategy: public IntegratorStrategy {
		public:
			BLOCK_VALUE step(Integrator *integrator, Block* block, TIME time);
	};
	
	// Integrator
	class Integrator: public StateBlock {
		friend class IntegratorStrategy;
		friend class EulerIntegratorStrategy;
		
		public:
			Integrator();
			Integrator(IntegratorStrategy* strategy);
			
		protected:
			IntegratorStrategy* strategy;
			
		private:
			BLOCK_VALUE value(Block* block, TIME time);
			
	};

}

#endif	/* INTEGRATOR_H */

