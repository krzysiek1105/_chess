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
    sf::Texture promotionTexture;
    promotionTexture.loadFromFile("img/promotionSquares.png");
    boardTextures.push_back(whiteSquareTexture);
    boardTextures.push_back(blackSquareTexture);
    boardTextures.push_back(promotionTexture);
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
            pieces[count].setPosition(x * (WINDOW_SIZE / 8) + ((SQUARE_SIZE - PIECE_SIZE) / 2), WINDOW_SIZE - y * (WINDOW_SIZE / 8) - PIECE_SIZE - ((SQUARE_SIZE - PIECE_SIZE) / 2));
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
        Piece pieceToMove = logicBoard.getPieceAt(from);
        Side currentSide = logicBoard.getCurrentSide();

        if (m.from == from)
        {
            int n = m.to.x + m.to.y * 8;
            squares[n].setColor(color);
        }
        else if (m.moveType == Chessboard::CASTLING && m.side == currentSide && pieceToMove.pieceType == KING && pieceToMove.side == currentSide)
        {
            if (m.side == WHITE)
            {
                if (m.kingSideCastle)
                    for (int i = 4; i <= 7; i++)
                        squares[i].setColor(color);
                else
                    for (int i = 0; i <= 4; i++)
                        squares[i].setColor(color);
            }
            else
            {
                if (m.kingSideCastle)
                    for (int i = 63 - 0; i >= 63 - 3; i--)
                        squares[i].setColor(color);
                else
                    for (int i = 63 - 3; i >= 63 - 7; i--)
                        squares[i].setColor(color);
            }
        }
    }
}

void ChessboardGUI::resetHighlighting()
{
    for (int i = 0; i < 64; i++)
        squares[i].setColor(sf::Color(255, 255, 255, 255));
}

PieceType ChessboardGUI::showPromotion(Side side)
{
    sf::Sprite piece, background;
    background.setTexture(boardTextures[2]);
    background.setPosition(2 * (WINDOW_SIZE / 8), WINDOW_SIZE * 3 / 8);
    window->draw(background);
    for (int i = 0; i < 4; i++)
    {
        piece.setPosition((i + 2) * (WINDOW_SIZE / 8), WINDOW_SIZE / 2 - PIECE_SIZE);
        piece.setTexture(piecesTextures[i + 1 + (side == WHITE ? 0 : 6)]);
        window->draw(piece);
    }
    window->display();

    sf::Event event;
    Position click, release;
    while (1)
    {
        window->pollEvent(event);
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::MouseButtonPressed)
        {
            int fieldX = (event.mouseButton.x / (float)WINDOW_SIZE) * 8;
            int fieldY = 8 - (event.mouseButton.y / (float)WINDOW_SIZE) * 8;
            click = Position(fieldX, fieldY);
            if (click.y != 4 || click.x < 2 || click.x > 5)
                continue;
            switch (click.x)
            {
            case 2:
                return BISHOP;
            case 3:
                return KNIGHT;
            case 4:
                return ROOK;
            case 5:
                return QUEEN;
            }
        }
    }
}
