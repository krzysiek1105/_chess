#include "chessboard.h"

bool Chessboard::arePointsCollinear(Position a, Position b)
{
    if (a.x == b.x || a.y == b.y)
        return true;
    if (abs(a.x - a.y) == abs(b.x - b.y))
        return true;
    return false;
}

bool Chessboard::isPieceBetweenPoints(Position a, Position b)
{
    if (!arePointsCollinear(a, b))
        return false;

    Position dir = Position(b.x - a.x, b.y - a.y);
    int len;
    if (a.x == b.x)
        len = abs(a.y - b.y);
    else
        len = abs(a.x - b.x);

    for (int i = 1; i < len; i++)
        if (pieces[a.x + dir.x * i][a.y + dir.y * i].pieceType != EMPTY)
            return false;

    return true;
}