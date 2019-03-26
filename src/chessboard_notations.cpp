#include "chessboard.h"

void Chessboard::setSanString(PieceType promotion)
{
    int moveNumber = moveHistory.size();
    Chessboard::Move lastMove = moveHistory[moveNumber - 1];
    sanString = "";
    if (moveNumber % 2 != 0)
        sanString += std::to_string(moveNumber / 2 + 1) + ". ";

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
    sanString += moveNumber % 2 != 0 ? "  " : "\n";
}

std::string Chessboard::getSanString()
{
    return sanString;
}

Chessboard::Move Chessboard::moveFromSAN(std::string san)
{
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
        if (san.find_first_of("BNRQK") != std::string::npos)
        {
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
            Position to(san[1 + isBeating] - 'a', san[2 + isBeating] - '1');

            for (Move &m : legalMoves)
                if ((m.moveType == isBeating ? Chessboard::BEATING : Chessboard::NORMAL) && m.to == to && m.pieceOnMove == pieceType)
                    return m;
        }
        else // It's a pawn move.
        {
            bool isPromoted = (san.find("=") != std::string::npos);
            Position to(san[0 + isBeating * 2] - 'a', san[1 + isBeating * 2] - '1');
            int column;
            if (isBeating)
                column = san[0];

            MoveType moveType = NORMAL;
            if (isBeating)
            {
                if (isPromoted)
                    moveType = PAWN_PROMOTION_WITH_BEATING;
                else
                    moveType = BEATING;
            }

            for (Move &m : legalMoves)
            {
                if ((m.moveType == moveType || m.moveType == EN_PASSANT) && m.to == to)
                {
                    if (isBeating && m.from.x != column)
                        continue;
                    return m;
                }
            }
        }
    }

    return Chessboard::Move(Position(), Position(), EMPTY);
}

std::vector<Chessboard::Move> movesFromPGN(std::string pgn)
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
        result.push_back(tmp);
    }

    return result;
}