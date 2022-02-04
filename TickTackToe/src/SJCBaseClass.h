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
		init_SJCBaseClass();
		dout << "SJCBaseClass Default constructor of "; print_NameLn();
	}
	SJCBaseClass(unsigned int players) {
		dout << "SJCBaseClass Default constructor with int of "; print_NameLn();
		init_SJCBaseClass(players);
	}
	//Copy constructor
	SJCBaseClass(const SJCBaseClass& other) {
		init_SJCBaseClass(other.playerCount());
		dout << "SJCBaseClass Copy constructor from "; other.print_NameLn();
		rename(other.name + "_copy");
	}
	//Move constructor
	SJCBaseClass(SJCBaseClass&& rhs) noexcept {
		players = std::exchange(rhs.players, nullptr);
		name = std::exchange(rhs.name, "husk(Empty and deletable)");
		dout << "SJCBaseClass Move constructor from "; print_NameLn();
	}
	//Destructor
	~SJCBaseClass() { dout << "SJCBaseClass Destructor of "; print_NameLn(); }
	//By-val assignment operator overload
	SJCBaseClass& operator=(SJCBaseClass copy) {
		dout << "SJCBaseClass by-value assignment (=) operator copying from "; copy.print_NameLn();
		copy.swap(*this);
		return *this;
	}
	//Member swap method
	void swap(SJCBaseClass& rhs) noexcept {
		dout << "Swapping "; this->print_Name(); dout << "with "; rhs.print_NameLn();
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
	void print_Name() const {
		if (name.size() > 0) dout << name << " ";
		else dout << "not_named ";
	}
	void print_NameLn() const { print_Name();  dout << std::endl; }
	void rename(const std::string& newName) {
		print_Name();
		dout << "renamed to ";
		name = newName;
		print_NameLn();
	}
private:
	void init_SJCBaseClass(const unsigned int numP = MAX_PLAYER_NUMS) {
		numPlayers = numP;
		players = std::make_unique<Player[]>(numP);
		name = std::string(DEFAULT_NAME);
		quit = false;
		userEntry = "";
	}
};