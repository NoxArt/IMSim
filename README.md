IMSim
==================

Simple, older and non-complete C++ simulation library as a school project for FIT VUT Brno.


Example
-------
```cpp
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "IMSim.h"

using namespace std;
using namespace IMSim;

const double g = -9.81;
const double dimnish = -0.8;

class Ball: public Process {
	public:
		Ball(Simulation *simulation, BLOCK_VALUE initialPosition): Process(simulation) {
			this->force = B_INTEG_IN( NUM(g), 0, 0.0 );
			this->movement = B_INTEG_IN(this->force, initialPosition, 0.0 );
			
			this->cond = new Condition(this->movement, 0.0, CALLBACK(Ball::jump));
			ConditionDescending* condFix = new ConditionDescending(this->movement, -0.00001, CALLBACK(Ball::fix));
			
			this->getBlockContainer()->addBlock(this->movement);
			this->getConditionsContainer()->addCondition(this->cond);
			this->getConditionsContainer()->addCondition(condFix);
		}
		
	private:
		void action() {}
		
		void jump() {
			std::cout << "# jumping" << std::endl;
			
			this->movement->getIntegrator()->setCurrent(0, this->now());
			
			BLOCK_VALUE up = this->force->value(this->now()) * dimnish;
			this->force->getIntegrator()->setCurrent(up, this->now());
		}
		
		void fix() {
			std::cout << "# fixing" << std::endl;
			
			this->getSimulation()->abort();
			
			this->cond->disable();
			
			delete this->force;
			delete this->movement;
			this->movement = NUM(0);
		}
		
		Condition* cond;
		Block* movement;
		Block* force;
	
};

int main(int argc, char** argv) {
	
	// init
	Simulator *sim = new Simulator();
	Simulation *mySim = sim->createSimulation("Ball example");
	mySim->setStep(1e-8, 1e-3);
	
	// setup
	mySim->setOutput(&std::cout, Simulation::OUTPUT_VERBOSE | Simulation::OUTPUT_DEBUG);		
	(new Ball(mySim, 1.0))->schedule();
	
	// run
	try{
		mySim->run(0.0, 4);
	} catch( std::runtime_error e ) {
		(*mySim->getOutput()) << std::endl << "ERROR: " << e.what() << std::endl << std::endl;
	}
			
	// cleanup
	delete mySim;
	delete sim;
	return EXIT_SUCCESS;
}
```
