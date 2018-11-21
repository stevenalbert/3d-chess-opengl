#include <cstddef>

#include "GameStatus.h"

GameStatus::GameStatus()
{
	whiteKingMove = false;
	blackKingMove = false;
	whitePieceEnPassant = NULL;
	blackPieceEnPassant = NULL;
	whiteFirstColRookMove = false;
	whiteLastColRookMove = false;
	blackFirstColRookMove = false;
	blackLastColRookMove = false;
}

GameStatus::GameStatus(const GameStatus& gameStatus)
{
	this->whiteKingMove = gameStatus.whiteKingMove;
	this->blackKingMove = gameStatus.blackKingMove;
	this->whitePieceEnPassant = gameStatus.whitePieceEnPassant;
	this->blackPieceEnPassant = gameStatus.blackPieceEnPassant;
	this->whiteFirstColRookMove = gameStatus.whiteFirstColRookMove;
	this->whiteLastColRookMove = gameStatus.whiteLastColRookMove;
	this->blackFirstColRookMove = gameStatus.blackFirstColRookMove;
	this->blackLastColRookMove = gameStatus.blackLastColRookMove;
}

void GameStatus::setKingMove(PieceColor color)
{
	if(color==PieceColor::WHITE) whiteKingMove = true;
	else if(color==PieceColor::BLACK) blackKingMove = true;
}

void GameStatus::setPieceEnPassantable(PieceColor color, Piece *piece)
{
	if(color==PieceColor::WHITE) whitePieceEnPassant = piece;
	else if(color==PieceColor::BLACK) blackPieceEnPassant = piece;
}

void GameStatus::setFirstColRookMove(PieceColor color)
{
	if(color==PieceColor::WHITE) whiteFirstColRookMove = true;
	else if(color==PieceColor::BLACK) blackFirstColRookMove = true;
}

void GameStatus::setLastColRookMove(PieceColor color)
{
	if(color==PieceColor::WHITE) whiteLastColRookMove = true;
	else if(color==PieceColor::BLACK) blackLastColRookMove = true;
}

bool GameStatus::isKingMove(PieceColor color)
{
	if(color==PieceColor::WHITE) return whiteKingMove;
	else if(color==PieceColor::BLACK) return blackKingMove;
}

Piece* GameStatus::pieceEnPassantable(PieceColor color)
{
	if(color==PieceColor::WHITE) return whitePieceEnPassant;
	else if(color==PieceColor::BLACK) return blackPieceEnPassant;
}

bool GameStatus::isFirstColRookMove(PieceColor color)
{
	if(color==PieceColor::WHITE) return whiteFirstColRookMove;
	else if(color==PieceColor::BLACK) return blackFirstColRookMove;
}

bool GameStatus::isLastColRookMove(PieceColor color)
{
	if(color==PieceColor::WHITE) return whiteLastColRookMove;
	else if(color==PieceColor::BLACK) return blackLastColRookMove;
}
