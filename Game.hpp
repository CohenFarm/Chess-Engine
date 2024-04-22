#ifndef GAME_H
#define GAME_H

#include "Engine.hpp"
#include <stdlib.h>
// #include <cstring>
#include <chrono>

Engine mainEngine;

class Game {
    public:
        void MainLoop();
        void setUp();
        char StartPos[64] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        char HardPos[71] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
        char AnotherPos[80] = "8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1";
        char *x = StartPos;
        ChessBoard gameBoard;
        Move makeMove;
        void parseMove(char *moveString);
        void parsePosition(char* command);
        void parseGo(char* command);
        void mainUCILoop();
        int notCheckMate = 1;
        void checkCheckMate();
        void randomMove();
        void EngineVsEngine(int depth);
};
inline void Game::randomMove() {
    moveList move_list[1];
    makeMove.generateAllMoves(gameBoard, move_list);
    int moveAvailable = move_list->count;
    int move = rand() % moveAvailable;
    if (makeMove.make_move(move_list->moves[move], all_moves, &gameBoard) == 0) {
        checkCheckMate();
        if (notCheckMate) {
        int i = 0;
        while (makeMove.make_move(move_list->moves[i], all_moves, &gameBoard) == 0) {
            i += 1;
            checkCheckMate();
        }
    }
    } 
    if (notCheckMate) {
        gameBoard.printBoard();
        cout << "\nEvaulation: " << mainEngine.evaluation(gameBoard) << "\n";
    }
    cout << "\n";
}

inline void Game::mainUCILoop() {
    //reset stdin and stdout buffer
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    //define user / GUI input buffer
    char input[2000];

    cout << "id name BBC\n";
    cout << "id name Cohen Farmer\n";
    cout << "uciok\n";

    //main UCI loop
    while (notCheckMate) {
        //reset user / GUI input
        memset(input, 0, sizeof(input));

        fflush(stdout);

        //get user / gui input
        if ((!fgets(input, 2000, stdin))) {
            continue;
        }
        if (input[0] == '\n') {
            continue;
        }

        if (strncmp(input, "isready", 7) == 0) {
            cout << "readyok\n";
            continue;
        }

        else if (strncmp(input, "position", 8) == 0) {
            parsePosition(input);
        }

        //parse uci new game
        else if (strncmp(input, "ucinewgame", 10) == 0) {
            char posStart[20] = "position startpos";
            parsePosition(posStart);
        }

        else if (strncmp(input, "go", 2) == 0) {
            parseGo(input);
        }

        else if (strncmp(input, "quit", 4) == 0) {
            break;
        } 
        
        else if (strncmp(input, "move", 4) == 0) {
            char *currentPoint = NULL;
            currentPoint = input;
            currentPoint += 5;
            int sideBeforeMove = gameBoard.side;
            parseMove(currentPoint);
            gameBoard.printBoard();
            cout << "\nEvaluation: " << mainEngine.evaluation(gameBoard) << "\n";
            cout << "\n";
            if (gameBoard.side != sideBeforeMove) {
                mainEngine.searchPosition(4, gameBoard);
                makeMove.make_move(mainEngine.bestMove, all_moves, &gameBoard);
                gameBoard.printBoard();
                cout << "\nEvaluation: " << mainEngine.evaluation(gameBoard) << "\n";
                cout << "\n";
            }
        }

        else if (strncmp(input, "uci", 3)== 0) {
            cout << "id name BBC\n";
            cout << "id name Cohen Farmer\n";
            cout << "uciok\n";
        }
    }
}

inline void Game::parseGo(char *command) {
    int depth = -1;

    char *currentDepth = NULL;

    if (currentDepth = strstr(command, "depth")) {
        depth = atoi(currentDepth + 6);

        cout << "Depth: " << depth << "\n";
        
    } else {
        cout << "Depth: " << 6 << "\n";
    }
    //makeMove.searchPosition(depth, gameBoard);
    gameBoard.printBoard();

}

inline void Game::parsePosition(char *command) {
    command += 9;
    //cout << command;
    char *currentChar = command;

    if (strncmp(command, "startpos", 8) == 0) {
        gameBoard.parseFEN(StartPos);
    } else {
        currentChar = strstr(command, "fen");
        if (currentChar == NULL) {
            gameBoard.parseFEN(StartPos);
        }
        //else case found FEN so use it
        else {
            //shift point right
            currentChar += 4;
            gameBoard.parseFEN(currentChar);
        }
    }
    //parse moves now
    currentChar = strstr(command, "moves");
    if (currentChar != NULL) {
        currentChar += 6;
        while (*currentChar) {
            //int move = 
            parseMove(currentChar);
            /*if (move == 0) {
                break;
            }*/
            //makeMove.make_move(move, all_moves, &gameBoard);
            while (*currentChar && *currentChar != ' ') {
                currentChar++;
            }
            currentChar++;

        }
    }
    gameBoard.printBoard();

}

inline void Game::parseMove(char *moveString) {
    moveList move_list[1];
    int moveToMake = 0;
    makeMove.generateAllMoves(gameBoard, move_list);
    //parse source sqr
    int sourceSquare = moveString[0] - 'a' + ((8 - (moveString[1] - '0')) * 8);
    //cout << squareToCords[sourceSquare];
    int targetSquare = moveString[2] - 'a' + ((8 - (moveString[3] - '0')) * 8);
    //cout << squareToCords[targetSquare];

    for (int move = 0; move < move_list->count; move++) {
        int currMove = move_list->moves[move];
        if (sourceSquare == makeMove.getMoveSource(currMove) && targetSquare == makeMove.getMoveTarget(currMove)) {
            //define promoted piece
            int promoted_piece = makeMove.getMovePromoted(currMove);
            if (promoted_piece) {
                if ((promoted_piece == Q || promoted_piece == q) && moveString[4] == 'q') {
                    //return currMove;
                    moveToMake = currMove;

                } else if ((promoted_piece == R || promoted_piece == r) && moveString[4] == 'r') {
                    //return currMove;
                    moveToMake = currMove;

                } else if ((promoted_piece == N || promoted_piece == n) && moveString[4] == 'n') {
                    //return currMove;
                    moveToMake = currMove;

                } else if ((promoted_piece == B || promoted_piece == b) && moveString[4] == 'b') {
                    //return currMove;
                    moveToMake = currMove;
                }
                continue;
            }
            moveToMake = currMove;
            //return 1;
        }
    }
    if (moveToMake == 0) {
        cout << "Invalid\n";
    } else {
        makeMove.make_move(moveToMake, all_moves, &gameBoard);
    }
    //makeMove.make_move(moveToMake, all_moves, &gameBoard);
    //return 0;
}

inline void Game::setUp() {
    makeMove.initAttacks();
}

inline void Game::EngineVsEngine(int depth) {
    gameBoard.parseFEN(StartPos);
    int let = 1;
    int count = 0;
    int previousEval, currentEval;
    while (let) {
        currentEval = mainEngine.searchPosition(depth, gameBoard);
        mainEngine.scoreMoves(mainEngine.bestMove, gameBoard);
        makeMove.make_move(mainEngine.bestMove, all_moves, &gameBoard);
        gameBoard.printBoard();
        cout << "\nEvaluation: " << currentEval << "\n";
        count++;
        if (count == 10) {
            let = 0;
        }
    }
}

inline void Game::MainLoop() {
    srand(time(NULL));
    cout << "Enter ucinewgame, then it will setup a board and to move use the instructions; 'move e2e4' without the ''\n";
    cout << "If the move is invalid you will just need to enter a new move. For promotions put the piece you want to promote to after the directions\n";
    cout << "So 'move e7e8q' would be valid\n"; 
    mainUCILoop();
}

#endif