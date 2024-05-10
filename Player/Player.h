//
// Created by Victor Navarro on 15/02/24.
//

#pragma once
#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H
#include "../Character/Character.h"
#include "../Enemy/Enemy.h"
#include "../Combat/Action.h"
#include <vector>

class Enemy;

class Player : public Character
{
private:
    int level;
    int experience;
    int requiredExp;

public:
    Player(string _name, int _health, int _attack, int _defense, int _speed);
    void doAttack(Character *target) override;
    void takeDamage(int damage) override;
    Character *selectTarget(vector<Enemy *> possibleTargets);
    Action takeAction(vector<Enemy *> enemies);

    void buffPlayer(int exp);
};

#endif // RPG_PLAYER_H
