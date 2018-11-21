#ifndef PIECE_H
#define PIECE_H

#include "PieceType.h"
#include "PieceColor.h"

class Piece
{
	private:
		PieceType type;
		PieceColor color;
	public:
		Piece(PieceType pType, PieceColor pColor);
		Piece(const Piece &piece);
		PieceType getType();
		PieceColor getColor();
};

#endif