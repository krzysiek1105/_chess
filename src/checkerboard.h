#ifndef __CHECKERBOARD_H__
#define __CHECKERBOARD_H__

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include "position.h"

typedef enum _PieceType
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
} PieceType;

class Piece
{
  public:
    PieceType pieceType;

    Piece(PieceType pieceType)
    {
        this->pieceType = pieceType;
    }
};

class Checkerboard
{
  public:
    std::unordered_map<Position, Piece> whitePieces;
    std::unordered_map<Position, Piece> blackPieces;
    int32_t movesDone;

    Checkerboard()
    {
        movesDone = 0;

        whitePieces.emplace(std::make_pair(Position("a2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("b2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("c2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("d2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("e2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("f2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("g2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("h2"), Piece(PAWN)));
        whitePieces.emplace(std::make_pair(Position("a1"), Piece(ROOK)));
        whitePieces.emplace(std::make_pair(Position("h1"), Piece(ROOK)));
        whitePieces.emplace(std::make_pair(Position("b1"), Piece(BISHOP)));
        whitePieces.emplace(std::make_pair(Position("g1"), Piece(BISHOP)));
        whitePieces.emplace(std::make_pair(Position("c1"), Piece(KNIGHT)));
        whitePieces.emplace(std::make_pair(Position("f1"), Piece(KNIGHT)));
        whitePieces.emplace(std::make_pair(Position("d1"), Piece(QUEEN)));
        whitePieces.emplace(std::make_pair(Position("e1"), Piece(KING)));

        blackPieces.emplace(std::make_pair(Position("a7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("b7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("c7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("d7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("e7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("f7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("g7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("h7"), Piece(PAWN)));
        blackPieces.emplace(std::make_pair(Position("a8"), Piece(ROOK)));
        blackPieces.emplace(std::make_pair(Position("h8"), Piece(ROOK)));
        blackPieces.emplace(std::make_pair(Position("b8"), Piece(BISHOP)));
        blackPieces.emplace(std::make_pair(Position("g8"), Piece(BISHOP)));
        blackPieces.emplace(std::make_pair(Position("c8"), Piece(KNIGHT)));
        blackPieces.emplace(std::make_pair(Position("f8"), Piece(KNIGHT)));
        blackPieces.emplace(std::make_pair(Position("d8"), Piece(QUEEN)));
        blackPieces.emplace(std::make_pair(Position("e8"), Piece(KING)));
    }

    friend std::ostream &operator<<(std::ostream &s, const Checkerboard &c)
    {
        for (int8_t row = 7; row >= 0; row--)
        {
            for (int8_t col = 0; col <= 7; col++)
            {
                Position position(row, col);

                auto searchWhite = c.whitePieces.find(position);
                if (searchWhite == c.whitePieces.end())
                {
                    auto searchBlack = c.blackPieces.find(position);
                    s << (searchBlack == c.blackPieces.end() ? '.' : "pbkrq*"[searchBlack->second.pieceType]);
                }
                else
                    s << "PBKRQ^"[searchWhite->second.pieceType];
            }
            s << std::endl;
        }
        return s;
    }
};

#endif //__CHECKERBOARD_H__