

#include <iostream>

#include "ticktacktoegrid.h"
#include "utility.h"

void TickTackToeGrid::drawGame() {
	std::cout.flush();
	Utility::clear_screen();
	printEntries();
	//Grid is 3 x (cellwidth + 2 separatorlinewidths) wide
	//Grid is 3 x (cellheight + 2 separatorlineheigts) high

	//Go screen line by line
	for (unsigned int i = 0; i < DISPLAY_ROWS; i++) {
		//Row loop
		std::cout << "\n";
		//Space to line up row labels
		if (i < 10) std::cout << " ";
		std::cout << i << ": ";
		for (unsigned int j = 0; j < DISPLAY_COLS; j++) {
			//Column loop

			if ((i + 1) % DISPLAY_V_STRIDE == 0) {
				//Row of dividers
				if ((j + 1) % DISPLAY_H_STRIDE == 0) std::cout << DIVIDER_CROSS_CHAR;
				else std::cout << DIVIDER_H_CHAR;
			}
			else {
				//Not row of dividers
				if ((j + 1) % DISPLAY_H_STRIDE == 0) {
					//Divider in this column
					std::cout << DIVIDER_V_CHAR;
				}
				//Check for entries and virtical dividers
				else {
					if ((j + FIRST_ENTRY_COL) % DISPLAY_H_STRIDE == 0) {
						//Column has entries
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
					else std::cout << SPACE_CHAR;
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
bool TickTackToeGrid::isGridFull() {
	auto it = std::find(entries.begin(), entries.end(), ENTRIES_DEFAULT_CHAR);
	return it == entries.end();
}
void TickTackToeGrid::printEntries() {
	for (char c : entries)
		std::cout << c << ",";
	std::cout << std::endl;
}
bool TickTackToeGrid::newEntry(unsigned int index, char symbol) {
	//entry is in range?
	if (index >= GRID_CELLS) return false;
	//Cell is open?
	if (entries[index] != ENTRIES_DEFAULT_CHAR) return false;
	std::cout << "Index into entries:" << index;
	std::cout << " Current symbol: " << symbol << std::endl;
	entries[index] = symbol;
	return true;
}