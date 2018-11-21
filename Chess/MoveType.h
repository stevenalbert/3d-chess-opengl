#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H

enum class MoveType : int
{
	NORMAL,
	CAPTURE,
	EN_PASSANT,
	CASTLING
};

#endif