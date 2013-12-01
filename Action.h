/* 
 * File:   Action.h
 * Author: Nox
 *
 * Created on 7. prosinec 2011, 15:49
 */

#ifndef ACTION_H
	#define	ACTION_H

#include "IMSimInternal.h"

namespace IMSim {

	class Event;
	
	class Action {
		public:
			Action(Event *event, ACTION_CALLBACK callback = NULL, TIME interval = 0);
			Action(Event *event, ACTION_CALLBACK callback = NULL, TIME interval = 0, void* data = NULL);
			
			Action* execute();
			
			Event* getEvent();
			ACTION_CALLBACK getCallback();
			TIME getInterval();
			
		private:
			Event* event;
			ACTION_CALLBACK callback;
			TIME interval;
			void* data;
			
	};

}

#endif	/* ACTION_H */

