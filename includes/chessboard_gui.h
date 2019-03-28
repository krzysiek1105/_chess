#pragma once
#include <vector>
#include "chessboard.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#define PIECE_SIZE 80
#define SQUARE_SIZE 110
#define CHESSBOARD_SIZE (8 * SQUARE_SIZE)
#define SIDE_PANEL_WIDTH 270
#define SIDE_PANEL_PADDING 10

class ChessboardGUI
{
  public:
    sf::RenderWindow *window;
	sf::Font font;
    tgui::Gui gui;
    tgui::Theme theme;
    tgui::Button::Ptr resetButton;
	tgui::ListView::Ptr movesPanel;
    tgui::TextBox::Ptr PGNTextBox;
    tgui::Button::Ptr PGNButton;
    Chessboard logicBoard;
    std::vector<sf::Texture> piecesTextures;
    std::vector<sf::Texture> boardTextures;
    std::vector<std::string> sanMoves;
    std::vector<sf::String> san;

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
    void resetAll();
    void loadFromPGNTextBox();
    bool tryMove(Position from, Position to);
    void updateAfterMove();
    void draw();
	PieceType showPromotion();

    ChessboardGUI()
    {
        window = new sf::RenderWindow(sf::VideoMode(CHESSBOARD_SIZE + SIDE_PANEL_WIDTH, CHESSBOARD_SIZE), "Chess", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(30);
		theme.load("themes/Black.txt");
        gui.setTarget(*window);

		font.loadFromFile("arial.ttf");
		tgui::setGlobalFont(font);

        resetButton = tgui::Button::create("Reset");
        resetButton->setRenderer(theme.getRenderer("Button"));
        resetButton->setSize(100, 30);
        resetButton->setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, 0);
        resetButton->connect("pressed", &ChessboardGUI::resetAll, this);
        gui.add(resetButton);

		movesPanel = tgui::ListView::create();
		movesPanel->setRenderer(theme.getRenderer("ListView"));
		movesPanel->setSize(250, 200);
		movesPanel->setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, 85);
		movesPanel->setTextSize(18);
		movesPanel->setExpandLastColumn(true);
		movesPanel->addColumn("no", 34);
		movesPanel->addColumn("white", 92);
		movesPanel->addColumn("black", 92);
		movesPanel->setGridLinesWidth(0);
		gui.add(movesPanel);

        PGNTextBox = tgui::TextBox::create();
        PGNTextBox->setRenderer(theme.getRenderer("TextBox"));
        PGNTextBox->setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, 385);
        gui.add(PGNTextBox);

        PGNButton = tgui::Button::create("Load PGN");
        PGNButton->setRenderer(theme.getRenderer("Button"));
        PGNButton->setSize(100, 30);
        PGNButton->setPosition(CHESSBOARD_SIZE + SIDE_PANEL_PADDING, 550);
        PGNButton->connect("pressed", &ChessboardGUI::loadFromPGNTextBox, this);
        gui.add(PGNButton);

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