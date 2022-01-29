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
void Game::clearEntries() {
	for (char& c : entries)
		c = ENTRIES_DEFAULT_CHAR;
}
void Game::drawGame() {
	std::cout.flush();
	Utility::clear_screen();
	printEntries();
	//3 rows and three char widths for each space
	//grid is 3 x 3 + 2(separator lines) = 11 chars wide
	//Grid is 3 x 3 + 2(separator lines) = 11 lines high
	for (unsigned int i = 0; i < DISPLAY_ROWS; i++) {
		//Rows
		std::cout << "\n";
		if (i < 10) std::cout << " ";
		std::cout << i << ": ";
		for (unsigned int j = 0; j < DISPLAY_COLS; j++) {
			//Columns
			if ((i + 1) % (2 + 2 * NUM_V_SPACES) == 0) {
				//Row of dividers
				if ((j + 1) % (2 + 2 * NUM_H_SPACES) == 0)
					std::cout << '+';
				else std::cout << char(196);
			}
			else {
				//Not row of dividers
				if ((j + 1) % (2 + 2 * NUM_H_SPACES) == 0) {
					//Divider in this column
					std::cout << '|';
				}
				//Check for entries and virtical dividers
				else {
					if ((j + NUM_H_SPACES + 2) % (2 + 2 * NUM_H_SPACES) == 0) {
						//Row has entries
						if ((i + NUM_V_SPACES + 2) % (2 + 2 * NUM_V_SPACES) == 0) {
							//At column containing entry
							unsigned int gridRow = (i / (2 + 2 * NUM_V_SPACES));
							unsigned int gridCol = (j / (2 + 2 * NUM_H_SPACES));
							//std::cout << "(" << gridRow << ", " << gridCol << ")";
							//std::cout << gridRow * GRID_COLS + gridCol;
							std::cout << entries[gridRow * GRID_COLS + gridCol];
						}
						else std::cout << ' ';
					}
					else std::cout << ' ';
				}
			}
		}
	}
	std::cout << std::endl;
	printGameResult();
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
void Game::getUserInput() {
	unsigned int inputCode;
	while (1) {
		printCurrentPlayerName(); std::cout <<"'s move. Enter cell number..." << std::endl;
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
	if ((entries[a] == entries[b]) && (entries[a] == entries[c]) && entries[a] != ENTRIES_DEFAULT_CHAR) {
		setWinner(a);
		gameState = GameState::GAME_WON;
		return true;
	}
	return false;
}
void Game::newGame() {
	clearEntries();
	currentPlayer = PLAYER_ONES_TURN;
	gameState = GameState::ACTIVE_GAME;
	std::cout << "NEW GAME!!!\n";
	drawGame();
}
void Game::newSession() {
	getGameTypeFromUser();
	//Todo load player score history if any
	newGame();
}
void Game::nextMove() {
	if (gameState != GameState::ACTIVE_GAME) return;
	//Get input from next player
	getUserInput();
	processInput();// Evaluate if the game is won, tied or lost and sett state codes
	//Switch players if still playing
	changePlayers();
	//Update display, displaying current play or winner or tie
	drawGame();
}
void Game::printCurrentPlayerName() {
	std::cout << players[currentPlayer].getName();
	std::cout << "( " << players[currentPlayer].getSymbol() << " )";
}
void Game::printEntries() {
	for (char c : entries)
		std::cout << c << ",";
	std::cout << std::endl;
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
}
void Game::processInput() {
	switch (userEntry) {
	case 'c':
	case 'C':
		//change session
		gameState = GameState::GAME_OVER;
		sessionState = SessionState::SESSION_CHANGE_REQUESTED;
		break;
	case 'e':
	case 'E':
		//End current game
		gameState = GameState::GAME_OVER;
		break;
	case 'n':
	case 'N':
		//new session
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
	case 57:
		updateEntries();
		break;
		default:
			break;
	}
}
void Game::run(){
	while (!isSessionOver()) {
		newSession();
		while (!newSessionRequest()){
			while (!isGameOver()) {
				nextMove();
			}
			//Todo Show cumulative Scores, 

			//Todo play another, change sides, new players, quit?
			getPostGameInputFromUser();
			processInput();
		}
	}
	//Todo: Save score history for each player?
}
void Game::setWinner(unsigned int a) {
	//a is index of winning symbol
	if (entries[a] == players[0].getSymbol()) winner = 0;
	else winner = 1;
}
void Game::updateEntries(){
	unsigned int index = userEntry - 48;
	if (index > GRID_CELLS) return;
	if (entries[index] != ENTRIES_DEFAULT_CHAR) return;
	std::cout << "Index into entries:" << index;
	std::cout << " Current symbol: " << players[currentPlayer].getSymbol() << std::endl;
	entries[index] = players[currentPlayer].getSymbol();
	updateGameState();
}
void Game::updateGameState() {
	//Check game for win or tie, set game state accordingly
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
		//check for full grid
		auto it = std::find(entries.begin(), entries.end(), ENTRIES_DEFAULT_CHAR);
		if (it == entries.end()) {
			gameState = GameState::GAME_TIED;
			break;
		}
		break; // should not be needed, loop only runs once
	}
}