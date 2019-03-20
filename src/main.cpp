#include "chessboard.h"
#include "piece.h"
#include "chessboard_gui.h"
#include <windows.h>

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
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
				from.x = (event.mouseButton.x / (float)WINDOW_SIZE) * 8;
				from.y = 8 - (event.mouseButton.y / (float)WINDOW_SIZE) * 8;

				chessboardGUI.highlight(from);
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				to.x = (event.mouseButton.x / (float)WINDOW_SIZE) * 8;
				to.y = 8 - (event.mouseButton.y / (float)WINDOW_SIZE) * 8;

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
					if (chessboardGUI.logicBoard.getPieceAt(from).pieceType == PAWN && (to.y == 0 || to.y == 7)) {
						PieceType pieceType = chessboardGUI.showPromotion(chessboardGUI.logicBoard.getPieceAt(from).side);
						chessboardGUI.logicBoard.makeMove(pieceType, from, to);
					}
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