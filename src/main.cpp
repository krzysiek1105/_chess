#include "chessboard.h"
#include "piece.h"
#include "chessboard_gui.h"

int main()
{
	ChessboardGUI chessboardGUI;

	Position from;
	Position to;

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return 1;
	sf::Text moveList;
	moveList.setFont(font);
	moveList.setFillColor(sf::Color(118, 77, 46, 255));
	moveList.setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, SIDE_PANEL_PADDING);
	std::string moves;

	while (chessboardGUI.window->isOpen())
	{
		sf::Event event;
		while (chessboardGUI.window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				chessboardGUI.window->close();

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
					if (currentPiece.pieceType == PAWN && currentPiece.side == chessboardGUI.logicBoard.getCurrentSide() && (to.y == 0 || to.y == 7))
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
					int moveNumber = chessboardGUI.logicBoard.moveHistory.size();
					Chessboard::Move lastMove = chessboardGUI.logicBoard.moveHistory[moveNumber - 1];

					if (moveNumber % 2 != 0)
						moves += std::to_string(moveNumber / 2 + 1) + ". ";

					if (lastMove.moveType != Chessboard::CASTLING)
					{
						if (lastMove.pieceOnMove != PAWN) {
							moves += " PBNRQK"[lastMove.pieceOnMove];
						}
						if(lastMove.ambiguousX == true)
							moves += "abcdefgh"[lastMove.from.x];
						if(lastMove.ambiguousY == true)
							moves += "12345678"[lastMove.from.y];
						if (lastMove.moveType == Chessboard::BEATING || lastMove.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING || lastMove.moveType == Chessboard::EN_PASSANT)
						{
							if (lastMove.pieceOnMove == PAWN)
								moves += "abcdefgh"[lastMove.from.x];
							moves += "x";
						}
						moves += "abcdefgh"[lastMove.to.x];
						moves += "12345678"[lastMove.to.y];
					}
					if (lastMove.moveType == Chessboard::PAWN_PROMOTION || lastMove.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING)
					{
						moves += "=";
						moves += " PBNRQK"[chessboardGUI.logicBoard.getPieceAt(to).pieceType];
					}
					else if (lastMove.moveType == Chessboard::CASTLING)
					{
						if (lastMove.kingSideCastle)
							moves += "O-O";
						else
							moves += "O-O-O";
					}
					chessboardGUI.logicBoard.getLegalMoves();
					
					if (chessboardGUI.logicBoard.getGameState() == Chessboard::CHECK)
						moves += '+';
					if (chessboardGUI.logicBoard.getGameState() == Chessboard::MATE)
						moves += '#';
					moves += moveNumber % 2 != 0 ? "  " : "\n";

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

		moveList.setString(moves);
		chessboardGUI.window->draw(moveList);
		chessboardGUI.window->display();
	}

	return 0;
}