#include "chessboard.h"

void Chessboard::setSanString(PieceType promotion)
{
    Chessboard::Move lastMove = moveHistory[moveHistory.size() - 1];
    sanString = "";

    if (lastMove.moveType != Chessboard::CASTLING)
    {
        if (lastMove.pieceOnMove != PAWN)
        {
            sanString += " PBNRQK"[lastMove.pieceOnMove];
            if (lastMove.ambiguousX == true)
                sanString += "abcdefgh"[lastMove.from.x];
            if (lastMove.ambiguousY == true)
                sanString += "12345678"[lastMove.from.y];
        }
        if (lastMove.moveType == Chessboard::BEATING || lastMove.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING || lastMove.moveType == Chessboard::EN_PASSANT)
        {
            if (lastMove.pieceOnMove == PAWN)
                sanString += "abcdefgh"[lastMove.from.x];
            sanString += "x";
        }
        sanString += "abcdefgh"[lastMove.to.x];
        sanString += "12345678"[lastMove.to.y];
    }
    if (lastMove.moveType == Chessboard::PAWN_PROMOTION || lastMove.moveType == Chessboard::PAWN_PROMOTION_WITH_BEATING)
    {
        sanString += "=";
        sanString += " PBNRQK"[promotion];
    }
    else if (lastMove.moveType == Chessboard::CASTLING)
    {
        if (lastMove.kingSideCastle)
            sanString += "O-O";
        else
            sanString += "O-O-O";
    }

    if (getGameState() == Chessboard::CHECK)
        sanString += '+';
    if (getGameState() == Chessboard::MATE)
        sanString += '#';
	sanString += " ";
}

std::string Chessboard::getSanString()
{
    return sanString;
}

Chessboard::Move Chessboard::moveFromSAN(std::string san)
{
    getLegalMoves();
    if (san == "O-O")
    {
        for (Move &m : legalMoves)
            if (m.moveType == Chessboard::CASTLING && m.kingSideCastle)
                return m;
    }
    else if (san == "O-O-O")
    {
        for (Move &m : legalMoves)
            if (m.moveType == Chessboard::CASTLING && !m.kingSideCastle)
                return m;
    }
    else
    {
        int isBeating = !!(san.find("x") != std::string::npos);
        int isCheckOrMateMove = !!(san.find_first_of("+#") != std::string::npos);
        bool isPromoted = !!(san.find("=") != std::string::npos);

        if (san.find_first_of("BNRQK") != std::string::npos && !isPromoted)
        {
            int column = -1;
            int row = -1;

            if (san.length() > 3 + isBeating + isCheckOrMateMove)
            {
                if (san[1] >= '1' && san[1] <= '8')
                    row = san[1] - '1';
                else if (san[1] >= 'a' && san[1] <= 'h')
                {
                    column = san[1] - 'a';
                    if (san[2] >= '1' && san[2] <= '8')
                        row = san[2] - '1';
                }
            }

            PieceType pieceType;
            switch (san[0])
            {
            case 'B':
                pieceType = BISHOP;
                break;
            case 'N':
                pieceType = KNIGHT;
                break;
            case 'R':
                pieceType = ROOK;
                break;
            case 'Q':
                pieceType = QUEEN;
                break;
            case 'K':
                pieceType = KING;
                break;
            }
            Position to(san[1 + isBeating + (column >= 0) + (row >= 0)] - 'a', san[2 + isBeating + (column >= 0) + (row >= 0)] - '1');

            for (Move &m : legalMoves)
            {
                if ((m.moveType == (isBeating ? Chessboard::BEATING : Chessboard::NORMAL)) && m.to == to && m.pieceOnMove == pieceType)
                {
                    if (column >= 0)
                        if (m.from.x != column)
                            continue;
                    if (row >= 0)
                        if (m.from.y != row)
                            continue;
                    return m;
                }
            }
        }
        else // It's a pawn move.
        {
            PieceType promoted = EMPTY;
            Position to(san[0 + isBeating * 2] - 'a', san[1 + isBeating * 2] - '1');
            int column;
            if (isBeating)
                column = san[0] - 'a';

            MoveType moveType = NORMAL;
            if (isPromoted)
                moveType = PAWN_PROMOTION;
            if (isBeating)
            {
                if (isPromoted)
                    moveType = PAWN_PROMOTION_WITH_BEATING;
                else
                    moveType = BEATING;
            }

            if (isPromoted)
            {
                switch (san[3 + isBeating * 2])
                {
                case 'B':
                    promoted = BISHOP;
                    break;
                case 'N':
                    promoted = KNIGHT;
                    break;
                case 'R':
                    promoted = ROOK;
                    break;
                case 'Q':
                    promoted = QUEEN;
                    break;
                }
            }

            for (Move &m : legalMoves)
            {
                if ((m.moveType == moveType || m.moveType == EN_PASSANT) && m.to == to && m.pieceOnMove == PAWN)
                {
                    if (isBeating && m.from.x != column)
                        continue;

                    m.pieceType = promoted;
                    return m;
                }
            }
        }
    }

    return Chessboard::Move(Position(), Position(), EMPTY);
}

std::vector<Chessboard::Move> Chessboard::movesFromPGN(std::string pgn)
{
    std::vector<Chessboard::Move> result;
    std::istringstream iss(pgn);
    std::string token;
    for (int i = 0; std::getline(iss, token, ' '); i++)
    {
        if (i % 3 == 0) // We don't need move number.
            continue;
        // trim string
        token.erase(0, token.find_first_not_of(' '));
        token.erase(token.find_last_not_of(' ') + 1);

        Chessboard::Move tmp = Chessboard::moveFromSAN(token);
        makeMove(tmp);
        result.push_back(tmp);
    }

    reset();
    return result;
}