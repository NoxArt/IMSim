#ifndef CONDITION_H
	#define	CONDITION_H

#include "../IMSimInternal.h"

#define CONDITION(type)		class Condition##type: public Condition { public:\
								Condition##type(Block *boundTo, BLOCK_VALUE value, Action* action): Condition(boundTo, value, action){}\
								bool isValid(TIME time); };

namespace IMSim {

	class Action;
	class Block;
	
	class Condition {
		public:
			Condition(Block* boundTo, BLOCK_VALUE value, Action* action);

			virtual bool isValid(TIME time);
			bool trigger();
			void disable();
			
		protected:
			Action* action;
			Block* boundTo;
			BLOCK_VALUE value;
			bool disabled;
			
	};

	
	/* ---- Core Conditions ---- */
	
	CONDITION(Up);
	CONDITION(Down);
	CONDITION(Descending);
	CONDITION(Rising);
	
}

#endif	/* CONDITION_H */

