#pragma once
#include <vector>


class TickTackToeGrid {
	
	const unsigned int	GRID_ROWS = 3;
	const unsigned int	GRID_COLS = 3;
	const unsigned int	GRID_CELLS = GRID_ROWS * GRID_COLS;
	const unsigned int  NUM_ENTRY_CHARS = 1;
	const unsigned int	NUM_H_SPACES = 2; //Number of horizontal spaces surrounding an entry
	const unsigned int	NUM_V_SPACES = 1; //Number of vertical spaces surrounding an entry
	const unsigned int  NUM_H_DIVIDER_CHARS = 1;
	const unsigned int  NUM_V_DIVIDER_CHARS = 1;
	const unsigned int  FIRST_ENTRY_ROW = 1 + NUM_V_SPACES + NUM_ENTRY_CHARS;
	const unsigned int  FIRST_ENTRY_COL = 1 + NUM_H_SPACES + NUM_ENTRY_CHARS;
	const unsigned int	DISPLAY_ROWS = GRID_ROWS * (NUM_V_SPACES * 2 + NUM_ENTRY_CHARS) + (GRID_ROWS - 1);	//Grid rows - 1 as no divider at bottom of grid
	const unsigned int	DISPLAY_COLS = GRID_COLS * (NUM_H_SPACES * 2 + 1) + (GRID_COLS - 1);	//3 colss, each with 1 entries 2x spaceCnt, 2 dividers
	const unsigned int  DISPLAY_H_STRIDE = NUM_ENTRY_CHARS + NUM_H_DIVIDER_CHARS + 2 * NUM_H_SPACES;
	const unsigned int  DISPLAY_V_STRIDE = NUM_ENTRY_CHARS + NUM_V_DIVIDER_CHARS + 2 * NUM_V_SPACES;

	//Grid characters
	const char SPACE_CHAR = ' ';
	const char DIVIDER_V_CHAR = char(179);
	const char DIVIDER_H_CHAR = char(196);
	const char DIVIDER_CROSS_CHAR = char(197);
	const char ENTRIES_DEFAULT_CHAR = SPACE_CHAR;

	std::vector<char>	entries;

public:
	TickTackToeGrid() { initGrid(); };

	void clearEntries();
	void drawGame();
	unsigned int GetCells() { return GRID_CELLS; }
	char getEntry(unsigned int index) { return entries[index]; }
	bool isEntryOpen(const unsigned int index) { return entries[index] == ENTRIES_DEFAULT_CHAR; }
	bool isGridFull();
	bool isLine(unsigned int a, unsigned int b, unsigned int c) { return (entries[a] == entries[b]) && (entries[a] == entries[c]) && (entries[a] != ENTRIES_DEFAULT_CHAR); }
	bool newEntry(unsigned int index, char symbol);
	void printEntries();

private:
	void initGrid() {
		//Set up entry vector
		for (int i = 0; i < GRID_CELLS; i++)
			entries.push_back(ENTRIES_DEFAULT_CHAR);
	}

};