#pragma once
#include <cstdint>
#include <vector>

typedef enum piece_type
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING,
    EMPTY
} PieceType;

typedef enum side
{
    WHITE,
    BLACK,
    NONE
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

    bool operator==(position_t &p)
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
    bool castleAvailable;

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
        castleAvailable = true;
    }
};