#include <chrono>
#include <conio.h>
#include <iostream>
#include <string>
#include <thread>

#include "Controller.h"
#include "Outcomes.h"
#include "util_console.h"
void Controller::gamePlay_ChangeToNextPlayer() {
	if (!isGameActive()) return;
	if (currentPlayer == cPlayerNumOne) currentPlayer = cPlayerNumTwo;
	else currentPlayer = cPlayerNumOne;
}
void Controller::gamePlay_ChangeSides() {
	//Todo
	//Implement copy constructor and copy assignment
	//Implement move constructor and move assignment
	//Implement swaps
	// 
	//Tell user what happened

	//swap(player[0], player[1]);
}
void Controller::gamePlay_EndCurrentGame() {
	if(isGameActive() || isGamePending())
		setGameState(GameState::State::GAME_CANCELLED);
		
	print_GameInfo();
}
void Controller::gamePlay_GetNextMove() {
	gamePlay_SetMoveIsLegal();
	if (players[(currentPlayer)].isCom()) strategy_NextMoveFromCom();
	else prompt_NextMove();
}
void Controller::gamePlay_NewGame() {
	ttt.blank();
	if (!players[cPlayerNumOne].canPlay() || !players[cPlayerNumTwo].canPlay()) {
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::State::SESSION_REQUEST_NEW);
	}
	else {
		setGameState(GameState::State::GAME_ACTIVE);
		gameNum++;
		gamePlay_RandomizeCurrentPlayer();
		//Update display
		printSessionInfo();
		printPlayerInfo(cPlayerNumOne);
		printPlayerInfo(cPlayerNumTwo);
	}
}
void Controller::gamePlay_NextMove() {
	if (!isGameActive()) return;
	gamePlay_GetNextMove();
	prompt_ProcessInput();
	if (gamePlay_IsMoveLegal()) {
		if (gamePlay_ProcessMove()) {
			gamePlay_UpdatePlayerResults();
			print_GameInfo();
		}
		if(isGameActive()) gamePlay_ChangeToNextPlayer();
	}
}
bool Controller::gamePlay_ProcessMove() {
	//Check grid for win or tie, set game state accordingly
	//Game over
	//Was it a win? if not, it is a tie
	bool foundResult = true;
	bool isWin = false;
	while(1) {
		//Might be a tie
		if (ttt.isGridFull()) {
			setGameState(GameState::State::GAME_TIED);
			break;
		}
		//Rows 0=1=2, 3=4=5, 6=7=8
		if (isWin = strategy_IsLine(0, 1, 2)) break;
		if (isWin = strategy_IsLine(3, 4, 5)) break;
		if (isWin = strategy_IsLine(6, 7, 8)) break;
		//Colums 0=3=6, 1=4=7, 2=5=8
		if (isWin = strategy_IsLine(0, 3, 6)) break;
		if (isWin = strategy_IsLine(1, 4, 7)) break;
		if (isWin = strategy_IsLine(2, 5, 8)) break;
		//Diagonals 0=4=8, 2=4=6
		if (isWin = strategy_IsLine(0, 4, 8)) break;
		if (isWin = strategy_IsLine(2, 4, 6)) break;	
		foundResult = false;
		break; //loop only runs once
	}
	return foundResult;
}
void Controller::gamePlay_RandomizeCurrentPlayer() {
	currentPlayer = ((double)rand() / (RAND_MAX)) < 0.5f ? cPlayerNumOne : cPlayerNumTwo;
}
void Controller::gamePlay_SetWinner(unsigned int a) {
	//a is index of winning symbol
	winner = ttt.getEntry(a) == players[cPlayerNumOne].getSymbol() ? cPlayerNumOne : cPlayerNumTwo;
	setGameState(GameState::State::GAME_WON);
}
void Controller::gamePlay_UpdatePlayerResults() {
	if (gameResult.getOutcome() == Outcomes::eUndefined) return;
	//Add the results of the current game to each player
	players[winner].enterPlayerResult(PlayerResult(gameResult.getSessionNum(), gameResult.getOutcome()));
	if(gameResult.getOutcome() == Outcomes::eTie)
		players[gamePlay_getLoserIndex()].enterPlayerResult(PlayerResult(gameResult.getSessionNum(), Outcomes::eTie));
}
void Controller::gamePlay_UpdateEntries(){
	if(ttt.addNewEntry(userEntry - 48, players[static_cast<unsigned int>(currentPlayer)].getSymbol())) gamePlay_SetMoveIsLegal();
}
void Controller::init_Controller() {
	//Order of these init routines matter
	init_util_console();
	//Establish where things will be drawn/written-
	init_GameInfo();
	init_Prompts();
	init_Game();
	init_Players();
	init_Session();
}
void Controller::init_Game() {
	ttt.setDividerDouble();
	gamePlay_DrawGame();
	setGameState(GameState::State::GAME_PENDING);
}
void Controller::init_GameInfo() {
	gameInfoBlock.setV(ttt.getLineAfterGrid());
	gameInfoBlock.setH(0);
	gameInfoBlock.setVlen(2);
	gameInfoBlock.setHlen(cMaxGameInfoChars);
	gameInfoBlock.fillBlankingStr(cMaxGameInfoChars);
	gameInfoBlock.blank();
	
	gamePlayInfoBlock.setV(gameInfoBlock.getLineAfter());
	gamePlayInfoBlock.setH(0);
	gamePlayInfoBlock.setVlen(2);
	gamePlayInfoBlock.setHlen(cMaxGameInfoChars);
	gamePlayInfoBlock.fillBlankingStr(cMaxGamePlayInfoChars);
	gamePlayInfoBlock.blank();
}
void Controller::init_Players()
{
	players.push_back(Player());	// create player 1
	players.push_back(Player());	// create player 2
	players[0].setSymbol(PLAYER_NUM1_DEFAULT_SYMBOL);
	players[1].setSymbol(PLAYER_NUM2_DEFAULT_SYMBOL);
}
void Controller::init_Prompts() {
	//Session prompts
	sessionInfoBlock.setV(gameInfoBlock.getLineAfter());
	sessionInfoBlock.setH(0);
	sessionInfoBlock.setVlen(1);
	sessionInfoBlock.setHlen(cMaxSessionInfoChars);
	sessionInfoBlock.fillBlankingStr(cMaxUserInputChars);
	sessionInfoBlock.blank();
	
	//Player name prompts
	playerNamePromptBlock.push_back(util_console::ConsoleTextBlock()); //input pos for player 1 name prompt
	playerNamePromptBlock.push_back(util_console::ConsoleTextBlock()); //input pos for player 2 name prompt
	playerNamePromptBlock[cPlayerNumOne].setV(sessionInfoBlock.getLineAfter());
	playerNamePromptBlock[cPlayerNumOne].setH(0);
	playerNamePromptBlock[cPlayerNumOne].setVlen(1);
	playerNamePromptBlock[cPlayerNumOne].setHlen(cMaxUserInputChars);
	playerNamePromptBlock[cPlayerNumOne].fillBlankingStr(cMaxUserInputChars);
	playerNamePromptBlock[cPlayerNumOne].blank();

	playerNamePromptBlock[cPlayerNumTwo].setV(playerNamePromptBlock[cPlayerNumOne].getLineAfter());
	playerNamePromptBlock[cPlayerNumTwo].setH(0);
	playerNamePromptBlock[cPlayerNumTwo].setVlen(1);
	playerNamePromptBlock[cPlayerNumTwo].setHlen(cMaxUserInputChars);
	playerNamePromptBlock[cPlayerNumTwo].fillBlankingStr(cMaxUserInputChars);
	playerNamePromptBlock[cPlayerNumTwo].blank();

	//Post game prompts
	postGamePromptBlock.setV(gameInfoBlock.getLineAfter());
	postGamePromptBlock.setH(0);
	postGamePromptBlock.setVlen(4);
	postGamePromptBlock.setHlen(cMaxUserInputChars);
	postGamePromptBlock.fillBlankingStr(cMaxUserInputChars);
	postGamePromptBlock.blank();

	//Exit msg prompt
	exitMsgBlock.setV(gameInfoBlock.getLineAfter());
	exitMsgBlock.setH(0);
	exitMsgBlock.setVlen(1);
	exitMsgBlock.setHlen(cMaxUserInputChars);
	exitMsgBlock.fillBlankingStr(cMaxUserInputChars);
	exitMsgBlock.blank();

}
void Controller::init_Session() {
	setSession_State(SessionState::State::SESSION_REQUEST_NEW);
	gameNum = 0;
}
void Controller::init_util_console() {
	utility.randomSeed();
}
void Controller::print_CurrentPlayerName() {
	std::cout << players[static_cast<unsigned int>(currentPlayer)].getName();
	std::cout << "( " << players[static_cast<unsigned int>(currentPlayer)].getSymbol() << " )";
}
void Controller::print_GameInfo() {
	gameInfoBlock.blank();
	std::cout << "Game " << gameNum << "  ";
	switch (gameState) {
	case GameState::State::GAME_PENDING:
		std::cout << "NEW GAME!!!\n";
		break;
	case GameState::State::GAME_CANCELLED:
		std::cout << "Game Cancelled\n";
		break;
	case GameState::State::GAME_WON:
		std::cout << players[winner].getName() << " won!!!\n";
		break;
	case GameState::State::GAME_TIED:
		std::cout << "Bummer, it's a tie\n";
		break;
	default:
		break;
	}
}
void Controller::printPlayerInfo(unsigned int playerNum){
	playerNamePromptBlock[playerNum].blank();
	std::cout << players[playerNum].getName() << " is Player" << playerNum + 1 << " ( " << players[playerNum].getSymbol() << "s )";
}
void Controller::printSessionInfo() {
	sessionInfoBlock.blank();
	switch (sessionState) {
	case SessionState::State::SESSION_PENDING:

		break;
	case SessionState::State::SESSION_ONE_PLAYER:
		std::cout << "1 Player vs Com";
		break;
	case SessionState::State::SESSION_TWO_PLAYERS:
		std::cout << "2 Players";
		break;
	case SessionState::State::SESSION_AUTOMATED:
		std::cout << "Automated Play";
		break;
	case SessionState::State::SESSION_REQUEST_NEW:

		break;
	case SessionState::State::SESSION_OVER:

		break;
	default:
		break;

	}
}
void Controller::prompt_AutomatedSessionInterrupted() {
	if (prompt_GetAsyncInput()) {
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::State::SESSION_REQUEST_NEW);
	}
}
void Controller::prompt_ExitMsg() {
	postGamePromptBlock.blank();
	exitMsgBlock.blank();
	std::cout << "So long. Thanks for playing!\n\n";
}
bool Controller::prompt_GetAsyncInput() {
	//Peek first, if char there read it else return false
	//std::cin >> std::ws;  // eat up any leading white spaces
	if (_kbhit()) {
		prompt_SetUserEntryFromAlphaKeyCode(_getch());
		prompt_ProcessInput(); //ignore whether a command was found
		return true; //return true regardless of whether the key is a command key
	}
	return false;
}
void Controller::prompt_NewSessionType() {
	int numPlayers = 99;	//ensures user prompt_
	sessionInfoBlock.blank();
	std::cout << "How many players? (0, 1, 2) ";
	utility.storeCursorPos();
	while (numPlayers > 2) {
		numPlayers = _getch() - 48;
		utility.restoreStoredCursorPos();
	}
	if (numPlayers == 0) setSession_TypeZeroPlayer();
	else setSession_TypeOneOrTwoPlayers(numPlayers);
}
void Controller::prompt_NextMove() {
	gameInfoBlock.blank();
	print_CurrentPlayerName(); std::cout << "'s move. Enter cell number...";
	utility.storeCursorPos();
	while (1) {
		if (prompt_SetUserEntryFromAlphaNumKeyCode(_getch())) break;
		utility.restoreStoredCursorPos();
	}
}
void Controller::prompt_PlayerName(unsigned int playerNum) {
	if (playerNum > cPlayerNumTwo) return;
	playerNamePromptBlock[playerNum].blank();
	std::cout << "Enter Player" << playerNum + 1 << " name ( " << players[playerNum].getSymbol() << " ): ";
	utility.storeCursorPos();
	std::string inputStr;
	while(1){
		std::cin >> std::ws;  // eat up any leading white spaces
		std::cin >> inputStr;
		utility.restoreStoredCursorPos();
		if (inputStr.size() > 0) {
			//Name entered
			players[playerNum].setName(inputStr);
			printPlayerInfo(playerNum);
			break;
		}
	}
}
void Controller::prompt_PostGame() {
	//play another, change sides, new players, quit?
	postGamePromptBlock.blank();
	std::cout << "Play again(p)\nChange sides(c)\nNew players(n)\nQuit(q)";
	utility.storeCursorPos();
	while (1) {
		if (prompt_SetUserEntryFromAlphaKeyCode(_getch())) break;
		utility.restoreStoredCursorPos();	
	}
}
bool Controller::prompt_ProcessInput() {
	switch (userEntry) {
	case 'b':
	case 'B':
		//end current game, change sides
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::State::SESSION_OVER);
		std::cout << "ERROR!! Cant find valid entry\n";
		break;
	case 'c':
	case 'C':
		//end current game, change sides
		gamePlay_EndCurrentGame();
		gamePlay_ChangeSides();
		break;
	case 'e':
	case 'E':
		//End current game, same session
		gamePlay_EndCurrentGame();
		break;
	case 'n':
	case 'N':
		//End this game, start new session
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::State::SESSION_REQUEST_NEW);
		break;
	case 'p':
	case 'P':
		//Play another
		gamePlay_EndCurrentGame();
		setGameState(GameState::State::GAME_PENDING);

		//Todo print out player playerNamePromptBlock


		break;
	case 'q':
	case 'Q':
		//End this game, End this session
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::State::SESSION_OVER);
		break;
	case 'x':
	case 'X':
		gamePlay_EndCurrentGame();
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
		gamePlay_UpdateEntries();
		break;
	default:
		return false;
		break;
	}
	return true;
}
bool Controller::prompt_SetUserEntryFromCellNum(unsigned int cellNum) { 
	if (cellNum < ttt.GetNumCells()) {
		userEntry = cellNum + 48;
		return true;
	}
	return false;
}
bool Controller::prompt_SetUserEntryFromAlphaKeyCode(char keyCode) {
	if (std::isalpha(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
bool Controller::prompt_SetUserEntryFromAlphaNumKeyCode(char keyCode) {
	if (std::isalpha(keyCode) || std::isdigit(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
void Controller::run(){
	while (!isSessionOver()){
		//Session loop
		if(isSessionRequestNew()) prompt_NewSessionType();
		while (1) {
			//Automated play loop
			gamePlay_NewGame();
			while (isGameActive()) {
				//Game loop
				gamePlay_NextMove();
				if (isSessionAutomated()) prompt_AutomatedSessionInterrupted();
				//End of game loop
			}
			if (isSessionAutomated()) setGameState(GameState::State::GAME_PENDING);
			if (isGamePending()) continue;
			else break;
			//End of automated play loop
		}
		if (isSessionOver()) break;
			
		//Todo Show cumulative Scores, 

		while (1) {
			prompt_PostGame(); //Play another, change sides, new players, quit?
			if (prompt_ProcessInput()) {
				postGamePromptBlock.blank();
				break;
			}
		}
		//Todo: Save score history for each player?

		if (isSessionOver()) break;
	}
	prompt_ExitMsg();
}
void Controller::setGameState(GameState::State newGameState) {
	gameState.setState(newGameState);
	switch (gameState) {
	case GameState::State::GAME_ACTIVE:
		gameResult.setOutcome(Outcomes::eUndefined);
		break;
	case GameState::State::GAME_TIED:
		gameResult.setSessionNum(gameNum);
		gameResult.setOutcome(Outcomes::eTie);
		break;
	case GameState::State::GAME_WON:
		gameResult.setSessionNum(gameNum);
		gameResult.setOutcome(Outcomes::eWin);
		break;
	default:
		break;
	}
}
void Controller::setSession_State(SessionState::State newSessionState) {
	if ((newSessionState != sessionState) 
		&& (   (newSessionState == SessionState::State::SESSION_ONE_PLAYER)
			|| (newSessionState == SessionState::State::SESSION_TWO_PLAYERS)
			|| (newSessionState == SessionState::State::SESSION_AUTOMATED)
			)
		) sessionNum++;
	sessionState.setState(newSessionState);
	printSessionInfo();
}
void Controller::setSession_TypeOneOrTwoPlayers(const unsigned int numPlayers) {
	if (numPlayers > cMaxPlayers) return;
	else if (numPlayers == 1) setSession_State(SessionState::State::SESSION_ONE_PLAYER);
	else setSession_State(SessionState::State::SESSION_TWO_PLAYERS);
	//1 & 2 player sessions both have humans as player[0]
	players[cPlayerNumOne].setHuman();
	prompt_PlayerName(cPlayerNumOne);
	if (numPlayers == 1) players[cPlayerNumTwo].setPlayer(cComPlayer, cPlayer2DefaultName);
	else {
		//2 players
		players[cPlayerNumTwo].setHuman();
		prompt_PlayerName(cPlayerNumTwo);
	}
}
void Controller::setSession_TypeZeroPlayer(){
	setSession_State(SessionState::State::SESSION_AUTOMATED);
	players[0].setPlayer(cComPlayer, cPlayer1DefaultName);
	players[1].setPlayer(cComPlayer, cPlayer2DefaultName);
}
void Controller::strategy_Advanced() {
	//Todo: replace this code with Advanced strategy_
	strategy_Novice();
}
bool Controller::strategy_BlockRowOfTwo() {
	const std::vector<unsigned int> opponentLineBlocks = (std::move(ttt.getLineCompletions(playerOpponentSymbol())));
	unsigned int maxSoFar = 0, maxCellIndex = 0;
	strategy_FindMax(opponentLineBlocks, maxSoFar, maxCellIndex);
	//If more than one line uses an open cell, block that cell preferentially
	if (maxSoFar > 0) {
		if(prompt_SetUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Controller::strategy_FindMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax){
	for (auto i = 0; i < ttt.GetNumCells(); i++) {
		if (vec[i] > maxSoFar) {
			maxSoFar = vec[i];
			indexOfMax = i;
		}
	}
}
void Controller::strategy_Intermediate() {
	//Intermediate players take any three in a row that they can,
	//else block their opponents if two in a row.
	//Otherwise, they play like a novice and choose random moves.

	//First seek wins i.e. three in a row
	if (strategy_MakeRowOfThree()) return;
	//Look for lines to block
	if (strategy_BlockRowOfTwo()) return;
	//No blocking move, try to make two in a row
	//Note getMakeTwoLines returnvector indicates how many lines-of-two each blank cell creates
	if (strategy_MakeTwoInARow()) return;
	//No blocks, no oportunistic lines, choose random
	strategy_Novice();
}
bool Controller::strategy_IsLine(unsigned int a, unsigned int b, unsigned int c) {
	if (ttt.strategy_IsLine(a, b, c)) {
		gamePlay_SetWinner(a);
		return true;
	}
	return false;
}
bool Controller::strategy_MakeRowOfThree() {
	const std::vector<unsigned int> lineCompletions = (std::move(ttt.getLineCompletions(playerCurrentPlayerSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	strategy_FindMax(lineCompletions, maxSoFar, maxCellIndex);
	//Check if winning move identified
	if (maxSoFar > 0) {
		if(prompt_SetUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
bool Controller::strategy_MakeTwoInARow() {
	const std::vector<unsigned int> makeTwoLines = (std::move(ttt.getMakeTwoLines(playerCurrentPlayerSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	strategy_FindMax(makeTwoLines, maxSoFar, maxCellIndex);
	//If more than one cell creates two in a row, use that cell preferentially
	if (maxSoFar > 0) {
		if(prompt_SetUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Controller::strategy_NextMoveFromCom() {
	//strategy_Novice();
	strategy_Intermediate();
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
void Controller::strategy_Novice() {
	const std::vector<unsigned int> emptyCells = std::move(ttt.getEmptyCells());
	if (emptyCells.size() == 1) prompt_SetUserEntryFromCellNum(emptyCells[0]);
	else if (prompt_SetUserEntryFromCellNum(emptyCells[rand() % emptyCells.size()])) return;
	else prompt_SetUserEntryFromAlphaKeyCode('b');
}
