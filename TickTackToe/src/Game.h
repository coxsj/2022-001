#pragma once
#include <vector>
#include<cstdlib>
#include<ctime>

#include "ticktacktoegrid.h"
#include "Utility.h"


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
		GAME_ABANDONNED,
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
	bool blockRowOfTwo();
	void changeToNextPlayer();
	void changeSides();
	void comMoveAdvanced();
	void comMoveIntermediate();
	void comMoveNovice();
	void drawGame() { grid.drawGame(); printGameResult(); }
	void findMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax);
	void gameAbandon();
	void gameEnd();
	const char getCurrentSymbol() { return players[currentPlayer].getSymbol(); }
	void getGameTypeFromUser();
	const unsigned int getOpponentPlayer() { return currentPlayer ^ 1; }
	const char getOpponentSymbol() { return players[getOpponentPlayer()].getSymbol(); }
	void getPostGameInputFromUser();
	void getPlayerName(unsigned int player);
	void getMoveInput();
	void getMoveInputFromCom();
	void getMoveInputFromHuman();
	void initGame() {
		//Randomize computer random cell picker
		srand(static_cast<unsigned int>(time(0)));
		grid.setDividerDouble();
		gameState = GameState::PENDING_GAME;
		sessionState = SessionState::PENDING_SESSION;
		players.push_back(Player());	// create player 1
		players.push_back(Player());	// create player 2
		players[0].setSymbol(PLAYER1_DEFAULT_SYMBOL);
		players[1].setSymbol(PLAYER2_DEFAULT_SYMBOL);
		std::cout << "Created game\n";
	}
	bool isActiveGame() { return gameState == GameState::ACTIVE_GAME; }
	bool isGameOver();
	bool isLine(unsigned int a, unsigned int b, unsigned int c);
	bool isSessionOver() { return sessionState == SessionState::SESSION_OVER; }
	bool makeRowOfThree();
	bool makeTwoInARow();
	bool moveIsLegal() { return moveLegal; }
	void newGame();
	void newSession();
	void nextMove();
	bool newSessionRequest() { return sessionState == SessionState::NEW_SESSION_REQESTED; }
	void printCurrentPlayerName();
	void printGameResult();
	bool processInput();
	void resetMoveIsLegal() { moveLegal = false; }
	void sessionEnd();
	void setMoveIsLegal()	{ moveLegal = true; }
	bool setUserEntryFromCellNum(unsigned int cellNum);
	bool setUserEntryFromAlphaKeyCode(char keyCode);
	bool setUserEntryFromAlphaNumKeyCode(char keyCode);
	void setWinner(unsigned int a);
	void updateEntries();
	void updateGameState();
};

