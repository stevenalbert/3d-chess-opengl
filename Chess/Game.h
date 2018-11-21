#ifndef GAME_H
#define GAME_H

#include <stack>

#include "Piece.h"
#include "Gameplay.h"
#include "Board.h"
#include "GameStatus.h"
#include "Move.h"

class Game
{
	private:
		std::stack<Move> log;
		Gameplay* gameplay;
		Board* board;
		GameStatus* status;
		int turn;
	public:
		Game();
		void setInitialPieces(PieceColor color);
		std::stack<Move> getAllLog();
		Board* getBoard();
		GameStatus* getGameStatus();
		std::vector<Move> getValidMoves(int fromRow, int fromCol);
		bool move(int fromRow, int fromCol, int toRow, int toCol);
		PieceColor getPieceColor(int fromRow, int fromCol);
		Piece* getPiece(int fromRow, int fromCol);
		bool isSquareOccupied(int fromRow, int fromCol);
		bool inCheckState();
		bool inCheckMateState();
		int getTurn();
		void nextTurn();
		PieceColor getTurnColor();
		bool promote(int row, int col, PieceType type);
};

#endif
