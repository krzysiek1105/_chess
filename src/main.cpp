#include <stdio.h>
#include <string>
#include "chessboard.h"
#include "piece.h"
#include <stdlib.h>
#include <windows.h>

int main()
{
    Chessboard board;
    while (true)
    {
        std::cout << board;

        std::string cmd;
        std::cin >> cmd;

        Position from = Position(cmd[0] - 'a', cmd[1] - '1');
        Position to = Position(cmd[2] - 'a', cmd[3] - '1');
        board.makeMove(from, to);
    }

    return 0;
}