#include <iostream>
#include <conio.h>
#include <string>

#include "Controller.h"
#include "utility.h"
void Controller::gamePlay_ChangeToNextPlayer() {
	if (gameState != GameState::GAME_ACTIVE) return;
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
	gameState = GameState::GAME_ABANDONNED;
	print_GameInfo();
}
void Controller::gamePlay_GetNextMove() {
	gamePlay_SetMoveIsLegal();
	if (players[(currentPlayer)].isCom()) strategy_NextMoveFromCom();
	else prompt_NextMove();
}
void Controller::gamePlay_NewGame() {
	if (!players[cPlayerNumOne].canPlay() || !players[cPlayerNumTwo].canPlay()) {
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::SESSION_REQUEST_NEW);
	}
	else {
		gameState = GameState::GAME_ACTIVE;
		gameResult.setOutcome(GameResult::Outcomes::eUndefined);
	}
}
void Controller::gamePlay_NextMove() {
	if (gameState != GameState::GAME_ACTIVE) return;
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
			gameState = GameState::GAME_TIED;
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
	if (ttt.getEntry(a) == players[0].getSymbol()) winner = static_cast<unsigned int>(cPlayerNumOne);
	else winner = static_cast<unsigned int>(cPlayerNumTwo);
	gameState = GameState::GAME_WON;
	gameResult.setSession_Num(sessionNum);
	gameResult.setOutcome(GameResult::Outcomes::eWin);
}
void Controller::gamePlay_UpdatePlayerResults() {
	if (gameResult.getOutcome() == GameResult::Outcomes::eUndefined) return;
	//Add the results of the current game to each player
	players[winner].enterResult(gameResult);
}
void Controller::gamePlay_UpdateEntries(){
	if(ttt.addNewEntry(userEntry - 48, players[static_cast<unsigned int>(currentPlayer)].getSymbol())) gamePlay_SetMoveIsLegal();
}
void Controller::init_Controller() {
	init_Utility();
	init_Game();
	init_Session();
	init_Players();
	init_GameInfo();
	init_prompt_s();
}
void Controller::init_Game() {
	ttt.setDividerDouble();
	gameState = GameState::GAME_PENDING;
}
void Controller::init_GameInfo() {
	gameInfoPos.setV(ttt.getLnieAfterGrid());
	gameInfoPos.setH(0);
	gameInfoPos.setVlen(2);
	gameInfoPos.setHlen(cMaxGameInfoChars);

	for (auto i = 0; i < cMaxGameInfoChars; i++)
		gameInfoBlankingStr.push_back(' ');
}
void Controller::init_Players()
{
	players.push_back(Player());	// create player 1
	players.push_back(Player());	// create player 2
	players[0].setSymbol(PLAYER_NUM1_DEFAULT_SYMBOL);
	players[1].setSymbol(PLAYER_NUM2_DEFAULT_SYMBOL);
}
void Controller::init_prompt_s() {
	//Set up blanking string for user input
	for (auto i = 0; i < cMaxUserInputChars; i++)
		inputprompt_BlankingStr.push_back(' ');
	
	//Session info prompt_s
	sessionInputPos.setV(gameInfoPos.getLineAfter());
	sessionInputPos.setH(0);
	sessionInputPos.setVlen(1);
	sessionInputPos.setHlen(cMaxUserInputChars);
	
	//Get player name prompt_s
	playerNameInputPos.push_back(Utility::ConsolePos()); //input pos for player 1 name prompt_
	playerNameInputPos.push_back(Utility::ConsolePos()); //input pos for player 2 name prompt_
	playerNameInputPos[cPlayerNumOne].setV(sessionInputPos.getLineAfter());
	playerNameInputPos[cPlayerNumOne].setH(0);
	playerNameInputPos[cPlayerNumOne].setVlen(1);
	playerNameInputPos[cPlayerNumOne].setHlen(cMaxUserInputChars);
	playerNameInputPos[cPlayerNumTwo].setV(playerNameInputPos[cPlayerNumOne].getLineAfter());
	playerNameInputPos[cPlayerNumTwo].setH(0);
	playerNameInputPos[cPlayerNumTwo].setVlen(1);
	playerNameInputPos[cPlayerNumTwo].setHlen(cMaxUserInputChars);

	//Post game prompt_s
	postGameInputPos.setV(gameInfoPos.getLineAfter());
	postGameInputPos.setH(0);
	postGameInputPos.setVlen(4);
	postGameInputPos.setHlen(cMaxUserInputChars);
}
void Controller::init_Session() {
	sessionState = SessionState::SESSION_PENDING;
	sessionNum = 0;
}
void Controller::init_Utility() {
	utility.randomSeed();
}
void Controller::print_CurrentPlayerName() {
	std::cout << players[static_cast<unsigned int>(currentPlayer)].getName();
	std::cout << "( " << players[static_cast<unsigned int>(currentPlayer)].getSymbol() << " )";
}
void Controller::print_GameInfo() {
	utility.cursorTo(gameInfoPos.getV(), gameInfoPos.getH());
	utility.storeCursorPos();
	std::cout << gameInfoBlankingStr;
	utility.restoreStoredCursorPos();
	switch (gameState) {
	case GameState::GAME_PENDING:
		std::cout << "NEW GAME!!!\n";
		break;
	case GameState::GAME_ABANDONNED:
		std::cout << "Game Cancelled\n";
		break;
	case GameState::GAME_WON:
		std::cout << players[winner].getName() << " won!!!\n";
		break;
	case GameState::GAME_TIED:
		std::cout << "Bummer, it's a tie\n";
		break;
	default:
		break;
	}
}
bool Controller::prompt_ProcessInput() {
	switch (userEntry) {
	case 'a':
	case 'A':
		//Automated session 


		gamePlay_EndCurrentGame();
		setSession_State(SessionState::SESSION_OVER);
		setSession_TypeZeroPlayer();
		break;
	case 'b':
	case 'B':
		//end current game, change sides
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::SESSION_OVER);
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
		sessionState = SessionState::SESSION_REQUEST_NEW;
		break;
	case 'p':
	case 'P':
		//Play another
		gamePlay_EndCurrentGame();
		setGameState(GameState::GAME_PENDING);
		break;
	case 'q':
	case 'Q':
		//End this game, End this session
		gamePlay_EndCurrentGame();
		setSession_State(SessionState::SESSION_OVER);
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
void Controller::run(){
	gamePlay_DrawGame();
	setSession_State(SessionState::SESSION_REQUEST_NEW);
	while (!isSessionOver()){
		//Session loop
		if(isSessionRequestNew()) prompt_NewSessionType();
		while (1) {
			//Automated play loop
			gamePlay_NewGame();
			if (isGameActive()) {
				ttt.blank();
				gamePlay_RandomizeCurrentPlayer();
			}
			while (isGameActive()) {
				//Game loop
				gamePlay_NextMove();
				if (isSessionAutomated()) {
					if (prompt_GetAsyncInput()) {
						gamePlay_EndCurrentGame();
						setSession_State(SessionState::SESSION_REQUEST_NEW);
					}
				}
				//End of game loop
			}
			if (isSessionAutomated()) setGameState(GameState::GAME_PENDING);
			if (isGamePending()) continue;
			else break;
			//End of automated play loop
		}
		if (isSessionOver()) break;
			
		//Todo Show cumulative Scores, 

		while (1) {
			prompt_PostGame(); //Play another, change sides, new players, quit?
			if (prompt_ProcessInput()) {
				prompt_ClearPostGameText();
				break;
			}
		}
		//Todo: Save score history for each player?

		if (isSessionOver()) break;
	}
	std::cout << "Thanks for playing\n";
}
void Controller::setSession_State(Controller::SessionState newSessionState) {
	if ((newSessionState != sessionState) 
		&& ((newSessionState == SessionState::SESSION_ONE_PLAYER)
			|| (newSessionState == SessionState::SESSION_TWO_PLAYERS)
			|| (newSessionState == SessionState::SESSION_AUTOMATED)
			)
		) sessionNum++;
	sessionState = newSessionState;
}
void Controller::setSession_TypeOneOrTwoPlayers(const unsigned int numPlayers) {
	//1 & 2 player sessions both have humans as player[0]
	sessionState = SessionState::SESSION_ONE_PLAYER;
	players[cPlayerNumOne].setHuman();
	prompt_PlayerName(cPlayerNumOne);
	if (numPlayers == 1) players[cPlayerNumTwo].setPlayer(cComPlayer, cPlayer2DefaultName);
	else {
		//2 players
		sessionState = SessionState::SESSION_TWO_PLAYERS;
		players[cPlayerNumTwo].setHuman();
		prompt_PlayerName(cPlayerNumTwo);
	}
}
void Controller::setSession_TypeZeroPlayer(){
	sessionState = SessionState::SESSION_AUTOMATED;
	players[0].setPlayer(cComPlayer, cPlayer1DefaultName);
	players[1].setPlayer(cComPlayer, cPlayer2DefaultName);
}
void Controller::prompt_ClearPostGameText() {
	utility.cursorTo(postGameInputPos.getV(), postGameInputPos.getH()); 
	for (auto i = 0; i < postGameInputPos.getVlen(); i++) {
		std::cout << inputprompt_BlankingStr;
		if (i != postGameInputPos.getVlen() - 1) std::cout << std::endl;
	}
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
	utility.cursorTo(sessionInputPos.getV(), sessionInputPos.getH());
	utility.storeCursorPos();
	std::string howManyPlayersStr("How many players? (0, 1, 2) ");
	while (numPlayers > 2) {
		std::cout << howManyPlayersStr;
		numPlayers = _getch() - 48;
		utility.restoreStoredCursorPos();
	}
	std::cout << inputprompt_BlankingStr;
	utility.restoreStoredCursorPos();
	std::cout << numPlayers << " Players";
	if (numPlayers == 0) setSession_TypeZeroPlayer();
	else setSession_TypeOneOrTwoPlayers(numPlayers);
}
void Controller::prompt_NextMove() {
	unsigned int inputCode;
	utility.cursorTo(gameInfoPos.getV(), gameInfoPos.getH());
	utility.storeCursorPos();
	while (1) {
		utility.restoreStoredCursorPos();
		std::cout << inputprompt_BlankingStr;
		utility.restoreStoredCursorPos();
		print_CurrentPlayerName(); std::cout << "'s move. Enter cell number...";
		inputCode = _getch();
		if (prompt_SetUserEntryFromAlphaNumKeyCode(inputCode)) break;
	}
}
void Controller::prompt_PlayerName(unsigned int playerNum) {
	if (playerNum > cPlayerNumTwo) return;
	std::string inputStr;
	std::string enterNameStr("Enter Player" + std::to_string(playerNum + 1) + std::string(" name ( ") + players[playerNum].getSymbol() + " ): ");
	utility.cursorTo(playerNameInputPos[playerNum].getV(), playerNameInputPos[playerNum].getH());
	utility.storeCursorPos();
	while(1){
		std::cout << enterNameStr;
		std::cin >> std::ws;  // eat up any leading white spaces
		std::cin >> inputStr;
		utility.restoreStoredCursorPos();
		if (inputStr.size() > 0) {
			//Name entered
			std::cout << inputprompt_BlankingStr;
			utility.restoreStoredCursorPos();
			players[playerNum].setName(inputStr);
			std::cout << players[playerNum].getName() << " is Player" << playerNum + 1 << " ( " << players[playerNum].getSymbol() << "s )";
			break;
		}
	}
}
void Controller::prompt_PostGame() {
	//play another, change sides, new players, quit?
	utility.cursorTo(postGameInputPos.getV(), postGameInputPos.getH());
	utility.storeCursorPos();
	prompt_ClearPostGameText();
	utility.restoreStoredCursorPos();
	std::cout << "Play again(p)\nChange sides(c)\nNew players(n)\nQuit(q)";
	while (1) {
		if (prompt_SetUserEntryFromAlphaKeyCode(_getch())) break;
	}
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
}
void Controller::strategy_Novice() {
	const std::vector<unsigned int> emptyCells = std::move(ttt.getEmptyCells());
	if (emptyCells.size() == 1) prompt_SetUserEntryFromCellNum(emptyCells[0]);
	else if (prompt_SetUserEntryFromCellNum(emptyCells[rand() % emptyCells.size()])) return;
	else prompt_SetUserEntryFromAlphaKeyCode('b');
}
