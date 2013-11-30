#ifndef ENTITY_H
	#define	ENTITY_H

#include "IMSimInternal.h"

namespace IMSim {

	class Simulation;
	
	class Entity {
		public:
			explicit Entity(Simulation *simulation);
			Entity(Simulation *simulation, const char* name);
			~Entity();
		
			char* getName();
			ENTITY_ID getId();
			
		protected:
			Simulation *getSimulation();
						
		private:
			Simulation *simulation;
			const char* name;
			ENTITY_ID id;

	};

}

#endif	/* ENTITY_H */

