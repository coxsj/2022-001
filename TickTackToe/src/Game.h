#pragma once
#include <vector>

#include "ticktacktoegrid.h"


class Game
{
	struct Player{
		std::string getName() { return name; }
		char getSymbol() { return symbol; }
		bool isCom() { return com; }
		void setCom() { com = true; }
		void setName(const char* s) { name = std::string(s); }
		void setName(std::string& s) { name = s; }
		void setPlayer(bool newCom, const char* s) {
			com = newCom;
			name = std::string(s);
		}
		void setSymbol(const char c) { symbol = c; }
		void resetCom() { com = false; }
	private:
		bool com;
		std::string name;
		char symbol;
	};
	enum class GameState{
		PENDING_GAME,
		ACTIVE_GAME,
		GAME_OVER,	//no result
		GAME_WON,
		GAME_TIED
	};
	enum class SessionState {
		PENDING_SESSION,
		ACTIVE_SESSION,
		SESSION_CHANGE_REQUESTED,
		NEW_SESSION_REQESTED,
		SESSION_OVER
	};
	enum class GameType {
		ZERO_PLAYER,
		ONE_PLAYER,
		TWO_PLAYER
	};


	const unsigned int PLAYER_ONES_TURN = 0;
	const unsigned int PLAYER_TWOS_TURN = 1;
	const char* PLAYER1_DEFAULT_NAME = "Player1";
	const char* PLAYER2_DEFAULT_NAME = "Player2";
	const bool				comPlayer = true;
	const bool				humanPlayer = !comPlayer;
	std::vector<Player>		players;
	GameState		gameState;
	SessionState	sessionState;
	GameType		gameType;
	unsigned int 	currentPlayer;
	unsigned int	winner;
	const char PLAYER1_DEFAULT_SYMBOL = 'X';
	const char PLAYER2_DEFAULT_SYMBOL = 'O';

	TickTackToeGrid		grid;
	unsigned int		userEntry;
	bool				moveLegal;
	
public:
	Game() { initGame(); }

	void run();
	
private:
	void changePlayers();
	void changeSides();
	void getGameTypeFromUser();
	void getPostGameInputFromUser();
	void getPlayerName(unsigned int player);
	void getMoveInput();
	void getMoveInputFromCom();
	void getMoveInputFromHuman();
	void initGame() {
		grid.setDividerDouble();
		gameState = GameState::PENDING_GAME;
		sessionState = SessionState::PENDING_SESSION;
		players.push_back(Player());	// create player 1
		players.push_back(Player());	// create player 2
		players[0].setSymbol(PLAYER1_DEFAULT_SYMBOL);
		players[1].setSymbol(PLAYER2_DEFAULT_SYMBOL);
		std::cout << "Created game\n";
	}
	bool isGameOver();
	bool isLine(unsigned int a, unsigned int b, unsigned int c);
	bool isSessionOver() { return sessionState == SessionState::SESSION_OVER; }
	bool moveIsLegal() { return moveLegal; }
	void newGame();
	void newSession();
	void nextMove();
	bool newSessionRequest() { return sessionState == SessionState::NEW_SESSION_REQESTED; }
	void printCurrentPlayerName();
	void printGameResult();
	void processInput();
	void resetMoveIsLegal() { moveLegal = false; }
	void setMoveIsLegal()	{ moveLegal = true; }
	void setWinner(unsigned int a);
	void updateEntries();
	void updateGameState();
};

