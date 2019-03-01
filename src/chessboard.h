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
	std::vector<Move> getLegalMovesPawn(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesKnight(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesBishop(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesRook(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesQueen(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesKing(Position position, Side side);
	std::vector<Move> getLegalMovesAt(Position position);
	std::vector<Move> getCastling();
	std::vector<Move> getLegalMoves();
	bool makeMove(Position from, Position to);
	bool makeMove(Side side, bool isKingSideCastle);
	bool makeMove(PieceType promoted, Position from, Position to);
	friend std::ostream& operator<<(std::ostream& s, const Chessboard& c);

	bool arePointsCollinear(Position a, Position b);
	int piecesBetweenPoints(Position a, Position b);
	Position getDirectionFromPoints(Position a, Position b);
	bool isPointBetweenPoints(Position a, Position b, Position toCheck);

	bool isSquareSafe(Position position, Side side);
	Position isGuardian(Position position);
	std::vector<Position> getChecks();
    bool isCastleSafe(Side side, bool isKingSideCastle);
};