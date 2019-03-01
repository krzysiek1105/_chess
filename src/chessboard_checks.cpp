#include "chessboard.h"

bool Chessboard::isSquareSafe(Position position, Side side)
{
	std::vector<Chessboard::Move> legalMoves;
	Position notGuardian = Position(0, 0);
	legalMoves = getLegalMovesKnight(position, notGuardian, side);
	for (Move move : legalMoves)
	{
		if (pieces[move.to.x][move.to.y].pieceType == KNIGHT && pieces[move.to.x][move.to.y].side == side * -1)
			return false;
	}
	legalMoves = getLegalMovesBishop(position, notGuardian, side);
	for (Move move : legalMoves)
	{
		if ((pieces[move.to.x][move.to.y].pieceType == BISHOP || pieces[move.to.x][move.to.y].pieceType == QUEEN) && pieces[move.to.x][move.to.y].side == side * -1)
			return false;
	}
	legalMoves = getLegalMovesRook(position, notGuardian, side);
	for (Move move : legalMoves)
	{
		if ((pieces[move.to.x][move.to.y].pieceType || pieces[move.to.x][move.to.y].pieceType == QUEEN) && pieces[move.to.x][move.to.y].side == side * -1)
			return false;
	}
	legalMoves = getLegalMovesPawn(position, notGuardian, side);
	for (Move move : legalMoves)
	{
		if (pieces[move.to.x][move.to.y].pieceType == PAWN && pieces[move.to.x][move.to.y].side == side * -1 && move.to.x != position.x)
			return false;
	}
	legalMoves = getLegalMovesKing(position, side);
	for (Move move : legalMoves)
	{
		if (pieces[move.to.x][move.to.y].pieceType == KING && pieces[move.to.x][move.to.y].side == side * -1)
			return false;
	}
	return true;
}
Position Chessboard::isGuardian(Position position)
{
	Piece piece = pieces[position.x][position.y];
	if(piece.pieceType == KING || piece.pieceType == EMPTY)
		return Position(0, 0);
	Position king = piece.side == WHITE ? whiteKing : blackKing;
	if (piecesBetweenPoints(position, king))
		return Position(0, 0);
	Position dir = getDirectionFromPoints(king, position);
	
	for (int i = 1;; i++) {
		int x = position.x + dir.x * i, y = position.y + dir.y * i;
		if (x < 0 || x > 7 || y < 0 || y > 7)
			return Position(0, 0);
		if (pieces[x][y].pieceType == EMPTY)
			continue;
		if (pieces[x][y].side == piece.side * -1) {
			if (pieces[x][y].pieceType == QUEEN)
				return dir;
			if (dir.x == 0 || dir.y == 0) {
				if (pieces[x][y].pieceType == ROOK)
					return dir;
			}
			else if (pieces[x][y].pieceType == BISHOP)
				return dir;
		}
		break;
	}

	return Position(0, 0);
}

std::vector<Position> Chessboard::getChecks()
{
	Side side = movesDone % 2 == 0 ? WHITE : BLACK;
	Position kingPos = movesDone % 2 == 0 ? whiteKing : blackKing;

	std::vector<Position> result;

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (pieces[x][y].side == side * -1)
			{
				std::vector<Move> moves = getLegalMovesAt(Position(x, y));
				for (Move move : moves)
					if (move.to == kingPos)
						result.push_back(Position(move.from));
			}
		}
	}

	return result;
}