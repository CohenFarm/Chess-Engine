
#include <cstdint>
#include <iostream>

#define U64 unsigned long long

using namespace std;

class BitBoard {
    public:
        U64 bitBoard = 0ULL;
        static inline int countBits(U64 board) {return __popcount(board);/*use c++ built in pop count*/};
        static inline int getLeastSigBit(U64 board);
        void setBit(int square) {bitBoard |= (1ULL << square);};
        inline U64 getBit(int square) {return (bitBoard & (1ULL << square));};
        inline void popBit(int square) {getBit(square) ? bitBoard ^= (1ULL << square) : 0;};
        void printBitBoard();
};

inline int BitBoard::getLeastSigBit(U64 board) {
    if (board) {
        return countBits((board & -board) - 1);
    } else {
        return -1;
    }
}

inline void BitBoard::printBitBoard() {
    cout << "\n   A B C D E F G H \n\n";
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            //converts to square inde
            int square = rank * 8 + file;
            if (!file) {
                cout << 8 - rank << "  ";
            }
            //print whether bit is in that square or not
            cout << (bitBoard & (1ULL << square) ? 1 : 0) << " ";
            if (file == 7) {
                cout << " "<< 8 - rank;
            }
        }
        cout << "\n";
    }
    cout << "\n   A B C D E F G H \n";
    cout << "  " << bitBoard << "\n";
}