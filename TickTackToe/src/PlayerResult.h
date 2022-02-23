#pragma once
#include "Outcomes.h"

class PlayerResult
{
public:
	
	PlayerResult(unsigned int newSessionNum=0, Outcomes newOutcome=Outcomes::eUndefined, long long newDuration_ms=0) : 
		gameNum(newSessionNum), 
		outcome(newOutcome), 
		duration_ms(newDuration_ms) {}

	Outcomes getOutcome() { return outcome; }
	unsigned int getSessionNum() { return gameNum; }
	void setOutcome(Outcomes newOutcome) { outcome = newOutcome; }
	void setSessionNum(unsigned int newSessionNum) { gameNum = newSessionNum; }
private:
	unsigned int gameNum;
	Outcomes outcome;
	long long duration_ms;
};

