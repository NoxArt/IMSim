#ifndef BLOCKCONTAINER_H
	#define	BLOCKCONTAINER_H

#include <list>
#include "../IMSimInternal.h"

using std::list;

namespace IMSim {

	class Block;
	
	class BlockContainer {
		public:
			BlockContainer();
			
			void addBlock(Block *block);
			void removeBlock(Block *block);
			void printBlocks(TIME time);
			
			bool isEmpty();
			
			void step(TIME time);
			void saveState(TIME time);
			void restoreState(TIME time);
			
		private:
			list<Block*> blocks;
			list<Block*>::iterator _it;

	};

}

#endif	/* BLOCKCONTAINER_H */

