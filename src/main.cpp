#include "chessboard.h"
#include "piece.h"
#include "chessboard_gui.h"

int main()
{
	ChessboardGUI chessboardGUI;
	// std::string game("1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6 5. O-O Be7 6. Re1 b5 7. Bb3 d6 8. c3 O-O 9. h3 Nb8 10. d4 Nbd7 ");
	// game.append("11. c4 c6 12. cxb5 axb5 13. Nc3 Bb7 14. Bg5 b4 15. Nb1 h6 16. Bh4 c5 17. dxe5 Nxe4 18. Bxe7 Qxe7 19. exd6 Qf6 ");
	// game.append("20. Nbd2 Nxd6 21. Nc4 Nxc4 22. Bxc4 Nb6 23. Ne5 Rae8 24. Bxf7+ Rxf7 25. Nxf7 Rxe1+ 26. Qxe1 Kxf7 27. Qe3 Qg5 ");
	// game.append("28. Qxg5 hxg5 29. b3 Ke6 30. a3 Kd6 31. axb4 cxb4 32. Ra5 Nd5 33. f3 Bc8 34. Kf2 Bf5 35. Ra7 g6 36. Ra6+ Kc5 ");
	// game.append("37. Ke1 Nf4 38. g3 Nxh3 39. Kd2 Kb5 40. Rd6 Kc5 41. Ra6 Nf2 42. g4 Bd3 43. Re6 1/2-1/2");
	std::string game("1. e4 f5 2. exf5 g6 3. fxg6 Nf6 4. g7 Ne4 5. gxh8=Q e6 6. Qf3 Nc5 7. h4 Qe7 8. h5 Qd8 9. h6 Qe7 10. Qxh7 Qd6 11. Qhe4 Nb3 12. h7 Qe7 13. h8=Q d5 14. Qeh7 Nd7 15. Qg3 Ndc5 16. Qgg8 Nd7 17. Qh8g7");

	std::vector<Chessboard::Move>
		moves = chessboardGUI.logicBoard.movesFromPGN(game);

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
				successfulMove = chessboardGUI.logicBoard.makeMove(moves[chessboardGUI.logicBoard.moveHistory.size()]);
				// if (fromString == "e1" && (toString == "h1" || toString == "g1"))
				// successfulMove = chessboardGUI.logicBoard.makeMove(WHITE, true);
				// else if (fromString == "e1" && (toString == "a1" || toString == "c1"))
				// successfulMove = chessboardGUI.logicBoard.makeMove(WHITE, false);
				// else if (fromString == "e8" && (toString == "h8" || toString == "g8"))
				// successfulMove = chessboardGUI.logicBoard.makeMove(BLACK, true);
				// else if (fromString == "e8" && (toString == "a8" || toString == "c8"))
				// successfulMove = chessboardGUI.logicBoard.makeMove(BLACK, false);
				// else
				// {
				// 	Piece currentPiece = chessboardGUI.logicBoard.getPieceAt(from);
				// 	if (chessboardGUI.logicBoard.getGameState() == Chessboard::IN_GAME && currentPiece.pieceType == PAWN && currentPiece.side == chessboardGUI.logicBoard.getCurrentSide() && (to.y == 0 || to.y == 7))
				// 	{
				// 		bool canBePromoted = false;
				// 		std::vector<Chessboard::Move> moves = chessboardGUI.logicBoard.getLegalMovesAt(from);
				// 		for (Chessboard::Move move : moves)
				// 			if ((move.moveType == Chessboard::PAWN_PROMOTION || move.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING) && move.from == from && move.to == to)
				// 			{
				// 				canBePromoted = true;
				// 				break;
				// 			}

				// 		if (canBePromoted)
				// 		{
				// 			PieceType pieceType = chessboardGUI.showPromotion();
				// 			successfulMove = chessboardGUI.logicBoard.makeMove(pieceType, from, to);
				// 		}
				// 	}
				// 	else
				// 		successfulMove = chessboardGUI.logicBoard.makeMove(from, to);
				// }

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