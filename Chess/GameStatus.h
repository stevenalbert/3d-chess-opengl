#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "Piece.h"
#include "PieceColor.h"

class GameStatus
{
	private:
		bool whiteKingMove;
		bool blackKingMove;
		
		Piece* whitePieceEnPassant;
		Piece* blackPieceEnPassant;
		
		bool whiteFirstColRookMove;
		bool whiteLastColRookMove;
		bool blackFirstColRookMove;
		bool blackLastColRookMove;
	
	public:
		GameStatus();
		GameStatus(const GameStatus& gameStatus);
		
		void setKingMove(PieceColor color);
		void setPieceEnPassantable(PieceColor color, Piece *piece);
		void setFirstColRookMove(PieceColor color);
		void setLastColRookMove(PieceColor color);
		
		bool isKingMove(PieceColor color);
		Piece* pieceEnPassantable(PieceColor color);
		bool isFirstColRookMove(PieceColor color);
		bool isLastColRookMove(PieceColor color);
};

#endif