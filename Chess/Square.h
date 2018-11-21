#ifndef SQUARE_H
#define SQUARE_H

#include "Piece.h"

class Square
{
	private:
		Piece* occupyingPiece;
	public:
		Square();
		Square(const Square& square);
		bool occupiedState();
		bool occupySquare(Piece* piece);
		Piece* removeOccupyingPiece();
		Piece* getOccupyingPiece();
};

#endif