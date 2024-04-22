#ifndef ENGINE_H
#define ENGINE_H

#define U64 unsigned long long
#include "Move.hpp"

//setup objects
Move makeMove;
Constants boardConstants;

class Engine {
    public:
        static int evaluation(ChessBoard bitboard);
        int negamax(int alpha, int beta, int depth, ChessBoard *board);
        int ply = 0;
        int bestMove;
        int searchPosition(int depth, ChessBoard board);
        int quiesence(int alpha, int beta, ChessBoard *board);
        int scoreMoves(int move, ChessBoard board);
        void sortMoves(moveList *move_list, ChessBoard board);
    private:
};

inline void Engine::sortMoves(moveList *move_list, ChessBoard board) {
    int moveScores[move_list->count];
    for (int count = 0; count < move_list->count; count++) {
        moveScores[count] = scoreMoves(move_list->moves[count], board);
    }

    for (int current = 0; current < move_list->count; current++) {
        for (int next = current + 1; next < move_list->count; next++) {
            if (moveScores[current] < moveScores[next]) {
                int tempScore = moveScores[current];
                moveScores[current] = moveScores[next];
                moveScores[next] = tempScore;

                int tempMove = move_list->moves[current];
                move_list->moves[current] = move_list->moves[next];
                move_list->moves[next] = tempMove;
            }
        }
    } 
}

inline int Engine::scoreMoves(int move, ChessBoard board) {
    if (makeMove.getMoveCapture(move)) {
        int targetPiece = P;
        //current move is capturing smthn
        int startPiece, endPiece;

        if (!board.side) {
            startPiece = p;
            endPiece = k;
        }
        //black move vv
        else {
            startPiece = P;
            endPiece = K;
        }

        for (int bbPiece = startPiece; bbPiece <= endPiece; bbPiece++) {
            if (board.bitBoards[bbPiece].getBit(makeMove.getMoveTarget(move))) {
                targetPiece = bbPiece;
                break;
            }
        }
        return boardConstants.mvvlvaValue(makeMove.getMovePiece(move), targetPiece);
    } else {

    }
    return 0;
}

inline int Engine::quiesence(int alpha, int beta, ChessBoard *board) {
    int eval = evaluation(*board);
    if (eval >= beta) {
        //node for move fails high
        return beta;
    }
        //found better movee
    if (eval > alpha) {
        //Principal variations
        alpha = eval;
    }
    nodes++;

    moveList move_list[1];
    makeMove.generateAllMoves(*board, move_list);
    sortMoves(move_list, *board);

    for (int count = 0; count < move_list->count; count++) {
        ChessBoard boardCopy = *board;
        ply++;
        if (makeMove.make_move(move_list->moves[count], only_captues, &(*board)) == 0) {
            ply--;
            continue;
        }
        int score = -quiesence(-beta, -alpha, &(*board));
        ply--;
        *board = boardCopy;
        if (score >= beta) {
            //node for move fails high
            return beta;
        }
        //found better movee
        if (score > alpha) {
            //Principal variations
            alpha = score;
        }
    }
    return alpha;
}

inline int Engine::searchPosition(int depth, ChessBoard board) {
    int score = negamax(-50000, 50000, depth, &board);
    return score;
}

inline int Engine::negamax(int alpha, int beta, int depth, ChessBoard *board) {
    if (depth == 0) {
        return quiesence(alpha, beta, &(*board));
    }
    nodes++;

    int oldAlpha = alpha;
    int bestMoveSoFar = 0;
    int legalMoves = 0;
    int inCheck = makeMove.isSquareUnderAttack((!(*board).side) ? board->mainBoard.getLeastSigBit(board->bitBoards[K].bitBoard) :  board->mainBoard.getLeastSigBit(board->bitBoards[k].bitBoard), board->side ^ 1, *board);

    if (inCheck) {depth++;}

    moveList move_list[1];
    makeMove.generateAllMoves(*board, move_list);

    sortMoves(move_list, *board);



    for (int count = 0; count < move_list->count; count++) {
        ChessBoard boardCopy = *board;
        ply++;
        if (!makeMove.make_move(move_list->moves[count], all_moves, &(*board))) {
            ply--;
            continue;
        }
        legalMoves++;

        int score = -negamax(-beta, -alpha, depth - 1, &(*board));
        ply--;
        *board = boardCopy;
        if (score >= beta) {
            //node for move fails high
            return beta;
        }
        //found better movee
        if (score > alpha) {
            //Principal variations
            alpha = score;
            //if (ply > -10) {
                //cout << "Ply: " << ply << "\n";
            //}
            if (ply == 0) {
                bestMoveSoFar = move_list->moves[count];
            }
        }
    }
    if (legalMoves == 0) {
        if (inCheck) {
            return -49000 + ply;
        }
        else {
            return 0;
        }
    }

    if (oldAlpha != alpha) {
        bestMove = bestMoveSoFar;
    }
    //move fails low
    return alpha;
}

inline int Engine::evaluation(ChessBoard bitboard) {
    int score = 0;
    //current pieces bitboard copy
    ChessBoard bitBoardCopy;

    int piece, square;
    for (int bbPiece = P; bbPiece <= k; bbPiece++) {
        bitBoardCopy.mainBoard.bitBoard = bitboard.bitBoards[bbPiece].bitBoard;
        while (bitBoardCopy.mainBoard.bitBoard) {
            piece = bbPiece;
            square = bitBoardCopy.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
            score += boardConstants.getMaterialScore(piece);

            switch (piece)
            {
            case P:
                score += pawn.getPositionInTable(square);
                break;
            case N:
                score += knight.getPositionInTable(square);
                break;
            case B:
                score += bishop.getPositionInTable(square);
                break;
            case R:
                score += rook.getPositionInTable(square);
                break;
            case K:
                score += king.getPositionInTable(square);
                break;
            case p:
                score -= pawn.getPositionInTable(boardConstants.getMirroredBoardPlace(square));
                break;
            case n:
                score -= knight.getPositionInTable(boardConstants.getMirroredBoardPlace(square));
                break;
            case b:
                score -= bishop.getPositionInTable(boardConstants.getMirroredBoardPlace(square));
                break;
            case r:
                score -= rook.getPositionInTable(boardConstants.getMirroredBoardPlace(square));
                break;
            case k:
                score -= king.getPositionInTable(boardConstants.getMirroredBoardPlace(square));
                break;
            }
            //pop lsb
            bitBoardCopy.mainBoard.popBit(square);
        }
    }
    return ((bitboard.side == white) ? score : -score);
}

#endif