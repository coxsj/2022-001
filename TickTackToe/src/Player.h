#pragma once
#include <iostream>

class Player {
public:

	Player(){}
	std::string getName() { return name; }
	char getSymbol() { return symbol; }
	bool isCom() { return com; }
	void setHuman() { com = false; }
	void setCom() { com = true; }
	void setName(const char* s) { name = std::string(s); }
	void setName(std::string& s) { name = s; }
	void setPlayer(bool newCom, const char* s) { com = newCom; name = std::string(s); }
	void setSymbol(const char c) { symbol = c; }

private:
	bool com;
	std::string name;
	char symbol;
};

