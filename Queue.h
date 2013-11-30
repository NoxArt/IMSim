/* 
 * File:   Queue.h
 * Author: Nox
 *
 * Created on 4. prosinec 2011, 16:47
 */

#ifndef QUEUE_H
	#define	QUEUE_H

#include <queue>
#include "IMSimInternal.h"
#include "Entity.h"

using std::queue;

namespace IMSim {
	
	class Simulation;
	
	class Queue: public Entity {
		public:
			explicit Queue(Simulation *simulation): Entity(simulation) {}
			Queue(Simulation *simulation, const char* name): Entity(simulation, name) {}
			
			bool empty();
			actionMeta pop();
			push( actionMeta entry );
			
			
		private:
			queue<actionMeta> data;
			
	};

}

#endif	/* QUEUE_H */

