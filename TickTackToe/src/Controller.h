#pragma once
#include <vector>
//#include<cstdlib>


#include "GameResult.h"
#include "Player.h"
#include "score.h"
#include "ticktacktoegrid.h"
#include "Utility.h"

class Controller
{
	//Players
	std::vector<Player>	players;
	const unsigned int	cPlayerNumOne = 0;
	const unsigned int	cPlayerNumTwo = 1;
	const char*			cPlayer1DefaultName	= "Player1";	//Needed to name com players
	const char*			cPlayer2DefaultName	= "Player2";	//Needed to name com players
	const bool			cComPlayer			= true;
	const bool			cHumanPlayer		= !cComPlayer;
	unsigned int		currentPlayer;
	unsigned int		winner;
	
	//Scoreboard
	std::vector<Score> scoreboard;

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
		ONE_PLAYER_NUM_SESSION,
		TWO_PLAYER_NUM_SESSION,
		AUTOMATED_SESSION,
		NEW_SESSION_REQUESTED,
		SESSION_OVER
	};
	SessionState sessionState;
	unsigned int sessionNum;
	
	//Game
	TickTackToeGrid	ttt;
	const char		PLAYER_NUM1_DEFAULT_SYMBOL = 'X';
	const char		PLAYER_NUM2_DEFAULT_SYMBOL = 'O';

	//Game Result
	GameResult gameResult;
	
	//Input General
	unsigned int userEntry;
	const short cMaxUserInputChars = 40;
	std::string inputPromptBlankingStr;

	//Input: Game info
	Utility::ConsolePos gameInfoPos;
	const short cMaxGameInfoChars = 40;
	std::string gameInfoBlankingStr;

	//Input: Session info
	Utility::ConsolePos sessionInputPos;
	
	//Input: Player names
	std::vector<Utility::ConsolePos> playerNameInputPos;

	//Input: Post game
	Utility::ConsolePos postGameInputPos;

	//Utility
	Utility utility;

public:
	Controller() { initController(); }

	void run();
	
private:
	bool blockRowOfTwo();
	void changeToNextPlayer();
	void changeSides();
	void clearPostGameText();
	void comMoveAdvanced();
	void comMoveIntermediate();
	void comMoveNovice();
	void drawGame() { ttt.printEntryList(); ttt.drawGrid(); printGameInfo(); }
	void endCurrentGame();
	void findMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax);
	bool getASyncInput();
	const char getCurrentSymbol() { return players[static_cast<unsigned int>(currentPlayer)].getSymbol(); }
	unsigned int getOpponentPlayer() { return currentPlayer == cPlayerNumOne ? cPlayerNumTwo : cPlayerNumOne; }
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
	bool isNewSessionRequested() { return sessionState == SessionState::NEW_SESSION_REQUESTED; }
	bool isSessionOver() { return sessionState == SessionState::SESSION_OVER; }
	bool makeRowOfThree();
	bool makeTwoInARow();
	bool moveIsLegal() { return moveLegal; }
	void newGame();
	void nextMove();
	void printCurrentPlayerName();
	void printGameInfo();
	bool processInput();
	void randomizeCurrentPlayer();
	void resetMoveIsLegal() { moveLegal = false; }
	void setGamePending() { gameState = GameState::PENDING_GAME; }
	void setMoveIsLegal() { moveLegal = true; }
	void setNamesGamesWithHumans(const unsigned int numPlayers);
	void setSessionEnd() { sessionState = SessionState::SESSION_OVER; }
	void setSessionRequested() { sessionState = SessionState::NEW_SESSION_REQUESTED; }
	void setSessionStateZeroPlayer();
	bool setUserEntryFromCellNum(unsigned int cellNum);
	bool setUserEntryFromAlphaKeyCode(char keyCode);
	bool setUserEntryFromAlphaNumKeyCode(char keyCode);
	void setWinner(unsigned int a);
	void updateEntries();
	bool updateGameState();
	void updatePlayerResults();
};

