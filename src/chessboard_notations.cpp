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