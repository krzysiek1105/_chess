#include "chessboard.h"

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
			if ((current_y == 7 && side == WHITE) || (current_y == 0 && side == BLACK) && !guardian)
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
					if ((current_y == 7 && side == WHITE) || (current_y == 0 && side == BLACK))
						result.push_back(Move(PAWN_PROMOTION_WITH_BEATING, position, Position(current_x, current_y)));
					else
						result.push_back(Move(BEATING, position, Position(current_x, current_y)));
				}
			}
			//en passant
			else if (pieces[current_x][current_y].pieceType == EMPTY && pieces[current_x][current_y - direction].pieceType == PAWN && lastMove.x == current_x && lastMove.y == current_y - direction && pieces[lastMove.x][lastMove.y].twoSquares)
				if (!guardian || axis.x * axis.y == horizontal * direction)
					result.push_back(Move(EN_PASSANT, position, Position(current_x, current_y)));
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
				if (pieces[current_x][current_y].pieceType == EMPTY)
					result.push_back(Move(position, Position(current_x, current_y)));
				else if (pieces[current_x][current_y].side == side * -1)
					result.push_back(Move(BEATING, position, Position(current_x, current_y)));
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
					result.push_back(Move(BEATING, position, Position(current_x, current_y)));
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
				result.push_back(Move(BEATING, position, Position(current_x, current_y)));
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
			if (pieces[current_x][current_y].pieceType == EMPTY)
				result.push_back(Move(position, Position(current_x, current_y)));
			else if (pieces[current_x][current_y].side == side * -1)
				result.push_back(Move(BEATING, position, Position(current_x, current_y)));
		}
	}
	return result;
}