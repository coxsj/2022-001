#pragma once
#include <vector>
#include <string>

#include "Utility.h"


class TickTackToeGrid {
	//Title
	Utility::ConsolePos titlePos;
	const short cNumTitleExtraLines = 1;

	//Entry List
	Utility::ConsolePos entryListPos;
	const short cNumEntrySpaces = 1;
	const short cNumEntrySeparators = 1;
	short cEntryListStride;
	std::string entryListSeparatorStr;
	std::string entryListBlankingString;
	
	//Grid Display Location
	Utility::ConsolePos gridPos;
	
	//Grid Sizing Parameters
	const short	cNumVGridCells = 3;
	const short	cNumHGridCells = 3;
	const size_t cNumGridCells = cNumVGridCells * cNumHGridCells;
	const short cNumEntryCharsInCell = 1;
	const short	cNumHSpacePadding = 2; //Number of horizontal spaces surrounding an entry
	const short	cNumVSpacePadding = 1; //Number of vertical spaces surrounding an entry
	const short cNumHDividerChars = 1;
	const short cNumVDividerChars = 1;
	const short	cNumDisplayRows = cNumVGridCells * (cNumVSpacePadding * 2 + cNumEntryCharsInCell) + (cNumVGridCells - 1);	//Grid rows - 1 as no divider at bottom of grid
	const short cDisplayHStride = cNumEntryCharsInCell + cNumHDividerChars + 2 * cNumHSpacePadding;
	const short cDisplayVStride = cNumEntryCharsInCell + cNumVDividerChars + 2 * cNumVSpacePadding;
	const short cMaxHGridLineLabelWithPadding = 4;
	const short cNumDisplayCols = cNumHGridCells * (cNumHSpacePadding * 2 + 1) + (cNumHGridCells - 1);	//3 cols, each with 1 entries 2x spaceCnt, 2 dividers
	short firstEntryRow;
	short firstEntryCol;
	short firstGridCellCol;
	std::vector<short> entryHPos;
	std::vector<short> entryVPos;

	//Grid characters
	const char cSpaceChar = ' ';
	const char cDividerVChar = char(179);
	const char cDividerHChar = char(196);
	const char cDividerCrossChar = char(197);
	const char cDividerDblVChar = char(186);
	const char cDividerDblHChar = char(205);
	const char cDividerDblCrossChar = char(206);
	const char cEndtriesDefaultChar = cSpaceChar;
	bool divSingle = true;
	std::vector<char> entries;
	
	//Utility
	Utility utility;

public:
	TickTackToeGrid() { initTTT(); };

	void blank();
	void blankEntryListDisplay();
	void blankGrid();
	void clearEntries();
	void drawGame();
	void drawGrid();
	std::vector<unsigned int> getEmptyCells();
	std::vector<char> getEntries() { return entries; }
	char getEntry(unsigned int index) { return entries[index]; }
	unsigned int const getLastDisplayRow() { return gridPos.getLineAfter(); }
	size_t GetNumCells() { return cNumGridCells; }
	std::vector<unsigned int> getLineCompletions(char symbol);
	std::vector<unsigned int> getMakeTwoLines(char symbol);
	bool isEntryOpen(const unsigned int index) { return entries[index] == cEndtriesDefaultChar; }
	bool isGridFull();
	bool isLine(unsigned int a, unsigned int b, unsigned int c) { return (entries[a] == entries[b]) && (entries[a] == entries[c]) && (entries[a] != cEndtriesDefaultChar); }
	bool newEntry(unsigned int index, char symbol);
	void printEntryList();
	void setDividerDouble() { divSingle = false; }
	void setDividerSingle() { divSingle = true;  }
	
private:
	void clearEntryListDisplay();
	void initEntryList();
	void initEntryVector();
	void initGrid();
	void initTitle();
	void initTTT() {
		//Title
		initTitle();
		//Entry vector
		initEntryVector();
		//Entry List
		initEntryList();
		//Initialize grid with single line dividers
		initGrid();
	}
	const int oneInLine(char symbol, unsigned int a, unsigned int b, unsigned int c);
	const int pendingLine(const char symbol, const unsigned int a, const unsigned int b, const unsigned int c);
	void const printGameName();
	void const printGridLineLabel(unsigned int i);
	void const printNewEntry (const unsigned int& index, const char& symboll);
	void const printNewEntryOnEntryList(const unsigned int& index, const char& symbol);
	void const printNewEntryOnGrid (const unsigned int& index, const char& symbol);
	void processOneInLine(char symbol, std::vector < unsigned int>& vec, unsigned int a, unsigned int b, unsigned int c);
	
	};