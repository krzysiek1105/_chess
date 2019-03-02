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

	assert(c.piecesBetweenPoints(Position(1, 2), Position(2, 4)) == -1);
	assert(c.piecesBetweenPoints(Position(0, 2), Position(0, 5)) == 1);
	assert(c.piecesBetweenPoints(Position(0, 2), Position(4, 6)) == 0);
	assert(c.piecesBetweenPoints(Position(0, 1), Position(7, 1)) == 6);
	assert(c.piecesBetweenPoints(Position(0, 7), Position(7, 0)) == 2);

	c.makeMove(Position(4, 6), Position(4, 5));
	c.makeMove(Position(1, 1), Position(1, 2));
	c.makeMove(Position(3, 7), Position(7, 3));
	std::cout << c;

	assert(c.isGuardian(Position(5, 1)) == Position(1, 1));
	assert(c.isGuardian(Position(6, 2)) == Position(0, 0));

	c.makeMove(Position(6, 1), Position(6, 2));
	std::cout << c;

	assert(c.isGuardian(Position(5, 1)) == Position(0, 0));
	assert(c.isGuardian(Position(6, 2)) == Position(0, 0));

	return 0;
}