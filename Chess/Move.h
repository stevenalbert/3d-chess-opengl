#ifndef MOVE_H
#define MOVE_H

#include <utility>
#include <cstddef>

#include "Piece.h"
#include "MoveType.h"

class Move
{
	private:
		int fromRow;
		int fromCol;
		Piece* movePiece;
		int toRow;
		int toCol;
		Piece* capturedPiece;
		MoveType type;
		
	public:
		Move(MoveType mType, int fRow, int fCol, int tRow, int tCol, Piece* moved, Piece* captured=NULL);
		Move(const Move& move);
		std::pair<int, int> getOriginPosition();
		std::pair<int, int> getDestinationPosition();
		Piece* getMovedPiece();
		Piece* getCapturedPiece();
		MoveType getType();
};

#endif