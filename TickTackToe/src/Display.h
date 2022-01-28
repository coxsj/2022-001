#pragma once
class Display
{
	char entries[9];
	const unsigned int NUM_SPACES = 1; //Number of spaces surrounding an entry
	const unsigned int NUM_ROWS = 3 + NUM_SPACES * 6 + 2;	//3 entries, x spaces, 2 dividers
	const unsigned int NUM_COLS = 3 + NUM_SPACES * 6 + 2;




	void drawGrid() {
		//3 rows and three char widths for each space
		//grid is 3 x 3 + 2(separator lines) = 11 chars wide
		//Grid is 3 x 3 + 2(separator lines) = 11 lines high
		for (auto i = 1; i < NUM_ROWS; i++) {
			for (auto j = 1; j < NUM_COLS; j++) {
				if ((j- NUM_SPACES)%(2 + 2 * NUM_SPACES) == 0) {
					//At column containing entry
					std::cout << 1 << ", " << j << std::endl;
				}
				else {
					//No entry in this column, only dividers

				}
					
					
					
					std::cout << entries[j / 11 + 0];
				

			}
		}
	}
	void initDisplay() {
		for (auto c : entries)
			c = ' ';
	}
};

