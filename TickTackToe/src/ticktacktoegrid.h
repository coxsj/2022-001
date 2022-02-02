#pragma once
#include <vector>
#include <string>


class TickTackToeGrid {
	
	//Grid Sizing Parameters
	const unsigned int	GRID_ROWS = 3;
	const unsigned int	GRID_COLS = 3;
	const size_t		NUM_CELLS = GRID_ROWS * GRID_COLS;
	const unsigned int  NUM_ENTRY_CHARS = 1;
	const unsigned int	NUM_H_SPACES = 2; //Number of horizontal spaces surrounding an entry
	const unsigned int	NUM_V_SPACES = 1; //Number of vertical spaces surrounding an entry
	const unsigned int  NUM_H_DIVIDER_CHARS = 1;
	const unsigned int  NUM_V_DIVIDER_CHARS = 1;
	const unsigned int  FIRST_DISPLAY_COL = 0;
	const unsigned int  FIRST_DISPLAY_ROW = 0;
	const unsigned int  FIRST_ENTRY_ROW = 1 + NUM_V_SPACES + NUM_ENTRY_CHARS;
	const unsigned int  FIRST_ENTRY_COL = 1 + NUM_H_SPACES + NUM_ENTRY_CHARS;
	const unsigned int	NUM_DISPLAY_ROWS = GRID_ROWS * (NUM_V_SPACES * 2 + NUM_ENTRY_CHARS) + (GRID_ROWS - 1);	//Grid rows - 1 as no divider at bottom of grid
	const unsigned int	NUM_DISPLAY_COLS = GRID_COLS * (NUM_H_SPACES * 2 + 1) + (GRID_COLS - 1);	//3 colss, each with 1 entries 2x spaceCnt, 2 dividers
	const unsigned int  DISPLAY_H_STRIDE = NUM_ENTRY_CHARS + NUM_H_DIVIDER_CHARS + 2 * NUM_H_SPACES;
	const unsigned int  DISPLAY_V_STRIDE = NUM_ENTRY_CHARS + NUM_V_DIVIDER_CHARS + 2 * NUM_V_SPACES;

	//Grid characters
	const char SPACE_CHAR = ' ';
	const char DIVIDER_V_CHAR = char(179);
	const char DIVIDER_H_CHAR = char(196);
	const char DIVIDER_CROSS_CHAR = char(197);
	const char DBL_DIVIDER_V_CHAR = char(186);
	const char DBL_DIVIDER_H_CHAR = char(205);
	const char DBL_DIVIDER_CROSS_CHAR = char(206);
	const char ENTRIES_DEFAULT_CHAR = SPACE_CHAR;
	bool divSingle = true;
	std::vector<char>	entries;

	//Grid Display Location
	int gridStartVertical;
	int gridStartHorizontal;
	const int GRID_START_VERTICAL_DEFAULT = 2;
	const int GRID_START_HORIZONTAL_DEFAULT = 0;

	//Entry List
	int entryListStartVertical;
	int entryListStartHorizontal;
	const unsigned int cNumEntrySpaces = 1;
	const unsigned int cNumEntrySeparators = 1;
	std::string entryListSeparatorStr;
	unsigned int cEntryListStride;
	std::string entryListBlankString;
	const int ENTRY_LIST_START_VERTICAL_DEFAULT = 0;
	const int ENTRY_LIST_START_HORIZONTAL_DEFAULT = 0;

public:
	TickTackToeGrid() { initGrid(); };

	void blank();
	void blankEntryListDisplay();
	void blankGrid();
	void clearEntries();
	void drawGame();
	void drawGrid();
	std::vector<unsigned int> getEmptyCells();
	std::vector<char> getEntries() { return entries; }
	char getEntry(unsigned int index) { return entries[index]; }
	unsigned int const getLastDisplayRow() { return gridStartVertical + NUM_DISPLAY_ROWS; }
	size_t GetNumCells() { return NUM_CELLS; }
	std::vector<unsigned int> getLineCompletions(char symbol);
	std::vector<unsigned int> getMakeTwoLines(char symbol);
	bool isEntryOpen(const unsigned int index) { return entries[index] == ENTRIES_DEFAULT_CHAR; }
	bool isGridFull();
	bool isLine(unsigned int a, unsigned int b, unsigned int c) { return (entries[a] == entries[b]) && (entries[a] == entries[c]) && (entries[a] != ENTRIES_DEFAULT_CHAR); }
	bool newEntry(unsigned int index, char symbol);
	void printEntryList();
	void setDividerDouble() { divSingle = false; }
	void setDividerSingle() { divSingle = true;  }
	void setEntryListPosition(const int newVertical, const int newHorizontal);
	void setGridPosition(const int newVertical, const int newHorizontal);

private:
	void clearEntryListDisplay();
	void initGrid() {
		//Set up entry vector
		for (int i = 0; i < NUM_CELLS; i++)
			entries.push_back(ENTRIES_DEFAULT_CHAR);
		//Entry List
		int numChars = (NUM_CELLS * NUM_ENTRY_CHARS) + (NUM_CELLS - 1)*(cNumEntrySpaces + cNumEntrySeparators);
		for (int i = 0; i < numChars; i++)
			entryListBlankString.push_back(SPACE_CHAR);
		entryListSeparatorStr = ", ";
		cEntryListStride = entryListSeparatorStr.size() + NUM_ENTRY_CHARS;
		setEntryListPosition(ENTRY_LIST_START_VERTICAL_DEFAULT, ENTRY_LIST_START_HORIZONTAL_DEFAULT);
		printEntryList();
		//Initialize grid with single line dividers
		divSingle = true;
		setGridPosition(GRID_START_VERTICAL_DEFAULT, GRID_START_HORIZONTAL_DEFAULT);
		
	}
	const int oneInLine(char symbol, unsigned int a, unsigned int b, unsigned int c);
	const int pendingLine(const char symbol, const unsigned int a, const unsigned int b, const unsigned int c);
	void const printNewEntry (const unsigned int& index, const char& symboll);
	void const printNewEntryOnEntryList(const unsigned int& index, const char& symbol);
	void const printNewEntryOnGrid (const unsigned int& index, const char& symbol);
	void processOneInLine(char symbol, std::vector < unsigned int>& vec, unsigned int a, unsigned int b, unsigned int c);
	
	};