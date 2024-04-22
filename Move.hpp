#ifndef MOVE_H
#define MOVE_H

#define U64 unsigned long long

#include <iostream>
#include <cctype>
#include <string.h>

//#include "ChessBoard.hpp"

#include "Pawn.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Rook.hpp"

long int nodes = 0;

using namespace std;

//ChessBoard gameBoard;
Rook rook;
Bishop bishop;
Pawn pawn;
Knight knight;
King king;

enum {all_moves, only_captues, only_quiet};

struct moveList {
    //actual moves
    int moves[256];

    //move count
    int count = 0;

};

class Move
{
    public:
        void initAttacks();
        int isSquareUnderAttack(int square, int side, ChessBoard board);
        void printAttackedSquares(int side, ChessBoard board);
        void generateAllMoves(ChessBoard board, moveList *move_list);

        int encodeMove(int source, int target, int piece, int promoted, int capture, int doublePawn, int enpassant, int castling);

        int getMoveSource(int move) {return move & 0x3f;};
        int getMoveTarget(int move) {return ((move & 0xfc0) >> 6);};
        int getMovePiece(int move) {return ((move & 0xf000) >> 12);};
        int getMovePromoted(int move) {return ((move & 0xf0000) >> 16);};
        int getMoveCapture(int move) {return (move & 0x100000);};
        int getMoveDoublePawn(int move) {return (move & 0x200000);};
        int getMoveEnpassant(int move) {return (move & 0x400000);};
        int getMoveCastling(int move) {return (move & 0x800000);};

        void printMove(int move);
        void printMoveList(moveList *move_list);
        void addMoveToMoveList(int move, moveList *move_list);

        int make_move(int move, int move_flag, ChessBoard *board);

    private:
};

inline int Move::make_move(int move, int move_flag, ChessBoard *board) {
    ChessBoard boardCopy3 = *board;
    if (move_flag == all_moves) {
        //copyBoard(board);
        int sourceSquare = getMoveSource(move);
        int targetSquare = getMoveTarget(move);
        int piece = getMovePiece(move);
        int promoted = getMovePromoted(move);
        int capture = getMoveCapture(move);
        int doublePawn = getMoveDoublePawn(move);
        int enpassant = getMoveEnpassant(move);
        int castling = getMoveCastling(move);

        //handle piece movement
        board->bitBoards[piece].popBit(sourceSquare);
        board->bitBoards[piece].setBit(targetSquare);

        //handle captures
        if (getMoveCapture(move)) {
            //current move is capturing smthn
            int startPiece, endPiece;

            if (!board->side) {
                startPiece = p;
                endPiece = k;
            }
            //black move vv
            else {
                startPiece = P;
                endPiece = K;
            }

            for (int bbPiece = startPiece; bbPiece <= endPiece; bbPiece++) {
                if (board->bitBoards[bbPiece].getBit(targetSquare)) {
                    board->bitBoards[bbPiece].popBit(targetSquare);
                    break;
                }
            }
        }
        if (promoted) {
            //erase target pawn
            board->bitBoards[((!board->side) ? P : p)].popBit(targetSquare);
            board->bitBoards[promoted].setBit(targetSquare);
        }

        if (enpassant) {
            ///erase pawn depending on side
            (!board->side) ? board->bitBoards[p].popBit(targetSquare + 8) : board->bitBoards[P].popBit(targetSquare - 8);
        }
        board->enPassant = no_sq;
        //reset enpass

        //now set enpass after double pawn
        if (doublePawn) {
            (!board->side) ? board->enPassant = targetSquare + 8 : board->enPassant = targetSquare - 8;
        }
        if (castling) {
            switch(targetSquare)
            {
                case(g1):
                    board->bitBoards[R].popBit(h1);
                    board->bitBoards[R].setBit(f1);
                    break;
                case(c1):
                    board->bitBoards[R].popBit(a1);
                    board->bitBoards[R].setBit(d1);
                    break;
                case(g8):
                    board->bitBoards[r].popBit(h8);
                    board->bitBoards[r].setBit(f8);
                    break;
                case(c8):
                    board->bitBoards[r].popBit(a8);
                    board->bitBoards[r].setBit(d8);
                    break;
            }
        }
        board->castle &= rook.getCastleRights(sourceSquare);
        board->castle &= rook.getCastleRights(targetSquare);

        //update occupancies
        board->occupancies[white].bitBoard = 0ULL;
        board->occupancies[black].bitBoard = 0ULL;
        board->occupancies[both].bitBoard = 0ULL;

        for (int bbPiece = P; bbPiece <= K; bbPiece++) {
            //udate white occ
            board->occupancies[white].bitBoard |= board->bitBoards[bbPiece].bitBoard;
        }
        for (int bbPiece = p; bbPiece <= k; bbPiece++) {
            //udate black occpacy
            board->occupancies[black].bitBoard |= board->bitBoards[bbPiece].bitBoard;
        }
        board->occupancies[both].bitBoard |= (board->occupancies[white].bitBoard | board->occupancies[black].bitBoard);

        board->side ^= 1;
        
        if (isSquareUnderAttack(((!board->side) ? board->bitBoards[k].getLeastSigBit(board->bitBoards[k].bitBoard) : board->bitBoards[K].getLeastSigBit(board->bitBoards[K].bitBoard)), board->side, *board)) {
            *board = boardCopy3;
            //illegal move
            return 0;
        } else {
            //legal move
            return 1;
        }
    } 
    
    else {
        if (getMoveCapture(move)) {
            return make_move(move, all_moves, &(*board));
        } else {
            return 0;
        }
    }
   return 0;
}

inline void Move::addMoveToMoveList(int move, moveList *move_list) {
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

inline void Move::printMove(int move) {
    cout << constantVariables.getCords(getMoveSource(move)) << constantVariables.getCords(getMoveTarget(move)) << ((getMovePromoted(move)) ? (char)(((int)constantVariables.convertIntToAscii(getMovePromoted(move))) + 32) : ' ');
}

inline void Move::printMoveList(moveList *move_list) {
    if (!move_list->count) {
        cout << "\nNo Moves in move list\n";
        return;
    } 
    cout << "\nmove   piece capture double enpassant castling\n";
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //cout << "move   piece capture double enpassant castling\n";
        int move = move_list->moves[move_count];
        cout << constantVariables.getCords(getMoveSource(move)) << constantVariables.getCords(getMoveTarget(move)) << ((getMovePromoted(move)) ? (char)(((int)constantVariables.convertIntToAscii(getMovePromoted(move))) + 32) : ' ');
        cout << "  " << constantVariables.convertIntToAscii(getMovePiece(move));
        cout << "     " << (getMoveCapture(move) ? 1 : 0);
        cout << "       " << (getMoveDoublePawn(move) ? 1 : 0);
        cout << "      " << (getMoveEnpassant(move) ? 1 : 0);
        cout << "         " << (getMoveCastling(move) ? 1 : 0) << "\n";
    }
    cout << "\nTotal Number of Moves: " << move_list->count;
}

//encode move
inline int Move::encodeMove(int source, int target, int piece, int promoted, int capture, int doublePawn, int enpassant, int castling) {
    return (source | (target << 6) | (piece << 12) | (promoted << 16) |(capture << 20) | (doublePawn << 21) | (enpassant << 22) |
      (castling << 23));
}

inline void Move::generateAllMoves(ChessBoard board, moveList *move_list) {
    move_list->count = 0;
    int sourceSquare, targetSquare;

    //init cirrent pieces bitboard copy
    ChessBoard bitBoardCopy, attacks;
    for (int piece = P; piece <= k; piece++) {
        bitBoardCopy.mainBoard.bitBoard = board.bitBoards[piece].bitBoard;
        
        //generate white pawns and white king castling
        if (!(board.side)) {
            //pickup white bitboard ind
            if (piece == P) {
                while (bitBoardCopy.mainBoard.bitBoard) {
                    sourceSquare = bitBoardCopy.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                    //cout << "\nWhite Pawn: " << squareToCords[sourceSquare];

                    targetSquare = sourceSquare - 8;

                    if (!(targetSquare < a8) && !board.occupancies[both].getBit(targetSquare)) {
                        if (sourceSquare >= a7 && sourceSquare <= h7) {
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, Q, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, R, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, B, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, N, 0, 0, 0, 0), move_list);
                        } else {
                            //if not promotion, then it's one square ahead
                            //or two squares ahead
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                            if ((sourceSquare >= a2 && sourceSquare <= h2) && !board.occupancies[both].getBit(targetSquare - 8)) {
                                addMoveToMoveList(encodeMove(sourceSquare, targetSquare - 8, piece, 0, 0, 1, 0, 0), move_list);
                            }
                        }
                    }

                    attacks.mainBoard.bitBoard = (pawn.pieceAttack(sourceSquare, board.side, 0ULL) & board.occupancies[black].bitBoard);
                    //generate pawn captures
                    while (attacks.mainBoard.bitBoard) {
                        //init targ sqr
                        targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);
                        if (sourceSquare >= a7 && sourceSquare <= h7) {
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, Q, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, R, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, B, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, N, 1, 0, 0, 0), move_list);
                        } else {
                            //if not promotion, then it's one square ahead
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                        }
                        attacks.mainBoard.popBit(targetSquare);
                    }
                    if (board.enPassant != no_sq) {
                        //look at all pawn attacks, then and with enpassant squares
                        ChessBoard enpassantAttacksW;
                        enpassantAttacksW.mainBoard.bitBoard = (pawn.pieceAttack(sourceSquare, board.side, 0ULL) & (1ULL << board.enPassant));

                        //if enpassant capture is available
                        if (enpassantAttacksW.mainBoard.bitBoard) {
                            int targetEnpassant = enpassantAttacksW.mainBoard.getLeastSigBit(enpassantAttacksW.mainBoard.bitBoard);
                            addMoveToMoveList(encodeMove(sourceSquare, targetEnpassant, piece, 0, 1, 0, 1, 0), move_list);
                        }
                    }
                    //pop lsfb
                    bitBoardCopy.mainBoard.popBit(sourceSquare);
                }
            }
            if (piece == K) {
                if (board.castle & wk) {
                    //first check that suare between king and rook empty
                    if (!board.occupancies[both].getBit(f1) && !board.occupancies[both].getBit(g1)) {
                        //make sure their will be no checks or are any
                        if (!isSquareUnderAttack(e1, black, board) && !isSquareUnderAttack(f1, black, board)) {
                            addMoveToMoveList(encodeMove(e1, g1, piece, 0, 0, 0, 0, 1), move_list);
                        } 
                    }
                }
                if (board.castle & wq) {
                    //first check that suare between queen and rook empty
                    if (!board.occupancies[both].getBit(d1) && !board.occupancies[both].getBit(c1) && !board.occupancies[both].getBit(b1)) {
                        //make sure their will be no checks or are any
                        if (!isSquareUnderAttack(e1, black, board) && !isSquareUnderAttack(d1, black, board)) {
                            addMoveToMoveList(encodeMove(e1, c1, piece, 0, 0, 0, 0, 1), move_list);
                        } 
                    }
                }
            }
        }
        //generate black pawn and black king castling
        else {
            if (piece == p) {
                //cout << "\nPiece:" << piece;
                while (bitBoardCopy.mainBoard.bitBoard) {
                    sourceSquare = bitBoardCopy.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                    //cout << "\nBlack Pawn: " << squareToCords[sourceSquare];

                    targetSquare = sourceSquare + 8;

                    if (!(targetSquare > h1) && !board.occupancies[both].getBit(targetSquare)) {
                        if (sourceSquare >= a2 && sourceSquare <= h2) {
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, q, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, r, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, b, 0, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, n, 0, 0, 0, 0), move_list);
                        } else {
                            //if not promotion, then it's one square ahead
                            //or two squares ahead
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                            if ((sourceSquare >= a7 && sourceSquare <= h7) && !board.occupancies[both].getBit(targetSquare + 8)) {
                                addMoveToMoveList(encodeMove(sourceSquare, targetSquare + 8, piece, 0, 0, 1, 0, 0), move_list);
                            }
                        }
                    }

                    
                    attacks.mainBoard.bitBoard = (pawn.pieceAttack(sourceSquare, board.side, 0ULL) & board.occupancies[white].bitBoard);
                    //generate pawn captures
                    while (attacks.mainBoard.bitBoard) {
                        //init targ sqr
                        targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);
                        if (sourceSquare >= a2 && sourceSquare <= h2) {
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, q, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, r, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, b, 1, 0, 0, 0), move_list);
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, n, 1, 0, 0, 0), move_list);
                        } else {
                            //if not promotion, then it's one square ahead
                            //or two squares ahead
                            addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                        }
                        attacks.mainBoard.popBit(targetSquare);
                    }
                    if (board.enPassant != no_sq) {
                        //look at all pawn attacks, then and with enpassant squares
                        ChessBoard enpassantAttacksB;
                        enpassantAttacksB.mainBoard.bitBoard = (pawn.pieceAttack(sourceSquare, board.side, 0ULL) & (1ULL << board.enPassant));

                        //if enpassant capture is available
                        if (enpassantAttacksB.mainBoard.bitBoard) {
                            int targetEnpassant = enpassantAttacksB.mainBoard.getLeastSigBit(enpassantAttacksB.mainBoard.bitBoard);
                            addMoveToMoveList(encodeMove(sourceSquare, targetEnpassant, piece, 0, 1, 0, 1, 0), move_list);
                        }
                    }

                    //pop lsfb
                    bitBoardCopy.mainBoard.popBit(sourceSquare);
                }
            }
            if (piece == k) {
                if (board.castle & bk) {
                    //first check that suare between king and rook empty
                    if (!board.occupancies[both].getBit(f8) && !board.occupancies[both].getBit(g8)) {
                        //make sure their will be no checks or are any
                        if (!isSquareUnderAttack(e8, white, board) && !isSquareUnderAttack(f8, white, board)) {
                            addMoveToMoveList(encodeMove(e8, g8, piece, 0, 0, 0, 0, 1), move_list);
                        } 
                    }
                }
                if (board.castle & bq) {
                    //first check that suare between queen and rook empty
                    if (!board.occupancies[both].getBit(d8) && !board.occupancies[both].getBit(c8) && !board.occupancies[both].getBit(b8)) {
                        //make sure their will be no checks or are any
                        if (!isSquareUnderAttack(e8, white, board) && !isSquareUnderAttack(d8, white, board)) {
                            addMoveToMoveList(encodeMove(e8, c8, piece, 0, 0, 0, 0, 1), move_list);
                        } 
                    }
                }
            }
        }
        if ((!board.side) ? piece == N : piece == n) {
            while(bitBoardCopy.mainBoard.bitBoard) {
                sourceSquare = board.bitBoards[piece].getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                //init piece attacks
                attacks.mainBoard.bitBoard = knight.pieceAttack(sourceSquare, 0, 0ULL) & ((!board.side) ? ~board.occupancies[white].bitBoard : ~board.occupancies[black].bitBoard);

                while (attacks.mainBoard.bitBoard) {
                    targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);

                    if ((!board.side) ? (!board.occupancies[black].getBit(targetSquare)) : (!board.occupancies[white].getBit(targetSquare))) {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                    } else {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                    }


                    attacks.mainBoard.popBit(targetSquare);
                }

                bitBoardCopy.mainBoard.popBit(sourceSquare);
            }
        }

        if ((!board.side) ? piece == B : piece == b) {
            while(bitBoardCopy.mainBoard.bitBoard) {
                sourceSquare = board.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                //init piece attacks
                attacks.mainBoard.bitBoard = bishop.pieceAttack(sourceSquare, 0, board.occupancies[both].bitBoard) & ((!board.side) ? ~board.occupancies[white].bitBoard : ~board.occupancies[black].bitBoard);
                while (attacks.mainBoard.bitBoard) {
                    targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);

                    if ((!board.side) ? (!board.occupancies[black].getBit(targetSquare)) : (!board.occupancies[white].getBit(targetSquare))) {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                    } else {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                    }


                    attacks.mainBoard.popBit(targetSquare);
                }

                bitBoardCopy.mainBoard.popBit(sourceSquare);
            }
        }
        if ((!board.side) ? piece == R : piece == r) {
            while(bitBoardCopy.mainBoard.bitBoard) {
                sourceSquare = board.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                //init piece attacks
                attacks.mainBoard.bitBoard = rook.pieceAttack(sourceSquare, 0, board.occupancies[both].bitBoard) & ((!board.side) ? ~board.occupancies[white].bitBoard : ~board.occupancies[black].bitBoard);
                while (attacks.mainBoard.bitBoard) {
                    targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);

                    if ((!board.side) ? (!board.occupancies[black].getBit(targetSquare)) : (!board.occupancies[white].getBit(targetSquare))) {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                    } else {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                    }


                    attacks.mainBoard.popBit(targetSquare);
                }

                bitBoardCopy.mainBoard.popBit(sourceSquare);
            }
        }
        if ((!board.side) ? piece == Q : piece == q) {
            while(bitBoardCopy.mainBoard.bitBoard) {
                sourceSquare = board.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                //init piece attacks
                attacks.mainBoard.bitBoard = (rook.pieceAttack(sourceSquare, 0, board.occupancies[both].bitBoard) | bishop.pieceAttack(sourceSquare, 0, board.occupancies[both].bitBoard)) & ((!board.side) ? ~board.occupancies[white].bitBoard : ~board.occupancies[black].bitBoard);
                while (attacks.mainBoard.bitBoard) {
                    targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);

                    if ((!board.side) ? (!board.occupancies[black].getBit(targetSquare)) : (!board.occupancies[white].getBit(targetSquare))) {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                    } else {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                    }


                    attacks.mainBoard.popBit(targetSquare);
                }

                bitBoardCopy.mainBoard.popBit(sourceSquare);
            }
        }
        if ((!board.side) ? piece == K : piece == k) {
            while(bitBoardCopy.mainBoard.bitBoard) {
                sourceSquare = board.mainBoard.getLeastSigBit(bitBoardCopy.mainBoard.bitBoard);
                //init piece attacks
                attacks.mainBoard.bitBoard = king.pieceAttack(sourceSquare, 0, 0ULL) & ((!board.side) ? ~board.occupancies[white].bitBoard : ~board.occupancies[black].bitBoard);

                while (attacks.mainBoard.bitBoard) {
                    targetSquare = attacks.mainBoard.getLeastSigBit(attacks.mainBoard.bitBoard);

                    if ((!board.side) ? (!(board.occupancies[black].getBit(targetSquare))) : (!(board.occupancies[white].getBit(targetSquare)))) {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 0, 0, 0, 0), move_list);
                    } else {
                        addMoveToMoveList(encodeMove(sourceSquare, targetSquare, piece, 0, 1, 0, 0, 0), move_list);
                    }


                    attacks.mainBoard.popBit(targetSquare);
                }

                bitBoardCopy.mainBoard.popBit(sourceSquare);
            }
        }
    }
}

inline int Move::isSquareUnderAttack(int square, int side, ChessBoard board) {
    //attackeed by pawns, white or black
    if ((side == white) && (pawn.pieceAttack(square, black, 0ULL) & board.bitBoards[P].bitBoard)) {
        return 1;
    }
    if ((side == black) && (pawn.pieceAttack(square, white, 0ULL) & board.bitBoards[p].bitBoard)) {
        return 1;
    }
    if (knight.pieceAttack(square, 0, 0ULL) & ((!side) ? board.bitBoards[N].bitBoard : board.bitBoards[n].bitBoard)) {
        return 1;
    }
    //king attack
    if (king.pieceAttack(square, 0, 0ULL) & ((!side) ? board.bitBoards[K].bitBoard : board.bitBoards[k].bitBoard)) {
        return 1;
    }
    if (bishop.pieceAttack(square, 0, board.occupancies[both].bitBoard) & ((!side) ? board.bitBoards[B].bitBoard : board.bitBoards[b].bitBoard)) {
        return 1;
    }
    if (rook.pieceAttack(square, 0, board.occupancies[both].bitBoard) & ((!side) ? board.bitBoards[R].bitBoard : board.bitBoards[r].bitBoard)) {
        return 1;
    }
    if ((rook.pieceAttack(square, 0, board.occupancies[both].bitBoard) | bishop.pieceAttack(square, 0, board.occupancies[both].bitBoard)) & ((!side) ? board.bitBoards[Q].bitBoard : board.bitBoards[q].bitBoard)) {
        return 1;
        //queen.pieceAttack(square, 0, board.occupancies[both].bitBoard)
    }

    return 0;
}

inline void Move::printAttackedSquares(int side, ChessBoard board) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (!file) {
                cout << " " << 8 - rank;
            }
            //check whether current square is attacked
            cout << " " << isSquareUnderAttack(square, side, board);
        }
        cout << "\n";
    }
    cout << "   a b c d e f g h\n";
}

inline void Move::initAttacks() {
        bishop.initSliderAttacks();
        rook.initSliderAttacks();
}

#endif