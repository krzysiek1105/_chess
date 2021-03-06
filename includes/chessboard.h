#pragma once
#include "piece.h"
#include <array>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utility>
#include <sstream>

class Chessboard
{
  public:
	typedef enum move_type
	{
		NORMAL,
		CASTLING,
		EN_PASSANT,
		PAWN_PROMOTION,
		PAWN_PROMOTION_WITH_BEATING,
		BEATING
	} MoveType;

	typedef struct move_t
	{
		PieceType pieceOnMove;

		MoveType moveType;
		Position from;
		Position to;
		// Used if moveType is CASTLING
		Side side;
		bool kingSideCastle;
		// Used if moveType is PAWN_PROMOTION
		PieceType pieceType;
		// Used in SAN notation to determine if it is a need to put row/column in output string
		bool ambiguousX;
		bool ambiguousY;

		// That's all we need if moveType is NORMAL
		move_t(Position from, Position to, PieceType pieceOnMove)
		{
			moveType = NORMAL;
			this->from = from;
			this->to = to;
			this->pieceOnMove = pieceOnMove;
			ambiguousX = false;
			ambiguousY = false;
			pieceType = EMPTY;
		}

		// If moveType is CASTLING
		move_t(Side side, bool kingSideCastle)
		{
			moveType = CASTLING;
			this->side = side;
			this->kingSideCastle = kingSideCastle;
			ambiguousX = false;
			ambiguousY = false;
			pieceType = EMPTY;
		}

		move_t(MoveType moveType, Position from, Position to, PieceType pieceOnMove)
		{
			this->moveType = moveType;
			this->from = from;
			this->to = to;
			this->pieceOnMove = pieceOnMove;
			ambiguousX = false;
			ambiguousY = false;
			pieceType = EMPTY;
		}
	} Move;

	typedef enum game_state
	{
		IN_GAME,
		CHECK,
		MATE,
		STALEMATE
	} GameState;

  private:
	std::array<std::array<Piece, 8>, 8> pieces;
	int movesDone;
	Position lastMove;
	Position whiteKing;
	Position blackKing;
	GameState gameState;
	std::string sanString;

	std::vector<Move> getLegalMovesPawn(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesKnight(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesBishop(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesRook(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesQueen(Position position, Position axis, Side side);
	std::vector<Move> getLegalMovesKing(Position position, Side side);
	std::vector<Move> getCastling();

	bool arePointsCollinear(Position a, Position b);
	int piecesBetweenPoints(Position a, Position b);
	Position getDirectionFromPoints(Position a, Position b);
	bool isPointBetweenPoints(Position a, Position b, Position toCheck);

	bool isSquareSafe(Position position, Side side);
	Position isGuardian(Position position);
	std::vector<Position> getChecks();
	bool isCastleSafe(Side side, bool isKingSideCastle);

  public:
	Chessboard();
	std::vector<Move> legalMoves;
	std::vector<Move> getLegalMovesAt(Position position);
	bool makeMove(Position from, Position to);
	bool makeMove(Side side, bool isKingSideCastle);
	bool makeMove(PieceType promoted, Position from, Position to);
	bool makeMove(Move);
	bool makeMove(std::string pgn);
	friend std::ostream &operator<<(std::ostream &s, const Chessboard &c);
	GameState getGameState();
	Side getCurrentSide();
	Piece getPieceAt(Position position);
	void getLegalMoves();
	void setSanString(PieceType promotion);
	std::string getSanString();
	std::vector<Move> moveHistory;
	void reset();
	Move moveFromSAN(std::string san);
	std::vector<Move> movesFromPGN(std::string pgn);
};