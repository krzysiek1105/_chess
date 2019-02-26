#include <stdio.h>
#include <string>
#include "chessboard.h"
#include "piece.h"
#include <stdlib.h>

int main()
{
    Chessboard board;
    while (true)
    {
        std::cout << board;

        std::string cmd;
        std::cin >> cmd;

        if (cmd[0] == 'O' || cmd[0] == 'o')	// for O-O or O-O-O, c was already in use 
        {
            Side side = board.movesDone % 2 == 0 ? WHITE : BLACK;
			bool castle = 1;
			bool isKingSideCastle;
			if (cmd == "O-O" || cmd == "o-o")
				isKingSideCastle = 1;
			else if (cmd == "O-O-O" || cmd == "o-o-o")
				isKingSideCastle = 0;
			else
				castle = 0;
			if(castle)
				board.makeMove(side, isKingSideCastle);
        }
        else
        {
            Position from = Position(cmd[0] - 'a', cmd[1] - '1');
            Position to = Position(cmd[2] - 'a', cmd[3] - '1');
            board.makeMove(from, to);
        }
    }

    return 0;
}