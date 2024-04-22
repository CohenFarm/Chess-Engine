#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "BitBoard.hpp"
#include "Constants.hpp"

#include <cstring>

Constants constantVariables;

class ChessBoard : public BitBoard
{
    public:
        BitBoard mainBoard;
        BitBoard bitBoards[12];
        BitBoard occupancies[3];
        void printBoard();
        int side = 0;
        int enPassant = no_sq;
        int castle;
        void parseFEN(char *fen);
        int char_pieces(char f);
    private:
};

inline void ChessBoard::parseFEN(char *fen) {
    //reset bitboards
    memset(bitBoards, 0ULL, sizeof(bitBoards));
    //reset occupancies
    memset(occupancies, 0ULL, sizeof(occupancies));
    //reset side
    side = 0;
    enPassant = no_sq;
    castle = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                int piece = char_pieces(*fen);
                bitBoards[piece].setBit(square);
                fen++;
            }
            if (*fen >= '0' && *fen <= '9') {
                //convert char to int, using - 0
                int offset = *fen - '0';
                int piece = -1;

                for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                    if (bitBoards[bb_piece].getBit(square)) {
                        piece = bb_piece;
                    }
                }
                if (piece == -1) {
                    file--;
                }

                file += offset;
                //rank += offset % 8;
                fen++;
            }
            if (*fen == '/') {
                fen++;
            }
        }
    }
    fen++;

    (*fen == 'w') ? (side = white) : (side = black);

    fen += 2;

    while (*fen != ' ') {

        switch(*fen) {
            case 'K': castle |= wk;
                break;
            case 'Q': castle |= wq;
                break;
            case 'k': castle |= bk;
                break;
            case 'q': castle |= bq;
                break;
            case '-':
                break;
        }

        *fen++;
    }
    *fen++;

    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enPassant = rank * 8 + file;
    }
    //no enpass
    else {enPassant = no_sq;};

    //init white occp
    for (int piece = P; piece <= K; piece++) {
        occupancies[white].bitBoard |= bitBoards[piece].bitBoard;
    }
    for (int piece = p; piece <= k; piece++) {
        occupancies[black].bitBoard |= bitBoards[piece].bitBoard;
    }
    occupancies[both].bitBoard |= (occupancies[black].bitBoard | occupancies[white].bitBoard);
}

inline int ChessBoard::char_pieces(char f) {
    int val;
    if (f == 'P') {
        val = 0;
    } else if (f == 'N') {
        val = 1;
    } else if (f == 'B') {
        val = 2;
    } else if (f == 'R') {
        val = 3;
    } else if (f == 'Q') {
        val = 4;
    } else if (f == 'K') {
        val = 5;
    } else if (f == 'p') {
        val = 6;
    } else if (f == 'n') {
        val = 7;
    } else if (f == 'b') {
        val = 8;
    } else if (f == 'r') {
        val = 9;
    } else if (f == 'q') {
        val = 10;
    } else if (f == 'k') {
        val = 11;
    }
    return val;
}

inline void ChessBoard::printBoard() {
    for  (int rank = 0; rank < 8; rank++) {
        cout << 8 - rank << "  "; 
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int piece = -1;
            for (int bb_count = P; bb_count <= k; bb_count++) {
                if (bitBoards[bb_count].getBit(square)) {
                    piece = bb_count;
                }
            }
            if (piece == -1) {
                cout << ". ";
            } else {
                cout << constantVariables.convertIntToAscii(piece) << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n   a b c d e f g h\n";
    cout << "\n   Side to Move: " << (!side ? "White" : "Black");
    cout << "\n   Enpassant: " << ((enPassant != no_sq) ? constantVariables.getCords(enPassant) : "No"); 
    cout << "\n   Castling: " << ((castle & wk) ? 'K' : '-') << ((castle & wq) ? 'Q' : '-') << ((castle & bk) ? 'k' : '-') << ((castle & bq) ? 'q' : '-') << "\n\n";

}
#endif