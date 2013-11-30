#ifndef CALENDAR_H
	#define	CALENDAR_H

#include <map>
#include <vector>
#include "IMSimInternal.h"

using std::map;
using std::vector;

namespace IMSim {
	
	class Action;
	class Event;
	
	class Calendar {
		public:
			explicit Calendar(TIME now);
			
			Calendar* executeNext(TIME limit);
			void moveToNextScheduled();
			TIME nextScheduled();
			bool hasEventsScheduled();
			bool canExecuteNext(TIME limit);
			void schedule(Action* entry, TIME at, PRIORITY priority = 0, bool omnipresent = false);
			
			TIME now();
			void setTimeNow(TIME now);
			void moveTimeTo(TIME now);
			Action* getCurrentlyRunning();
			
		private:
			void eraseUntil(TIME time);
			
			TIME timeNow;
			actionList actions;
			Action* currentlyRunning;
			
			// outer iterator cached
			actionList::iterator _it;
			// inner iterator cached
			actionMap::reverse_iterator _map;
	};

}

#endif	/* CALENDAR_H */

