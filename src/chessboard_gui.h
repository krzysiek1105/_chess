#pragma once
#include <vector>
#include "chessboard.h"
#include <SFML/Graphics.hpp>

#define PIECE_SIZE 80
#define SQUARE_SIZE 110
#define CHESSBOARD_SIZE (8 * SQUARE_SIZE)
#define SIDE_PANEL_WIDTH 240
#define SIDE_PANEL_PADDING 25

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

    ChessboardGUI()
    {
        window = new sf::RenderWindow(sf::VideoMode(CHESSBOARD_SIZE + SIDE_PANEL_WIDTH, CHESSBOARD_SIZE), "Chess", sf::Style::Titlebar | sf::Style::Close);
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