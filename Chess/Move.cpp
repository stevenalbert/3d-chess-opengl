#include "Move.h"

Move::Move(MoveType mType, int fRow, int fCol, int tRow, int tCol, Piece* moved, Piece* captured)
{
	type = mType;
	fromRow = fRow;
	fromCol = fCol;
	toRow = tRow;
	toCol = tCol;
	movePiece = moved;
	capturedPiece = captured;
}

Move::Move(const Move& move)
{
	this->type = move.type;
	this->fromRow = move.fromRow;
	this->fromCol = move.fromCol;
	this->toRow = move.toRow;
	this->toCol = move.toCol;
	this->movePiece = move.movePiece;
	this->capturedPiece = move.capturedPiece;
}

std::pair<int, int> Move::getOriginPosition()
{
	return std::make_pair(fromRow, fromCol);
}

std::pair<int, int> Move::getDestinationPosition()
{
	return std::make_pair(toRow, toCol);
}

Piece* Move::getMovedPiece()
{
	return movePiece;
}

Piece* Move::getCapturedPiece()
{
	return capturedPiece;
}

MoveType Move::getType()
{
	return type;
}