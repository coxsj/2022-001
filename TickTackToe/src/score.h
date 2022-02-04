#pragma once
class Score
{
	unsigned int games;
	unsigned int ties;
	unsigned int wins;

public:
	Score() : games(0), ties(0), wins(0) {}

	void print_();
	void print_Heading();
};

