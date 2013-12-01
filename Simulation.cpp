/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 * 
 * @filesource
 */
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "Continuous/ConditionsContainer.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Simulation.h"
#include "Calendar.h"
#include "Continuous/BlockContainer.h"

#define ABORT_IF_REQUESTED()		if( this->abortFlag ) { this->abortFlag = false; break; }

using std::endl;

namespace IMSim {
		
	const char* ERROR_ENDTIME = "End time cannot be set to earlier than startTime";
	
	const char* MSG_START = "# Starting simulation <%s:%d> with range (%f, %f)";


	Simulation::Simulation()
	{
		this->init();
		
		this->minStep = defaultMinStep;
		this->maxStep = defaultMaxStep;		
		this->precision = defaultPrecision;	
	}
	
	Simulation::Simulation(char* name)
	{
		this->init();
		
		this->name = name;
		this->minStep = defaultMinStep;
		this->maxStep = defaultMaxStep;		
		this->precision = defaultPrecision;
	}

	Simulation::Simulation(STEP minStep, STEP maxStep, PRECISION precision)
	{
		this->init();
		
		this->minStep = minStep;
		this->maxStep = maxStep;		
		this->precision = (int)precision == 0 ? defaultPrecision : precision;
	}
	
	
	void Simulation::init() {
		this->name = "";
		this->output = NULL;
		this->calendar = NULL;
		this->conditionsContainer = NULL;
		this->blockContainer = NULL;
		this->countOfRuns = 0;
		this->start = 0.0;
		this->end = 0.0;
		this->outputFlags = Simulation::OUTPUT_STANDARD;
		this->phase = Simulation::PHASE_INIT;
		
		this->reset();
	}


	Simulation::~Simulation() {
		if( this->calendar != NULL )
			delete this->calendar;
	}
	
	
	void Simulation::reset() {
		if( this->isInLoop() )
			throw std::runtime_error("Cannot reset while already running");
		
		this->current = this->start;
		this->lastId = 0;
		this->abortFlag = false;
		
		if( this->calendar != NULL )
		{
			delete this->calendar;
			this->calendar = NULL;
		}
		if( this->conditionsContainer != NULL )
		{
			delete this->conditionsContainer;
			this->conditionsContainer = NULL;
		}
		
		
		this->phase = Simulation::PHASE_CLEAR;
	}
	
	
	
	void Simulation::setName(char* name) {
		this->name = name;
	}
	
	void Simulation::setTimeRange(TIME startTime, TIME endTime) {		
		if( this->isInLoop() )
			throw std::runtime_error("Cannot set time while already running");
			
		if( endTime <= startTime )
			throw std::invalid_argument("End time cannot be the same or earlier than start time");
		
		this->start = startTime;
		this->end = endTime;
		this->phase = Simulation::PHASE_INIT;		
	}
	
	void Simulation::setStep(STEP minStep, STEP maxStep) {
		this->minStep = minStep;
		this->maxStep = maxStep;
	}
	
	bool Simulation::isReady() {
		return this->end > 0 && this->maxStep > 0 && this->minStep > 0;
	}

	bool Simulation::isInRange(TIME time) {
		return time >= this->start && time <= this->end;
	}
	
	bool Simulation::isInRange(TIME time, TIME interval) {
		return (time >= this->start || interval > 0) && time <= this->end;
	}
	
	bool Simulation::isDebug() {
		return this->outputFlags & Simulation::OUTPUT_DEBUG;
	}
	
	bool Simulation::isInLoop() {
		return this->phase == Simulation::PHASE_RUNNING || this->phase == Simulation::PHASE_PAUSED;
	}
	
	inline bool Simulation::hasConditionsContainer() {
		return this->conditionsContainer != NULL;
	}
	
	inline bool Simulation::hasBlockContainer() {
		return this->blockContainer != NULL;
	}
	
	
	ENTITY_ID Simulation::getNextId() {
		return ++this->lastId;
	}
	
	
	void Simulation::setOutput( std::ostream* stream ) {
		this->setOutput(stream, Simulation::OUTPUT_STANDARD);
	}		
	
	void Simulation::setOutput( std::ostream* stream, unsigned char flags ) {		
		this->output = stream;
		this->outputFlags = flags;
	}
	

	std::ostream* Simulation::getOutput() const {
		return this->output;
	}


	Calendar* Simulation::getCalendar() {
		if( this->calendar == NULL )
			this->calendar = new Calendar( 0 );
		
		return this->calendar;
	}
	
	
	ConditionsContainer* Simulation::getConditionsContainer() {
		if( this->conditionsContainer == NULL )
			this->conditionsContainer = new ConditionsContainer();
		
		return this->conditionsContainer;
	}
	
	BlockContainer* Simulation::getBlockContainer() {
		if( this->blockContainer == NULL )
			this->blockContainer = new BlockContainer();
		
		return this->blockContainer;
	}
	
	
	
	
	void Simulation::run() {
		return this->run(0.0, 0.0);
	}
	
	
	/**
	 * Here it comes
     */
	void Simulation::run(TIME startTime, TIME endTime) {
		
		if( endTime > 0 )
			this->setTimeRange(startTime, endTime);
		else if( this->end <= 0 )
			throw new std::logic_error("Time range has not been set OR end time was set to 0 (or less)");
		
		// init time
		this->getCalendar()->setTimeNow(startTime);
		this->countOfRuns++;
		
		// print start
		if( this->output != NULL and this->outputFlags & Simulation::OUTPUT_VERBOSE )
		{
			/** @todo bleh */
			int chars = 100;
			char* msg = new char[chars+1];
			sprintf(msg, MSG_START, this->name, this->countOfRuns, this->start, this->end);
			
			(*this->output) << msg << endl;
		}
		
		
		/*
		 * Main loop
		 */
		
		// init
		TIME nextDiscrete = this->getCalendar()->nextScheduled();	// get first entry
		TIME step = this->maxStep;
		bool executeDiscreet = false;
		bool searching = false;
		TIME searchStart = 0;

		
		// loop
		while( this->end >= this->now() /* do not modify */ )
		{
			this->phase = Simulation::PHASE_RUNNING;
			
			DEBUG(("step: %.10f", step), this->now());
			
			// --- continous part -----------------------------------
			if( this->hasBlockContainer() )
			{
				// continuous core
				if( searching == false )
					this->getBlockContainer()->saveState(this->now() - step);
				
				// continuous step
				this->getBlockContainer()->step(this->now());
				
							
				// check conditioned events				
				if( this->getConditionsContainer()->changed(this->now()) )
				{
					if( searching == false )
						searchStart = this->now() - step < 0 ? 0 : this->now() - step;
					else
						DEBUG(("searching..."), this->now());
					
					if( step <= this->minStep )
					{
						DEBUG(("triggering !"), this->now());
						
						this->getConditionsContainer()->triggerChanged(this->now());
						this->getConditionsContainer()->clearChanged();
						
						searching = false;
						executeDiscreet = this->getCalendar()->canExecuteNext(this->end);
						step = this->maxStep;
						
						this->getBlockContainer()->printBlocks(this->now());
					}
					else
					{
						DEBUG(("halving the step"), this->now());
						
						this->getBlockContainer()->restoreState(searchStart);
								
						executeDiscreet = false;
						searching = true;
						
						step = step / 2;
						if( step < this->minStep )
							step = this->minStep;
						
						this->setCurrentTime(searchStart + step);
					}
				}
				else if( searching == true )
				{
					DEBUG(("starting the search over"), this->now());
					
					searching = false;
					this->getBlockContainer()->saveState(this->now());					
					step = this->maxStep;
				}
				else
					this->getBlockContainer()->printBlocks(this->now());
				
				ABORT_IF_REQUESTED();
			}
			
			
			// --- discrete part ------------------------------------			
			if( executeDiscreet )
			{
				this->setCurrentTime( nextDiscrete );
				this->getCalendar()->moveTimeTo(this->now());
				this->getCalendar()->executeNext(this->end);
				nextDiscrete = this->getCalendar()->nextScheduled();
			}
			
			executeDiscreet = false;
			
			
			// --- calculate next step size -------------------------
			if( searching == false )
				step = this->maxStep;
			
			// we can't overstep next scheduled discreet action
			if( step >= (nextDiscrete - this->now()) && nextDiscrete >= 0  )
			{
				step = nextDiscrete - this->now();
				executeDiscreet = true;
			}
			
			// step to end if overstepped
			if( this->now() < this->end && this->now() + step > this->end && step <= this->maxStep )
				this->setCurrentTime( this->end );
			else
				this->setCurrentTime( this->now() + step );
			
			
			ABORT_IF_REQUESTED();
		}
		
		// termination
		this->terminate();
	}
	
	void Simulation::abort() {
		DEBUG(("Aborting simulation"), this->now());
		
		this->abortFlag = true;
	}
	
	void Simulation::terminate() {
		DEBUG(("Simulation terminated"), this->now());
		
		this->phase = Simulation::PHASE_TERMINATED;
		this->reset();
	}


	void Simulation::setCurrentTime(TIME now) {
		this->current = now;
	}
	
	TIME Simulation::getCurrentTime() {
		return this->current;
	}
	
	TIME Simulation::now() {		
		return this->current;
	}
	
}
