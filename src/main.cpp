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

    sf::Texture boardTexture;
    boardTexture.loadFromFile("img/board.png");
    sf::Sprite boardSprite;
    boardSprite.setTexture(boardTexture);

    Position from;
    Position to;

    Chessboard chessboard;

    std::vector<sf::Texture> textures;
    for(int i = 1; i < 13; i++)
    {
        sf::Texture tmp;
        std::string name = "img/" + std::to_string(i) + ".png";
        tmp.loadFromFile(name);

        textures.push_back(tmp);
    }

    std::vector<sf::Sprite> pieces;
    for(int i = 0; i < 32; i++)
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

                chessboard.makeMove(from, to);
                std::cout << chessboard;
                setPieces(chessboard, pieces, textures);
            }
        }

        window.clear();
        window.draw(boardSprite);
        for(int i = 0; i < 32; i++)
            window.draw(pieces[i]);
        window.display();
    }

    return 0;
}