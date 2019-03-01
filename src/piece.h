#pragma once
#include <cstdint>
#include <vector>

typedef enum piece_type
{
	EMPTY,
	PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING,
} PieceType;

typedef enum side
{
	BLACK = -1,
	NONE,
	WHITE
} Side;

typedef struct position_t
{
    int x;
    int y;

    position_t()
    {
        this->x = 0;
        this->y = 0;
    }

    position_t(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    bool operator==(position_t p)
    {
        return this->x == p.x && this->y == p.y;
    }
} Position;

class Piece
{
  public:
    PieceType pieceType;
    Side side;
    bool firstMoveDone;
	bool twoSquares;

    Piece()
    {
        pieceType = EMPTY;
        side = NONE;
    }

    Piece(PieceType pieceType, Side side)
    {
        this->pieceType = pieceType;
        this->side = side;
        firstMoveDone = false;
		twoSquares = false;
    }
};