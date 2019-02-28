#include "chessboard.h"

bool Chessboard::arePointsCollinear(Position a, Position b)
{
    if (a.x == b.x || a.y == b.y)
        return true;
    if ((a.x - a.y == b.x - b.y) || (a.x + a.y == b.x + b.y))
        return true;
    return false;
}

Position Chessboard::getDirectionFromPoints(Position a, Position b)
{
    return Position(b.x > a.x ? 1 : (b.x < a.x ? -1 : 0), b.y > a.y ? 1 : (b.y < a.y ? -1 : 0));
}

int Chessboard::piecesBetweenPoints(Position a, Position b)
{
    if (!arePointsCollinear(a, b))
        return -1;

    Position dir = getDirectionFromPoints(a, b);
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

bool Chessboard::isPointBetweenPoints(Position a, Position b, Position toCheck)
{
    if (!arePointsCollinear(a, toCheck) || !arePointsCollinear(a, b))
        return false;
    Position dir1 = getDirectionFromPoints(a, b);
    Position dir2 = getDirectionFromPoints(a, toCheck);
    if (dir1 == dir2)
    {
        int diff1 = abs(a.x - b.x) + abs(a.y - b.y);
        int diff2 = abs(a.x - toCheck.x) + abs(a.y - toCheck.y);
        if (diff2 <= diff1)
            return true;
    }
    return false;
}