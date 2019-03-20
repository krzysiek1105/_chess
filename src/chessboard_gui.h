#pragma once
#include <vector>
#include "chessboard.h"
#include <SFML/Graphics.hpp>

#define WINDOW_SIZE 928

class ChessboardGUI
{
  public:
    sf::RenderWindow *window;
    Chessboard logicBoard;
    std::vector<sf::Texture> piecesTextures;
    std::vector<sf::Texture> boardTextures;

    std::vector<sf::Sprite> squares;
    std::vector<sf::Sprite> pieces;

    int piecesOnBoard;

    void loadPiecesTextures();
    void loadBoardTextures();
    void setCheckerboard();
    void setPieces();
    void updatePieces();
    void highlight(Position from);
    void resetHighlighting();
	PieceType showPromotion(Side side);
    ChessboardGUI()
    {
        window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(30);

        loadPiecesTextures();
        loadBoardTextures();

        setCheckerboard();
        setPieces();

        updatePieces();
    }

    ~ChessboardGUI()
    {
        delete window;
    }
};