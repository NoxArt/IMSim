/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 * 
 * @filesource
 */
#include <iostream>

#include "Simulator.h"
#include "Simulation.h"

namespace IMSim {
	
	Simulation* Simulator::createSimulation() {
		Simulation* simulation = new Simulation();

		return this->addSimulation(simulation);
	}
	
	Simulation* Simulator::createSimulation(char* name) {
		Simulation* simulation = new Simulation(name);
		
		return this->addSimulation(simulation);
	}

	Simulation* Simulator::createSimulation(STEP minStep, STEP maxStep, PRECISION precision) {
		Simulation* simulation = new Simulation(minStep, maxStep, precision);

		return this->addSimulation(simulation);
	}

	Simulation* Simulator::addSimulation(Simulation* simulation) {
		this->simulations.push_back(simulation);

		return simulation;
	}
	
}