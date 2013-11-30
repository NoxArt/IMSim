#include <stdexcept>
#include <stdlib.h>

#include "Event.h"
#include "Calendar.h"
#include "Simulation.h"
#include "Continuous/Condition.h"
#include "Continuous/ConditionsContainer.h"
#include "Action.h"
#include "Continuous/BlockContainer.h"

namespace IMSim {
	
	void Event::activate() {
		this->action();
	}
	
	void Event::terminate() {
		DEBUG(("EVENT [ %d ] terminate called", this->getId()), this->getSimulation()->getCalendar()->now());
		
		if( this->getSimulation()->getCalendar()->getCurrentlyRunning() &&
			this->getSimulation()->getCalendar()->getCurrentlyRunning()->getEvent() != this )
			delete this;
	}
	
	void Event::schedule() {		
		this->_schedule(this->now());
	}
	
	void Event::schedule(PRIORITY priority) {
		this->_schedule(this->now(), 0, priority);
	}
	
	void Event::scheduleAt(TIME at, PRIORITY priority) {		
		this->_schedule(at, 0, priority);
	}

	void Event::scheduleOmnipresent(TIME from, TIME interval, PRIORITY priority) {		
		this->_schedule(from, interval, priority, true);
	}
	
	void Event::schedulePeriodically(TIME from, TIME interval, PRIORITY priority) {		
		this->_schedule(from, interval, priority);
	}
	
	void Event::_schedule(TIME at, TIME interval, PRIORITY priority, bool omnipresent, ACTION_CALLBACK callback) {
		this->getSimulation()->getCalendar()->schedule(new Action(this, callback, interval), at, priority, omnipresent);
	}
	
	
	void Event::addCondition(Condition* condition) {
		this->getConditionsContainer()->addCondition(condition);
	}
	
	ConditionsContainer* Event::getConditionsContainer() {
		return this->getSimulation()->getConditionsContainer();
	}
	
	BlockContainer* Event::getBlockContainer() {
		return this->getSimulation()->getBlockContainer();
	}
	
	
	TIME Event::now() {
		return this->getSimulation()->now();
	}

}
