#include <stdio.h>
#include <string>
#include "chessboard.h"
#include "piece.h"
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 929

void setPieces(Chessboard &chessboard, std::vector<sf::Sprite> &pieces, std::vector<sf::Texture> &textures)
{
	int i = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (chessboard.pieces[x][y].pieceType == EMPTY)
				continue;

			Piece piece = chessboard.pieces[x][y];
			pieces[i].setPosition(x * (WINDOW_SIZE / 8), WINDOW_SIZE - y * (WINDOW_SIZE / 8) - 96);
			int type = piece.pieceType + (piece.side == BLACK ? 6 : 0) - 1;
			pieces[i].setTexture(textures[type]);
			i++;
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(30);

	sf::Texture whiteSquareTexture;
	whiteSquareTexture.loadFromFile("img/whiteSquare.png");
	sf::Texture blackSquareTexture;
	blackSquareTexture.loadFromFile("img/blackSquare.png");

	std::vector<sf::Sprite> squares;
	for (int y = 7; y >= 0; y--)
	{
		for (int x = 0; x < 8; x++)
		{
			sf::Sprite tmp;
			if ((x + y) % 2 == 0)
				tmp.setTexture(whiteSquareTexture);
			else
				tmp.setTexture(blackSquareTexture);
			tmp.setPosition(x * (WINDOW_SIZE / 8), y * (WINDOW_SIZE / 8));

			squares.push_back(tmp);
		}
	}

	Position from;
	Position to;

	Chessboard chessboard;

	std::vector<sf::Texture> textures;
	for (int i = 1; i < 13; i++)
	{
		sf::Texture tmp;
		std::string name = "img/" + std::to_string(i) + ".png";
		tmp.loadFromFile(name);

		textures.push_back(tmp);
	}

	std::vector<sf::Sprite> pieces;
	for (int i = 0; i < 32; i++)
	{
		sf::Sprite tmp;
		pieces.push_back(tmp);
	}

	setPieces(chessboard, pieces, textures);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x = event.mouseButton.x;
				int y = event.mouseButton.y;
				int fieldX = (x / (float)WINDOW_SIZE) * 8;
				int fieldY = 8 - (y / (float)WINDOW_SIZE) * 8;

				from.x = fieldX;
				from.y = fieldY;

				std::vector<Chessboard::Move> legalMoves = chessboard.getLegalMoves();
				Side side = chessboard.movesDone % 2 == 0 ? WHITE : BLACK;
				for (Chessboard::Move m : legalMoves)
				{
					if (m.from == from)
					{
						int n = m.to.x + m.to.y * 8;
						squares[n].setColor(sf::Color(140, 205, 16, 255));
					}
					else if (m.moveType == Chessboard::CASTLING && m.side == side)
					{
						if (m.side == WHITE)
						{
							if (m.kingSideCastle)
							{
								squares[4].setColor(sf::Color(255, 205, 16, 255));
								squares[5].setColor(sf::Color(255, 205, 16, 255));
								squares[6].setColor(sf::Color(255, 205, 16, 255));
								squares[7].setColor(sf::Color(255, 205, 16, 255));
							}
							else
							{
								squares[0].setColor(sf::Color(255, 205, 16, 255));
								squares[1].setColor(sf::Color(255, 205, 16, 255));
								squares[2].setColor(sf::Color(255, 205, 16, 255));
								squares[3].setColor(sf::Color(255, 205, 16, 255));
								squares[4].setColor(sf::Color(255, 205, 16, 255));
							}
						}
						else
						{
							if (m.kingSideCastle)
							{
								squares[63 - 0].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 1].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 2].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 3].setColor(sf::Color(255, 205, 16, 255));
							}
							else
							{
								squares[63 - 3].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 4].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 5].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 6].setColor(sf::Color(255, 205, 16, 255));
								squares[63 - 7].setColor(sf::Color(255, 205, 16, 255));
							}
						}
					}
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				int x = event.mouseButton.x;
				int y = event.mouseButton.y;
				int fieldX = (x / (float)WINDOW_SIZE) * 8;
				int fieldY = 8 - (y / (float)WINDOW_SIZE) * 8;

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
					chessboard.makeMove(WHITE, true);
				else if (fromString == "e1" && toString == "a1")
					chessboard.makeMove(WHITE, false);
				else if (fromString == "e8" && toString == "h8")
					chessboard.makeMove(BLACK, true);
				else if (fromString == "e8" && toString == "a8")
					chessboard.makeMove(BLACK, false);
				else
					chessboard.makeMove(from, to);

				std::cout << chessboard;
				setPieces(chessboard, pieces, textures);

				for (int i = 0; i < 64; i++)
					squares[i].setColor(sf::Color(255, 255, 255, 255));

				std::string windowTitle = chessboard.movesDone % 2 == 0 ? "White move " : "Black move ";
				if (chessboard.getChecks().size() > 0)
					windowTitle.append("CHECK");
				window.setTitle(windowTitle);
			}
		}

		window.clear();
		for (int i = 0; i < 64; i++)
			window.draw(squares[i]);
		for (int i = 0; i < 32; i++)
			window.draw(pieces[i]);
		window.display();
	}

	return 0;
}