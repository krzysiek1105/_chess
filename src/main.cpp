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
			chessboardGUI.gui.handleEvent(event);
			if (event.type == sf::Event::Closed)
				chessboardGUI.window->close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				from.x = (event.mouseButton.x / (float)CHESSBOARD_SIZE) * 8;
				from.y = 8 - (event.mouseButton.y / (float)CHESSBOARD_SIZE) * 8;
				if (from.x < 8)
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
				if (chessboardGUI.tryMove(from, to))
					chessboardGUI.updateAfterMove();
				chessboardGUI.resetHighlighting();
			}
		}

		chessboardGUI.draw();
		chessboardGUI.window->display();
	}

	return 0;
}