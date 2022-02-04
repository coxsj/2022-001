#pragma once
class GameResult
{
public:
	enum class Outcomes {
		eUndefined,
		eLoss,
		eTie,
		eWin
	};
	GameResult() : sessionNum(0), outcome(Outcomes::eUndefined), duration_ms(0) {}

	Outcomes getOutcome() { return outcome; }
	unsigned int getSessionNum() { return sessionNum; }
	void setOutcome(Outcomes newOutcome) { outcome = newOutcome; }
	void setSession_Num(unsigned int newSessionNum) { sessionNum = newSessionNum; }
private:
	unsigned int sessionNum;
	Outcomes outcome;
	long long duration_ms;
};

