#include "chessboard.h"
#include "piece.h"
#include "chessboard_gui.h"

int main()
{
	ChessboardGUI chessboardGUI;

	Position from;
	Position to;
	while (chessboardGUI.window->isOpen())
	{
		sf::Event event;
		while (chessboardGUI.window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				chessboardGUI.window->close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				int fieldX = (event.mouseButton.x / (float)WINDOW_SIZE) * 8;
				int fieldY = 8 - (event.mouseButton.y / (float)WINDOW_SIZE) * 8;

				from.x = fieldX;
				from.y = fieldY;

				chessboardGUI.highlight(from);
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				int fieldX = (event.mouseButton.x / (float)WINDOW_SIZE) * 8;
				int fieldY = 8 - (event.mouseButton.y / (float)WINDOW_SIZE) * 8;

				to.x = fieldX;
				to.y = fieldY;

				std::string fromString;
				fromString += "abcdefgh"[from.x];
				fromString += "12345678"[from.y];

				std::string toString;
				toString += "abcdefgh"[to.x];
				toString += "12345678"[to.y];

				std::cout << fromString << " " << toString << std::endl;

				if (fromString == "e1" && toString == "h1")
					chessboardGUI.logicBoard.makeMove(WHITE, true);
				else if (fromString == "e1" && toString == "a1")
					chessboardGUI.logicBoard.makeMove(WHITE, false);
				else if (fromString == "e8" && toString == "h8")
					chessboardGUI.logicBoard.makeMove(BLACK, true);
				else if (fromString == "e8" && toString == "a8")
					chessboardGUI.logicBoard.makeMove(BLACK, false);
				else
				{
					if (chessboardGUI.logicBoard.getPieceAt(from).pieceType == PAWN && (to.y == 0 || to.y == 7))
						chessboardGUI.logicBoard.makeMove(QUEEN, from, to);
					else
						chessboardGUI.logicBoard.makeMove(from, to);
				}

				std::cout << chessboardGUI.logicBoard;
				chessboardGUI.updatePieces();
				chessboardGUI.resetHighlighting();

				std::string windowTitle = chessboardGUI.logicBoard.getCurrentSide() == WHITE ? "White move " : "Black move ";
				switch (chessboardGUI.logicBoard.getGameState())
				{
				case Chessboard::CHECK:
					windowTitle.append("(CHECK)");
					break;
				case Chessboard::MATE:
					windowTitle.append("(MATE)");
					break;
				case Chessboard::STALEMATE:
					windowTitle.append("(STALEMATE)");
					break;
				}
				chessboardGUI.window->setTitle(windowTitle);
			}
		}

		chessboardGUI.window->clear();
		for (int i = 0; i < 64; i++)
			chessboardGUI.window->draw(chessboardGUI.squares[i]);
		for (int i = 0; i < chessboardGUI.piecesOnBoard; i++)
			chessboardGUI.window->draw(chessboardGUI.pieces[i]);
		chessboardGUI.window->display();
	}

	return 0;
}