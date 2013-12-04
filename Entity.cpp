#include "Entity.h"
#include "Simulation.h"

namespace IMSim {
	
	Entity::Entity(Simulation* simulation) {
		this->simulation = simulation;
		this->name = NULL;
		
		if( simulation != NULL ) {
			this->id = simulation->getNextId();

			DEBUG(("ENTITY [ %d:%p ] created", this->id, this), this->simulation->now());
		}
	}
	
	Entity::Entity(Simulation* simulation, const char* name) {
		this->simulation = simulation;
		this->id = simulation->getNextId();
		this->name = name;
		
		DEBUG(("ENTITY [ %d:%p '%s' ] created", this->id, this, this->name), this->simulation->now());
	}
	
	Entity::~Entity() {
		DEBUG(("ENTITY [ %d:%p %s ] destroyed", this->id, this, this->name == NULL ? "" : this->name ), this->simulation->now());
	}
	
	Simulation* Entity::getSimulation()  {
		return this->simulation;
	}
	
	char* Entity::getName() {
		return (char *)this->name;
	}
	
	ENTITY_ID Entity::getId() {
		return this->id;
	}
	
}