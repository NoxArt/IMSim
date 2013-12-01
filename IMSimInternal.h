/** 
 * FIT VUT Brno
 * IMS: Projekt "Knihovna pro kombinovanou simulaci"
 * 
 * @author Jiri Petruzelka xpetru07@stud.fit.vutbr.cz
 * @author Petr Mejzlik xmejzl03@stud.fit.vutbr.cz
 */

#ifndef IMSIMINTERNAL_H
	#define	IMSIMINTERNAL_H

/* Includes */
#include <stdio.h>
#include <sys/time.h>
#include <vector>
#include <map>
#include <climits>

using std::vector;
using std::map;

/* Environment */

/**
 * @see http://stackoverflow.com/a/1505631/387503
 * 
 * not used yet, can be possibly removed
 */
// Check windows
#if _WIN32 || _WIN64
	#define WINDOWS
		#if _WIN64
			#define ENVIRONMENT64
		#else
			#define ENVIRONMENT32
		#endif

#endif

// Check GCC
#if __GNUC__
	#define UNIX

	#if __x86_64__ || __ppc64__
		#define ENVIRONMENT64
	#else
		#define ENVIRONMENT32
	#endif
#endif


/* Identification section */
#define IMSIM_VERSION	0x002	// 0.2.0

#define PATH_DELIMITER	'\\'


/* Declarations section */
namespace IMSim {
	
	class Action;
	class Event;
	class Process;
	
}

// simulation time type
typedef double TIME;
typedef unsigned int PRIORITY;
typedef unsigned int ENTITY_ID;
typedef double STEP;
typedef double PRECISION;
typedef unsigned long SEED;
#define SEED_MAX ULONG_MAX
typedef double RANDOM;
typedef void (IMSim::Event::*ACTION_CALLBACK)(void* data);
typedef unsigned long INSTRUCTION;
typedef double BLOCK_VALUE;

typedef IMSim::Action* actionMeta;

typedef vector<IMSim::Action*> actionMetaVector;
typedef map<PRIORITY, actionMetaVector> actionMap;
typedef map<TIME, actionMap> actionList;

// Defaults
const SEED defaultSeed = 0;
const STEP defaultMinStep = 1e-8;
const STEP defaultMaxStep = 1e-1;
const PRECISION defaultPrecision = 1e-3;


/* Debugging */
#define IMSIM_DEBUG		false
#define DEBUG(x, t) \
	if( IMSIM_DEBUG ) { \
		printf("# DEBUG: T=%-10g ", t); \
		printf x; \
		printf(" (%s: %d)", __FILE__, __LINE__);\
		printf("\n"); \
	}

inline int startWatch(struct timeval *tv) {
	return gettimeofday(tv, NULL);
}

/*inline int startWatch(struct timeval *tv) {
	return gettimeofday(tv, NULL);
}*/

#endif	/* IMSIMINTERNAL_H */