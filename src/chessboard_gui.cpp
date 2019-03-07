#include "chessboard_gui.h"

void ChessboardGUI::loadPiecesTextures()
{
    for (int i = 1; i < 13; i++)
    {
        sf::Texture tmp;
        std::string name = "img/" + std::to_string(i) + ".png";
        tmp.loadFromFile(name);

        piecesTextures.push_back(tmp);
    }
}

void ChessboardGUI::loadBoardTextures()
{
    sf::Texture whiteSquareTexture;
    whiteSquareTexture.loadFromFile("img/whiteSquare.png");
    sf::Texture blackSquareTexture;
    blackSquareTexture.loadFromFile("img/blackSquare.png");

    boardTextures.push_back(whiteSquareTexture);
    boardTextures.push_back(blackSquareTexture);
}

void ChessboardGUI::setCheckerboard()
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            sf::Sprite tmp;
            if ((x + y) % 2 == 0)
                tmp.setTexture(boardTextures[0]);
            else
                tmp.setTexture(boardTextures[1]);
            tmp.setPosition(x * (WINDOW_SIZE / 8), y * (WINDOW_SIZE / 8));
            squares.push_back(tmp);
        }
    }
}

void ChessboardGUI::setPieces()
{
    for (int i = 0; i < 32; i++)
    {
        sf::Sprite tmp;
        pieces.push_back(tmp);
    }
}

void ChessboardGUI::updatePieces()
{
    int count = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (logicBoard.getPieceAt(Position(x, y)).pieceType == EMPTY)
                continue;

            Piece piece = logicBoard.getPieceAt(Position(x, y));
            pieces[count].setPosition(x * (WINDOW_SIZE / 8), WINDOW_SIZE - y * (WINDOW_SIZE / 8) - 96);
            int type = piece.pieceType + (piece.side == BLACK ? 6 : 0) - 1;
            pieces[count].setTexture(piecesTextures[type]);
            count++;
        }
    }

    piecesOnBoard = count;
}

void ChessboardGUI::highlight(Position from)
{
    sf::Color color(127, 127, 127, 255);
    for (Chessboard::Move m : logicBoard.legalMoves)
    {
        if (m.from == from)
        {
            int n = m.to.x + m.to.y * 8;
            squares[n].setColor(color);
        }
        else if (m.moveType == Chessboard::CASTLING && m.side == logicBoard.getCurrentSide() && logicBoard.getPieceAt(from).pieceType == KING)
        {
            if (m.side == WHITE)
            {
                if (m.kingSideCastle)
                {
                    squares[4].setColor(color);
                    squares[5].setColor(color);
                    squares[6].setColor(color);
                    squares[7].setColor(color);
                }
                else
                {
                    squares[0].setColor(color);
                    squares[1].setColor(color);
                    squares[2].setColor(color);
                    squares[3].setColor(color);
                    squares[4].setColor(color);
                }
            }
            else
            {
                if (m.kingSideCastle)
                {
                    squares[63 - 0].setColor(color);
                    squares[63 - 1].setColor(color);
                    squares[63 - 2].setColor(color);
                    squares[63 - 3].setColor(color);
                }
                else
                {
                    squares[63 - 3].setColor(color);
                    squares[63 - 4].setColor(color);
                    squares[63 - 5].setColor(color);
                    squares[63 - 6].setColor(color);
                    squares[63 - 7].setColor(color);
                }
            }
        }
    }
}