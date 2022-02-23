#pragma once
#include <vector>
//#include<cstdlib>


#include "GameResult.h"
#include "Outcomes.h"
#include "Player.h"
#include "score.h"
#include "ticktacktoegrid.h"
#include "util_console.h"

class Controller
{
	//Players
	std::vector<Player>	players;
	const unsigned int	cPlayerNumOne = 0;
	const unsigned int	cPlayerNumTwo = 1;
	const unsigned int  cMaxPlayers = 2;
	const char*			cPlayer1DefaultName	= "Com1";	//Needed to name com players
	const char*			cPlayer2DefaultName	= "Com2";	//Needed to name com players
	const bool			cComPlayer			= true;
	const bool			cHumanPlayer		= !cComPlayer;
	unsigned int		currentPlayer;
	unsigned int		winner;
	
	//Game
	TickTackToeGrid	ttt;
	const char		PLAYER_NUM1_DEFAULT_SYMBOL = 'X';
	const char		PLAYER_NUM2_DEFAULT_SYMBOL = 'O';

	//Game State
	struct GameState {
		enum class State{
			GAME_PENDING,
			GAME_ACTIVE,
			GAME_CANCELLED,
			GAME_WON,
			GAME_TIED
		};
		//default constructor
		GameState() : state(State::GAME_PENDING) {}
		//Destuctor
		~GameState()=default;
		//Copy constructor
		GameState(const GameState& other) {state = other.state;}
		//Move constructor
		GameState(GameState&& rhs) noexcept { state = std::exchange(rhs.state, State::GAME_PENDING); }
		//By-val assignment operator overload
		GameState& operator=(GameState copy) {
			copy.swap(*this);
			return *this;
		}
		//Member swap method
		void swap(GameState& rhs) noexcept {
			using std::swap;
			swap(state, rhs.state);
		}
		// Allow switch and comparisons
		constexpr operator State() const { return state; }
		//Friend 2 parameter swap function wrapper for member swap method
		friend void swap(GameState& a, GameState& b) noexcept { a.swap(b); }

		State getState() { return state; }
		void setState(State newState) { state = newState; }
		
	private:
		State state;
	};
	
	GameState		gameState;
	unsigned int	gameNum;
	bool			moveLegal;
	
	//Game Result
	GameResult gameResult;
	
	//Scoreboard
	std::vector<Score> scoreboard;

	///Session
	struct SessionState {
		enum class State {
			SESSION_PENDING,
			SESSION_ONE_PLAYER,
			SESSION_TWO_PLAYERS,
			SESSION_AUTOMATED,
			SESSION_REQUEST_NEW,
			SESSION_OVER
		};
		//Default constructor
		SessionState() : state(State::SESSION_PENDING) {}
		//Destuctor
		~SessionState()=default;
		//Copy constructor
		SessionState(const SessionState& other) { state = other.state; }
		//Move constructor
		SessionState(SessionState&& rhs) noexcept { state = std::exchange(rhs.state, State::SESSION_PENDING); }
		//By-val assignment operator overload
		SessionState& operator=(SessionState copy) {
			copy.swap(*this);
			return *this;
		}
		//Member swap method
		void swap(SessionState& rhs) noexcept {
			using std::swap;
			swap(state, rhs.state);
		}
		// Allow switch and comparisons
		constexpr operator State() const { return state; }
		//Friend 2 parameter swap function wrapper for member swap method
		friend void swap(SessionState& a, SessionState& b) noexcept { a.swap(b); }

		State getState() { return state; }
		void setState(State newState) { state = newState; }

	private:
		State state;
	};
	
	SessionState sessionState;
	unsigned int sessionNum;
	
	//Prompt: General
	unsigned int userEntry;
	const short cMaxUserInputChars = 40;

	//Game info
	util_console::ConsoleTextBlock gameInfoBlock;
	const short cMaxGameInfoChars = 40;

	//GamePlay info
	util_console::ConsoleTextBlock gamePlayInfoBlock;
	const short cMaxGamePlayInfoChars = 60;

	//Info Session
	util_console::ConsoleTextBlock sessionInfoBlock;
	const short cMaxSessionInfoChars = 40;

	//Prompt: Player names
	std::vector<util_console::ConsoleTextBlock> playerNamePromptBlock;

	//Prompt: Post game
	util_console::ConsoleTextBlock postGamePromptBlock;

	//Prompt: Exit
	util_console::ConsoleTextBlock exitMsgBlock;

	//util_console
	util_console utility;

public:
	Controller() { init_Controller(); }

	void run();
	
private:
	void gamePlay_ChangeSides();
	void gamePlay_ChangeToNextPlayer();
	void gamePlay_DrawGame() { ttt.print_EntryList(); ttt.drawGrid(); print_GameInfo(); }
	void gamePlay_EndCurrentGame();
	unsigned int gamePlay_getLoserIndex() { return winner == cPlayerNumOne ? cPlayerNumTwo : cPlayerNumOne; }
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
	void init_Prompts();
	void init_Session();
	void init_util_console();
	bool isGameActive() { return gameState == GameState::State::GAME_ACTIVE; }
	bool isGamePending() { return gameState == GameState::State::GAME_PENDING; }
	bool isSessionAutomated() { return sessionState == SessionState::State::SESSION_AUTOMATED; }
	bool isSessionOnePlayer() { return sessionState == SessionState::State::SESSION_ONE_PLAYER; }
	bool isSessionOver() { return sessionState == SessionState::State::SESSION_OVER; }
	bool isSessionPending() {return sessionState == SessionState::State::SESSION_PENDING; }
	bool isSessionRequestNew() { return sessionState == SessionState::State::SESSION_REQUEST_NEW; }
	bool isSessionTwoPlayers() { return sessionState == SessionState::State::SESSION_TWO_PLAYERS; }
	unsigned int playerCurrentPlayerOpponent() { return currentPlayer == cPlayerNumOne ? cPlayerNumTwo : cPlayerNumOne; }
	const char playerCurrentPlayerSymbol() { return players[(currentPlayer)].getSymbol(); }
	const char playerOpponentSymbol() { return players[(playerCurrentPlayerOpponent())].getSymbol(); }
	void print_CurrentPlayerName();
	void print_GameInfo();
	void printPlayerInfo(unsigned int playerNum);
	void printSessionInfo();
	void prompt_AutomatedSessionInterrupted();
	void prompt_ExitMsg();
	bool prompt_GetAsyncInput();
	void prompt_NewSessionType();
	void prompt_NextMove();
	void prompt_PostGame();
	void prompt_PlayerName(unsigned int player);
	bool prompt_ProcessInput();
	bool prompt_SetUserEntryFromCellNum(unsigned int cellNum);
	bool prompt_SetUserEntryFromAlphaKeyCode(char keyCode);
	bool prompt_SetUserEntryFromAlphaNumKeyCode(char keyCode);
	void setGameState(GameState::State newGameState);
	void setSession_State(SessionState::State newSessionState);
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

