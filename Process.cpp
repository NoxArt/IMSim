#include "Process.h"
#include <stdexcept>

namespace IMSim {
	
	void Process::activate(ACTION_CALLBACK callback) {
		if( callback == NULL )
			throw std::runtime_error("Callback is null");
			
		(this->*callback)();
	}
	
	void Process::scheduleNext(TIME at, ACTION_CALLBACK callback) {
		this->_schedule(at, 0, 0, false, callback);
	}

}