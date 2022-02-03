#pragma once
#include <vector>
//#include<cstdlib>

#include "Player.h"
#include "score.h"
#include "ticktacktoegrid.h"
#include "Utility.h"

class Controller
{
	//Players
	enum class PLAYER : unsigned int {
		ONE = 0,
		TWO = 1
	};
	std::vector<Player>	players;
	const unsigned int	PLAYER_ONE				= 0;
	const unsigned int	PLAYER_TWO				= PLAYER_ONE + 1;
	const char*			PLAYER1_DEFAULT_NAME	= "Player1";
	const char*			PLAYER2_DEFAULT_NAME	= "Player2";
	const bool			COM_PLAYER				= true;
	const bool			HUMAN_PLAYER			= !COM_PLAYER;
	PLAYER 				currentPlayer;
	unsigned int		winner;
	
	//Scores
	std::vector<Score> scores;

	//Game
	enum class GameState{
		PENDING_GAME,
		ACTIVE_GAME,
		GAME_ABANDONNED,
		GAME_WON,
		GAME_TIED
	};
	GameState		gameState;
	bool			moveLegal;
	
	//Session
	enum class SessionState {
		PENDING_SESSION,
		ONE_PLAYER_SESSION,
		TWO_PLAYER_SESSION,
		AUTOMATED_SESSION,
		NEW_SESSION_REQUESTED,
		SESSION_OVER
	};
	SessionState	sessionState;
	
	//Grid
	TickTackToeGrid	ttt;
	const char		PLAYER1_DEFAULT_SYMBOL = 'X';
	const char		PLAYER2_DEFAULT_SYMBOL = 'O';

	//Game info
	Utility::ConsolePos gameInfoPos;
	const short cMaxGameInfoChars = 40;
	std::string gameInfoBlankingStr;

	//Input
	unsigned int userEntry;
	Utility::ConsolePos getSessionInputPos;
	const short cMaxUserInputChars = 40;
	std::string inputPromptBlankingStr;
	std::vector<Utility::ConsolePos> getPlayerNamePos;

	//Utility
	Utility utility;

public:
	Controller() { initController(); }

	void run();
	
private:
	bool blockRowOfTwo();
	void changeToNextPlayer();
	void changeSides();
	void comMoveAdvanced();
	void comMoveIntermediate();
	void comMoveNovice();
	void drawGame() { ttt.printEntryList(); ttt.drawGrid(); printGameResult(); }
	void endCurrentGame();
	void findMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax);
	bool getASyncInput();
	const char getCurrentSymbol() { return players[static_cast<unsigned int>(currentPlayer)].getSymbol(); }
	PLAYER getOpponentPlayer() { return currentPlayer == PLAYER::ONE ? PLAYER::TWO : PLAYER::ONE; }
	const char getOpponentSymbol() { return players[static_cast<unsigned int>(getOpponentPlayer())].getSymbol(); }
	void getPostGameInputFromUser();
	void getPlayerName(unsigned int player);
	void getMoveInput();
	void getMoveInputFromCom();
	void getMoveInputFromHuman();
	void getNewSessionFromUser();
	void initController();
	void initGame();
	void initGameInfo();
	void initGetUserInput();
	void initPlayers();
	void initSession();
	void initUtility();
	bool isGameActive() { return gameState == GameState::ACTIVE_GAME; }
	bool isGamePending() { return gameState == GameState::PENDING_GAME; }
	bool isLine(unsigned int a, unsigned int b, unsigned int c);
	bool isSessionOver() { return sessionState == SessionState::SESSION_OVER; }
	bool makeRowOfThree();
	bool makeTwoInARow();
	bool moveIsLegal() { return moveLegal; }
	void newGame();
	void nextMove();
	bool newSessionRequest() { return sessionState == SessionState::NEW_SESSION_REQUESTED; }
	void printCurrentPlayerName();
	void printGameResult();
	bool processInput();
	void randomizeCurrentPlayer();
	void resetMoveIsLegal() { moveLegal = false; }
	void setSessionEnd() { sessionState = SessionState::SESSION_OVER; }
	void setGamePending() { gameState = GameState::PENDING_GAME; }
	void setMoveIsLegal() { moveLegal = true; }
	void setNamesGamesWithHumans(const unsigned int numPlayers);
	void setSessionStateZeroPlayer();
	bool setUserEntryFromCellNum(unsigned int cellNum);
	bool setUserEntryFromAlphaKeyCode(char keyCode);
	bool setUserEntryFromAlphaNumKeyCode(char keyCode);
	void setWinner(unsigned int a);
	void updateEntries();
	bool updateGameState();
};

