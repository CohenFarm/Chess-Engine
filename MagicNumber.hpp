#ifndef MAGICNUMBER_H
#define MAGICNUMBER_H

#define U64 unsigned long long

#include "Move.hpp"
#include <cstring>

Move moveMake;

class MagicNumber
{
    public:
        unsigned int getRandomNumU32();
        U64 genMagicNumCand();
        U64 getU64RandNum();
        unsigned int randomState = 1804289383;
        U64 findMagicNum(int square, int relBits, int bishop);
};

inline U64 MagicNumber::findMagicNum(int square, int relBits, int bishop) {

    //init occupancies
    U64 occuppancies[4096];

    U64 attacks[4096];

    U64 usedAttacks[4096];

    U64 attackMask = bishop ? moveMake.bishopAttack(square) : moveMake.rookAttack(square);

    int occupancyIndicies = 1 << relBits;

    for (int index = 0; index < occupancyIndicies; index++) {
        occuppancies[index] = moveMake.setOccupancy(index, relBits, attackMask);

        attacks[index] = bishop ? moveMake.bishopAttackOnFly(square, occuppancies[index]) : moveMake.rookAttackOnFly(square, occuppancies[index]);
    }

    for (int randomCount = 0; randomCount < 100000000; randomCount++) {
        U64 magicNum = genMagicNumCand();
        BitBoard magicNumb;

        if (magicNumb.countBits((attackMask * magicNum) & 0xFF00000000000000) < 6) continue;

        memset(usedAttacks, 0ULL, sizeof(usedAttacks));

        int index, fail;

        //test magic index
        for (index = 0, fail = 0; !fail && index < occupancyIndicies; index++) {
            //init magic index

            int magicIndex = (int)((occuppancies[index] * magicNum) >> (64 - relBits));

            if (usedAttacks[magicIndex] == 0ULL) {
                usedAttacks[magicIndex] = attacks[index];
            } else if (usedAttacks[magicIndex] != attacks[index]) {
                fail = 1;
            } 

        }
        if (!fail) {
            return magicNum;
        }
    }
    cout << "Magic num fails\n";
    return 0ULL;
}

inline U64 MagicNumber::genMagicNumCand() {
    return getU64RandNum() & getU64RandNum() & getU64RandNum();
}

inline U64 MagicNumber::getU64RandNum() {
    U64 n1, n2, n3, n4;
    n1 = (U64)(getRandomNumU32()) & 0xFFFF;
    n2= (U64)(getRandomNumU32()) & 0xFFFF;
    n3 = (U64)(getRandomNumU32()) & 0xFFFF;
    n4 = (U64)(getRandomNumU32()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
    }

inline unsigned int MagicNumber::getRandomNumU32() {
    unsigned int num1 = randomState;

    num1 ^= num1 << 13;
    num1 ^= num1 >> 17;
    num1 ^= num1 << 5;

    randomState = num1;

    return num1;
}

#endif