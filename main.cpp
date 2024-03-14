#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"

int main() {
    Player *player = new Player("Goku", 60, 10, 4, 6);
    Enemy *enemy = new Enemy("Freezer", 50, 6, 2, 5, 5);
    Enemy *enemy2 = new Enemy("Cell", 50, 6, 2, 5, 5);

    vector<Character*> participants;

    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat *combat = new Combat(participants);
    combat->doCombat();

    delete player;
    delete enemy;
    delete combat;
    return 0;
}
