/* 
 * File:   Calendar.cpp
 * Author: Nox
 * 
 * Created on 3. listopad 2011, 17:33
 */

#include <iostream>
#include <stdio.h>

#include "Calendar.h"
#include "Event.h"
#include "Process.h"
#include "Action.h"

#define ID "Calendar: "

using std::pair;

/**
 * @todo maybe optimize data manipulation?
 * @todo convert return to error messages/codes
 */
namespace IMSim {
	
	Calendar::Calendar(TIME currentTime) {
		this->currentlyRunning = NULL;
		this->setTimeNow(currentTime);
		
		DEBUG(("%s initialized", ID), this->timeNow);
	}

	void Calendar::schedule(Action* entry, TIME at, PRIORITY priority, bool omnipresent) {
		TIME interval = entry->getInterval();
		
		// schedule future event if is omnipresent and has interval
		if( at < this->now() && omnipresent && interval > 0 )
		{
			for(TIME i = at; i < this->now(); i += interval )
				at += interval;
			
			return this->schedule(entry, at, priority, omnipresent);
		}
		else if( at < this->now() )
			return;
		
		//DEBUG(("%s - scheduling event at %g (priority: %d | interval: %g | omnipresent: %d)", ID, at, priority, interval, omnipresent), this->now());
		
		// create freshly new
		if( this->actions.count(at) == 0 )
		{
			actionMetaVector actionMetaData;
			actionMetaData.push_back(entry);
			
			actionMap actionMapData;
			actionMapData.insert( std::pair<PRIORITY, actionMetaVector>(priority, actionMetaData) );
			
			this->actions.insert( std::pair<TIME,actionMap>(at, actionMapData) );
		}
		// add to existing
		else
		{
			if( this->actions[at].count(priority) > 0 )
				this->actions[at][priority].push_back(entry);
			else
			{
				actionMetaVector actionMetaData;
				actionMetaData.push_back(entry);
				
				this->actions[at].insert( std::pair<PRIORITY, actionMetaVector>(priority, actionMetaData) );
			}
		}
		
		//DEBUG(("%s - scheduled", ID), this->now());
	}
	
	
	
	
	
	bool Calendar::hasEventsScheduled() {		
		return this->actions.empty() == false;
	}
	
	
	bool Calendar::canExecuteNext(TIME limit) {		
		return this->hasEventsScheduled() && this->now() <= limit;
	}
	
	Action* Calendar::getCurrentlyRunning() {
		return this->currentlyRunning;
	}
	
	
	
	void Calendar::moveToNextScheduled() {
		// get first item (FIFO)
		this->_it = this->actions.begin();
		
		// now the simulation has moved to the given time
		this->moveTimeTo( this->_it->first );
	}
	
	
	
	TIME Calendar::nextScheduled() {
		return this->actions.empty() ? -1 : this->actions.begin()->first;
	}
	
	
	Calendar* Calendar::executeNext(TIME limit) {
		
		this->_it = this->actions.begin();
		
		// if in range (or before range -> we can schedule future events if has interval)
		if( this->now() > limit )
		{
			DEBUG(("%s out of range -> exitting", ID), this->now());
			
			return this;
		}
		
		// must be checked anyway
		if( this->hasEventsScheduled() == false )
		{
			DEBUG(("%s empty -> cancelling execution", ID), this->now());
			
			return this;
		}
		
		if( this->_it->second.empty() == false )
		{			
			// higher number => higher priority => execute first
			for(this->_map = this->_it->second.rbegin(); this->_map != this->_it->second.rend(); this->_map++)
			{
				// iterate over particular entries
				for(unsigned int i = 0; i < this->_map->second.size(); i++)
				{
					DEBUG(("%s ! dispatching event [%d] at %g", ID, this->_map->second[i]->getEvent()->getId(), this->now()), this->now());
					
					this->currentlyRunning = this->_map->second[i];
					
					// execute action
					delete this->_map->second[i]->execute();


					// schedule if has interval
					if( this->_map->second[i]->getInterval() > 0 )
					   this->schedule(
						   // action
							this->_map->second[i],
						   // at
							this->now() + this->_map->second[i]->getInterval(),
						   // priority
							this->_map->first,
						   // omnipresent (meaningful only on the first request anyway)
							false
						);
				}
				
				/**
				 * @todo Make sure this is really the right place and procedure !!!
				*/
				this->currentlyRunning = NULL;
			}
		}
		
		// delete executed entries
		this->actions.erase(this->_it);

		return this;
	}
	
	void Calendar::setTimeNow(TIME now) {		
		this->timeNow = now;
	}
	
	void Calendar::moveTimeTo(TIME now) {		
		this->timeNow = now;
		
		this->eraseUntil(now);
	}
	
	void Calendar::eraseUntil(TIME time) {
		actionList::iterator eraseIt;
		
		for(eraseIt = this->actions.begin(); eraseIt->first < time; eraseIt++ );
		
		this->actions.erase(this->actions.begin(), eraseIt);
	}
	
	TIME Calendar::now() {		
		return this->timeNow;
	}

}
