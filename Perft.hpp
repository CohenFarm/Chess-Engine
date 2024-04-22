


/*inline void Move::perft_test(int depth, ChessBoard board) {
    ChessBoard boardCopy;
    moveList move_list[1];
    generateAllMoves(board, move_list);
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //int move = move_list->moves[move_count];
        boardCopy = board;

        if (!(make_move(move_list->moves[move_count], all_moves, &board))) {
            //skip to the next movee
            continue;
        }

        long cumulativeNodes = nodes;
        perft_driver(depth - 1, board);
        long oldNodes = nodes - cumulativeNodes;
        board = boardCopy;

        cout << "Move: ";
        printMove(move_list->moves[move_count]);
        cout << "  Nodes: " << oldNodes << "\n";
    }
    cout << "\nDepth: " << depth;
    cout << "\nNodes: " << nodes;
    cout << "\n";
    //board.printBoard();
}

inline void Move::perft_driver(int depth, ChessBoard board) {
    ChessBoard boardCopy2;
    //recursion base case
    if (depth == 0) {
        nodes++;
        return;
    }
    moveList move_list[1];
    generateAllMoves(board, move_list);
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        //int move = move_list->moves[move_count];
        boardCopy2 = board;
        if (!(make_move(move_list->moves[move_count], all_moves, &board))) {
            //cout << "Skipped";
            continue;
        }
        //board.printBoard();
        perft_driver(depth - 1, board);
        board = boardCopy2;
    }
}*/