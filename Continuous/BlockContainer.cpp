#include "BlockContainer.h"
#include "Block.h"

namespace IMSim {
	
	BlockContainer::BlockContainer() {}
	
	void BlockContainer::addBlock(Block* block) {
		this->blocks.push_back(block);
	}
	
	void BlockContainer::removeBlock(Block* block) {
		this->blocks.remove(block);
	}
	
	bool BlockContainer::isEmpty() {
		return this->blocks.empty();
	}
	
	void BlockContainer::step(TIME time) {
		if( this->blocks.empty() )
			return;
		
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			(*this->_it)->value(time);
	}
	
	void BlockContainer::saveState(TIME time) {
		if( this->blocks.empty() )
			return;
		
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			(*this->_it)->saveState(time);
		
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			(*this->_it)->clearSaveFlag();
	}
	
	void BlockContainer::restoreState(TIME time) {
		if( this->blocks.empty() )
			return;
		
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			(*this->_it)->restoreState(time);
		
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			(*this->_it)->clearRestoreFlag();
	}
	
	void BlockContainer::printBlocks(TIME time) {
		for(this->_it = this->blocks.begin(); this->_it != this->blocks.end(); this->_it++)
			printf("%f\t%.10f\n", time, (*this->_it)->value(time));
	}	
	
	
}