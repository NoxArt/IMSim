/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 */

#ifndef SIMULATOR_H
	#define	SIMULATOR_H

#include <vector>

#include "IMSimInternal.h"

using std::vector;

namespace IMSim {
	
	class Simulation;
	
	/**
	 * Responsible for handling the simulation experiment
     */
	class Simulator {
		
		public:
			
			Simulation* createSimulation();
			Simulation* createSimulation(char* name);
			Simulation* createSimulation(STEP minStep, STEP maxStep, PRECISION precision = 0);
		
		private:
			Simulation* addSimulation(Simulation * simulation);
			
			vector<Simulation*> simulations;
		
	};
	
}

#endif	/* SIMULATOR_H */

