

#include <iostream>

#include "ticktacktoegrid.h"
#include "utility.h"

void TickTackToeGrid::drawGame() {
	std::cout.flush();
	//Utility::clear_screen();
	Utility::cursorToZeroZero();
	printEntries();
	//Grid is 3 x (cellwidth + 2 separatorlinewidths) wide
	//Grid is 3 x (cellheight + 2 separatorlineheigts) high

	char crossDivCh = divSingle ? DIVIDER_CROSS_CHAR : DBL_DIVIDER_CROSS_CHAR;
	char hDivCh = divSingle ? DIVIDER_H_CHAR : DBL_DIVIDER_H_CHAR;
	char vDivCh = divSingle ? DIVIDER_V_CHAR : DBL_DIVIDER_V_CHAR;

	//Go screen line by line
	for (unsigned int i = FIRST_DISPLAY_ROW; i < NUM_DISPLAY_ROWS; i++) {
		//Row loop
		std::cout << "\n";
		//Space to line up row labels
		if (i < 10) std::cout << " ";
		std::cout << i << ": ";
		//Draw chars in this row (column loop)
		for (unsigned int j = FIRST_DISPLAY_COL; j < NUM_DISPLAY_COLS; j++) {
			//Row of horizontal dividers?
			if ((i + 1) % DISPLAY_V_STRIDE == 0) {
				
				if ((j + 1) % DISPLAY_H_STRIDE == 0) std::cout << crossDivCh;
				else std::cout << hDivCh;
			}
			else {
				//This row contains cell info and vertical dividers
				//Check columns for cell elements and dividers
				if ((j + 1) % DISPLAY_H_STRIDE == 0) std::cout << vDivCh;
				else {
					//Check for cell entries
					if ((j + FIRST_ENTRY_COL) % DISPLAY_H_STRIDE == 0) {
						//Current column has entries
						//Are we at a row that has entries?
						if ((i + FIRST_ENTRY_ROW) % DISPLAY_V_STRIDE == 0) {
							//At column containing entry
							unsigned int gridRow = (i / DISPLAY_V_STRIDE);
							unsigned int gridCol = (j / DISPLAY_H_STRIDE);
							//std::cout << "(" << gridRow << ", " << gridCol << ")";
							//std::cout << gridRow * GRID_COLS + gridCol;
							std::cout << entries[gridRow * GRID_COLS + gridCol];
						}
						else std::cout << SPACE_CHAR;
					}
					else {
						//Does this column have cell identifiers?
						if ((j + FIRST_DISPLAY_COL) % DISPLAY_H_STRIDE == 0) {
							//Current column has cell identifiers
							//Are we at a row that has cell identifiers?
							if ((i + FIRST_DISPLAY_ROW) % DISPLAY_V_STRIDE == 0) {
								//At column containing cell identifier
								unsigned int gridRow = (i / DISPLAY_V_STRIDE);
								unsigned int gridCol = (j / DISPLAY_H_STRIDE);
								//std::cout << "(" << gridRow << ", " << gridCol << ")";
								std::cout << gridRow * GRID_COLS + gridCol;
							}
							else std::cout << SPACE_CHAR;
						}
						else std::cout << SPACE_CHAR;
					}
				}
			}
		}
	}
	std::cout << std::endl;
}
void TickTackToeGrid::clearEntries() {
	for (char& c : entries)
		c = ENTRIES_DEFAULT_CHAR;
}
std::vector<unsigned int> TickTackToeGrid::getEmptyCells() {
	std::vector<unsigned int> emptyCells;
	for (auto i = 0; i < NUM_CELLS; i++)
		if (entries[i] == ENTRIES_DEFAULT_CHAR) emptyCells.push_back(i);
	return emptyCells;
}
std::vector<unsigned int> TickTackToeGrid::getLineCompletions(char symbol) {
	//Analyzes each line looking for two opponent symbols
	//if found, the entry for that cell is incremented
	std::vector<unsigned int> retVec(NUM_CELLS, 0);
	int indexOfSpace;
	if ( (indexOfSpace = pendingLine(symbol, 0, 1, 2)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 3, 4, 5)) >= 0)
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 6, 7, 8)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 0, 4, 8)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 0, 3, 6)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 1, 4, 7)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 2, 5, 8)) >= 0) 
		retVec[indexOfSpace]++;
	if ( (indexOfSpace = pendingLine(symbol, 2, 4, 6)) >= 0) 
		retVec[indexOfSpace]++;
	return retVec;
}
std::vector<unsigned int> TickTackToeGrid::getMakeTwoLines(char symbol) {
	//returns a vector with indications of how many lines of two each blank cell would create
	std::vector<unsigned int> retVec(NUM_CELLS, 0);
	processOneInLine(symbol, retVec, 0, 1, 2);
	processOneInLine(symbol, retVec, 3, 4, 5);
	processOneInLine(symbol, retVec, 6, 7, 8);
	processOneInLine(symbol, retVec, 0, 4, 8);
	processOneInLine(symbol, retVec, 0, 3, 6);
	processOneInLine(symbol, retVec, 1, 4, 7);
	processOneInLine(symbol, retVec, 2, 5, 8);
	processOneInLine(symbol, retVec, 2, 4, 6);
	return retVec;
}
bool TickTackToeGrid::isGridFull() {
	auto it = std::find(entries.begin(), entries.end(), ENTRIES_DEFAULT_CHAR);
	return it == entries.end();
}
bool TickTackToeGrid::newEntry(unsigned int index, char symbol) {
	//entry is in range?
	if (index >= NUM_CELLS) return false;
	//Cell is open?
	if (entries[index] != ENTRIES_DEFAULT_CHAR) return false;
	std::cout << "Index into entries:" << index;
	std::cout << " Current symbol: " << symbol << std::endl;
	entries[index] = symbol;
	return true;
}
const int TickTackToeGrid::oneInLine(char symbol, unsigned int a, unsigned int b, unsigned int c) {
	//Seeking a line with only one of my symbols the rest blanks. If so, return the index of my symbol.
	//Check for row of spaces
	if ((entries[a] == entries[b]) && (entries[a] == entries[c])) return -1;
	//Patterns of interest: My symbol and two spaces
	if (((entries[a] == symbol) && (entries[b] == ENTRIES_DEFAULT_CHAR) && (entries[c] == ENTRIES_DEFAULT_CHAR)))
		return a;
	if (((entries[b] == symbol) && (entries[a] == ENTRIES_DEFAULT_CHAR) && (entries[c] == ENTRIES_DEFAULT_CHAR)))
		return b;
	if (((entries[c] == symbol) && (entries[a] == ENTRIES_DEFAULT_CHAR) && (entries[b] == ENTRIES_DEFAULT_CHAR)))
		return c;
	//Pattern not found
	return -1;
}
const int TickTackToeGrid::pendingLine(const char symbol, const unsigned int a, const unsigned int b, const unsigned int c) {
	//Looking for O,_,O, O,O,_, or _,O,O
	//If no match return -1;
	//If match return index of space char
	if (entries[b] == ENTRIES_DEFAULT_CHAR) {
		//Check for two O's in a and c, if so return b
		if ((entries[a] == entries[c]) && (entries[a] == symbol)) return b;
	}
	//If b is O, check for a = O and c = _ (return c) or a = _ and c = O (return a)
	if ((entries[b] == symbol)) {
		if ((entries[a] == symbol) && (entries[c] == ENTRIES_DEFAULT_CHAR)) return c;
		if ((entries[a] == ENTRIES_DEFAULT_CHAR) && (entries[c] == symbol)) return a;
	}
	//No match for any pattern of two symbols and a space in this line
	return -1;
}
void TickTackToeGrid::printEntries() {
	for (char c : entries)
		std::cout << c << ",";
	std::cout << std::endl;
}
void TickTackToeGrid::processOneInLine(char symbol, std::vector < unsigned int>& vec, unsigned int a, unsigned int b, unsigned int c) {
	int indexOfMySymbol;
	if (      (indexOfMySymbol = oneInLine(symbol, a, b, c)) == a) { vec[b]++; vec[c]++; }
	else if ( (indexOfMySymbol = oneInLine(symbol, a, b, c)) == b) { vec[a]++; vec[c]++; }
	else if ( (indexOfMySymbol = oneInLine(symbol, a, b, c)) == c) { vec[a]++; vec[b]++; }
}
