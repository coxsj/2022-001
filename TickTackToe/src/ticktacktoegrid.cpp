

#include <iostream>

#include "ticktacktoegrid.h"
#include "utility.h"

void TickTackToeGrid::blank() {
	clearEntries();
	for (auto i = 0; i < cNumGridCells; i++)
		printNewEntry(i, cSpaceChar);
}
void TickTackToeGrid::blankEntryListDisplay() {
	for (auto i = 0; i < cNumGridCells; i++)
		printNewEntryOnEntryList(i, cSpaceChar);
}
void TickTackToeGrid::blankGrid() {
	for (auto i = 0; i < cNumGridCells; i++)
		printNewEntryOnGrid(i, cSpaceChar);
}
void TickTackToeGrid::clearEntryListDisplay() {
	//Move cursor to start position of Entry List
	utility.cursorTo(entryListPos.getV(), entryListPos.getH());
	//Print spaces to maximum length of list including separators and spaces
	std::cout << entryListBlankingString;
}
void TickTackToeGrid::drawGame() {
	printEntryList();
	drawGrid();
}
void TickTackToeGrid::drawGrid() {
	//Grid is 3 x (cellwidth + 2 separatorlinewidths) wide
	//Grid is 3 x (cellheight + 2 separatorlineheigts) high
	char crossDivCh = divSingle ? cDividerCrossChar : cDividerDblCrossChar;
	char hDivCh		= divSingle ? cDividerHChar		: cDividerDblHChar;
	char vDivCh		= divSingle ? cDividerVChar		: cDividerDblVChar;
	//Go screen line by line
	utility.cursorTo(gridPos.getV(), gridPos.getH());
	for (unsigned int i = 0; i < cNumDisplayRows; i++) {
		//Row loop
		printGridLineLabel(i);
		//Draw chars in this row
		for (unsigned int j = 0; j < cNumDisplayCols; j++) {
			//Column loop
			if ((i + 1) % cDisplayVStride == 0) {
				//Row of horizontal dividers?
				if ((j + 1) % cDisplayHStride == 0) std::cout << crossDivCh;
				else std::cout << hDivCh;
			}
			else {
				//This row contains cell info and/or vertical dividers
				if ((j + 1) % cDisplayHStride == 0) std::cout << vDivCh; //Vertical divider
				else {
					if (j % cDisplayHStride == 0) {
						//Current column has cell identifiers
						if (i % cDisplayVStride == 0) {
							//At column and row containing cell identifier
							unsigned int gridRow = (i / cDisplayVStride);
							unsigned int gridCol = (j / cDisplayHStride);
							std::cout << gridRow * cNumHGridCells + gridCol;
						}
						else std::cout << cSpaceChar;
					}
					else std::cout << cSpaceChar;
					
				}
			}
		}
		std::cout << std::endl;
	}
}
void TickTackToeGrid::clearEntries() {
	for (char& c : entries)
		c = cEndtriesDefaultChar;
}
std::vector<unsigned int> TickTackToeGrid::getEmptyCells() {
	std::vector<unsigned int> emptyCells;
	for (auto i = 0; i < cNumGridCells; i++)
		if (entries[i] == cEndtriesDefaultChar) emptyCells.push_back(i);
	return emptyCells;
}
std::vector<unsigned int> TickTackToeGrid::getLineCompletions(char symbol) {
	//Analyzes each line looking for two opponent symbols
	//if found, the entry for that cell is incremented
	std::vector<unsigned int> retVec(cNumGridCells, 0);
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
	std::vector<unsigned int> retVec(cNumGridCells, 0);
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
void TickTackToeGrid::initEntryList(){
	entryListSeparatorStr = ", ";
	int numChars = (cNumGridCells * cNumEntryCharsInCell) + (cNumGridCells - 1) * sizeof(entryListSeparatorStr);
	for (int i = 0; i < numChars; i++)
		entryListBlankingString.push_back(cSpaceChar);
	entryListPos.setV(titlePos.getLineAfter());
	entryListPos.setH(0);
	entryListPos.setVlen(1);
	entryListPos.setHlen(numChars);
	cEntryListStride = entryListSeparatorStr.size() + cNumEntryCharsInCell;
}
void TickTackToeGrid::initEntryVector() {
	for (int i = 0; i < cNumGridCells; i++)
		entries.push_back(cEndtriesDefaultChar);
}
void TickTackToeGrid::initGrid(){
	//Default to single dividers
	divSingle = true;
	//Set default position off grid (including line labels) relative to entryList
	gridPos.setV(entryListPos.getLineAfter());
	gridPos.setH(0);
	gridPos.setVlen(cNumVGridCells);
	gridPos.setHlen(cNumVGridCells);
	
	//Abs position of Cell 0 entry
	short hPosFirstGridEntry = gridPos.getH() + cMaxHGridLineLabelWithPadding + cNumHSpacePadding + cNumEntryCharsInCell - 1;
	short vPosFirstGridEntry = gridPos.getV() + cNumVSpacePadding + cNumEntryCharsInCell - 1;

	//Set positions of each grid entry for fast updating
	for (auto i = 0; i < cNumGridCells; i++) {
		//base horizontal position + cell col number * horizontal stride
		entryHPos.push_back(hPosFirstGridEntry + (i / cNumHGridCells) * cDisplayHStride);
		//base vertical position + cell row number * horizontal stride
		entryVPos.push_back(vPosFirstGridEntry + (i / cNumVGridCells) * cDisplayVStride);
	}
}
void TickTackToeGrid::initTitle(){
	titlePos.setV(0);
	titlePos.setH(0);
	titlePos.setVlen(1 + cNumTitleExtraLines);
	titlePos.setHlen(1);
	printGameName();
}
bool TickTackToeGrid::isGridFull() {
	auto it = std::find(entries.begin(), entries.end(), cEndtriesDefaultChar);
	return it == entries.end();
}
bool TickTackToeGrid::newEntry(unsigned int index, char symbol) {
	//entry is in range?
	if (index >= cNumGridCells) return false;
	//Cell is open?
	if (entries[index] != cEndtriesDefaultChar) return false;
	entries[index] = symbol;
	printNewEntry(index, symbol);
	return true;
}
const int TickTackToeGrid::oneInLine(char symbol, unsigned int a, unsigned int b, unsigned int c) {
	//Seeking a line with only one of my symbols the rest blanks. If so, return the index of my symbol.
	//Check for row of spaces
	if ((entries[a] == entries[b]) && (entries[a] == entries[c])) return -1;
	//Patterns of interest: My symbol and two spaces
	if (((entries[a] == symbol) && (entries[b] == cEndtriesDefaultChar) && (entries[c] == cEndtriesDefaultChar)))
		return a;
	if (((entries[b] == symbol) && (entries[a] == cEndtriesDefaultChar) && (entries[c] == cEndtriesDefaultChar)))
		return b;
	if (((entries[c] == symbol) && (entries[a] == cEndtriesDefaultChar) && (entries[b] == cEndtriesDefaultChar)))
		return c;
	//Pattern not found
	return -1;
}
const int TickTackToeGrid::pendingLine(const char symbol, const unsigned int a, const unsigned int b, const unsigned int c) {
	//Looking for O,_,O, O,O,_, or _,O,O
	//If no match return -1;
	//If match return index of space char
	if (entries[b] == cEndtriesDefaultChar) {
		//Check for two O's in a and c, if so return b
		if ((entries[a] == entries[c]) && (entries[a] == symbol)) return b;
	}
	//If b is O, check for a = O and c = _ (return c) or a = _ and c = O (return a)
	if ((entries[b] == symbol)) {
		if ((entries[a] == symbol) && (entries[c] == cEndtriesDefaultChar)) return c;
		if ((entries[a] == cEndtriesDefaultChar) && (entries[c] == symbol)) return a;
	}
	//No match for any pattern of two symbols and a space in this line
	return -1;
}
void TickTackToeGrid::printEntryList() {
	//Move cursor to start position
	utility.cursorTo(entryListPos.getV(), entryListPos.getH());
	for (char c : entries)
		std::cout << c << entryListSeparatorStr;
}
void const TickTackToeGrid::printGameName() {
	utility.printAt(titlePos.getV(), titlePos.getH(), "TickTackToe\n");
	for (int i = 0; i < cNumTitleExtraLines; i++)
		std::cout << std::endl;
}
void const TickTackToeGrid::printGridLineLabel(unsigned int n){
	std::string outStr;
	//Space to line up row labels for small numbers
	if (n < 10) outStr.push_back(' ');
	outStr += std::to_string(n);
	outStr.push_back(':');
	for(auto i = outStr.size(); i < cMaxHGridLineLabelWithPadding; i++)
		outStr.push_back(' ');
	std::cout << outStr;
}
void const TickTackToeGrid::printNewEntry (const unsigned int& index, const char& symbol) {
	//Print new entry
	printNewEntryOnEntryList(index, symbol);
	//Update entry on grid
	printNewEntryOnGrid(index, symbol);
}
void const TickTackToeGrid::printNewEntryOnEntryList(const unsigned int& index, const char& symbol) {
	//Print new entry on entry list
	utility.cursorTo(entryListPos.getV(), entryListPos.getH() + cEntryListStride * index);
	std::cout << symbol;
}
void const TickTackToeGrid::printNewEntryOnGrid(const unsigned int& index, const char& symbol) {
	//Update entry on grid
	utility.cursorTo(entryVPos[index], entryHPos[index]);
	std::cout << symbol;
}
void TickTackToeGrid::processOneInLine(char symbol, std::vector < unsigned int>& vec, unsigned int a, unsigned int b, unsigned int c) {
	int indexOfMySymbol;
	if (      (indexOfMySymbol = oneInLine(symbol, a, b, c)) == a) { vec[b]++; vec[c]++; }
	else if ( (indexOfMySymbol = oneInLine(symbol, a, b, c)) == b) { vec[a]++; vec[c]++; }
	else if ( (indexOfMySymbol = oneInLine(symbol, a, b, c)) == c) { vec[a]++; vec[b]++; }
}
