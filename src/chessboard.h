#pragma once
#include "piece.h"
#include <array>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utility>

class Chessboard
{
  public:
    typedef enum move_type
    {
        NORMAL,
        CASTLING,
        EN_PASSANT,
        PAWN_PROMOTION
    } MoveType;

    typedef struct move_t
    {
        MoveType moveType;
        Position from;
        Position to;
        // Used if moveType is CASTLING
        Side side;
        bool kingSideCastle;
        // Used if moveType is PAWN_PROMOTION
        PieceType pieceType;

        // That's all we need if moveType is NORMAL
        move_t(Position from, Position to)
        {
            moveType = NORMAL;
            this->from = from;
            this->to = to;
        }

        // If moveType is CASTLING
        move_t(Side side, bool kingSideCastle)
        {
            moveType = CASTLING;
            this->side = side;
            this->kingSideCastle = kingSideCastle;
        }

        move_t(MoveType moveType, Position from, Position to)
        {
            this->moveType = moveType;
            this->from = from;
            this->to = to;
        }
    } Move;

    std::array<std::array<Piece, 8>, 8> pieces;
    int movesDone;
    Position lastMove;
	Position whiteKing;
	Position blackKing;

    Chessboard();
    std::vector<Move> getLegalMovesAt(Position position);
    std::vector<Move> getCastling();
    std::vector<Move> getLegalMoves();
    bool makeMove(Position from, Position to);
    bool makeMove(Side side, bool isKingSideCastle);
    bool makeMove(PieceType promoted, Position from, Position to);
    friend std::ostream &operator<<(std::ostream &s, const Chessboard &c);

    bool arePointsCollinear(Position a, Position b);
    int piecesBetweenPoints(Position a, Position b);

	bool isGuardian(Position position);
    std::vector<Position> getChecks();
};