#ifndef Rook_H
#define Rook_H

#include "Piece.hpp"

#define U64 unsigned long long

class Rook : public Piece
{
    public:
        U64 pieceAttack(int square, int, U64 occupancy) override {
            // get rook attacks assuming current board occupancy
            occupancy &= rookMasks[square];
            occupancy *= rookMagicNumber[square];
            occupancy >>= 64 - rookRelevantOccupancyCount[square];

            return rookAttacksTable[square][occupancy];
        }
        void initSliderAttacks() override;
        U64 rookAttackOnFly(int square, U64 block);
        int getCastleRights(int square) {return castleRights[square];};
        int getPositionInTable(int square) override {return rookPositionalTable[square];};
        U64 rookAttack(int square);

    private:
        /*const U64 rookAttacks[64] = {282578800148862, 565157600297596, 1130315200595066, 2260630401190006, 4521260802379886,
         9042521604759646, 18085043209519166, 36170086419038334, 282578800180736, 565157600328704, 1130315200625152,
          2260630401218048, 4521260802403840, 9042521604775424, 18085043209518592, 36170086419037696, 282578808340736,
           565157608292864, 1130315208328192, 2260630408398848, 4521260808540160, 9042521608822784, 18085043209388032,
            36170086418907136, 282580897300736, 565159647117824, 1130317180306432, 2260632246683648, 4521262379438080,
             9042522644946944, 18085043175964672, 36170086385483776, 283115671060736, 565681586307584, 1130822006735872,
              2261102847592448, 4521664529305600, 9042787892731904, 18085034619584512, 36170077829103616, 420017753620736,
               699298018886144, 1260057572672512, 2381576680245248, 4624614895390720, 9110691325681664, 18082844186263552,
                36167887395782656, 35466950888980736, 34905104758997504, 34344362452452352, 33222877839362048,
                 30979908613181440, 26493970160820224, 17522093256097792, 35607136465616896, 9079539427579068672,
                  8935706818303361536, 8792156787827803136, 8505056726876686336, 7930856604974452736, 6782456361169985536,
                   4485655873561051136, 9115426935197958144};*/
        U64 rookMasks[64];
        const int castleRights[64] = {
        7, 15, 15, 15, 3, 15, 15, 11,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        13, 15, 15, 15, 12, 15, 15, 14
};
        U64 rookAttacksTable[64][4096];
        const int rookRelevantOccupancyCount[64] = {12, 11, 11, 11, 11, 11, 11, 12, 
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            11, 10, 10, 10, 10, 10, 10, 11,
                                            12, 11, 11, 11, 11, 11, 11, 12};
        const U64 rookMagicNumber[64] = {0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL,
         0x200020010080420ULL, 0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,
          0x2024401000200040ULL, 0x100802000801000ULL, 0x120800800801000ULL, 0x208808088000400ULL, 0x2802200800400ULL,
           0x2200800100020080ULL, 0x801000060821100ULL, 0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL,
            0x140848010000802ULL, 0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
             0x100400080208000ULL, 0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL, 0x2000a00200410ULL,
              0x20080800400ULL, 0x80088400100102ULL, 0x80004600042881ULL, 0x4040008040800020ULL, 0x440003000200801ULL,
               0x4200011004500ULL, 0x188020010100100ULL, 0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL,
                0x200046502000484ULL, 0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL,
                 0x2002080100110004ULL, 0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL, 0x101002200408200ULL, 
                 0x40802000401080ULL, 0x4008142004410100ULL, 0x2060820c0120200ULL, 0x1001004080100ULL, 0x20c020080040080ULL,
                  0x2935610830022400ULL, 0x44440041009200ULL, 0x280001040802101ULL, 0x2100190040002085ULL,
         0x80c0084100102001ULL, 0x4024081001000421ULL, 0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL};
         const int rookPositionalTable[64] = {
            50, 50, 50, 50, 50, 50, 50, 50,
            50, 50, 50, 50, 50, 50, 50, 50,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 10, 20, 20, 10, 0, 0,
            0, 0, 0, 20, 20, 0, 0, 0,
        };
};

inline void Rook::initSliderAttacks() {
    for (int i = 0; i < 64; i++) {
        rookMasks[i] = rookAttack(i);
        //init attack mask
        BitBoard boardMask;
        U64 attackMask = rookMasks[i];
        int relevantBitsCount = boardMask.countBits(attackMask);
        //init occupancy ind
        int occupancyIndicies = (1 << relevantBitsCount);
        for (int index = 0; index < occupancyIndicies; index++) {
            U64 occupancy = setOccupancy(index, relevantBitsCount, attackMask);

            int magic_index = (occupancy * rookMagicNumber[i]) >> (64 - rookRelevantOccupancyCount[i]);

            rookAttacksTable[i][magic_index] = rookAttackOnFly(i, occupancy);
        }
    }
}

inline U64 Rook::rookAttackOnFly(int square, U64 block) {
    U64 attacks = 0ULL;
    int r,f;
    int tr = square / 8;
    int tf = square % 8;
    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) {break;}
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r * 8 + tf));
        if ((1ULL << (r * 8 + tf)) & block) {break;}
    }
    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) {break;}
    }
    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr * 8 + f));
        if ((1ULL << (tr * 8 + f)) & block) {break;}
    }
    
    return attacks;
}

inline U64 Rook::rookAttack(int square) {
    U64 attacks = 0ULL;
    int r,f;
    int tr = square / 8;
    int tf = square % 8;
    for (r = tr + 1; r <= 6; r++) {
        attacks |= (1ULL << (r * 8 + tf));
    }
    for (r = tr - 1; r >= 1; r--) {
        attacks |= (1ULL << (r * 8 + tf));
    }
    for (f = tf + 1; f <= 6; f++) {
        attacks |= (1ULL << (tr * 8 + f));
    }
    for (f = tf - 1; f >= 1; f--) {
        attacks |= (1ULL << (tr * 8 + f));
    }
    

    return attacks;
}

#endif