#pragma once
#include <vector>
#include "chessboard.h"
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 929

std::vector<sf::Texture> loadPiecesTextures()
{
	std::vector<sf::Texture> result;
	for (int i = 1; i < 13; i++)
	{
		sf::Texture tmp;
		std::string name = "img/" + std::to_string(i) + ".png";
		tmp.loadFromFile(name);

		result.push_back(tmp);
	}

	return result;
}

std::vector<sf::Texture> loadBoardTextures()
{
	std::vector<sf::Texture> result;

	sf::Texture whiteSquareTexture;
	whiteSquareTexture.loadFromFile("img/whiteSquare.png");
	sf::Texture blackSquareTexture;
	blackSquareTexture.loadFromFile("img/blackSquare.png");

	result.push_back(whiteSquareTexture);
	result.push_back(blackSquareTexture);

	return result;
}

std::vector<sf::Sprite> setCheckerboard(std::vector<sf::Texture> &boardTextures)
{
	std::vector<sf::Sprite> result;

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
			result.push_back(tmp);
		}
	}

	return result;
}

std::vector<sf::Sprite> setPieces()
{
	std::vector<sf::Sprite> result;
	for (int i = 0; i < 32; i++)
	{
		sf::Sprite tmp;
		result.push_back(tmp);
	}

	return result;
}

int updatePieces(Chessboard &chessboard, std::vector<sf::Sprite> &pieces, std::vector<sf::Texture> &piecesTextures)
{
	int count = 0;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (chessboard.getPieceAt(Position(x, y)).pieceType == EMPTY)
				continue;

			Piece piece = chessboard.getPieceAt(Position(x, y));
			pieces[count].setPosition(x * (WINDOW_SIZE / 8), WINDOW_SIZE - y * (WINDOW_SIZE / 8) - 96);
			int type = piece.pieceType + (piece.side == BLACK ? 6 : 0) - 1;
			pieces[count].setTexture(piecesTextures[type]);
			count++;
		}
	}

	return count;
}