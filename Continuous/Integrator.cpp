#include <stdexcept>
#include <iostream>

#include "Integrator.h"

namespace IMSim {
	
	Integrator::Integrator(IntegratorStrategy* strategy) {
		this->strategy = strategy;
		this->validUntil = -1;
	}
	
	Integrator::Integrator() {
		this->strategy = new EulerIntegratorStrategy;
		this->validUntil = -1;
	}
	
	BLOCK_VALUE Integrator::value(Block* block, TIME time) {
		return this->strategy->step(this, block, time);
	}
	
	BLOCK_VALUE EulerIntegratorStrategy::step(Integrator *integrator, Block* block, TIME time) {
		if( integrator->validUntil == -1 )
			integrator->currentValue = block->_value(time);
				
		if( integrator->validUntil < time )
		{
			integrator->previousValue = integrator->currentValue;
			integrator->currentValue += integrator->getDifference(time, 0) * block->_value(time);
			
			integrator->saveValidity(time);
		}
		
		return integrator->currentValue;
	}
	
}
