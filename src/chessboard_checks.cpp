#include "chessboard.h"

bool Chessboard::isGuardian(Position position)
{
	Piece piece = pieces[position.x][position.y];
	if(piece.pieceType == KING || piece.pieceType == EMPTY)
		return false;
	Position king = piece.side == WHITE ? whiteKing : blackKing;
	if (!arePointsCollinear(position, king))
		return false;
	Position dir = Position(position.x > king.x ? 1 : (position.x < king.x ? -1 : 0), position.y > king.y ? 1 : (position.y < king.y ? -1 : 0)); // Should be replaced by a function
	
	for (int i = 1;; i++) {
		int x = position.x + dir.x * i, y = position.y + dir.y * i;
		if (x < 0 || x > 7 || y < 0 || y > 7)
			return false;
		if (pieces[x][y].pieceType == EMPTY)
			continue;
		if (pieces[x][y].side == piece.side * -1) {
			int found = 0;
			if (pieces[x][y].pieceType == QUEEN)
				found = 1;
			if (dir.x == 0 || dir.y == 0) {
				if (pieces[x][y].pieceType == ROOK)
					found = 1;
			}
			else if (pieces[x][y].pieceType == BISHOP)
				found = 1;
			if (found && piecesBetweenPoints(Position(x, y), king) == 1)
				return true;
		}
		break;
	}

	return false;
}
