/* 
 * File:   ConditionsContainer.h
 * Author: Nox
 *
 * Created on 7. prosinec 2011, 13:11
 */

#ifndef CONDITIONSCONTAINER_H
	#define	CONDITIONSCONTAINER_H

#include <vector>
#include "../IMSimInternal.h"

using std::vector;

namespace IMSim {
	
	class Condition;
	
	typedef struct {
		Condition* condition;
		bool hasChanged;
	} conditionEntry;
	
	class ConditionsContainer {
		public:
			ConditionsContainer();
			explicit ConditionsContainer(Condition* condition);
			virtual ~ConditionsContainer();
			
			void addCondition(Condition* condition);
			bool hasConditions();
			
			bool changed(TIME time);
			void triggerChanged(TIME time);
			void clearChanged();
			
		private:
			vector<conditionEntry> conditions;
			actionMeta action;
			
			vector<conditionEntry>::iterator conditionsIteratorCached;
			
	};

}

#endif	/* CONDITIONSCONTAINER_H */

