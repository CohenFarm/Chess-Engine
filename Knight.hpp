#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.hpp"

#define U64 unsigned long long

class Knight : public Piece
{
    public:
        U64 pieceAttack(int square, int, U64) override;
        int getPositionInTable(int square) override {return knightPositionalTable[square];};


    private:
        U64 knightAttacks[64] = {132096, 329728, 659712, 1319424, 2638848, 5277696, 10489856, 4202496, 33816580, 84410376,
         168886289, 337772578, 675545156, 1351090312, 2685403152, 1075839008, 8657044482, 21609056261, 43234889994,
          86469779988, 172939559976, 345879119952, 687463207072, 275414786112, 2216203387392, 5531918402816, 11068131838464,
           22136263676928, 44272527353856, 88545054707712, 175990581010432, 70506185244672, 567348067172352, 1416171111120896,
            2833441750646784, 5666883501293568, 11333767002587136, 22667534005174272, 45053588738670592, 18049583422636032,
             145241105196122112, 362539804446949376, 725361088165576704, 1450722176331153408, 2901444352662306816,
              5802888705324613632, 11533718717099671552, 4620693356194824192, 288234782788157440, 576469569871282176,
               1224997833292120064, 2449995666584240128, 4899991333168480256, 9799982666336960512, 1152939783987658752,
                2305878468463689728, 1128098930098176, 2257297371824128, 4796069720358912, 9592139440717824,
                 19184278881435648, 38368557762871296, 4679521487814656, 9077567998918656};
        const int knightPositionalTable[64] = {
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, 0, 0, 10, 10, 0, 0, -5,
            -5, 5, 20, 20, 20, 20, 5, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 10, 20, 30, 30, 20, 10, -5,
            -5, 5, 20, 10, 10, 20, 5, -5,
            -5, 0, 0, 0, 0, 0, 0, -5,
            -5, -10, 0, 0, 0, 0, -10, -5,
        };

    
};

inline U64 Knight::pieceAttack(int square, int, U64) {
    U64 attacks = 0ULL;
    U64 bitBoard = 0ULL;
    bitBoard |= (1ULL << square);

    if ((bitBoard >> 17) & notHFile) {
        attacks |= (bitBoard >> 17);
    } if ((bitBoard >> 15) & notAFile) {
        attacks |= (bitBoard >> 15);
    } if ((bitBoard >> 6) & notABFile) {
        attacks |= (bitBoard >> 6);
    } if ((bitBoard >> 10) & notHGFile) {
        attacks |= (bitBoard >> 10);
    } if ((bitBoard << 17) & notAFile) {
        attacks |= (bitBoard << 17);
    } if ((bitBoard << 15) & notHFile) {
        attacks |= (bitBoard << 15);
    } if ((bitBoard << 6) & notHGFile) {
        attacks |= (bitBoard << 6);
    } if ((bitBoard << 10) & notABFile) {
        attacks |= (bitBoard << 10);
    }

    return attacks;
}

#endif