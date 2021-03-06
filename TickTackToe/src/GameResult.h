#pragma once
#include "Outcomes.h"

class GameResult
{
public:
	GameResult() : gameNum(0), outcome(Outcomes::eUndefined), duration_ms(0) {}

	Outcomes getOutcome() { return outcome; }
	unsigned int getSessionNum() { return gameNum; }
	void setOutcome(Outcomes newOutcome) { outcome = newOutcome; }
	void setSessionNum(unsigned int newSessionNum) { gameNum = newSessionNum; }
private:
	unsigned int gameNum;
	Outcomes outcome;
	long long duration_ms;
};

