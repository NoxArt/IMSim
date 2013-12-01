#include <stdexcept>

#include "Action.h"
#include "Process.h"

namespace IMSim {

	Action::Action(Event* event, ACTION_CALLBACK callback, TIME interval) {
		this->event = event;
		this->callback = callback;
		this->interval = interval;
		this->data = NULL;
	}
	
	Action::Action(Event* event, ACTION_CALLBACK callback, TIME interval, void* data) {
		this->event = event;
		this->callback = callback;
		this->interval = interval;
		this->data = data;
	}
	
	Action* Action::execute() {		
		if( this->event != NULL )
		{
			if( this->callback == NULL )
				this->event->activate();
			else
			{
				Process *p = dynamic_cast<Process *>(this->event);
				
				if( p != NULL )
					p->activate(this->callback, this->data);
				else
					throw std::runtime_error("Attempt to use Event as a Process");
			}
		}
		
		return this;
	}
	
	
	Event* Action::getEvent() {
		return this->event;
	}
	
	ACTION_CALLBACK Action::getCallback() {
		return this->callback;
	}
	
	TIME Action::getInterval() {
		return this->interval;
	}
	
}