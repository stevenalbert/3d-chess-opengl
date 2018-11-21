#include <cstddef>
#include <cmath>
#include <iostream>

#include "Gameplay.h"

bool Gameplay::move(GameStatus* status, Board* board, Move move)
{
	Piece* actPiece = move.getMovedPiece();
	int fromRow = move.getOriginPosition().first;
	int fromCol = move.getOriginPosition().second;
	int toRow = move.getDestinationPosition().first;
	int toCol = move.getDestinationPosition().second;
	switch(move.getType())
	{
		case MoveType::NORMAL:
		{
			if(actPiece->getType()==PieceType::PAWN && std::abs(toRow-fromRow)==2)
				status->setPieceEnPassantable(actPiece->getColor(), actPiece);
			else if(actPiece->getType()==PieceType::KING)
				status->setKingMove(actPiece->getColor());
			else if(actPiece->getType()==PieceType::ROOK)
			{
				if(fromCol == board->MIN_COL_INDEX) status->setFirstColRookMove(actPiece->getColor());
				else if(fromCol == board->MAX_COL_INDEX) status->setLastColRookMove(actPiece->getColor());
			}
			return board->getSquare(toRow, toCol)->occupySquare(board->getSquare(fromRow, fromCol)->removeOccupyingPiece());
		}
		case MoveType::CAPTURE:
		{
			if(actPiece->getType()==PieceType::KING)
				status->setKingMove(actPiece->getColor());
			else if(actPiece->getType()==PieceType::ROOK)
			{
				if(fromCol == board->MIN_COL_INDEX) status->setFirstColRookMove(actPiece->getColor());
				else if(fromCol == board->MAX_COL_INDEX) status->setLastColRookMove(actPiece->getColor());
			}
			board->getSquare(toRow, toCol)->removeOccupyingPiece();
			return board->getSquare(toRow, toCol)->occupySquare(board->getSquare(fromRow, fromCol)->removeOccupyingPiece());
		}
		case MoveType::EN_PASSANT:
		{
			board->getSquare(fromRow, toCol)->removeOccupyingPiece();
			return board->getSquare(toRow, toCol)->occupySquare(board->getSquare(fromRow, fromCol)->removeOccupyingPiece());
		}
		case MoveType::CASTLING:
		{
			int rookOriginCol = ((toCol < fromCol) ? board->MIN_COL_INDEX : board->MAX_COL_INDEX);
			int rookDestCol = ((toCol < fromCol) ? fromCol-1 : fromCol+1);
			board->getSquare(toRow, toCol)->occupySquare(board->getSquare(fromRow, fromCol)->removeOccupyingPiece());
			board->getSquare(toRow, rookDestCol)->occupySquare(board->getSquare(fromRow, rookOriginCol)->removeOccupyingPiece());
			status->setKingMove(actPiece->getColor());
			rookOriginCol > rookDestCol ? status->setLastColRookMove(actPiece->getColor()) : status->setFirstColRookMove(actPiece->getColor());
			return true;
		}
		default: break;
	}
	return false;
}

std::vector<Move> Gameplay::getPossibleMoves(GameStatus* status, Board* board, Piece* piece, int fromRow, int fromCol)
{
	std::vector<Move> possibleMove;
	int row, col;

	if(status == NULL || board == NULL || piece == NULL || fromRow < board->MIN_ROW_INDEX || fromRow > board->MAX_ROW_INDEX || fromCol < board->MIN_COL_INDEX || fromCol > board->MAX_COL_INDEX) return possibleMove;

	switch(piece->getType())
	{
		case PieceType::PAWN:
		{
			/*WHITE PIECE*/
			if(piece->getColor()==PieceColor::WHITE)
			{
				/*
					SINGLE MOVE
				*/
				row = fromRow+1;
				col = fromCol;
				if(row <= board->MAX_ROW_INDEX)
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
					if(board->getSquare(row, col)->occupiedState()) possibleMove.pop_back();
				}

				/*
					DOUBLE MOVE
				*/
				if(possibleMove.size() && fromRow == board->MIN_ROW_INDEX + 1)
				{
					row = fromRow+2;
					col = fromCol;
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
					if(board->getSquare(row, col)->occupiedState()) possibleMove.pop_back();
				}

				/*
					NORMAL CAPTURE
				*/
				row = fromRow+1;
				col = fromCol-1;
				if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
				{
					if(board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
							possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					}
				}

				row = fromRow+1;
				col = fromCol+1;
				if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
				{
					if(board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
							possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					}
				}

				/*
					EN PASSANT
				*/
				if(fromRow == board->MIN_ROW_INDEX + 4)
				{
					row = fromRow+1;
					col = fromCol-1;
					if(col >= board->MIN_COL_INDEX && !board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(fromRow, col)->occupiedState())
						{
							Piece* capPossibility = board->getSquare(fromRow, col)->getOccupyingPiece();
							if(capPossibility->getType() == PieceType::PAWN && capPossibility->getColor() != piece->getColor() && status->pieceEnPassantable(capPossibility->getColor()) == capPossibility)
								possibleMove.push_back(Move(MoveType::EN_PASSANT, fromRow, fromCol, row, col, piece, capPossibility));
						}
					}

					col = fromCol+1;
					if(col <= board->MAX_COL_INDEX && !board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(fromRow, col)->occupiedState())
						{
							Piece* capPossibility = board->getSquare(fromRow, col)->getOccupyingPiece();
							if(capPossibility->getType() == PieceType::PAWN && capPossibility->getColor() != piece->getColor() && status->pieceEnPassantable(capPossibility->getColor()) == capPossibility)
								possibleMove.push_back(Move(MoveType::EN_PASSANT, fromRow, fromCol, row, col, piece, capPossibility));
						}
					}
				}
			}

			/*BLACK PIECE*/
			else if(piece->getColor()==PieceColor::BLACK)
			{
				/*
					SINGLE MOVE
				*/
				row = fromRow-1;
				col = fromCol;
				if(row >= board->MIN_ROW_INDEX)
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
					if(board->getSquare(row, col)->occupiedState()) possibleMove.pop_back();
				}

				/*
					DOUBLE MOVE
				*/
				if(possibleMove.size() && fromRow == board->MAX_ROW_INDEX - 1)
				{
					row = fromRow-2;
					col = fromCol;
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
					if(board->getSquare(row, col)->occupiedState()) possibleMove.pop_back();
				}

				/*
					NORMAL CAPTURE
				*/
				row = fromRow-1;
				col = fromCol-1;
				if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
				{
					if(board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
							possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					}
				}

				row = fromRow-1;
				col = fromCol+1;
				if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
				{
					if(board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
							possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					}
				}

				/*
					EN PASSANT
				*/
				if(fromRow == board->MAX_ROW_INDEX - 4)
				{
					row = fromRow-1;
					col = fromCol-1;
					if(col >= board->MIN_COL_INDEX && !board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(fromRow, col)->occupiedState())
						{
							Piece* capPossibility = board->getSquare(fromRow, col)->getOccupyingPiece();
							if(capPossibility->getType() == PieceType::PAWN && capPossibility->getColor() != piece->getColor() && status->pieceEnPassantable(capPossibility->getColor()) == capPossibility)
								possibleMove.push_back(Move(MoveType::EN_PASSANT, fromRow, fromCol, row, col, piece, capPossibility));
						}
					}

					col = fromCol+1;
					if(col <= board->MAX_COL_INDEX && !board->getSquare(row, col)->occupiedState())
					{
						if(board->getSquare(fromRow, col)->occupiedState())
						{
							Piece* capPossibility = board->getSquare(fromRow, col)->getOccupyingPiece();
							if(capPossibility->getType() == PieceType::PAWN && capPossibility->getColor() != piece->getColor() && status->pieceEnPassantable(capPossibility->getColor()) == capPossibility)
								possibleMove.push_back(Move(MoveType::EN_PASSANT, fromRow, fromCol, row, col, piece, capPossibility));
						}
					}
				}
			}

			break;
		}
		case PieceType::ROOK:
		{
			/*
				Get possible moves from its position to lower index row
			*/
			row = fromRow-1;
			col = fromCol;
			while(row >= board->MIN_ROW_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				row--;
			}

			/*
				Get possible moves from its position to upper index row
			*/
			row = fromRow+1;
			col = fromCol;
			while(row <= board->MAX_ROW_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				row++;
			}

			/*
				Get possible moves from its position to lower index column
			*/
			row = fromRow;
			col = fromCol-1;
			while(col >= board->MIN_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				col--;
			}

			/*
				Get possible moves from its position to upper index column
			*/
			row = fromRow;
			col = fromCol+1;
			while(col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				col++;
			}
			break;
		}
		case PieceType::BISHOP:
		{
			/*
				NORMAL DIAGONALLY
			*/
			row = fromRow+1;
			col = fromCol-1;
			while(row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row++;
				col--;
			}

			row = fromRow+1;
			col = fromCol+1;
			while(row <= board->MAX_ROW_INDEX && col <= board->MAX_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row++;
				col++;
			}

			row = fromRow-1;
			col = fromCol+1;
			while(row >= board->MIN_ROW_INDEX && col <= board->MAX_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row--;
				col++;
			}

			row = fromRow-1;
			col = fromCol-1;
			while(row >= board->MIN_ROW_INDEX && col >= board->MIN_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row--;
				col--;
			}

			break;
		}
		case PieceType::KNIGHT:
		{
			/*
				NORMAL L-JUMP
			*/
			row = fromRow-2;
			col = fromCol-1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow-1;
			col = fromCol-2;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow+1;
			col = fromCol-2;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow+2;
			col = fromCol-1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow+2;
			col = fromCol+1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow+1;
			col = fromCol+2;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow-1;
			col = fromCol+2;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			row = fromRow-2;
			col = fromCol+1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			break;
		}
		case PieceType::QUEEN:
		{
			/*
				ROOK MOVE
			*/
			row = fromRow-1;
			col = fromCol;
			while(row >= board->MIN_ROW_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				row--;
			}

			/*
				Get possible moves from its position to upper index row
			*/
			row = fromRow+1;
			col = fromCol;
			while(row <= board->MAX_ROW_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				row++;
			}

			/*
				Get possible moves from its position to lower index column
			*/
			row = fromRow;
			col = fromCol-1;
			while(col >= board->MIN_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				col--;
			}

			/*
				Get possible moves from its position to upper index column
			*/
			row = fromRow;
			col = fromCol+1;
			while(col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() == piece->getColor())
						break;
					else
					{
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
						break;
					}
				}
				col++;
			}

			/*
				BISHOP MOVE
			*/
			row = fromRow+1;
			col = fromCol-1;
			while(row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row++;
				col--;
			}

			row = fromRow+1;
			col = fromCol+1;
			while(row <= board->MAX_ROW_INDEX && col <= board->MAX_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row++;
				col++;
			}

			row = fromRow-1;
			col = fromCol+1;
			while(row >= board->MIN_ROW_INDEX && col <= board->MAX_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row--;
				col++;
			}

			row = fromRow-1;
			col = fromCol-1;
			while(row >= board->MIN_ROW_INDEX && col >= board->MIN_COL_INDEX)
			{
				if(!board->getSquare(row, col)->occupiedState())
				{
					possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				}
				else
				{
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
					break;
				}
				row--;
				col--;
			}

			break;
		}
		case PieceType::KING:
		{
			/*
				Check move to position (fromRow-1, fromCol-1)
			*/
			row = fromRow-1;
			col = fromCol-1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow-1, fromCol)
			*/
			row = fromRow-1;
			col = fromCol;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow-1, fromCol+1)
			*/
			row = fromRow-1;
			col = fromCol+1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow, fromCol-1)
			*/
			row = fromRow;
			col = fromCol-1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow, fromCol+1)
			*/
			row = fromRow;
			col = fromCol+1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow+1, fromCol-1)
			*/
			row = fromRow+1;
			col = fromCol-1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow+1, fromCol)
			*/
			row = fromRow+1;
			col = fromCol;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				Check move to position (fromRow+1, fromCol+1)
			*/
			row = fromRow+1;
			col = fromCol+1;
			if(row >= board->MIN_ROW_INDEX && row <= board->MAX_ROW_INDEX && col >= board->MIN_COL_INDEX && col <= board->MAX_COL_INDEX)
			{
				possibleMove.push_back(Move(MoveType::NORMAL, fromRow, fromCol, row, col, piece));
				if(board->getSquare(row, col)->occupiedState())
				{
					possibleMove.pop_back();
					if(board->getSquare(row, col)->getOccupyingPiece()->getColor() != piece->getColor())
						possibleMove.push_back(Move(MoveType::CAPTURE, fromRow, fromCol, row, col, piece, board->getSquare(row, col)->getOccupyingPiece()));
				}
			}

			/*
				CASTLING
			*/
			if(!status->isKingMove(piece->getColor()) && !status->isFirstColRookMove(piece->getColor()))
			{
				if(!board->getSquare(fromRow, fromCol-1)->occupiedState() && !board->getSquare(fromRow, fromCol-2)->occupiedState())
					possibleMove.push_back(Move(MoveType::CASTLING, fromRow, fromCol, fromRow, fromCol-2, piece));
			}
			if(!status->isKingMove(piece->getColor()) && !status->isLastColRookMove(piece->getColor()))
			{
				if(!board->getSquare(fromRow, fromCol+1)->occupiedState() && !board->getSquare(fromRow, fromCol+2)->occupiedState())
					possibleMove.push_back(Move(MoveType::CASTLING, fromRow, fromCol, fromRow, fromCol+2, piece));
			}

			break;
		}
	}
	return possibleMove;
}

std::vector<Move> Gameplay::getValidMoves(GameStatus* status, Board* board, Piece* piece, int fromRow, int fromCol)
{
	std::vector<Move> validMoves = getPossibleMoves(status, board, piece, fromRow, fromCol);
	int sz = validMoves.size();
	for(int index=0; index<sz; index++)
	{
		/* Check move will make "CHECK" or not */
		Board* nextBoard = new Board(*board);
		GameStatus* nextStatus = new GameStatus(*status);
		move(nextStatus, nextBoard, validMoves[index]);
		if(isCheckState(nextStatus, nextBoard, piece->getColor()))
		{
			validMoves.erase(validMoves.begin()+index);
			sz--;
			index--;
		}

		delete(nextBoard);
		delete(nextStatus);
	}
	return validMoves;
}

bool Gameplay::isCheckState(GameStatus* status, Board* board, PieceColor color)
{
	int sz, kingRow = board->MIN_ROW_INDEX - 1, kingCol = board->MIN_COL_INDEX - 1;
	std::vector<Move> moves;
	Piece *actPiece;
	for(int row = board->MIN_ROW_INDEX; row <= board->MAX_ROW_INDEX; row++)
	{
		for(int col = board->MIN_COL_INDEX; col <= board->MAX_COL_INDEX; col++)
		{
			if(!board->getSquare(row, col)->occupiedState()) continue;
			actPiece = board->getSquare(row, col)->getOccupyingPiece();
			if(actPiece->getType() == PieceType::KING && actPiece->getColor() == color)
			{
				kingRow = row;
				kingCol = col;
				break;
			}
		}
		if(kingRow != board->MIN_ROW_INDEX - 1 && kingCol != board->MIN_COL_INDEX - 1)break;
	}

	for(int row = board->MIN_ROW_INDEX; row <= board->MAX_ROW_INDEX; row++)
	{
		for(int col = board->MIN_COL_INDEX; col <= board->MAX_COL_INDEX; col++)
		{
			if(!board->getSquare(row, col)->occupiedState()) continue;
			actPiece = board->getSquare(row, col)->getOccupyingPiece();
			if(actPiece->getColor() != color)
			{
				/* Get possible moves */
				moves = getPossibleMoves(status, board, actPiece, row, col);
				sz = moves.size();
				for(int index=0; index<sz; index++)
				{
					if(moves[index].getDestinationPosition().first == kingRow && moves[index].getDestinationPosition().second == kingCol) return true;
				}
				moves.clear();
			}
		}
	}
	return false;
}

bool Gameplay::isCheckMateState(GameStatus* status, Board* board, PieceColor color)
{
	return allValidMoveCount(status, board, color)==0;
}

int Gameplay::allValidMoveCount(GameStatus* status, Board* board, PieceColor color)
{
	int count = 0;
	Piece* actPiece;
	for(int row = board->MIN_ROW_INDEX; row <= board->MAX_ROW_INDEX; row++)
	{
		for(int col = board->MIN_COL_INDEX; col <= board->MAX_COL_INDEX; col++)
		{
			if(board->getSquare(row, col)->occupiedState())
			{
				actPiece = board->getSquare(row, col)->getOccupyingPiece();
				if(actPiece->getColor() == color)
				{
					count+=getValidMoves(status, board, actPiece, row, col).size();
				}
			}
		}
	}

	return count;
}

bool Gameplay::pawnPromotion(Board* board, int row, int col, PieceType promoteTo)
{
	Square* activeSquare;
	if(board->getSquare(row, col)->occupiedState())
	{
		activeSquare = board->getSquare(row, col);
		if(activeSquare->getOccupyingPiece()->getType() == PieceType::PAWN)
		{
			Piece* removed = activeSquare->removeOccupyingPiece();
			activeSquare->occupySquare(new Piece(promoteTo, removed->getColor()));
			return true;
		}
	}
	return false;
}
