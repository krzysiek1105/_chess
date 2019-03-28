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
            tmp.setPosition(x * (CHESSBOARD_SIZE / 8), y * (CHESSBOARD_SIZE / 8));
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
            pieces[count].setPosition(x * (CHESSBOARD_SIZE / 8) + ((SQUARE_SIZE - PIECE_SIZE) / 2), CHESSBOARD_SIZE - y * (CHESSBOARD_SIZE / 8) - PIECE_SIZE - ((SQUARE_SIZE - PIECE_SIZE) / 2));
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

PieceType ChessboardGUI::showPromotion()
{
    sf::Sprite piece, background;
    background.setTexture(boardTextures[2]);
    background.setPosition(2 * (CHESSBOARD_SIZE / 8), CHESSBOARD_SIZE * 3 / 8);
    window->draw(background);
    for (int i = 0; i < 4; i++)
    {
        piece.setPosition((i + 2) * (CHESSBOARD_SIZE / 8), CHESSBOARD_SIZE / 2 - PIECE_SIZE);
        piece.setTexture(piecesTextures[i + 1 + (logicBoard.getCurrentSide() == WHITE ? 0 : 6)]);
        window->draw(piece);
    }
    window->display();

    sf::Event event;
    while (window->isOpen())
    {
        window->pollEvent(event);
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::MouseButtonPressed)
        {
            int fieldX = (event.mouseButton.x / (float)CHESSBOARD_SIZE) * 8;
            int fieldY = 8 - (event.mouseButton.y / (float)CHESSBOARD_SIZE) * 8;
			if (fieldY != 4)
				return EMPTY;
            switch (fieldX)
            {
            case 2:
                return BISHOP;
            case 3:
                return KNIGHT;
            case 4:
                return ROOK;
            case 5:
                return QUEEN;
			default:
				return EMPTY;
            }
        }
    }
}

void ChessboardGUI::resetAll()
{
    logicBoard.reset();
    updatePieces();
    resetHighlighting();
    sanMoves.clear();
	movesPanel->removeAllItems();
}

void ChessboardGUI::loadFromPGNTextBox()
{
    resetAll();
    std::string data = PGNTextBox->getText();
    printf("%d\n", logicBoard.movesFromPGN(data).size());
    std::cout << logicBoard.getSanString() << std::endl;
    updatePieces();
}

void ChessboardGUI::updateAfterMove()
{
    if (logicBoard.moveHistory.size() % 2 != 0)
        san.push_back(std::to_string(logicBoard.moveHistory.size() / 2 + 1) + ". ");
    else
        movesPanel->removeItem(movesPanel->getItemCount() - 1);

    san.push_back(logicBoard.getSanString());
    movesPanel->addItem(san);
    if (logicBoard.moveHistory.size() % 2 == 0)
        san.clear();
    updatePieces();

    std::string windowTitle = logicBoard.getCurrentSide() == WHITE ? "White move " : "Black move ";
    switch (logicBoard.getGameState())
    {
    case Chessboard::CHECK:
        windowTitle.append("(CHECK)");
        break;
    case Chessboard::MATE:
        san.clear();
        windowTitle.append("(MATE)");
        break;
    case Chessboard::STALEMATE:
        san.clear();
        windowTitle.append("(STALEMATE)");
        break;
    }
    window->setTitle(windowTitle);
}

bool ChessboardGUI::tryMove(Position from, Position to)
{
    std::string fromString;
    fromString += "abcdefgh"[from.x];
    fromString += "12345678"[from.y];

    std::string toString;
    toString += "abcdefgh"[to.x];
    toString += "12345678"[to.y];

    bool successfulMove = false;
    if (fromString == "e1" && (toString == "h1" || toString == "g1"))
        successfulMove = logicBoard.makeMove(WHITE, true);
    else if (fromString == "e1" && (toString == "a1" || toString == "c1"))
        successfulMove = logicBoard.makeMove(WHITE, false);
    else if (fromString == "e8" && (toString == "h8" || toString == "g8"))
        successfulMove = logicBoard.makeMove(BLACK, true);
    else if (fromString == "e8" && (toString == "a8" || toString == "c8"))
        successfulMove = logicBoard.makeMove(BLACK, false);
    else
    {
        Piece currentPiece = logicBoard.getPieceAt(from);
        if (logicBoard.getGameState() == Chessboard::IN_GAME && currentPiece.pieceType == PAWN && currentPiece.side == logicBoard.getCurrentSide() && (to.y == 0 || to.y == 7))
        {
            bool canBePromoted = false;
            std::vector<Chessboard::Move> moves = logicBoard.getLegalMovesAt(from);
            for (Chessboard::Move move : moves)
                if ((move.moveType == Chessboard::PAWN_PROMOTION || move.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING) && move.from == from && move.to == to)
                {
                    canBePromoted = true;
                    break;
                }

            if (canBePromoted)
            {
                PieceType pieceType = showPromotion();
                successfulMove = logicBoard.makeMove(pieceType, from, to);
            }
        }
        else
            successfulMove = logicBoard.makeMove(from, to);
    }

    return successfulMove;
}

void ChessboardGUI::draw()
{
    window->clear(sf::Color(206, 186, 140, 255));
    for (int i = 0; i < 64; i++)
        window->draw(squares[i]);
    for (int i = 0; i < piecesOnBoard; i++)
        window->draw(pieces[i]);
    gui.draw();
}