#include "chessboard.h"

Chessboard::Chessboard()
{
    movesDone = 0;
    lastMove = Position(0, 0);
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

std::vector<Chessboard::Move> Chessboard::getLegalMovesAt(Position position)
{
    std::vector<Chessboard::Move> result;

    Piece &current = pieces[position.x][position.y];
    int direction = (current.side == WHITE ? 1 : -1);

    if (current.pieceType == PAWN)
    {
        int current_x = position.x;
        int current_y = position.y + direction;
        if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
        {
            if (pieces[current_x][current_y].pieceType == EMPTY)
            {
                // Pawn promotion
                if (current_y == (current.side == WHITE ? 7 : 0))
                    result.push_back(Move(PAWN_PROMOTION, Position(current_x, current_y)));
                else
                    result.push_back(Move(Position(current_x, current_y)));
            }
        }

        current_x = position.x;
        current_y = position.y + 2 * direction;
        if (!current.firstMoveDone)
            if (pieces[current_x][current_y].pieceType == EMPTY && pieces[current_x][current_y - direction].pieceType == EMPTY)
                result.push_back(Move(Position(current_x, current_y)));

        for (int horizontal = -1; horizontal <= 1; horizontal += 2)
        {
            current_x = position.x + horizontal;
            current_y = position.y + direction;
            if (!(current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7))
            {
                if (pieces[current_x][current_y].side == current.side * -1)
                {
                    // Pawn promotion
                    if (current_y == (current.side == WHITE ? 7 : 0))
                        result.push_back(Move(PAWN_PROMOTION, Position(current_x, current_y)));
                    else
                        result.push_back(Move(Position(current_x, current_y)));
                }
                else if (pieces[current_x][current_y].pieceType == EMPTY && pieces[current_x][current_y - direction].pieceType == PAWN && lastMove.x == current_x && lastMove.y == current_y - direction) //en passant
                    result.push_back(Move(Position(current_x, current_y)));
            }
        }
    }
    else if (current.pieceType == BISHOP || current.pieceType == QUEEN)
    {
        for (int horizontal = -1; horizontal <= 1; horizontal += 2)
        {
            for (int vertical = -1; vertical <= 1; vertical += 2)
            {
                for (int i = 1; i <= 7; i++)
                {
                    int current_x = position.x + horizontal * i;
                    int current_y = position.y + vertical * i;
                    if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                        break;
                    if (pieces[current_x][current_y].side == current.side * -1)
                    {
                        result.push_back(Move(Position(current_x, current_y)));
                        break;
                    }
                    if (!pieces[current_x][current_y].pieceType)
                        result.push_back(Move(Position(current_x, current_y)));
                }
            }
        }
    }
    else if (current.pieceType == KNIGHT)
    {
        for (int horizontal = -1; horizontal <= 1; horizontal += 2)
        {
            for (int vertical = -1; vertical <= 1; vertical += 2)
            {
                for (int x = -2; x <= -1; x++)
                {
                    int current_x = position.x + horizontal * x;
                    int current_y = position.y + vertical * (x == -2 ? 1 : 2);
                    if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                        continue;
                    if (pieces[current_x][current_y].pieceType == EMPTY || pieces[current_x][current_y].side == current.side * -1)
                        result.push_back(Move(Position(current_x, current_y)));
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
                int current_x = position.x + dir.x * i;
                int current_y = position.y + dir.y * i;
                if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                    break;
                if (pieces[current_x][current_y].side == current.side * -1)
                {
                    result.push_back(Move(Position(current_x, current_y)));
                    break;
                }
                if (!pieces[current_x][current_y].pieceType)
                    result.push_back(Move(Position(current_x, current_y)));
            }
        }
    }
    else if (current.pieceType == KING)
    {
        for (int horizontal = -1; horizontal <= 1; horizontal++)
        {
            for (int vertical = -1; vertical <= 1; vertical++)
            {
                int current_x = position.x + horizontal;
                int current_y = position.y + vertical;
                if (current_x < 0 || current_x > 7 || current_y < 0 || current_y > 7)
                    continue;
                if (pieces[current_x][current_y].pieceType == EMPTY || pieces[current_x][current_y].side == current.side * -1)
                    result.push_back(Move(Position(current_x, current_y)));
            }
        }
    }
    return result;
}

// Check if castling is available.
std::vector<Chessboard::Move> Chessboard::getCastling()
{
    std::vector<Chessboard::Move> result;
    for (int side = 0; side < 8; side += 7)
    {
        // Kingside
        if (!pieces[4][side].firstMoveDone && !pieces[7][side].firstMoveDone)
        {
            bool castleAvailable = true;
            for (int i = 1; i < 7 - 4; i++)
                if (pieces[4 + i][side].pieceType != EMPTY)
                {
                    castleAvailable = false;
                    break;
                }
            if (castleAvailable)
                result.push_back(Move(side == 0 ? WHITE : BLACK, true));
        }

        // Queenside
        if (!pieces[4][side].firstMoveDone && !pieces[0][side].firstMoveDone)
        {
            bool castleAvailable = true;
            for (int i = 1; i < 4 - 0; i++)
                if (pieces[4 - i][side].pieceType != EMPTY)
                {
                    castleAvailable = false;
                    break;
                }
            if (castleAvailable)
                result.push_back(Move(side == 0 ? WHITE : BLACK, false));
        }
    }
    return result;
}

bool Chessboard::makeMove(Position from, Position to)
{
    if ((!(movesDone % 2) && pieces[from.x][from.y].side != WHITE) || (movesDone % 2 && pieces[from.x][from.y].side != BLACK))
        return false;

    std::vector<Move> legalMoves = getLegalMovesAt(from);
    bool found = false;
    for (Move move : legalMoves)
        if (to == move.to)
        {
            found = true;
            break;
        }
    if (!found)
        return false;

    if (pieces[from.x][from.y].pieceType == PAWN && abs(from.x - to.x) == 2)
        pieces[to.x][to.y].twoSquares = true;
    if (pieces[from.x][from.y].pieceType == PAWN && pieces[to.x][to.y].pieceType == EMPTY && from.x != to.x) //en passant
    {
        pieces[to.x][from.y].side = NONE;
        pieces[to.x][from.y].pieceType = EMPTY;
    }
    pieces[from.x][from.y].firstMoveDone = true;
    pieces[to.x][to.y] = pieces[from.x][from.y];
    pieces[from.x][from.y].side = NONE;
    pieces[from.x][from.y].pieceType = EMPTY;
    lastMove = to;
    movesDone++;
    return true;
}

bool Chessboard::makeMove(Side side, bool isKingSideCastle)
{
    std::vector<Chessboard::Move> legalMoves = getCastling();
    for (Move move : legalMoves)
        if (move.side == side && move.kingSideCastle == isKingSideCastle)
        {
            int y = side == WHITE ? 0 : 7;
            int new_king_x = isKingSideCastle ? 6 : 2;
            int rook_x = isKingSideCastle ? 7 : 0;
            int new_rook_x = isKingSideCastle ? 5 : 3;

            pieces[4][y].firstMoveDone = true;
            pieces[rook_x][y].firstMoveDone = true;

            pieces[new_king_x][y] = pieces[4][y];
            pieces[new_rook_x][y] = pieces[rook_x][y];

            pieces[4][y].pieceType = EMPTY;
            pieces[4][y].side = NONE;
            pieces[rook_x][y].pieceType = EMPTY;
            pieces[rook_x][y].side = NONE;

            movesDone++;
            return true;
        }
    return false;
}

bool Chessboard::makeMove(PieceType promoted, Position from, Position to)
{
    if (pieces[from.x][from.y].pieceType != PAWN || promoted == EMPTY || promoted == PAWN || promoted == KING)
        return false;
    if ((!(movesDone % 2) && pieces[from.x][from.y].side != WHITE) || (movesDone % 2 && pieces[from.x][from.y].side != BLACK))
        return false;

    std::vector<Move> legalMoves = getLegalMovesAt(from);
    bool found = false;
    for (Move move : legalMoves)
        if (move.moveType == PAWN_PROMOTION && to == move.to)
        {
            found = true;
            break;
        }
    if (!found)
        return false;

    pieces[to.x][to.y] = Piece(promoted, pieces[from.x][from.y].side);
    pieces[from.x][from.y] = Piece();
    return true;
}

std::ostream &operator<<(std::ostream &s, const Chessboard &c)
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            if (c.pieces[x][y].pieceType == EMPTY)
                s << ".";
            else if (c.pieces[x][y].side == WHITE)
                s << ".PBNRQK"[c.pieces[x][y].pieceType];
            else
                s << ".pbnrqk"[c.pieces[x][y].pieceType];
        }

        s << std::endl;
    }
    return s;
}