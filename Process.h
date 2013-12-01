#ifndef PROCESS_H
	#define	PROCESS_H

#include "Event.h"

namespace IMSim {
	
	class Simulation;
	
	class Process: public Event {
		
		public:
			explicit Process(Simulation *simulation): Event(simulation) {}
			Process(Simulation *simulation, const char* name): Event(simulation, name) {}
			
			using Event::activate;
			void activate(ACTION_CALLBACK callback, void* data = NULL);
			void scheduleNext(TIME at, ACTION_CALLBACK callback, void* data = NULL);
			
			void addCondition(Condition *condition, ACTION_CALLBACK callback);

	};

}

#endif	/* PROCESS_H */

