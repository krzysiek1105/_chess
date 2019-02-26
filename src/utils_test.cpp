#include "chessboard.h"
#include <cassert>

int main()
{
    Chessboard c;
    assert(c.arePointsCollinear(Position(0, 0), Position(5, 5)) == true);
    assert(c.arePointsCollinear(Position(0, 1), Position(5, 5)) == false);
    assert(c.arePointsCollinear(Position(1, 1), Position(2, 1)) == true);
    assert(c.arePointsCollinear(Position(1, 1), Position(2, 1)) == true);
    assert(c.arePointsCollinear(Position(3, 2), Position(2, 1)) == true);
    assert(c.arePointsCollinear(Position(2, 1), Position(2, 1)) == true);
    assert(c.arePointsCollinear(Position(2, 5), Position(2, 0)) == true);
    assert(c.arePointsCollinear(Position(7, 2), Position(2, 1)) == false);

    c.makeMove(Position(0, 1), Position(0, 3));
    std::cout << c;

    assert(c.isPieceBetweenPoints(Position(0, 2), Position(0, 5)) == false);
    assert(c.isPieceBetweenPoints(Position(0, 2), Position(2, 4)) == true);

    return 0;
}