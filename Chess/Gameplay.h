#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <vector>

#include "Board.h"
#include "Move.h"
#include "Piece.h"
#include "PieceColor.h"
#include "MoveType.h"
#include "GameStatus.h"

class Gameplay
{
	public:
		bool move(GameStatus* status, Board* board, Move move);
		std::vector<Move> getPossibleMoves(GameStatus* status, Board* board, Piece* piece, int fromRow, int fromCol);
		std::vector<Move> getValidMoves(GameStatus* status, Board* board, Piece* piece, int fromRow, int fromCol);
		bool isCheckState(GameStatus* status, Board* board, PieceColor color);
		bool isCheckMateState(GameStatus* status, Board* board, PieceColor color);
		int allValidMoveCount(GameStatus* status, Board* board, PieceColor color);
		bool pawnPromotion(Board* board, int row, int col, PieceType promoteTo);
};

#endif