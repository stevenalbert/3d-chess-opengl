#include "Game.h"

Game::Game()
{
	gameplay = new Gameplay();
	board = new Board();
	status = new GameStatus();
	setInitialPieces(PieceColor::WHITE);
	setInitialPieces(PieceColor::BLACK);
	turn = 1;
}

void Game::setInitialPieces(PieceColor color)
{
	int firstRow = (color==PieceColor::WHITE)? board->MIN_ROW_INDEX : board->MAX_ROW_INDEX;
	int secondRow = (color==PieceColor::WHITE)? board->MIN_ROW_INDEX+1 : board->MAX_ROW_INDEX-1;
	int firstCol = board->MIN_COL_INDEX;
	/* PAWN */
	for(int col = firstCol; col<=board->MAX_COL_INDEX; col++)
	{
		board->getSquare(secondRow, col)->occupySquare(new Piece(PieceType::PAWN, color));
	}
	/* ROOK */
	board->getSquare(firstRow, firstCol)->occupySquare(new Piece(PieceType::ROOK, color));
	board->getSquare(firstRow, firstCol+7)->occupySquare(new Piece(PieceType::ROOK, color));
	/* KNIGHT */
	board->getSquare(firstRow, firstCol+1)->occupySquare(new Piece(PieceType::KNIGHT, color));
	board->getSquare(firstRow, firstCol+6)->occupySquare(new Piece(PieceType::KNIGHT, color));
	/* BISHOP */
	board->getSquare(firstRow, firstCol+2)->occupySquare(new Piece(PieceType::BISHOP, color));
	board->getSquare(firstRow, firstCol+5)->occupySquare(new Piece(PieceType::BISHOP, color));
	/* QUEEN */
	board->getSquare(firstRow, firstCol+3)->occupySquare(new Piece(PieceType::QUEEN, color));
	/* KING */
	board->getSquare(firstRow, firstCol+4)->occupySquare(new Piece(PieceType::KING, color));
}

std::stack<Move> Game::getAllLog()
{
	return log;
}

Board* Game::getBoard()
{
	return board;
}

GameStatus* Game::getGameStatus()
{
	return status;
}

std::vector<Move> Game::getValidMoves(int fromRow, int fromCol)
{
	return gameplay->getValidMoves(status, board, board->getSquare(fromRow, fromCol)->getOccupyingPiece(), fromRow, fromCol);
}

bool Game::move(int fromRow, int fromCol, int toRow, int toCol)
{
	std::vector<Move> valid = getValidMoves(fromRow, fromCol);
	int sz = valid.size();
	for(int index=0; index<sz; index++)
		if(valid[index].getDestinationPosition().first == toRow && valid[index].getDestinationPosition().second == toCol)
			if(gameplay->move(status, board, valid[index]))
			{
				log.push(valid[index]);
				return true;
			}

	return false;
}

PieceColor Game::getPieceColor(int fromRow, int fromCol)
{
	return board->getSquare(fromRow, fromCol)->getOccupyingPiece()->getColor();
}

Piece* Game::getPiece(int fromRow, int fromCol)
{
    return board->getSquare(fromRow, fromCol)->getOccupyingPiece();
}

bool Game::isSquareOccupied(int fromRow, int fromCol)
{
    return board->getSquare(fromRow, fromCol)->occupiedState();
}

bool Game::inCheckState()
{
	return gameplay->isCheckState(status, board, getTurnColor());
}

bool Game::inCheckMateState()
{
	return gameplay->isCheckMateState(status, board, getTurnColor());
}

int Game::getTurn()
{
	return turn;
}

void Game::nextTurn()
{
	turn++;
	status->setPieceEnPassantable(getTurnColor(), NULL);
}

PieceColor Game::getTurnColor()
{
	return turn & 1? PieceColor::WHITE : PieceColor::BLACK;
}

bool Game::promote(int row, int col, PieceType type)
{
	return gameplay->pawnPromotion(board, row, col, type);
}
