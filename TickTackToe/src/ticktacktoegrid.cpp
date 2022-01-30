

#include <iostream>

#include "ticktacktoegrid.h"
#include "utility.h"

void TickTackToeGrid::drawGame() {
	std::cout.flush();
	Utility::clear_screen();
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
	if (index >= NUM_CELLS) return false;
	//Cell is open?
	if (entries[index] != ENTRIES_DEFAULT_CHAR) return false;
	std::cout << "Index into entries:" << index;
	std::cout << " Current symbol: " << symbol << std::endl;
	entries[index] = symbol;
	return true;
}