#ifndef PIECE_H
#define PIECE_H

#include "ChessBoard.hpp"
#include <cstdint>
#define U64 unsigned long long

const U64 notAFile = 18374403900871474942ULL;
const U64 notHFile = 9187201950435737471ULL;
const U64 notHGFile = 4557430888798830399ULL;
const U64 notABFile = 18229723555195321596ULL;

//enum {rook, bishop};

class Piece
{
    public:
        virtual U64 pieceAttack(int square, int colour, U64 boardOccupancy) = 0;
        virtual void initSliderAttacks(){};
        U64 setOccupancy(int index, int bitsInMask, U64 attackMask);
        virtual int getPositionInTable(int square) {};

    private:
};

inline U64 Piece::setOccupancy(int index, int bitsInMask, U64 attackMask) {
    BitBoard occupancy;
    BitBoard spare;
    spare.bitBoard |= attackMask;
    for (int count = 0; count < bitsInMask; count++) {
        int square = spare.getLeastSigBit(spare.bitBoard);
        spare.popBit(square);
        if (index & (1 << count)) {
            occupancy.bitBoard |= (1ULL << square);
        }

    }
    return occupancy.bitBoard;
}

#endif