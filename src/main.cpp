#include "chessboard.h"
#include "piece.h"
#include "chessboard_gui.h"

int main()
{
	ChessboardGUI chessboardGUI;

	chessboardGUI.logicBoard.makeMove("e4");
	chessboardGUI.logicBoard.makeMove("e5");
	chessboardGUI.logicBoard.makeMove("Nf3");
	chessboardGUI.logicBoard.makeMove("Nc6");
	chessboardGUI.logicBoard.makeMove("Bb5");
	chessboardGUI.logicBoard.makeMove("a6");
	chessboardGUI.logicBoard.makeMove("Ba4");
	chessboardGUI.logicBoard.makeMove("Nf6");
	chessboardGUI.logicBoard.makeMove("O-O");
	chessboardGUI.logicBoard.makeMove("Be7");

	chessboardGUI.updatePieces();

	Position from;
	Position to;

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return 1;
	int startHistory = 0;

	while (chessboardGUI.window->isOpen())
	{
		sf::Event event;
		while (chessboardGUI.window->pollEvent(event))
		{
			chessboardGUI.gui.handleEvent(event);

			if (event.type == sf::Event::Closed)
				chessboardGUI.window->close();

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (chessboardGUI.sanMoves.size() >= MOVE_HISTORY_LINE_COUNT)
				{
					int delta = event.mouseWheelScroll.delta;
					startHistory += event.mouseWheelScroll.delta;
					if (startHistory < 0)
						startHistory = 0;
					if (startHistory > chessboardGUI.sanMoves.size() - 1)
						startHistory = chessboardGUI.sanMoves.size() - 1;
				}
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				from.x = (event.mouseButton.x / (float)CHESSBOARD_SIZE) * 8;
				from.y = 8 - (event.mouseButton.y / (float)CHESSBOARD_SIZE) * 8;
				if (from.x >= 8)
					continue;
				chessboardGUI.highlight(from);
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (from.x >= 8)
					continue;
				to.x = (event.mouseButton.x / (float)CHESSBOARD_SIZE) * 8;
				to.y = 8 - (event.mouseButton.y / (float)CHESSBOARD_SIZE) * 8;
				if (to.x >= 8)
					continue;
				std::string fromString;
				fromString += "abcdefgh"[from.x];
				fromString += "12345678"[from.y];

				std::string toString;
				toString += "abcdefgh"[to.x];
				toString += "12345678"[to.y];

				bool successfulMove = false;
				if (fromString == "e1" && (toString == "h1" || toString == "g1"))
					successfulMove = chessboardGUI.logicBoard.makeMove(WHITE, true);
				else if (fromString == "e1" && (toString == "a1" || toString == "c1"))
					successfulMove = chessboardGUI.logicBoard.makeMove(WHITE, false);
				else if (fromString == "e8" && (toString == "h8" || toString == "g8"))
					successfulMove = chessboardGUI.logicBoard.makeMove(BLACK, true);
				else if (fromString == "e8" && (toString == "a8" || toString == "c8"))
					successfulMove = chessboardGUI.logicBoard.makeMove(BLACK, false);
				else
				{
					Piece currentPiece = chessboardGUI.logicBoard.getPieceAt(from);
					if (chessboardGUI.logicBoard.getGameState() == Chessboard::IN_GAME && currentPiece.pieceType == PAWN && currentPiece.side == chessboardGUI.logicBoard.getCurrentSide() && (to.y == 0 || to.y == 7))
					{
						bool canBePromoted = false;
						std::vector<Chessboard::Move> moves = chessboardGUI.logicBoard.getLegalMovesAt(from);
						for (Chessboard::Move move : moves)
							if ((move.moveType == Chessboard::PAWN_PROMOTION || move.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING) && move.from == from && move.to == to)
							{
								canBePromoted = true;
								break;
							}

						if (canBePromoted)
						{
							PieceType pieceType = chessboardGUI.showPromotion();
							successfulMove = chessboardGUI.logicBoard.makeMove(pieceType, from, to);
						}
					}
					else
						successfulMove = chessboardGUI.logicBoard.makeMove(from, to);
				}

				if (successfulMove)
				{
					if (chessboardGUI.logicBoard.moveHistory.size() % 2 != 0)
					{
						chessboardGUI.sanMoves.push_back(chessboardGUI.logicBoard.getSanString());
						if (chessboardGUI.sanMoves.size() > MOVE_HISTORY_LINE_COUNT)
							startHistory = chessboardGUI.sanMoves.size() - MOVE_HISTORY_LINE_COUNT;
					}
					else
						chessboardGUI.sanMoves[chessboardGUI.sanMoves.size() - 1] += chessboardGUI.logicBoard.getSanString();

					std::cout << fromString << " " << toString << std::endl;
					std::cout << chessboardGUI.logicBoard;
					chessboardGUI.updatePieces();

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
				chessboardGUI.resetHighlighting();
			}
		}

		chessboardGUI.window->clear(sf::Color(206, 186, 140, 255));
		for (int i = 0; i < 64; i++)
			chessboardGUI.window->draw(chessboardGUI.squares[i]);
		for (int i = 0; i < chessboardGUI.piecesOnBoard; i++)
			chessboardGUI.window->draw(chessboardGUI.pieces[i]);

		std::vector<sf::Text> sanMovesText;
		for (int i = 0; i < MOVE_HISTORY_LINE_COUNT; i++)
		{
			int j = i + startHistory;
			if (j >= chessboardGUI.sanMoves.size())
				break;
			sf::Text tmp;
			tmp.setFont(font);
			tmp.setFillColor(sf::Color(118, 77, 46, 255));
			tmp.setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, SIDE_PANEL_PADDING * (i + 1));
			tmp.setString(chessboardGUI.sanMoves[j]);

			sanMovesText.push_back(tmp);
		}
		for (sf::Text &text : sanMovesText)
			chessboardGUI.window->draw(text);

		chessboardGUI.gui.draw();
		chessboardGUI.window->display();
	}

	return 0;
}