#include <cstddef>

#include "Square.h"

Square::Square()
{
	occupyingPiece = NULL;
}

Square::Square(const Square& square)
{
	this->occupyingPiece = square.occupyingPiece;
}

bool Square::occupiedState()
{
	return occupyingPiece==NULL? false : true;
}

bool Square::occupySquare(Piece* piece)
{
	if(occupiedState()) return false;
	else occupyingPiece = piece;
	return true;
}

Piece* Square::removeOccupyingPiece()
{
	Piece* removed = occupyingPiece;
	occupyingPiece = NULL;
	return removed;
}

Piece* Square::getOccupyingPiece()
{
	return occupyingPiece;
}