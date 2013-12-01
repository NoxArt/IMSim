#ifndef BLOCK_H
	#define	BLOCK_H

#include <iostream>
#include <cmath>
#include <map>
#include "../IMSimInternal.h"

using std::map;

namespace IMSim {
		
	class BlockStrategy;
	class Integrator;
	
	class Block {
		public:
			Block(BLOCK_VALUE input);
			Block(Block* input);
			Block(Block* input1, Block* input2);
			
			BLOCK_VALUE value(TIME time);
			BLOCK_VALUE _value(TIME time);
			BLOCK_VALUE getPreviousValue();
			void saveValidity(TIME time);
			TIME getValidUntil();
			TIME getDifference(TIME time, BLOCK_VALUE neutral);
			
			void setValue(BLOCK_VALUE value);
			void setValue(Block* value);
			
			void saveState(TIME time);
			void restoreState(TIME time);
			void clearSaveFlag();
			void clearRestoreFlag();
			
			Block* setStrategy(BlockStrategy* strategy);	
			Integrator* getIntegrator();
			Block* getChild();
			Block* getChild2();		
			
		protected:
			void init();
			
			void step(TIME time);
			BlockStrategy *strategy;
			
			// flags to prevent cyclic calling of save / restore
			// we call the children so we don't rely on every single
			// block being registered
			bool stateSaved;
			bool stateRestored;
			TIME timeSaved;
			
			BLOCK_VALUE savedValue;
			BLOCK_VALUE currentValue;
			BLOCK_VALUE previousValue;
			BLOCK_VALUE savedPreviousValue;
			Block *child;
			Block *child2;
			
			TIME validUntil;
	};
	
	
	class BlockStrategy {		
		public:
			BlockStrategy();
			
			virtual BLOCK_VALUE value(Block*, TIME) = 0;
			
			virtual void saveState(TIME) {};
			virtual void restoreState(TIME) {};
			
	};
	
	class StateBlock: public BlockStrategy {
		public:
			StateBlock* setCurrent(BLOCK_VALUE _value, TIME now) {
				this->saveValidity(now);
				this->currentValue = _value;
				this->previousValue = _value;
				return this;
			}
					
			TIME getDifference(TIME time, BLOCK_VALUE neutral) {
				return this->validUntil == -1 ? neutral : (time - this->validUntil);
			}
			void saveValidity(TIME time) {
				this->validUntil = time;
			}
			
			void saveState(TIME time) {				
				this->savedPreviousValue = this->previousValue;
				this->savedValue = this->currentValue;
				this->timeSaved = time;
			}

			void restoreState(TIME time) {				
				this->previousValue = this->savedPreviousValue;
				this->currentValue = this->savedValue;
				this->validUntil = time;
			}
			
			virtual BLOCK_VALUE getPreviousValue() {
				return this->previousValue;
			}
			
		protected:
			TIME validUntil;
			TIME timeSaved;

			BLOCK_VALUE currentValue;
			BLOCK_VALUE savedValue;
			BLOCK_VALUE previousValue;
			BLOCK_VALUE savedPreviousValue;
			
	};
	
	class LinearBlock: public StateBlock {
		public: 
			LinearBlock(BLOCK_VALUE coeficient) {
				this->coeficient = coeficient;
				this->coeficientDynamic = NULL;
				this->init();
			}
			LinearBlock(Block* coeficient) {
				this->coeficientDynamic = coeficient;
				this->init();
			}					
			
			BLOCK_VALUE value(Block* block, TIME time) {
				this->base = block->_value(time);
				
				if( this->validUntil >= time )
					return this->base + this->currentValue;
				else
				{
					this->previousValue = this->currentValue;
					this->currentValue += this->getDifference(time, 0) * this->getCoeficient(time);
					this->saveValidity(time);
					
					return this->base + this->currentValue;
				}
			}
			
			BLOCK_VALUE getPreviousValue() {
				return this->base + this->previousValue;
			}
			
			void restoreState(TIME time) {				
				this->currentValue = this->savedValue;
				this->previousValue = this->savedPreviousValue;
				this->validUntil = time;
			}
			
			BLOCK_VALUE getCoeficient(TIME time) {
				if( this->coeficientDynamic != NULL )
					return this->coeficientDynamic->value(time);
				else
					return this->coeficient;
			}
			
		private:
			void init() {
				this->validUntil = -1;
				this->currentValue = 0;
				this->previousValue = 0;
				this->savedValue = 0;
				this->savedPreviousValue = 0;
			}	
			
			BLOCK_VALUE base;
			
			BLOCK_VALUE coeficient;
			Block* coeficientDynamic;
			
	};
	
	class SinusBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return std::sin( block->getChild()->value(time) );
			}
	};
	
	class AddBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return block->getChild()->value(time) + block->getChild2()->value(time);
			}
	};
	
	class SubtrackBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return block->getChild()->value(time) - block->getChild2()->value(time);
			}
	};
	
	class MultiplyBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return block->getChild()->value(time) * block->getChild2()->value(time);
			}
	};
	
	class DivideBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return block->getChild()->value(time) / block->getChild2()->value(time);
			}
	};
	
	class PowerBlock: public BlockStrategy {
		public:
			BLOCK_VALUE value(Block* block, TIME time) {
				return std::pow(block->getChild()->value(time), block->getChild2()->value(time));
			}
	};
	
	

}

#endif	/* BLOCK_H */

