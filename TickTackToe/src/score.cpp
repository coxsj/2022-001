
#include <iostream>

#include "score.h"

void Score::print() {
	std::cout << games << "\t" << wins << "\t" << ties << "\t" << games - wins - ties << std::endl;
}
void Score::printHeading() {
	std::cout << "Games" << "\t" << "Wins" << "\t" << "Ties" << "\t" << "Losses" << std::endl;
	std::cout << "=====" << "\t" << "====" << "\t" << "====" << "\t" << "======" << std::endl;
}