#include <iostream>

using namespace std;

#include "Game.hpp"
#include <stdlib.h>

int main() {
    Game playGame;
    playGame.setUp();
    playGame.MainLoop();
}

