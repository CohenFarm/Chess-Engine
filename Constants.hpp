#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string.h>

enum {wk = 1, wq = 2, bk = 4, bq = 8};

enum {white, black, both};

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
    };

enum {P = 0, N = 1, B = 2, R = 3, Q = 4, K = 5, p = 6, n = 7, b = 8, r = 9, q = 10, k = 11};

class Constants
{
    private:
        const char ascii_Pieces[13] = "PNBRQKpnbrqk";
        const char *squareToCords[64] = {"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};

        const int materialsScore[12] = {
        100,
        300,
        350, 
        500,
        1000,
        10000,
        -100,
        -300,
        -350,
        -500,
        -1000,
        -10000
        };

        const int mirrorScore[64] = {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
        };

        const int mvv_lva[12][12] = {
 	    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

	    105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	    104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	    103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	    102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	    101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	    100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600 };

    public:
        char convertIntToAscii(int index) {return ascii_Pieces[index];};
        int mvvlvaValue(int piece1, int piece2) {return mvv_lva[piece1][piece2];};
        int getMirroredBoardPlace(int sourceSquare) {return mirrorScore[sourceSquare];};
        int getMaterialScore(int piece) {return materialsScore[piece];};
        char* getCords(int square) {
            char *ch = new char[3];
            strcpy(ch, squareToCords[square]);
            return ch;
            };

};

#endif
