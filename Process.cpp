#include "Process.h"
#include <stdexcept>

namespace IMSim {
	
	void Process::activate(ACTION_CALLBACK callback, void* data) {
		if( callback == NULL )
			throw std::runtime_error("Callback is null");
		
		(this->*callback)(data);
	}
	
	void Process::scheduleNext(TIME at, ACTION_CALLBACK callback, void* data) {
		this->_schedule(at, 0, 0, false, callback, data);
	}

}