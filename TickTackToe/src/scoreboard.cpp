
#include <iostream>

#include "scoreboard.h"

void Scoreboard::print_() {
	//std::cout << games << "\t" << wins << "\t" << ties << "\t" << games - wins - ties << std::endl;
}
void Scoreboard::print_Heading() {
	std::cout << "Games" << "\t" << "Wins" << "\t" << "Ties" << "\t" << "Losses" << std::endl;
	std::cout << "=====" << "\t" << "====" << "\t" << "====" << "\t" << "======" << std::endl;
}