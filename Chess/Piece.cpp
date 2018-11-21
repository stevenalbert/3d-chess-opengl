#include "Piece.h"

Piece::Piece(PieceType pType, PieceColor pColor)
{
	type = pType;
	color = pColor;
}

Piece::Piece(const Piece &piece)
{
	this->type = piece.type;
	this->color = piece.color;
}

PieceType Piece::getType()
{
	return type;
}

PieceColor Piece::getColor()
{
	return color;
}