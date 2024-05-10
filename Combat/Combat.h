//
// Created by Victor Navarro on 19/02/24.
//

#ifndef RPG_COMBAT_H
#define RPG_COMBAT_H
#include <vector>
#include "../Character/Character.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include <queue>

class Combat
{
private:
    int level;

    vector<Character *> participants;
    vector<Player *> partyMembers;
    vector<Enemy *> enemies;
    priority_queue<Action> actionQueue;
    void registerActions(vector<Character *>::iterator participant);
    int executeActions(vector<Character *>::iterator participant);
    int checkParticipantStatus(Character *participant);

    void combatPrep();
    Character *getTarget(Character *attacker);

public:
    Combat(vector<Character *> _participants, int combatLevel);
    Combat(vector<Player *> _partyMembers, vector<Enemy *> _enemies);
    int doCombat();
    void addParticipant(Character *participant);
};

#endif // RPG_COMBAT_H
