#pragma once

#include <iostream>
#include <memory>

#include "Player.h"

#ifdef DEBUG
#define dout std::out
#else
//If the preprocessor replaces 'dout' with '0 && cout',
//<< has higher precedence than && and short-circuit evaluation of && makes the 
//whole line evaluate to 0. Since the 0 is not used the compiler generates 
//no code at all for that line.
#define dout 0 && std::cout 
#endif

class SJCBaseClass
{
private:
#define MAX_PLAYER_NUMS 2
	unsigned int				numPlayers;
	std::unique_ptr<Player[]>	players;
	std::string					name;
#define DEFAULT_NAME "new_SJCBaseClass"
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
	SJCBaseClass() {
		initSJCBaseClass();
		dout << "SJCBaseClass Default constructor of "; printNameLn();
	}
	SJCBaseClass(unsigned int players) {
		dout << "SJCBaseClass Default constructor with int of "; printNameLn();
		initSJCBaseClass(players);
	}
	//Copy constructor
	SJCBaseClass(const SJCBaseClass& other) {
		initSJCBaseClass(other.playerCount());
		dout << "SJCBaseClass Copy constructor from "; other.printNameLn();
		rename(other.name + "_copy");
	}
	//Move constructor
	SJCBaseClass(SJCBaseClass&& rhs) noexcept {
		players = std::exchange(rhs.players, nullptr);
		name = std::exchange(rhs.name, "husk(Empty and deletable)");
		dout << "SJCBaseClass Move constructor from "; printNameLn();
	}
	//Destructor
	~SJCBaseClass() { dout << "SJCBaseClass Destructor of "; printNameLn(); }
	//By-val assignment operator overload
	SJCBaseClass& operator=(SJCBaseClass copy) {
		dout << "SJCBaseClass by-value assignment (=) operator copying from "; copy.printNameLn();
		copy.swap(*this);
		return *this;
	}
	//Member swap method
	void swap(SJCBaseClass& rhs) noexcept {
		dout << "Swapping "; this->printName(); dout << "with "; rhs.printNameLn();
		using std::swap;
		swap(players, rhs.players);
		swap(name, rhs.name);
	}
	//Two argument swap makes this type efficiently std::swappable
	// This is a non member function using the hidden friend idiom.
	// The friend function is in the body of our class ie in its namespace
	// Marking as friend lets the compiler know it is not a member
	friend void swap(SJCBaseClass& a, SJCBaseClass& b) noexcept {
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
	void initSJCBaseClass(const unsigned int numP = MAX_PLAYER_NUMS) {
		numPlayers = numP;
		players = std::make_unique<Player[]>(numP);
		name = std::string(DEFAULT_NAME);
		quit = false;
		userEntry = "";
	}
};