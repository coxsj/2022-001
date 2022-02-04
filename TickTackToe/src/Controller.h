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
		GAME_PENDING,
		GAME_ACTIVE,
		GAME_ABANDONNED,
		GAME_WON,
		GAME_TIED
	};
	GameState		gameState;
	bool			moveLegal;
	
	//Session
	enum class SessionState {
		SESSION_PENDING,
		SESSION_ONE_PLAYER,
		SESSION_TWO_PLAYERS,
		SESSION_AUTOMATED,
		SESSION_REQUEST_NEW,
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
	std::string inputprompt_BlankingStr;

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
	Controller() { init_Controller(); }

	void run();
	
private:
	void gamePlay_ChangeSides();
	void gamePlay_ChangeToNextPlayer();
	void gamePlay_DrawGame() { ttt.print_EntryList(); ttt.drawGrid(); print_GameInfo(); }
	void gamePlay_EndCurrentGame();
	void gamePlay_GetNextMove();
	bool gamePlay_IsMoveLegal() { return moveLegal; }
	void gamePlay_NewGame();
	void gamePlay_NextMove();
	bool gamePlay_ProcessMove();
	void gamePlay_RandomizeCurrentPlayer();
	void gamePlay_ResetMoveIsLegal() { moveLegal = false; }
	void gamePlay_SetMoveIsLegal() { moveLegal = true; }
	void gamePlay_SetWinner(unsigned int a);
	void gamePlay_UpdateEntries();
	void gamePlay_UpdatePlayerResults();
	void init_Controller();
	void init_Game();
	void init_GameInfo();
	void init_Players();
	void init_prompt_s();
	void init_Session();
	void init_Utility();
	bool isGameActive() { return gameState == GameState::GAME_ACTIVE; }
	bool isGamePending() { return gameState == GameState::GAME_PENDING; }
	bool isSessionAutomated() { return sessionState == SessionState::SESSION_AUTOMATED; }
	bool isSessionOnePlayer() { return sessionState == SessionState::SESSION_ONE_PLAYER; }
	bool isSessionOver() { return sessionState == SessionState::SESSION_OVER; }
	bool isSessionPending() {return sessionState == SessionState::SESSION_PENDING; }
	bool isSessionRequestNew() { return sessionState == SessionState::SESSION_REQUEST_NEW; }
	bool isSessionTwoPlayers() { return sessionState == SessionState::SESSION_TWO_PLAYERS; }
	unsigned int playerCurrentPlayerOpponent() { return currentPlayer == cPlayerNumOne ? cPlayerNumTwo : cPlayerNumOne; }
	const char playerCurrentPlayerSymbol() { return players[(currentPlayer)].getSymbol(); }
	const char playerOpponentSymbol() { return players[(playerCurrentPlayerOpponent())].getSymbol(); }
	void print_CurrentPlayerName();
	void print_GameInfo();
	void prompt_ClearPostGameText();
	bool prompt_GetAsyncInput();
	void prompt_NewSessionType();
	void prompt_NextMove();
	void prompt_PostGame();
	void prompt_PlayerName(unsigned int player);
	bool prompt_ProcessInput();
	bool prompt_SetUserEntryFromCellNum(unsigned int cellNum);
	bool prompt_SetUserEntryFromAlphaKeyCode(char keyCode);
	bool prompt_SetUserEntryFromAlphaNumKeyCode(char keyCode);
	void setGameState(GameState newGameState) { gameState = newGameState; }
	void setSession_State(Controller::SessionState newSessionState);
	void setSession_TypeOneOrTwoPlayers(const unsigned int numPlayers);
	void setSession_TypeZeroPlayer();
	void strategy_Advanced();
	bool strategy_BlockRowOfTwo();
	void strategy_FindMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax);
	void strategy_Intermediate();
	bool strategy_IsLine(unsigned int a, unsigned int b, unsigned int c);
	bool strategy_MakeRowOfThree();
	bool strategy_MakeTwoInARow();
	void strategy_NextMoveFromCom();
	void strategy_Novice();
	
};

