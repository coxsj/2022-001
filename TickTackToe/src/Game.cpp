#include <iostream>
#include <conio.h>
#include <string>

#include "Game.h"
#include "utility.h"

bool Game::blockRowOfTwo() {
	const std::vector<unsigned int> opponentLineBlocks = (std::move(grid.getLineCompletions(getOpponentSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	findMax(opponentLineBlocks, maxSoFar, maxCellIndex);
	//If more than one line uses an open cell, block that cell preferentially
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Game::changeToNextPlayer() {
	if (gameState != GameState::ACTIVE_GAME) return;
	if (currentPlayer == PLAYER_ONES_TURN) currentPlayer = PLAYER_TWOS_TURN;
	else currentPlayer = PLAYER_ONES_TURN;
}
void Game::changeSides() {
	//Todo
	//Implement copy constructor and copy assignment
	//Implement move constructor and move assignment
	//Implement swaps
	// 
	//Tell user what happened

	//swap(player[0], player[1]);
}
void Game::comMoveAdvanced() {
	//Todo: replace this code with Advanced strategy
	comMoveNovice();
}
void Game::comMoveIntermediate() {
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
void Game::comMoveNovice() {
	const std::vector<unsigned int> emptyCells = std::move(grid.getEmptyCells());
	if (emptyCells.size() == 1) userEntry = emptyCells[0];
	else if (setUserEntryFromCellNum(emptyCells[rand() % emptyCells.size()])) return;
	else setUserEntryFromAlphaKeyCode('b');
}
void Game::findMax(const std::vector<unsigned int>& vec, unsigned int& maxSoFar, unsigned int& indexOfMax)
{
	for (auto i = 0; i < grid.GetNumCells(); i++) {
		if (vec[i] > maxSoFar) {
			maxSoFar = vec[i];
			indexOfMax = i;
		}
	}
}
void Game::gameAbandon() {
	gameState = GameState::GAME_ABANDONNED;
}
void Game::gameEnd(){
	gameState = GameState::GAME_OVER;
}
void Game::getGameTypeFromUser() {
	int numPlayers = 99;	//ensures user prompt
	while (numPlayers > 2) {
		std::cout << "How many players? (0, 1, 2) " << std::endl;
		numPlayers = _getch() - 48;
	}
	std::cout << numPlayers << " player";
	if (numPlayers != 1) std::cout << "s";
	std::cout << " it is!\n";
	if (numPlayers == 0) {
		gameType = GameType::ZERO_PLAYER;
		players[0].setPlayer(comPlayer, PLAYER1_DEFAULT_NAME);
		players[1].setPlayer(comPlayer, PLAYER2_DEFAULT_NAME);
	}
	if (numPlayers == 1) {
		gameType = GameType::ONE_PLAYER;
		players[0].resetCom();
		getPlayerName(1);
		players[1].setPlayer(comPlayer, PLAYER2_DEFAULT_NAME);
	}
	if (numPlayers == 2) {
		gameType = GameType::TWO_PLAYER;
		players[0].resetCom();
		getPlayerName(1);
		players[1].resetCom();
		getPlayerName(2);
	}
}
void Game::getPlayerName(unsigned int player) {
	if ((player == 0) || (player > 2)) return;
	std::string inputStr;
	while (1) {
		std::cout << "Enter Player" << player << " name ( ";
		std::cout << ((player == 1) ? players[0].getSymbol() : players[1].getSymbol()) << " ): ";
		std::cin >> inputStr;
		if (inputStr.size() > 0) {
			if (player == 1) {
				players[0].setName(inputStr);
				break;
			}
			if (player == 2) {
				players[1].setName(inputStr);
				break;
			}
		}
	}
}
void Game::getPostGameInputFromUser() {
	//play another, change sides, new players, quit?
	std::cout << "Play again(p)\nChange sides(c)\nNew players(n)\nQuit(q)\n";
	while (1) {
		if (setUserEntryFromAlphaKeyCode(_getch())) break;
	}
}
void Game::getMoveInput() {
	resetMoveIsLegal();
	if (players[currentPlayer].isCom()) getMoveInputFromCom();
	else getMoveInputFromHuman();
}
void Game::getMoveInputFromCom() {
	//comMoveNovice();
	comMoveIntermediate();
}
void Game::getMoveInputFromHuman() {
	unsigned int inputCode;
	while (1) {
		drawGame();
		printCurrentPlayerName(); std::cout << "'s move. Enter cell number..." << std::endl;
		inputCode = _getch();
		if (setUserEntryFromAlphaNumKeyCode(inputCode)) break;
	}
}
bool Game::isGameOver() { 
	return (gameState == GameState::GAME_ABANDONNED)
		|| (gameState == GameState::GAME_OVER)
		|| (gameState == GameState::GAME_WON)
		|| (gameState == GameState::GAME_TIED);
}
bool Game::isLine(unsigned int a, unsigned int b, unsigned int c) {
	if (grid.isLine(a, b, c)) {
		setWinner(a);
		gameState = GameState::GAME_WON;
		return true;
	}
	return false;
}
bool Game::makeRowOfThree() {
	const std::vector<unsigned int> lineCompletions = (std::move(grid.getLineCompletions(getCurrentSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	findMax(lineCompletions, maxSoFar, maxCellIndex);
	//Check if winning move identified
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
bool Game::makeTwoInARow() {
	const std::vector<unsigned int> makeTwoLines = (std::move(grid.getMakeTwoLines(getCurrentSymbol())));
	unsigned int maxSoFar = 0;
	unsigned int maxCellIndex = 0;
	findMax(makeTwoLines, maxSoFar, maxCellIndex);
	//If more than one cell creates two in a row, use that cell preferentially
	if (maxSoFar > 0) {
		if(setUserEntryFromCellNum(maxCellIndex)) return true;
	}
	return false;
}
void Game::newGame() {
	grid.clearEntries();
	currentPlayer = PLAYER_ONES_TURN;
	gameState = GameState::ACTIVE_GAME;
	drawGame();
	std::cout << "NEW GAME!!!\n";
}
void Game::newSession() {
	sessionState = SessionState::ACTIVE_SESSION;
	getGameTypeFromUser();
	//Todo load player score history if any
}
void Game::nextMove() {
	if (gameState != GameState::ACTIVE_GAME) return;
	//Get input from next player
	getMoveInput();
	processInput();// Evaluate if the game is won, tied or lost and set state codes
	//Switch players if still playing AND move was good
	if (moveIsLegal()) {
		updateGameState();
		changeToNextPlayer();
		//Update display, displaying current play or winner or tie
		grid.drawGame();
		printGameResult();
	}
}
void Game::printCurrentPlayerName() {
	std::cout << players[currentPlayer].getName();
	std::cout << "( " << players[currentPlayer].getSymbol() << " )";
}
void Game::printGameResult() {
	switch (gameState) {
	case GameState::GAME_ABANDONNED:
		std::cout << "Game Cancelled\n";
		break;
	case GameState::GAME_OVER:
		std::cout << "Game Over\n";
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
	if(isSessionOver()) std::cout << "Session over\n";
	std::cout << std::endl;
}
bool Game::processInput() {
	switch (userEntry) {
	case 'b':
	case 'B':
		//end current game, change sides
		gameEnd();
		sessionEnd();
		std::cout << "ERROR!! Cant find valid entry\n";
		break;
	case 'c':
	case 'C':
		//end current game, change sides
		gameEnd();
		changeSides();
		break;
	case 'e':
	case 'E':
		//End current game, same session
		gameEnd();
		break;
	case 'n':
	case 'N':
		//End this game, start new session
		gameEnd();
		sessionState = SessionState::NEW_SESSION_REQESTED;
		break;
	case 'p':
	case 'P':
		//Play another
		newGame();
		break;
	case 'q':
	case 'Q':
		//End this game, quit this session
		gameAbandon();
		sessionEnd();
		break;
	case 'x':
	case 'X':
		gameAbandon();
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
void Game::run(){
	while (!isSessionOver()) {
		newSession();
		while (1){
			newGame();
			while (!isGameOver()) {
				nextMove();
			}
			drawGame();
			if (newSessionRequest() || isSessionOver()) break;
			
			//Todo Show cumulative Scores, 

			//Play another, change sides, new players, quit?
			while (1) {
				getPostGameInputFromUser();
				if (processInput()) break;
				drawGame();
			}
			if (newSessionRequest() || isSessionOver()) break;
		}
	}
	//Todo: Save score history for each player?
}
void Game::sessionEnd() {
	sessionState = SessionState::SESSION_OVER;
	std::cout << "Session Over!\n";
}
bool Game::setUserEntryFromCellNum(unsigned int cellNum) { 
	if (cellNum < grid.GetNumCells()) {
		userEntry = cellNum + 48;
		return true;
	}
	return false;
}
bool Game::setUserEntryFromAlphaKeyCode(char keyCode) {
	if (std::isalpha(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
bool Game::setUserEntryFromAlphaNumKeyCode(char keyCode) {
	if (std::isalpha(keyCode) || std::isdigit(keyCode)) {
		userEntry = keyCode;
		return true;
	}
	return false;
}
void Game::setWinner(unsigned int a) {
	//a is index of winning symbol
	if (grid.getEntry(a) == players[0].getSymbol()) winner = 0;
	else winner = 1;
}
void Game::updateEntries(){
	if(grid.newEntry(userEntry - 48, players[currentPlayer].getSymbol())) setMoveIsLegal();
}
void Game::updateGameState() {
	//Check grid for win or tie, set game state accordingly
	//Game over
	//Was it a win? if not, it is a tie
	bool isWin = false;
	for (int i = 0; i == 0; i++) {
		//This loop designed to execute only once
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
		//Stll in loop, might be a tie
		if (grid.isGridFull()) {
			gameState = GameState::GAME_TIED;
			break;
		}
		break; // should not be needed, loop only runs once
	}
}