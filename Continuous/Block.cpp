#include "Block.h"
#include "Integrator.h"

namespace IMSim {
	
	Block::Block(BLOCK_VALUE value) {
		this->currentValue = value;
		this->child = NULL;
		this->child2 = NULL;
		
		this->init();
	}
	
	Block::Block(Block* input) {
		this->currentValue = 0;
		this->child = input;
		this->child2 = NULL;
		
		this->init();
	}
	
	Block::Block(Block* input1, Block* input2) {
		this->currentValue = 0;
		this->child = input1;
		this->child2 = input2;
		
		this->init();
	}
	
	void Block::init() {
		this->strategy = NULL;
		this->validUntil = -1;
		this->stateSaved = false;
		this->stateRestored = false;
		this->savedValue = 0;
		this->previousValue = 0;
	}
	
	void Block::setValue(BLOCK_VALUE value) {
		if( this->child == NULL )
		{
			this->currentValue  = value;
			this->previousValue = this->currentValue;
		}
		else
			this->child->setValue(value);
	}
	
	void Block::setValue(Block* value) {
		this->child = value;
	}
	
	void Block::saveValidity(TIME time) {
		this->validUntil = time;
	}
	
	void Block::saveState(TIME time) {
		if( this->stateSaved == false )
		{			
			this->savedPreviousValue = this->previousValue;
			this->savedValue = this->currentValue;
			this->stateSaved = true;
			this->timeSaved = time;
			
			if( this->child != NULL )
				this->child->saveState(time);
			if( this->child2 != NULL )
				this->child2->saveState(time);
		}
		
		if( this->strategy != NULL )
			this->strategy->saveState(time);
	}
	
	void Block::restoreState(TIME time) {
		if( this->stateRestored == false )
		{			
			this->previousValue = this->savedPreviousValue;
			this->currentValue = this->savedValue;
			this->stateRestored = true;
			
			if( this->child != NULL )
				this->child->restoreState(time);
			if( this->child2 != NULL )
				this->child2->restoreState(time);
		}
		
		if( this->strategy != NULL )
			this->strategy->restoreState(time);
	}
	
	void Block::clearSaveFlag() {
		if( this->stateSaved == false )
			return;
		
		this->stateSaved = false;
		
		if( this->child != NULL )
			this->child->clearSaveFlag();
		if( this->child2 != NULL )
			this->child2->clearSaveFlag();
	}
	
	void Block::clearRestoreFlag() {
		if( this->stateRestored == false )
			return;
		
		this->stateRestored = false;
		
		if( this->child != NULL )
			this->child->clearRestoreFlag();
		if( this->child2 != NULL )
			this->child2->clearRestoreFlag();
	}
	
	
	
	TIME Block::getDifference(TIME time, BLOCK_VALUE neutral) {
		return this->validUntil == -1 ? neutral : (time - this->validUntil);
	}
	
	BLOCK_VALUE Block::value(TIME time) {
		//this->previousValue = this->currentValue;
		
		// common block
		if( this->strategy == NULL )
		{
			// is constant block
			if( this->child == NULL )
				return this->currentValue;
			// is only a container for
			else
				return this->child->value(time);
		}
		else
			return this->strategy->value(this, time);
	}
	
	BLOCK_VALUE Block::_value(TIME time) {
		return this->child == NULL ? this->currentValue : this->child->value(time);
	}
	
	BLOCK_VALUE Block::getPreviousValue() {
		if( this->strategy != NULL )
		{
			StateBlock *s = dynamic_cast<StateBlock *>(this->strategy);
			
			if( s != NULL )
				return s->getPreviousValue();
		}
		
		return this->previousValue;
	}
	
	TIME Block::getValidUntil() {
		return this->validUntil;
	}
	
	void Block::step(TIME time) {}
	
	Block* Block::getChild() {
		return this->child;
	}
	Block* Block::getChild2() {
		return this->child2;
	}
	
	
	Block* Block::setStrategy(BlockStrategy* strategy) {
		this->strategy = strategy;
		
		return this;
	}
	
	Integrator* Block::getIntegrator() {
		return dynamic_cast<Integrator *>(this->strategy);
	}
	
	BlockStrategy::BlockStrategy() {}

}
