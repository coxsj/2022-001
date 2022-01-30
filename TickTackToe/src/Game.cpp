#include <iostream>
#include <conio.h>
#include <string>

#include "Game.h"
#include "utility.h"

void Game::changePlayers() {
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
	while (1) {
		std::cout << "Play again(p)\nChange sides(c)\nNew players(n)\nQuit(q)\n";
		userEntry = _getch();
		if (std::isalpha(userEntry) || std::isdigit(userEntry)) break;
	}
}
void Game::getMoveInput() {
	resetMoveIsLegal();
	if (players[currentPlayer].isCom()) getMoveInputFromCom();
	else getMoveInputFromHuman();
}
void Game::getMoveInputFromCom() {
	for (auto i = 0; i < grid.GetCells(); i++) {
		if (grid.isEntryOpen(i)) {
			userEntry = i + 48; //simulate key code
			break;
		}
	 }
}
void Game::getMoveInputFromHuman() {
	unsigned int inputCode;
	while (1) {
		printCurrentPlayerName(); std::cout << "'s move. Enter cell number..." << std::endl;
		inputCode = _getch();
		if (std::isalpha(inputCode) || std::isdigit(inputCode)) {
			std::cout << inputCode << std::endl;
			userEntry = inputCode;
			break;
		}
	}
}
bool Game::isGameOver() { 
	return (gameState == GameState::GAME_OVER)
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
void Game::newGame() {
	grid.clearEntries();
	currentPlayer = PLAYER_ONES_TURN;
	gameState = GameState::ACTIVE_GAME;
	std::cout << "NEW GAME!!!\n";
	grid.drawGame();
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
	if(moveIsLegal()) changePlayers();
	//Update display, displaying current play or winner or tie
	grid.drawGame();
	printGameResult();
}
void Game::printCurrentPlayerName() {
	std::cout << players[currentPlayer].getName();
	std::cout << "( " << players[currentPlayer].getSymbol() << " )";
}
void Game::printGameResult() {
	switch (gameState) {
	case GameState::GAME_OVER:
		std::cout << "Unfinished game\n";
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
	std::cout << std::endl;
}
void Game::processInput() {
	switch (userEntry) {
	case 'c':
	case 'C':
		//end current game, change sides
		gameState = GameState::GAME_OVER;
		changeSides();
		break;
	case 'e':
	case 'E':
		//End current game, same session
		gameState = GameState::GAME_OVER;
		break;
	case 'n':
	case 'N':
		//End this game, start new session
		gameState = GameState::GAME_OVER;
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
		gameState = GameState::GAME_OVER;
		sessionState = SessionState::SESSION_OVER;
		std::cout << "Game over!!\n";
		break;
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
		updateEntries();
		break;
	default:
		break;
	}
}
void Game::run(){
	while (!isSessionOver()) {
		newSession();
		while (!newSessionRequest() && !isSessionOver()){
			newGame();
			while (!isGameOver()) {
				nextMove();
			}
			//Todo Show cumulative Scores, 

			//Play another, change sides, new players, quit?
			getPostGameInputFromUser();
			processInput();
		}
	}
	//Todo: Save score history for each player?
}
void Game::setWinner(unsigned int a) {
	//a is index of winning symbol
	if (grid.getEntry(a) == players[0].getSymbol()) winner = 0;
	else winner = 1;
}
void Game::updateEntries(){
	if(grid.newEntry(userEntry - 48, players[currentPlayer].getSymbol())) setMoveIsLegal();
	updateGameState();
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