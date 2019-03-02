#include "chessboard.h"

Chessboard::Chessboard()
{
	movesDone = 0;
	lastMove = Position(0, 0);
	whiteKing = Position(4, 0);
	blackKing = Position(4, 7);
	pieces[0][0] = Piece(ROOK, WHITE);
	pieces[1][0] = Piece(KNIGHT, WHITE);
	pieces[2][0] = Piece(BISHOP, WHITE);
	pieces[3][0] = Piece(QUEEN, WHITE);
	pieces[4][0] = Piece(KING, WHITE);
	pieces[5][0] = Piece(BISHOP, WHITE);
	pieces[6][0] = Piece(KNIGHT, WHITE);
	pieces[7][0] = Piece(ROOK, WHITE);

	pieces[0][1] = Piece(PAWN, WHITE);
	pieces[1][1] = Piece(PAWN, WHITE);
	pieces[2][1] = Piece(PAWN, WHITE);
	pieces[3][1] = Piece(PAWN, WHITE);
	pieces[4][1] = Piece(PAWN, WHITE);
	pieces[5][1] = Piece(PAWN, WHITE);
	pieces[6][1] = Piece(PAWN, WHITE);
	pieces[7][1] = Piece(PAWN, WHITE);

	pieces[0][7] = Piece(ROOK, BLACK);
	pieces[1][7] = Piece(KNIGHT, BLACK);
	pieces[2][7] = Piece(BISHOP, BLACK);
	pieces[3][7] = Piece(QUEEN, BLACK);
	pieces[4][7] = Piece(KING, BLACK);
	pieces[5][7] = Piece(BISHOP, BLACK);
	pieces[6][7] = Piece(KNIGHT, BLACK);
	pieces[7][7] = Piece(ROOK, BLACK);

	pieces[0][6] = Piece(PAWN, BLACK);
	pieces[1][6] = Piece(PAWN, BLACK);
	pieces[2][6] = Piece(PAWN, BLACK);
	pieces[3][6] = Piece(PAWN, BLACK);
	pieces[4][6] = Piece(PAWN, BLACK);
	pieces[5][6] = Piece(PAWN, BLACK);
	pieces[6][6] = Piece(PAWN, BLACK);
	pieces[7][6] = Piece(PAWN, BLACK);
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesPawn(Position position, Position axis, Side side)
{
	std::vector<Chessboard::Move> result;
	bool guardian = axis.x || axis.y ? true : false;
	int direction = (side == WHITE ? 1 : -1);
	int current_x = position.x;
	int current_y = position.y + direction;

	if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
	{
		if (pieces[current_x][current_y].pieceType == EMPTY)
		{
			// Pawn promotion
			if (current_y == 7 || current_y == 1 && !guardian)
				result.push_back(Move(PAWN_PROMOTION, position, Position(current_x, current_y)));
			else if (!guardian || (!axis.x && axis.y))
				result.push_back(Move(position, Position(current_x, current_y)));
		}
	}

	current_x = position.x;
	current_y = position.y + 2 * direction;
	if (!pieces[position.x][position.y].firstMoveDone)
		if (pieces[current_x][current_y].pieceType == EMPTY && pieces[current_x][current_y - direction].pieceType == EMPTY)
			if (!guardian || (!axis.x && axis.y))
				result.push_back(Move(position, Position(current_x, current_y)));

	for (int horizontal = -1; horizontal <= 1; horizontal += 2)
	{
		current_x = position.x + horizontal;
		current_y = position.y + direction;
		if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
		{
			if (pieces[current_x][current_y].side == side * -1)
			{
				// Pawn promotion
				if (!guardian || axis.x * axis.y == horizontal * direction)
				{
					if (current_y == side == WHITE ? 7 : 0)
						result.push_back(Move(PAWN_PROMOTION, position, Position(current_x, current_y)));
					else
						result.push_back(Move(position, Position(current_x, current_y)));
				}
			}
			//en passant
			else if (pieces[current_x][current_y].pieceType == EMPTY && pieces[current_x][current_y - direction].pieceType == PAWN && lastMove.x == current_x && lastMove.y == current_y - direction)
				if (!guardian || axis.x * axis.y == horizontal * direction)
					result.push_back(Move(position, Position(current_x, current_y)));
		}
	}
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesKnight(Position position, Position axis, Side side)
{
	std::vector<Chessboard::Move> result;
	bool guardian = axis.x || axis.y ? true : false;
	for (int horizontal = -1; horizontal <= 1; horizontal += 2)
	{
		if (guardian)
			break;
		for (int vertical = -1; vertical <= 1; vertical += 2)
		{
			for (int x = -2; x <= -1; x++)
			{
				int current_x = position.x + horizontal * x;
				int current_y = position.y + vertical * (x == -2 ? 1 : 2);
				if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
					continue;
				if (pieces[current_x][current_y].pieceType == EMPTY || pieces[current_x][current_y].side == side * -1)
					result.push_back(Move(position, Position(current_x, current_y)));
			}
		}
	}
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesBishop(Position position, Position axis, Side side)
{
	std::vector<Chessboard::Move> result;
	bool guardian = axis.x || axis.y ? true : false;
	for (int horizontal = -1; horizontal <= 1; horizontal += 2)
	{
		if (guardian && !(axis.x * axis.y))
			break;
		for (int vertical = -1; vertical <= 1; vertical += 2)
		{
			if (guardian && axis.x * axis.y != horizontal * vertical)
				continue;
			for (int i = 1; i <= 7; i++)
			{
				int current_x = position.x + horizontal * i;
				int current_y = position.y + vertical * i;
				if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
					break;
				if (!pieces[current_x][current_y].pieceType)
				{
					result.push_back(Move(position, Position(current_x, current_y)));
					continue;
				}
				if (pieces[current_x][current_y].side == side * -1)
					result.push_back(Move(position, Position(current_x, current_y)));
				break;
			}
		}
	}
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesRook(Position position, Position axis, Side side)
{
	std::vector<Chessboard::Move> result;
	bool guardian = axis.x || axis.y ? true : false;
	std::vector<Position> dirs;
	dirs.push_back(Position(-1, 0));
	dirs.push_back(Position(1, 0));
	dirs.push_back(Position(0, 1));
	dirs.push_back(Position(0, -1));
	for (Position dir : dirs)
	{
		if (guardian)
		{
			if (axis.x * axis.y)
				break;
			if (!(axis.x * dir.x) && !(axis.y * dir.y))
				continue;
		}
		for (int i = 1; i <= 7; i++)
		{
			int current_x = position.x + dir.x * i;
			int current_y = position.y + dir.y * i;
			if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
				break;
			if (!pieces[current_x][current_y].pieceType)
			{
				result.push_back(Move(position, Position(current_x, current_y)));
				continue;
			}
			else if (pieces[current_x][current_y].side == side * -1)
				result.push_back(Move(position, Position(current_x, current_y)));
			break;
		}
	}
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesQueen(Position position, Position axis, Side side)
{
	std::vector<Chessboard::Move> result;
	result = getLegalMovesBishop(position, axis, side);
	std::vector<Chessboard::Move> result2;
	result2 = getLegalMovesRook(position, axis, side);
	result.insert(result.end(), result2.begin(), result2.end());
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesKing(Position position, Side side)
{
	std::vector<Chessboard::Move> result;
	for (int horizontal = -1; horizontal <= 1; horizontal++)
	{
		for (int vertical = -1; vertical <= 1; vertical++)
		{
			int current_x = position.x + horizontal;
			int current_y = position.y + vertical;
			if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
				continue;
			if (pieces[current_x][current_y].pieceType == EMPTY || pieces[current_x][current_y].side == side * -1)
				result.push_back(Move(position, Position(current_x, current_y)));
		}
	}
	return result;
}
std::vector<Chessboard::Move> Chessboard::getLegalMovesAt(Position position)
{
	std::vector<Chessboard::Move> result;
	Piece &current = pieces[position.x][position.y];
	Position axis = isGuardian(position);

	switch (current.pieceType)
	{
	case PAWN:
		result = getLegalMovesPawn(position, axis, current.side);
		break;
	case BISHOP:
		result = getLegalMovesBishop(position, axis, current.side);
		break;
	case KNIGHT:
		result = getLegalMovesKnight(position, axis, current.side);
		break;
	case ROOK:
		result = getLegalMovesRook(position, axis, current.side);
		break;
	case QUEEN:
		result = getLegalMovesQueen(position, axis, current.side);
		break;
	case KING:
		result = getLegalMovesKing(position, current.side);
		break;
	}
	return result;
}

bool Chessboard::isCastleSafe(Side side, bool isKingSideCastle)
{
	int from, to;
	if (isKingSideCastle)
	{
		from = 4;
		to = 7;
	}
	else
	{
		from = 0;
		to = 3;
	}

	int y = side == WHITE ? 0 : 7;
	for (int x = from; x <= to; x++)
		if (!isSquareSafe(Position(x, y), side))
			return false;
	return true;
}

// Check if castling is available.
std::vector<Chessboard::Move> Chessboard::getCastling()
{
	std::vector<Chessboard::Move> result;
	for (int side = 0; side < 8; side += 7)
	{
		Side sideEnum = side == 0 ? WHITE : BLACK;
		// Kingside
		if (!pieces[4][side].firstMoveDone && !pieces[7][side].firstMoveDone)
		{
			bool castleAvailable = true;
			for (int i = 1; i < 7 - 4; i++)
				if (pieces[4 + i][side].pieceType != EMPTY)
				{
					castleAvailable = false;
					break;
				}

			if (castleAvailable && isCastleSafe(sideEnum, true))
				result.push_back(Move(sideEnum, true));
		}

		// Queenside
		if (!pieces[4][side].firstMoveDone && !pieces[0][side].firstMoveDone)
		{
			bool castleAvailable = true;
			for (int i = 1; i < 4 - 0; i++)
				if (pieces[4 - i][side].pieceType != EMPTY)
				{
					castleAvailable = false;
					break;
				}
			if (castleAvailable && isCastleSafe(sideEnum, false))
				result.push_back(Move(sideEnum, false));
		}
	}
	return result;
}

std::vector<Chessboard::Move> Chessboard::getLegalMoves()
{
	std::vector<Move> result;
	std::vector<Position> checks = getChecks();

	Side side = movesDone % 2 == 0 ? WHITE : BLACK;
	Position kingPos = movesDone % 2 == 0 ? whiteKing : blackKing;
	std::vector<Move> kingLegalMoves = getLegalMovesAt(kingPos);
	std::vector<Move> piecesLegalMoves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (pieces[x][y].side != side || pieces[x][y].pieceType == KING)
				continue;
			std::vector<Move> tmp = getLegalMovesAt(Position(x, y));
			piecesLegalMoves.insert(piecesLegalMoves.end(), tmp.begin(), tmp.end());
		}
	}

	if (checks.size() > 0)
	{
		// Check if the king can escape or beat attacking piece
		for (Move &m : kingLegalMoves)
		{
			if (isSquareSafe(m.to, side))
			{
				bool isFalsePositive = false;
				for (Position attacker : checks)
				{
					Piece att = pieces[attacker.x][attacker.y];
					if (att.pieceType == BISHOP || att.pieceType == QUEEN || att.pieceType == ROOK)
					{
						Position dir = getDirectionFromPoints(attacker, kingPos);
						Position field = kingPos + dir;
						if (m.to == field)
						{
							isFalsePositive = true;
							break;
						}
					}
				}

				if (!isFalsePositive)
					result.push_back(m);
			}
		}

		if (checks.size() == 1)
		{
			for (Move &move : piecesLegalMoves)
			{
				// Piece can beat opponent's piece
				if (move.to == checks[0])
					result.push_back(move);
				// Be a guard for the king
				if (isPointBetweenPoints(kingPos, checks[0], move.to))
					result.push_back(move);
			}
		}
	}
	else
	{
		std::vector<Move> castling = getCastling();
		result.insert(result.end(), castling.begin(), castling.end());

		for (Move &m : kingLegalMoves)
			if (isSquareSafe(m.to, side))
				result.push_back(m);

		result.insert(result.end(), piecesLegalMoves.begin(), piecesLegalMoves.end());
	}
	return result;
}

bool Chessboard::makeMove(Position from, Position to)
{
	if ((!(movesDone % 2) && pieces[from.x][from.y].side != WHITE) || (movesDone % 2 && pieces[from.x][from.y].side != BLACK))
		return false;

	std::vector<Move> legalMoves = getLegalMoves();
	bool found = false;
	for (Move move : legalMoves)
		if (from == move.from && to == move.to && move.moveType != PAWN_PROMOTION)
		{
			found = true;
			break;
		}
	if (!found)
		return false;

	if (pieces[from.x][from.y].pieceType == PAWN)
	{
		if (abs(from.x - to.x) == 2)
			pieces[to.x][to.y].twoSquares = true;
		else
			pieces[to.x][to.y].twoSquares = false;
		//en passant
		if (pieces[to.x][to.y].pieceType == EMPTY && from.x != to.x)
		{
			pieces[to.x][from.y].side = NONE;
			pieces[to.x][from.y].pieceType = EMPTY;
		}
	}
	if (pieces[from.x][from.y].pieceType == KING)
	{
		if (pieces[from.x][from.y].side == WHITE)
			whiteKing = to;
		else
			blackKing = to;
	}
	pieces[from.x][from.y].firstMoveDone = true;
	pieces[to.x][to.y] = pieces[from.x][from.y];
	pieces[from.x][from.y].side = NONE;
	pieces[from.x][from.y].pieceType = EMPTY;
	lastMove = to;
	movesDone++;
	return true;
}

bool Chessboard::makeMove(Side side, bool isKingSideCastle)
{
	Side currentSide = movesDone % 2 == 0 ? WHITE : BLACK;
	if (currentSide != side)
		return false;

	std::vector<Chessboard::Move> legalMoves = getCastling();
	for (Move move : legalMoves)
		if (move.side == side && move.kingSideCastle == isKingSideCastle)
		{
			int y = side == WHITE ? 0 : 7;
			int new_king_x = isKingSideCastle ? 6 : 2;
			int rook_x = isKingSideCastle ? 7 : 0;
			int new_rook_x = isKingSideCastle ? 5 : 3;

			pieces[4][y].firstMoveDone = true;
			pieces[rook_x][y].firstMoveDone = true;

			pieces[new_king_x][y] = pieces[4][y];
			pieces[new_rook_x][y] = pieces[rook_x][y];

			if (side == WHITE)
				whiteKing = Position(new_king_x, y);
			else
				blackKing = Position(new_king_x, y);
			pieces[4][y].pieceType = EMPTY;
			pieces[4][y].side = NONE;
			pieces[rook_x][y].pieceType = EMPTY;
			pieces[rook_x][y].side = NONE;
			lastMove = Position(new_king_x, y);
			movesDone++;
			return true;
		}
	return false;
}

bool Chessboard::makeMove(PieceType promoted, Position from, Position to)
{
	if (!(pieces[from.x][from.y].pieceType == PAWN && promoted != EMPTY && promoted != PAWN && promoted != KING))

		return false;
	if ((!(movesDone % 2) && pieces[from.x][from.y].side != WHITE) || (movesDone % 2 && pieces[from.x][from.y].side != BLACK))
		return false;

	std::vector<Move> legalMoves = getLegalMovesAt(from);
	bool found = false;
	for (Move move : legalMoves)
		if (move.from == from && move.moveType == PAWN_PROMOTION && to == move.to)
		{
			found = true;
			break;
		}
	if (!found)
		return false;

	pieces[to.x][to.y] = Piece(promoted, pieces[from.x][from.y].side);
	pieces[from.x][from.y] = Piece();
	lastMove = to;
	movesDone++;
	return true;
}

std::ostream &operator<<(std::ostream &s, const Chessboard &c)
{
	for (int y = 7; y >= 0; y--)
	{
		for (int x = 0; x < 8; x++)
		{
			if (c.pieces[x][y].pieceType == EMPTY)
				s << ". ";
			else if (c.pieces[x][y].side == WHITE)
				s << ".PBNRQK"[c.pieces[x][y].pieceType] << " ";
			else
				s << ".pbnrqk"[c.pieces[x][y].pieceType] << " ";
		}
		s << y + 1 << std::endl;
	}
	for (int i = 0; i < 8; i++)
		s << (char)('a' + i) << " ";
	s << std::endl
	  << std::endl;
	return s;
}