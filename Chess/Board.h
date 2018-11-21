#ifndef BOARD_H
#define BOARD_H

#include "Square.h"

class Board
{
	private:
		Square squares[10][10];
	public:
		const int MIN_COL_INDEX = 1;
		const int MIN_ROW_INDEX = 1;
		const int MAX_COL_INDEX = 8;
		const int MAX_ROW_INDEX = 8;
		Board();
		//~Board();
		Board(const Board& board);
		Square* getSquare(int row, int col);
		
};

#endif