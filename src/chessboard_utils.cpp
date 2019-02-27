#include "chessboard.h"

bool Chessboard::arePointsCollinear(Position a, Position b)
{
    if (a.x == b.x || a.y == b.y)
        return true;
    if ((a.x - a.y == b.x - b.y) || (a.x + a.y == b.x + b.y))
        return true;
    return false;
}

int Chessboard::piecesBetweenPoints(Position a, Position b)
{
    if (!arePointsCollinear(a, b))
        return -1;

    Position dir = Position(b.x > a.x ? 1 : (b.x < a.x ? -1 : 0), b.y > a.y ? 1 : (b.y < a.y ? -1 : 0));
    int len, count = 0;
    if (a.x == b.x)
        len = abs(a.y - b.y);
    else
        len = abs(a.x - b.x);

    for (int i = 1; i < len; i++)
        if (pieces[a.x + dir.x * i][a.y + dir.y * i].pieceType != EMPTY)
            count++;

    return count;
}