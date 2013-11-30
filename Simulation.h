/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 */

#ifndef SIMULATION_H
	#define	SIMULATION_H

#include <iostream>
#include "IMSimInternal.h"

using std::ostream;
using std::ofstream;
using std::istream;

namespace IMSim {
	
	class Calendar;
	class ConditionsContainer;
	class BlockContainer;
	
	/**
	 * Responsible for handling the simulation experiment
	  */
	class Simulation {
		
		public:	
			
			// construction
			Simulation();
			explicit Simulation(char* name);
			Simulation(STEP minStep, STEP maxStep, PRECISION precision = 0);
			
			~Simulation();
			
			// Basics
			void setStep(STEP minStep, STEP maxStep);
			void setPrecision(PRECISION precision);
			void setName(char* name);
			ENTITY_ID getNextId();
			TIME getCurrentTime();
			TIME now();
			
			// Output
			void setOutput( ostream* stream );
			void setOutput( ostream* stream, unsigned char flags );
			ostream* getOutput() const;			
			
			// Discrete support
			Calendar* getCalendar();
			
			// Continuous Support
			ConditionsContainer* getConditionsContainer();
			BlockContainer* getBlockContainer();
			
			// Checking
			bool isInRange(TIME time);
			bool isInRange(TIME time, TIME interval);
			bool isDebug();
			bool isReady();
			bool isInLoop();
			
			// Running			
			void run();
			void run(TIME startTime, TIME endTime);
			void abort();
			
			// Flags
			enum flags {
				OUTPUT_STANDARD = 0x0,
				OUTPUT_VERBOSE = 0x1,
				OUTPUT_DEBUG = 0x2
			};
			
			enum phase {
				PHASE_CLEAR = 0,
				PHASE_INIT = 1,
				PHASE_RUNNING = 2,
				PHASE_PAUSED = 3,
				PHASE_TERMINATED = 4
			};
		
		private:
			void init();
			
			TIME start;
			TIME end;
			TIME current;
			ENTITY_ID lastId;
			PRECISION precision;
			STEP minStep;
			STEP maxStep;
			unsigned int countOfRuns;
			unsigned char phase;
			char *name;
			
			void setCurrentTime(TIME now);
			void setTimeRange(TIME startTime, TIME endTime);
			
			bool hasConditionsContainer();
			bool hasBlockContainer();
			
			void reset();
			void terminate();
			bool abortFlag;
			
			ostream* output;
			unsigned char outputFlags;
			
			Calendar* calendar;
			ConditionsContainer* conditionsContainer;
			BlockContainer* blockContainer;
		
	};
	
}


#endif	/* SIMULATION_H */

