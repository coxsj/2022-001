#include <iostream>
#include <conio.h>
#include <string>

#include "Controller.h"
#include "utility.h"

bool Controller::blockRowOfTwo() {
	const std::vector<unsigned int> opponentLineBlocks = (std::move(ttt.getLineCompletions(getOpponentSymbol())));
	unsigned int maxSoFar = 0, maxCellIndex = 0;
	findMax(opponentLineBlocks, maxSoFar, maxCellIndex);
	//If more than one line uses an open cell, block that cell preferentially
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Controller::changeToNextPlayer() {
	if (gameState != GameState::ACTIVE_GAME) return;
	if (currentPlayer == PLAYER::ONE) currentPlayer = PLAYER::TWO;
	else currentPlayer = PLAYER::ONE;
}
void Controller::changeSides() {
	//Todo
	//Implement copy constructor and copy assignment
	//Implement move constructor and move assignment
	//Implement swaps
	// 
	//Tell user what happened

	//swap(player[0], player[1]);
}
void Controller::comMoveAdvanced() {
	//Todo: replace this code with Advanced strategy
	comMoveNovice();
}
void Controller::comMoveIntermediate() {
	//Intermediate players take any three in a row that they can,
	//else block their opponents if two in a row.
	//Otherwise, they play like a novice and choose random moves.

	//First seek wins i.e. three in a row
	if (makeRowOfThree()) return;
	//Look for lines to block
	if (blockRowOfTwo()) return;
	//No blocking move, try to make two in a row
	//Note getMakeTwoLines returnvector indicates how many lines-of-two each blank cell creates
	if (makeTwoInARow()) return;
	//No blocks, no oportunistic lines, choose random
	comMoveNovice();
}
void Controller::comMoveNovice() {
	const std::vector<unsigned int> emptyCells = std::move(ttt.getEmptyCells());
	if (emptyCells.size() == 1) setUserEntryFromCellNum(emptyCells[0]);
	else if (setUserEntryFromCellNum(emptyCells[rand() % emptyCells.size()])) return;
	else setUserEntryFromAlphaKeyCode('b');
}
void Controller::endCurrentGame() {
	if (isGameActive()) gameState = GameState::GAME_ABANDONNED; 
	else GameState::PENDING_GAME;
}
void Controller::findMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax){
	for (auto i = 0; i < ttt.GetNumCells(); i++) {
		if (vec[i] > maxSoFar) {
			maxSoFar = vec[i];
			indexOfMax = i;
		}
	}
}
bool Controller::getASyncInput() {
	//Peek first, if char there read it else return false
	//std::cin >> std::ws;  // eat up any leading white spaces
	if (_kbhit()) {
		setUserEntryFromAlphaNumKeyCode(_getch());
		processInput();
		return true;
	}
	return false;
}
void Controller::getNewSessionFromUser() {
	int numPlayers = 99;	//ensures user prompt
	utility.cursorTo(getSessionInputPos.getV(), getSessionInputPos.getH());
	utility.storeCursorPos();
	std::string howManyPlayersStr("How many players? (0, 1, 2) ");
	while (numPlayers > 2) {
		std::cout << howManyPlayersStr;
		numPlayers = _getch() - 48;
		utility.restoreStoredCursorPos();
	}
	std::cout << inputPromptBlankingStr;
	utility.restoreStoredCursorPos();
	std::cout << std::endl;
	if (numPlayers == 0) setSessionStateZeroPlayer();
	else setNamesGamesWithHumans(numPlayers);
}
void Controller::getPlayerName(unsigned int playerNum) {
	if (playerNum > PLAYER_TWO) return;
	std::string inputStr;
	std::string enterNameStr("Enter Player" + std::to_string(playerNum + 1) + std::string(" name ( ") + players[playerNum].getSymbol() + " ): ");
	utility.cursorTo(getPlayerNamePos[playerNum].getV(), getPlayerNamePos[playerNum].getH());
	utility.storeCursorPos();
	while (1) {
		std::cout << enterNameStr;
		std::cin >> std::ws;  // eat up any leading white spaces
		std::cin >> inputStr;
		utility.restoreStoredCursorPos();
		if (inputStr.size() > 0) {
			std::cout << inputPromptBlankingStr;
			utility.restoreStoredCursorPos();
			players[playerNum].setName(inputStr);
			break;
		}
	}
}
void Controller::getPostGameInputFromUser() {
	//play another, change sides, new players, quit?
	std::cout << "Play again(p)\nChange sides(c)\nNew players(n)\nQuit(q)\n";
	while (1) {
		if (setUserEntryFromAlphaKeyCode(_getch())) break;
	}
}
void Controller::getMoveInput() {
	resetMoveIsLegal();
	if (players[static_cast<unsigned int>(currentPlayer)].isCom()) getMoveInputFromCom();
	else getMoveInputFromHuman();
}
void Controller::getMoveInputFromCom() {
	//comMoveNovice();
	comMoveIntermediate();
}
void Controller::getMoveInputFromHuman() {
	unsigned int inputCode;
	while (1) {
		utility.cursorTo(ttt.getLastDisplayRow() + 2, 0);	//Todo replace 1 with constant
		printCurrentPlayerName(); std::cout << "'s move. Enter cell number..." << std::endl;
		inputCode = _getch();
		if (setUserEntryFromAlphaNumKeyCode(inputCode)) break;
	}
}
void Controller::initController() {
	initUtility();
	initGame();
	initSession();
	initPlayers();
	initGameInfo();
	initGetUserInput();
}
void Controller::initGame() {
	ttt.setDividerDouble();
	gameState = GameState::PENDING_GAME;
}
void Controller::initGameInfo() {
	gameInfoPos.setV(ttt.getLastDisplayRow() + 1);
	gameInfoPos.setH(0);
	gameInfoPos.setVlen(2);
	gameInfoPos.setHlen(cMaxGameInfoChars);

	for (auto i = 0; i < cMaxGameInfoChars; i++)
		gameInfoBlankingStr.push_back(' ');
}
void Controller::initGetUserInput() {
	getSessionInputPos.setV(gameInfoPos.getLineAfter());
	getSessionInputPos.setH(0);
	getSessionInputPos.setVlen(3);
	getSessionInputPos.setHlen(cMaxUserInputChars);
	for (auto i = 0; i < cMaxUserInputChars; i++)
		inputPromptBlankingStr.push_back(' ');
	//Get player name prompts
	getPlayerNamePos.push_back(Utility::ConsolePos()); //input pos for player 1 name prompt
	getPlayerNamePos.push_back(Utility::ConsolePos()); //input pos for player 2 name prompt
	getPlayerNamePos[PLAYER_ONE].setV(getSessionInputPos.getLineAfter());
	getPlayerNamePos[PLAYER_ONE].setH(0);
	getPlayerNamePos[PLAYER_ONE].setVlen(1);
	getPlayerNamePos[PLAYER_ONE].setHlen(cMaxUserInputChars);
	getPlayerNamePos[PLAYER_TWO].setV(getPlayerNamePos[PLAYER_ONE].getLineAfter());
	getPlayerNamePos[PLAYER_TWO].setH(0);
	getPlayerNamePos[PLAYER_TWO].setVlen(1);
	getPlayerNamePos[PLAYER_TWO].setHlen(cMaxUserInputChars);
}
void Controller::initPlayers()
{
	players.push_back(Player());	// create player 1
	players.push_back(Player());	// create player 2
	players[0].setSymbol(PLAYER1_DEFAULT_SYMBOL);
	players[1].setSymbol(PLAYER2_DEFAULT_SYMBOL);
}
void Controller::initSession() {
	sessionState = SessionState::PENDING_SESSION;
}
void Controller::initUtility() {
	utility.randomSeed();
}
bool Controller::isLine(unsigned int a, unsigned int b, unsigned int c) {
	if (ttt.isLine(a, b, c)) {
		setWinner(a);
		return true;
	}
	return false;
}
bool Controller::makeRowOfThree() {
	const std::vector<unsigned int> lineCompletions = (std::move(ttt.getLineCompletions(getCurrentSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	findMax(lineCompletions, maxSoFar, maxCellIndex);
	//Check if winning move identified
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
bool Controller::makeTwoInARow() {
	const std::vector<unsigned int> makeTwoLines = (std::move(ttt.getMakeTwoLines(getCurrentSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	findMax(makeTwoLines, maxSoFar, maxCellIndex);
	//If more than one cell creates two in a row, use that cell preferentially
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Controller::newGame() {
	drawGame();
	gameState = GameState::ACTIVE_GAME;
}
void Controller::nextMove() {
	if (gameState != GameState::ACTIVE_GAME) return;
	getMoveInput();
	processInput();
	if (moveIsLegal()) {
		if(updateGameState()) printGameResult();		
		if(isGameActive()) changeToNextPlayer();
	}
}
void Controller::printCurrentPlayerName() {
	std::cout << players[static_cast<unsigned int>(currentPlayer)].getName();
	std::cout << "( " << players[static_cast<unsigned int>(currentPlayer)].getSymbol() << " )";
}
void Controller::printGameResult() {
	utility.cursorTo(gameInfoPos.getV(), gameInfoPos.getH());
	utility.storeCursorPos();
	std::cout << gameInfoBlankingStr;
	switch (gameState) {
	case GameState::PENDING_GAME:
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
	if(isSessionOver()) std::cout << "Thanks for playing\n";
}
bool Controller::processInput() {
	switch (userEntry) {
	case 'a':
	case 'A':
		//Automated session 


		endCurrentGame();
		setSessionEnd();
		setSessionStateZeroPlayer();
		break;
	case 'b':
	case 'B':
		//end current game, change sides
		endCurrentGame();
		setSessionEnd();
		std::cout << "ERROR!! Cant find valid entry\n";
		break;
	case 'c':
	case 'C':
		//end current game, change sides
		endCurrentGame();
		changeSides();
		break;
	case 'e':
	case 'E':
		//End current game, same session
		endCurrentGame();
		break;
	case 'n':
	case 'N':
		//End this game, start new session
		endCurrentGame();
		sessionState = SessionState::NEW_SESSION_REQUESTED;
		break;
	case 'p':
	case 'P':
		//Play another
		endCurrentGame();
		setGamePending();
		break;
	case 'q':
	case 'Q':
		//End this game, End this session
		endCurrentGame();
		setSessionEnd();
		break;
	case 'x':
	case 'X':
		endCurrentGame();
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
		updateEntries();
		break;
	default:
		return false;
		break;
	}
	return true;
}
void Controller::randomizeCurrentPlayer() {
	currentPlayer = ((double)rand() / (RAND_MAX)) < 0.5f ? PLAYER::ONE : PLAYER::TWO;
}
void Controller::run(){
	while (!isSessionOver()) {
		getNewSessionFromUser();
		//Draw new game
		newGame();
		while (1){
			//Session loop
			while (1) {
				//Automated play loop
				ttt.blank();
				randomizeCurrentPlayer();
				while (isGameActive()) {
					//Game loop
					nextMove();
				}
				if (sessionState == SessionState::AUTOMATED_SESSION) {
					if (getASyncInput()) endCurrentGame();
					else setGamePending();
				}
				if (isGamePending()) continue;
				else break;
				//End of automated play loop
			}
			if (newSessionRequest() || isSessionOver()) break;
			
			//Todo Show cumulative Scores, 

			//Play another, change sides, new players, quit?
			while (1) {
				getPostGameInputFromUser();
				if (processInput()) break;
			}
			if (newSessionRequest() || isSessionOver()) break;
		}
	}
	//Todo: Save score history for each player?
}
void Controller::setNamesGamesWithHumans(const unsigned int numPlayers) {
	//1 & 2 player sessions both have humans as player[0]
	sessionState = SessionState::ONE_PLAYER_SESSION;
	players[PLAYER_ONE].setHuman();
	getPlayerName(PLAYER_ONE);
	if (numPlayers == 1) players[PLAYER_TWO].setPlayer(COM_PLAYER, PLAYER2_DEFAULT_NAME);
	else {
		//2 players
		sessionState = SessionState::TWO_PLAYER_SESSION;
		players[PLAYER_TWO].setHuman();
		getPlayerName(PLAYER_TWO);
	}
}
void Controller::setSessionStateZeroPlayer(){
	sessionState = SessionState::AUTOMATED_SESSION;
	players[0].setPlayer(COM_PLAYER, PLAYER1_DEFAULT_NAME);
	players[1].setPlayer(COM_PLAYER, PLAYER2_DEFAULT_NAME);
}
bool Controller::setUserEntryFromCellNum(unsigned int cellNum) { 
	if (cellNum < ttt.GetNumCells()) {
		userEntry = cellNum + 48;
		return true;
	}
	return false;
}
bool Controller::setUserEntryFromAlphaKeyCode(char keyCode) {
	if (std::isalpha(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
bool Controller::setUserEntryFromAlphaNumKeyCode(char keyCode) {
	if (std::isalpha(keyCode) || std::isdigit(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
void Controller::setWinner(unsigned int a) {
	//a is index of winning symbol
	if (ttt.getEntry(a) == players[0].getSymbol()) winner = static_cast<unsigned int>(PLAYER::ONE);
	else winner = static_cast<unsigned int>(PLAYER::TWO);
	gameState = GameState::GAME_WON;
}
void Controller::updateEntries(){
	if(ttt.newEntry(userEntry - 48, players[static_cast<unsigned int>(currentPlayer)].getSymbol())) setMoveIsLegal();
}
bool Controller::updateGameState() {
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
		if (isWin = isLine(0, 1, 2)) break;
		if (isWin = isLine(3, 4, 5)) break;
		if (isWin = isLine(6, 7, 8)) break;
		//Colums 0=3=6, 1=4=7, 2=5=8
		if (isWin = isLine(0, 3, 6)) break;
		if (isWin = isLine(1, 4, 7)) break;
		if (isWin = isLine(2, 5, 8)) break;
		//Diagonals 0=4=8, 2=4=6
		if (isWin = isLine(0, 4, 8)) break;
		if (isWin = isLine(2, 4, 6)) break;	
		foundResult = false;
		break; //loop only runs once
	}
	return foundResult;
}