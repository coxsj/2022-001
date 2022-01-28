#include <iostream>
#include "Controller.h"


int main() {
	std::cout << "Hello world\n";

	Controller controller;
	controller.rename("fred");
	Controller controller2(controller);
	controller2.rename("barney");

	std::cout << "\nController3 made by moving an anonymous controller into it\n";
	Controller controller3 = std::move(Controller());
	controller3.rename("betty");

	std::cout << "\nCopy another controller into controller3\n";
	controller3 = controller;
	controller3.printName();
	controller.printNameLn();

	std::cout << "\nMove an existing controller into controller3\n";
	Controller controller4 = std::move(controller2);
	controller4.printNameLn();
	controller2.printNameLn();	//Flagged as warning
								//Note move constructor leaves the 'donor' in an existing but empty state
}