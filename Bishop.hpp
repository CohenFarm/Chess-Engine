#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.hpp"

#define U64 unsigned long long


class Bishop : public Piece
{
    public:
        U64 pieceAttack(int square, int, U64 occupancy) override {
            // get bishop attacks assuming current board occupancy
            occupancy &= bishopMasks[square];
            occupancy *= bishopMagicNumber[square];
            occupancy >>= 64 - bishopRelevantOccupancyCount[square];
            //cout << bishopPiece.bishopAttacksTable[square][occupancy];
            return bishopAttacksTable[square][occupancy];
        }
        void initSliderAttacks() override;
        U64 bishopAttackOnFly(int square, U64 block);
        int getPositionInTable(int square) override {return bishopPositionalTable[square];};
        U64 bishopAttack(int square);

    private:
        /*const U64 bishopAttacks[64] = {770, 1797, 3594, 7188, 14376, 28752, 57504, 49216, 197123, 460039, 920078, 1840156, 3680312,
         7360624, 14721248, 12599488, 50463488, 117769984, 235539968, 471079936, 942159872, 1884319744, 3768639488, 3225468928,
          12918652928, 30149115904, 60298231808, 120596463616, 241192927232, 482385854464, 964771708928, 825720045568,
           3307175149568, 7718173671424, 15436347342848, 30872694685696, 61745389371392, 123490778742784, 246981557485568,
            211384331665408, 846636838289408, 1975852459884544, 3951704919769088, 7903409839538176, 15806819679076352,
            31613639358152704, 63227278716305408, 54114388906344448, 216739030602088448, 505818229730443264,
             1011636459460886528, 2023272918921773056, 4046545837843546112, 8093091675687092224, 16186183351374184448,
              13853283560024178688, 144959613005987840, 362258295026614272, 724516590053228544, 1449033180106457088,
               2898066360212914176, 5796132720425828352, 11592265440851656704, 4665729213955833856};*/
        U64 bishopMasks[64];
        U64 bishopAttacksTable[64][512];
        const int bishopRelevantOccupancyCount[64] = {6, 5, 5, 5, 5, 5, 5, 6, 
                                                5, 5, 5, 5, 5, 5, 5, 5,
                                                5, 5, 7, 7, 7, 7, 5, 5,
                                                5, 5, 7, 9, 9, 7, 5, 5,
                                                5, 5, 7, 9, 9, 7, 5, 5,
                                                5, 5, 7, 7, 7, 7, 5, 5,
                                                5, 5, 5, 5, 5, 5, 5, 5,
                                                6, 5, 5, 5, 5, 5, 5, 6}; 
        const U64 bishopMagicNumber[64] = {0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL,
         0x581104180800210ULL, 0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL, 0x4050404440404ULL, 
         0x21001420088ULL, 0x24d0080801082102ULL, 0x1020a0a020400ULL, 0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL,
          0x801010402020200ULL, 0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL,
           0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL, 0x220200865090201ULL,
            0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL, 0x4001001021004000ULL, 0x800040400a011002ULL,
             0xe4004081011002ULL, 0x1c004001012080ULL, 0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL,
              0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
               0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL, 0x90080104000041ULL,
                0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL, 0x180806108200800ULL,
                 0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL,
                  0x4010801011c04ULL, 0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL,
         0x8040002811040900ULL, 0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL};
        const int bishopPositionalTable[64] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 10, 10, 0, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 10, 0, 0, 0, 0, 10, 0,
            0, 30, 0, 0, 0, 0, 30, 0,
            0, 0, -10, 0, 0, -10, 0, 0,
        };
};

inline void Bishop::initSliderAttacks() {
     for (int i = 0; i < 64; i++) {
        bishopMasks[i] = bishopAttack(i);

        //init attack mask
        BitBoard boardMask;
        U64 attackMask = bishopMasks[i];
        int relevantBitsCount = boardMask.countBits(attackMask);
        //init occupancy ind
        int occupancyIndicies = (1 << relevantBitsCount);
        for (int index = 0; index < occupancyIndicies; index++) {
            U64 occupancy = setOccupancy(index, relevantBitsCount, attackMask);

            int magic_index = (occupancy * bishopMagicNumber[i]) >> (64 - bishopRelevantOccupancyCount[i]);
            bishopAttacksTable[i][magic_index] = bishopAttackOnFly(i, occupancy);
        }
    }
}

inline U64 Bishop::bishopAttackOnFly(int square, U64 block) {
    U64 attacks = 0ULL;
    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r * 8 + f)); 
        if ((1ULL << (r * 8 + f)) & block) {break;}
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r * 8 + f)); 
        if ((1ULL << (r * 8 + f)) & block) {break;}
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r * 8 + f)); 
        if ((1ULL << (r * 8 + f)) & block) {break;}
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r * 8 + f)); 
        if ((1ULL << (r * 8 + f)) & block) {break;}
    }

    return attacks;
}

inline U64 Bishop::bishopAttack(int square) {
    U64 attacks = 0ULL;
    int r,f;

    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) {
        attacks |= (1ULL << (r * 8 + f)); 
    }
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) {
        attacks |= (1ULL << (r * 8 + f)); 
    }
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) {
        attacks |= (1ULL << (r * 8 + f)); 
    }
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) {
        attacks |= (1ULL << (r * 8 + f)); 
    }

    return attacks;
}
#endif