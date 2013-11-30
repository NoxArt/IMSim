#include <iostream>
#include "Condition.h"
#include "../Action.h"
#include "../Process.h"
#include "Block.h"

namespace IMSim {
	
	Condition::Condition(Block *boundTo, BLOCK_VALUE value, Action* action) {
		this->action = action;
		this->boundTo = boundTo;
		this->value = value;
		this->disabled = false;
	}
	
	void Condition::disable() {
		this->disabled = true;
	}
	
	bool Condition::trigger() {		
		if( this->action != NULL && this->disabled == false )
			this->action->execute();
		
		return true;
	}
	
	bool Condition::isValid(TIME time) {
		BLOCK_VALUE current = this->boundTo->value(time);
		BLOCK_VALUE previous = this->boundTo->getPreviousValue();
		
		return (previous > this->value && current < this->value) || (previous < this->value && current > this->value);
	}
	
	/* ---- Core Conditions ---- */
	
	bool ConditionUp::isValid(TIME time) {
		BLOCK_VALUE current = this->boundTo->value(time);
		BLOCK_VALUE previous = this->boundTo->getPreviousValue();
		
		return previous < this->value && current > this->value;
	}
	
	bool ConditionDown::isValid(TIME time) {
		BLOCK_VALUE current = this->boundTo->value(time);
		BLOCK_VALUE previous = this->boundTo->getPreviousValue();
		
		return previous > this->value && current < this->value;
	}
	
	bool ConditionDescending::isValid(TIME time) {
		BLOCK_VALUE current = this->boundTo->value(time);
		BLOCK_VALUE previous = this->boundTo->getPreviousValue();
		
		return previous < this->value && current < this->value && previous > current;
	}
	
	bool ConditionRising::isValid(TIME time) {
		BLOCK_VALUE current = this->boundTo->value(time);
		BLOCK_VALUE previous = this->boundTo->getPreviousValue();
		
		return previous > this->value && current > this->value && previous < current;
	}
	
}
