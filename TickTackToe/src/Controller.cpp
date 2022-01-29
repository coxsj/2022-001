
#include <conio.h>
#include "Controller.h"

void Controller::getUserInput() {
	std::cout << "\nHow many players? (0, 1, 2) " << std::endl;
	userEntry = _getch();
	std::cout << userEntry << std::endl;
}

void Controller::play() {
	game->run();
	std::cout << "\n";
}

void Controller::processUserInput() {

}