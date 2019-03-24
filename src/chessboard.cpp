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

	for (int x = 0; x < 8; x++)
		for (int y = 2; y < 6; y++)
			pieces[x][y] = Piece();

	getLegalMoves();
}

std::vector<Chessboard::Move> Chessboard::getLegalMovesAt(Position position)
{
	std::vector<Chessboard::Move> result;
	Piece& current = pieces[position.x][position.y];
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

void Chessboard::getLegalMoves()
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
		gameState = CHECK;
		// Check if the king can escape or beat attacking piece
		for (Move& m : kingLegalMoves)
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
			for (Move& move : piecesLegalMoves)
			{
				// Piece can beat opponent's piece
				if (move.to == checks[0])
					result.push_back(move);
				// Be a guard for the king
				if (isPointBetweenPoints(kingPos, checks[0], move.to))
					result.push_back(move);
			}
		}

		if (result.size() == 0)
			gameState = MATE;
	}
	else
	{
		gameState = IN_GAME;
		std::vector<Move> castling = getCastling();
		result.insert(result.end(), castling.begin(), castling.end());

		for (Move& m : kingLegalMoves)
			if (isSquareSafe(m.to, side))
				result.push_back(m);

		result.insert(result.end(), piecesLegalMoves.begin(), piecesLegalMoves.end());

		if (result.size() == 0)
			gameState = STALEMATE;
		else
		{
			bool knightFound = false;
			bool stalemate = true;
			Side bishopSquareColor = NONE;
			for (int x = 0; x < 8 && stalemate; x++)
			{
				for (int y = 0; y < 8; y++)
				{
					PieceType piece = pieces[x][y].pieceType;
					if (piece == KING || piece == EMPTY)
						continue;
					else if (knightFound)
					{
						stalemate = 0;
						break;
					}
					else
					{
						if (piece == KNIGHT)
						{
							if (bishopSquareColor) {
								stalemate = false;
								break;
							}
							knightFound = 1;
						}
						else if (piece == BISHOP)
						{
							Side curColor = (x + y) % 2 ? WHITE : BLACK;
							if (bishopSquareColor && curColor != bishopSquareColor)
							{
								stalemate = false;
								break;
							}
							bishopSquareColor = curColor;
						}
						else {
							stalemate = false;
							break;
						}
					}
				}
			}
			if (stalemate)
				gameState = STALEMATE;
		}
	}

	for (Move &m1 : result)
		for (Move &m2 : result)
		{
			if (!(m1.from == m2.from) && m1.pieceOnMove == m2.pieceOnMove && m1.to == m2.to)
			{
				if(m1.from.x == m2.from.x)
				{
					m1.ambiguousY = true;
					m2.ambiguousY = true;
				}
				else
				{
					m1.ambiguousX = true;
					m2.ambiguousX = true;
				}
				
			}
		}

	legalMoves = result;
}

bool Chessboard::makeMove(Position from, Position to)
{
	if ((!(movesDone % 2) && pieces[from.x][from.y].side != WHITE) || (movesDone % 2 && pieces[from.x][from.y].side != BLACK))
		return false;

	bool found = false;
	for (Move move : legalMoves)
		if (from == move.from && to == move.to && move.moveType != PAWN_PROMOTION)
		{
			found = true;
			moveHistory.push_back(move);
			break;
		}
	if (!found)
		return false;

	bool twoSquares = false;
	if (pieces[from.x][from.y].pieceType == PAWN)
	{
		if (abs(from.y - to.y) == 2)
			twoSquares = true;
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
	pieces[to.x][to.y].twoSquares = twoSquares;
	pieces[from.x][from.y].side = NONE;
	pieces[from.x][from.y].pieceType = EMPTY;
	lastMove = to;
	movesDone++;

	getLegalMoves();
	setSanString(EMPTY);
	return true;
}

bool Chessboard::makeMove(Side side, bool isKingSideCastle)
{
	Side currentSide = movesDone % 2 == 0 ? WHITE : BLACK;
	if (currentSide != side)
		return false;

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

			moveHistory.push_back(move);

			getLegalMoves();
			setSanString(EMPTY);
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

	bool found = false;
	for (Move move : legalMoves)
		if (move.from == from && (move.moveType == PAWN_PROMOTION || move.moveType == PAWN_PROMOTION_WITH_BEATING) && to == move.to)
		{
			found = true;
			Move m = move;
			m.pieceType = promoted;
			moveHistory.push_back(m);
			break;
		}
	if (!found)
		return false;

	pieces[to.x][to.y] = Piece(promoted, pieces[from.x][from.y].side);
	pieces[from.x][from.y] = Piece();
	lastMove = to;
	movesDone++;

	getLegalMoves();
	setSanString(promoted);
	return true;
}

std::ostream& operator<<(std::ostream & s, const Chessboard & c)
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

Chessboard::GameState Chessboard::getGameState()
{
	return gameState;
}

Side Chessboard::getCurrentSide()
{
	return movesDone % 2 == 0 ? WHITE : BLACK;
}

Piece Chessboard::getPieceAt(Position position)
{
	return pieces[position.x][position.y];
}