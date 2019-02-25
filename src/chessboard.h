#pragma once
#include "piece.h"
#include <array>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utility>

class Chessboard
{
  public:
    std::array<std::array<Piece, 8>, 8> pieces;
    int movesDone;

    Chessboard()
    {
        movesDone = 0;
        pieces[0][0] = Piece(ROOK, WHITE);
        pieces[1][0] = Piece(KNIGHT, WHITE);
        pieces[2][0] = Piece(BISHOP, WHITE);
        pieces[3][0] = Piece(QUEEN, WHITE);
        pieces[4][0] = Piece(KING, WHITE);
        pieces[5][0] = Piece(BISHOP, WHITE);
        pieces[6][0] = Piece(KNIGHT, WHITE);
        pieces[7][0] = Piece(ROOK, WHITE);

        pieces[0][1] = Piece(PAWN, WHITE);
        pieces[1][1] = Piece(PAWN, WHITE);
        pieces[2][1] = Piece(PAWN, WHITE);
        pieces[3][1] = Piece(PAWN, WHITE);
        pieces[4][1] = Piece(PAWN, WHITE);
        pieces[5][1] = Piece(PAWN, WHITE);
        pieces[6][1] = Piece(PAWN, WHITE);
        pieces[7][1] = Piece(PAWN, WHITE);

        pieces[0][7] = Piece(ROOK, BLACK);
        pieces[1][7] = Piece(KNIGHT, BLACK);
        pieces[2][7] = Piece(BISHOP, BLACK);
        pieces[3][7] = Piece(QUEEN, BLACK);
        pieces[4][7] = Piece(KING, BLACK);
        pieces[5][7] = Piece(BISHOP, BLACK);
        pieces[6][7] = Piece(KNIGHT, BLACK);
        pieces[7][7] = Piece(ROOK, BLACK);

        pieces[0][6] = Piece(PAWN, BLACK);
        pieces[1][6] = Piece(PAWN, BLACK);
        pieces[2][6] = Piece(PAWN, BLACK);
        pieces[3][6] = Piece(PAWN, BLACK);
        pieces[4][6] = Piece(PAWN, BLACK);
        pieces[5][6] = Piece(PAWN, BLACK);
        pieces[6][6] = Piece(PAWN, BLACK);
        pieces[7][6] = Piece(PAWN, BLACK);
    }

    std::vector<std::pair<Position, Piece>> findPiece(PieceType pieceType, Side side)
    {
        std::vector<std::pair<Position, Piece>> out;
        for (int x = 0; x < 8; x++)
            for (int y = 0; y < 8; y++)
                if (pieces[x][y].pieceType == pieceType && pieces[x][y].side == side)
                    out.push_back(std::make_pair(Position(x, y), pieces[x][y]));
        return out;
    }

    std::vector<Position> getLegalMovesAt(Position position)
    {
        std::vector<Position> result;

        Piece &current = pieces[position.x][position.y];
        int direction = (current.side == WHITE ? 1 : -1);

        if (current.pieceType == PAWN)
        {
            int current_x = position.x;
            int current_y = position.y + direction;
            if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
                if (pieces[current_x][current_y].pieceType == EMPTY)
                    result.push_back(Position(current_x, current_y));

            current_x = position.x;
            current_y = position.y + 2 * direction;
            if (!current.firstMoveDone)
                if (pieces[current_x][current_y].pieceType == EMPTY)
                    result.push_back(Position(current_x, current_y));

            current_x = position.x - 1;
            current_y = position.y + direction;
            if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
                if (pieces[current_x][current_y].pieceType != EMPTY && pieces[current_x][current_y].side != current.side)
                    result.push_back(Position(current_x, current_y));

            current_x = position.x + 1;
            current_y = position.y + direction;
            if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
                if (pieces[current_x][current_y].pieceType != EMPTY && pieces[current_x][current_y].side != current.side)
                    result.push_back(Position(current_x, current_y));
        }
        else if (current.pieceType == BISHOP || current.pieceType == QUEEN)
        {
            for (int left = -1; left <= 1; left += 2)
            {
                for (int down = -1; down <= 1; down += 2)
                {
                    for (int i = 1; i <= 7; i++)
                    {
                        int current_x = position.x + left * i * direction;
                        int current_y = position.y + down * i * direction;
                        if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                            continue;
                        if (pieces[current_x][current_y].pieceType != EMPTY)
                            break;
                        if (pieces[current_x][current_y].side != current.side && pieces[current_x][current_y].pieceType != EMPTY)
                        {
                            result.push_back(Position(current_x, current_y));
                            break;
                        }
                        result.push_back(Position(current_x, current_y));
                    }
                }
            }
        }
        else if (current.pieceType == KNIGHT)
        {
            for (int left = -1; left <= 1; left += 2)
            {
                for (int down = -1; down <= 1; down += 2)
                {
                    for (int x = -2; x <= -1; x++)
                    {
                        int current_x = position.x + left * direction * x;
                        int current_y = position.y + down * direction * (x == -2 ? 1 : 2);
                        if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                            continue;
                        if (pieces[current_x][current_y].pieceType == EMPTY || (pieces[current_x][current_y].side != current.side && pieces[current_x][current_y].pieceType != EMPTY))
                            result.push_back(Position(current_x, current_y));
                    }
                }
            }
        }
        if (current.pieceType == ROOK || current.pieceType == QUEEN)
        {
            std::vector<Position> dirs;
            dirs.push_back(Position(-1, 0));
            dirs.push_back(Position(1, 0));
            dirs.push_back(Position(0, 1));
            dirs.push_back(Position(0, -1));
            for (Position dir : dirs)
            {
                for (int i = 1; i <= 7; i++)
                {
                    int current_x = position.x + dir.x * i * direction;
                    int current_y = position.y + dir.y * i * direction;
                    if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                        continue;
                    if (pieces[current_x][current_y].pieceType != EMPTY)
                        break;
                    if (pieces[current_x][current_y].side != current.side && pieces[current_x][current_y].pieceType != EMPTY)
                    {
                        result.push_back(Position(current_x, current_y));
                        break;
                    }
                    result.push_back(Position(current_x, current_y));
                }
            }

            // Check if castling is available.
            if (current.pieceType == ROOK)
            {
                Piece king;
                Position king_pos;
                bool found = false;
                for (int x = 0; x < 8; x++)
                {
                    for (int y = 0; y < 8; y++)
                    {
                        if (pieces[x][y].pieceType == KING && pieces[x][y].side == current.side)
                        {
                            king = pieces[x][y];
                            king_pos.x = x;
                            king_pos.y = y;
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }

                if (found)
                {
                    // First, assume that it is true and then test if it is indeed truth.
                    current.castleAvailable = true;

                    if (current.firstMoveDone || king.firstMoveDone)
                        current.castleAvailable = false;
                    else
                    {
                        // We are sure that the king and the rook are on the same row, because neither the king or the rook has moved.
                        int dir = king_pos.x - position.x;
                        int dir_normalized = dir / abs(dir);
                        for (int i = 1; i < abs(dir); i++)
                            if (pieces[position.x + dir_normalized * i][position.y].pieceType != EMPTY)
                                current.castleAvailable = false;
                    }
                }
            }
        }
        else if (current.pieceType == KING)
        {
            std::vector<Position> dirs;
            dirs.push_back(Position(-1, 0));
            dirs.push_back(Position(-1, 1));
            dirs.push_back(Position(0, 1));
            dirs.push_back(Position(1, 0));

            dirs.push_back(Position(1, -1));
            dirs.push_back(Position(-1, 0));
            dirs.push_back(Position(-1, -1));
            dirs.push_back(Position(-1, 0));

            for (Position dir : dirs)
            {
                int current_x = position.x + dir.x * direction;
                int current_y = position.y + dir.y * direction;
                if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                    continue;
                if (pieces[current_x][current_y].pieceType != EMPTY)
                    break;
                if (pieces[current_x][current_y].side != current.side && pieces[current_x][current_y].pieceType != EMPTY)
                {
                    result.push_back(Position(current_x, current_y));
                    break;
                }
                result.push_back(Position(current_x, current_y));
            }
        }
        return result;
    }

    bool makeMove(Position from, Position to)
    {
        std::vector<Position> legalMoves = getLegalMovesAt(from);
        bool found = false;
        for (Position p : legalMoves)
            if (to == p)
            {
                found = true;
                break;
            }
        if (!found)
            return false;

        pieces[from.x][from.y].firstMoveDone = true;
        pieces[to.x][to.y] = pieces[from.x][from.y];
        pieces[from.x][from.y].side = NONE;
        pieces[from.x][from.y].pieceType = EMPTY;
        movesDone++;
        return true;
    }

    friend std::ostream &operator<<(std::ostream &s, const Chessboard &c)
    {
        for (int y = 7; y >= 0; y--)
        {
            for (int x = 0; x < 8; x++)
            {
                if (c.pieces[x][y].pieceType == EMPTY)
                    std::cout << ".";
                else if (c.pieces[x][y].side == WHITE)
                    s << "PBKRQ^"[c.pieces[x][y].pieceType];
                else
                        s << "pbkrq*"[c.pieces[x][y].pieceType];
                }
            }
            s << std::endl;
        }
        return s;
    }
};