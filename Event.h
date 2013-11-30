#ifndef EVENT_H
	#define	EVENT_H

#include <vector>
#include "IMSimInternal.h"
#include "Entity.h"
#include "Simulation.h"
#include "Continuous/ConditionsContainer.h"

namespace IMSim {
	
	class Condition;
	class ConditionsContainer;
	class BlockContainer;
	
	class Event: public Entity {
		public:
			// construction
			explicit Event(Simulation *simulation): Entity(simulation) {}
			Event(Simulation *simulation, const char* name): Entity(simulation, name) {}
					
			// lifetime
			virtual void activate();
			void terminate();
			
			// scheduling
			void schedule();
			void schedule(PRIORITY priority );
			void scheduleAt(TIME at, PRIORITY priority = 0);
			void schedulePeriodically(TIME from, TIME interval = 0, PRIORITY priority = 0);
			void scheduleOmnipresent(TIME from, TIME interval = 0, PRIORITY priority = 0);
			
			// conditions
			void addCondition(Condition *condition);
			
			
		protected:
			void _schedule(TIME from, TIME interval = 0, PRIORITY priority = 0, bool omnipresent = false, ACTION_CALLBACK callback = NULL);
			
			ConditionsContainer* getConditionsContainer();
			BlockContainer* getBlockContainer();
			
			TIME now();
			
		private:
			virtual void action() = 0;
			
			
	};

}

#endif	/* EVENT_H */