#pragma once

#include <iostream>
#include <memory>

#include "Player.h"
#include "Game.h"

#ifdef DEBUG
#define dout std::out
#else
//If the preprocessor replaces 'dout' with '0 && cout',
//<< has higher precedence than && and short-circuit evaluation of && makes the 
//whole line evaluate to 0. Since the 0 is not used the compiler generates 
//no code at all for that line.
#define dout 0 && std::cout 
#endif

class Controller
{
private:
#define MAX_PLAYERS 2
	unsigned int				numPlayers;
	std::unique_ptr<Player[]>	players;
	std::unique_ptr<Game>		game;
	std::string					name;
#define DEFAULT_NAME "new_controller"
	bool						quit;
	std::string					userEntry;
public:

	/*
	std::unique_ptr<Base> ptr;  //points to Derived or some other derived class

	//rule of five
	~Foo() = default;
	Foo(Foo const& other) : ptr(other.ptr->clone()) {}
	Foo(Foo && other) = default;
	Foo& operator=(Foo const& other) { ptr = other.ptr->clone(); return *this; }
	Foo& operator=(Foo && other) = default;

	*/
	//Constructors
	Controller() {
		initController();
		dout << "Controller Default constructor of "; printNameLn();
	}
	Controller(unsigned int players) {
		dout << "Controller Default constructor with int of "; printNameLn();
		initController(players);
	}
	//Copy constructor
	Controller(const Controller& other) {
		initController(other.playerCount());
		dout << "Controller Copy constructor from "; other.printNameLn();
		rename(other.name + "_copy");
	}
	//Move constructor
	Controller(Controller&& rhs) noexcept {
		players = std::exchange(rhs.players, nullptr);
		game = std::exchange(rhs.game, nullptr);
		name = std::exchange(rhs.name, "husk(Empty and deletable)");
		dout << "Controller Move constructor from "; printNameLn();
	}
	//Destructor
	~Controller() { dout << "Controller Destructor of "; printNameLn(); }
	//By-val assignment operator overload
	Controller& operator=(Controller copy) {
		dout << "Controller by-value assignment (=) operator copying from "; copy.printNameLn();
		copy.swap(*this);
		return *this;
	}
	//Member swap method
	void swap(Controller& rhs) noexcept {
		dout << "Swapping "; this->printName(); dout << "with "; rhs.printNameLn();
		using std::swap;
		swap(players, rhs.players);
		swap(game, rhs.game);
		swap(name, rhs.name);
	}
	//Two argument swap makes this type efficiently std::swappable
	// This is a non member function using the hidden friend idiom.
	// The friend function is in the body of our class ie in its namespace
	// Marking as friend lets the compiler know it is not a member
	friend void swap(Controller& a, Controller& b) noexcept {
		//Just calls the member swap
		a.swap(b);
	}
	void play();
	unsigned int playerCount() const { return numPlayers; }
	void printName() const {
		if (name.size() > 0) dout << name << " ";
		else dout << "not_named ";
	}
	void printNameLn() const { printName();  dout << std::endl; }
	void rename(const std::string& newName) {
		printName();
		dout << "renamed to ";
		name = newName;
		printNameLn();
	}
private:
	void initController(const unsigned int numP = MAX_PLAYERS) {
		numPlayers = numP;
		players = std::make_unique<Player[]>(numP);
		game = std::make_unique<Game>();
		name = std::string(DEFAULT_NAME);
		quit = false;
		userEntry = "";
	}
};