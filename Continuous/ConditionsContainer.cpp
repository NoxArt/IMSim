#include <iostream>
#include "ConditionsContainer.h"
#include "Condition.h"

namespace IMSim {

	ConditionsContainer::ConditionsContainer() {
	}

	ConditionsContainer::ConditionsContainer(Condition* condition) {
		this->addCondition(condition);
	}

	ConditionsContainer::~ConditionsContainer() {}
	
	
	
	void ConditionsContainer::addCondition(Condition* condition) {
		this->conditions.push_back((conditionEntry){ condition, false });
	}
	
	bool ConditionsContainer::hasConditions() {
		return this->conditions.empty() == false;
	}
	
	bool ConditionsContainer::changed(TIME time) {		
		bool changed = false;
		
		for(this->conditionsIteratorCached = this->conditions.begin();
			this->conditionsIteratorCached != this->conditions.end();
			this->conditionsIteratorCached++)
			if( this->conditionsIteratorCached->condition->isValid(time) )
			{
				this->conditionsIteratorCached->hasChanged = true;
				changed = true;
			}
			
		return changed;
	}
	
	void ConditionsContainer::triggerChanged(TIME time) {
		for(this->conditionsIteratorCached = this->conditions.begin();
			this->conditionsIteratorCached != this->conditions.end();
			this->conditionsIteratorCached++)
			if( this->conditionsIteratorCached->hasChanged == true )
			{
				this->conditionsIteratorCached->condition->trigger();
			}
	}
	
	void ConditionsContainer::clearChanged() {
		for(this->conditionsIteratorCached = this->conditions.begin();
			this->conditionsIteratorCached != this->conditions.end();
			this->conditionsIteratorCached++)
			this->conditionsIteratorCached->hasChanged = false;
	}

}