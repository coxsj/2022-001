#pragma once
#include <iostream>
#include <vector>

#include "GameResult.h"

class Player {
private:
	bool com;
	std::string name;
	const unsigned int cMaxResults = 10;
	std::vector<GameResult> results;
	char symbol;

public:

	Player() : com(false), name("unnamed"), symbol('-') {}
	std::string getName() { return name; }
	
	bool canPlay() { return results_addNewRecord(); }
	void enterResult(GameResult r);
	GameResult getLastResult();
	GameResult getResultFromSession(unsigned int sessNum);
	char getSymbol() { return symbol; }
	bool isCom() { return com; }
	bool results_addNewRecord();
	void setHuman() { com = false; }
	void setCom() { com = true; }
	void setName(const char* s) { name = std::string(s); }
	void setName(std::string& s) { name = s; }
	void setPlayer(bool newCom, const char* s) { com = newCom; name = std::string(s); }
	void setSymbol(const char c) { symbol = c; }

};

