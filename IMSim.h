/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 */

#ifndef IMSIM_H
	#define	IMSIM_H

/* Includes */
#include <stdio.h>

/* Load basic symbols */

// load only if loaded from client script

#ifndef IMSIMINTERNAL_H
	#include "includes.h"

	// default implementation, does not have to be used
	//IMSim::Randomness::BasicCongruentGenerator* rnd = new IMSim::Randomness::BasicCongruentGenerator();
#endif


// Inlines

/**
 * Wraps the function name as a pointer and casts it to the common parent pointer type
 * 
 * @param function callback
 */
#define NEXT(callback)	((ACTION_CALLBACK)(&callback))
#define ARG(var)		((void*)&(var))
#define CALLSELF		new Action( this, NULL, 0.0 )
#define CALLBACK(fnc)	new Action( this, NEXT(fnc), 0.0 )

#define NUM(x)					(new Block((BLOCK_VALUE)x))
#define B_BLC(x)				(new Block(x))
#define B_SIN(x)				((new Block(x))->setStrategy(new SinusBlock))
#define B_LIN(x, c)				((new Block(x))->setStrategy(new LinearBlock(c)))
#define B_ADD(x, y)				((new Block(x, y))->setStrategy(new AddBlock))
#define B_SUB(x, y)				((new Block(x, y))->setStrategy(new SubtrackBlock))
#define B_MUL(x, y)				((new Block(x, y))->setStrategy(new MultiplyBlock))
#define B_DIV(x, y)				((new Block(x, y))->setStrategy(new DivideBlock))
#define B_INTEG(x)				((new Block(x))->setStrategy(new Integrator))
#define B_INTEG_IN(x, v, t)		((new Block(x))->setStrategy( (new Integrator)->setCurrent(v, t) ))
#define B_POW(x, y)				((new Block(x, y))->setStrategy(new PowerBlock))

/*inline void B_REG(IMSim::Simulation* sim, IMSim::Block* bl) {
	sim->getBlockContainer()->addBlock(bl);
}*/
	
/**
 * Experimental, not used yet
 * 
 * Returns the adress of the caller's instruction
 */
inline void* back(){
	return __builtin_return_address(0);
}


#endif	/* IMSIM_H */