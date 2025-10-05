#include "uno.h"
#include <iostream>

using namespace std;

int main() {
    auto game = UNOGame::create(2);
    game->initialize();  // Use fixed seed 1234
    cout << game->getState() << endl;
    game->playTurn();
    cout << game->getState() << endl;
    game->playTurn();
    cout << game->getState() << endl;
    game->playTurn();
    cout << game->getState() << endl;
    
    return 0;
}