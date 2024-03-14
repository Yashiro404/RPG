//
// Created by Victor Navarro on 15/02/24.
//

#include "Enemy.h"
#include "../Utils.h"
#include <iostream>
#include <map>
#include <any>

using namespace std;
using namespace combat_utils;

Enemy::Enemy(string _name, int _health, int _attack, int _defense, int _speed, int _experience) : Character(_name, _health, _attack, _defense, _speed, false, 0)
{
    experience = _experience;
}

void Enemy::doAttack(Character *target)
{
    target->takeDamage(getRolledAttack(attack));
}

void Enemy::takeDamage(int damage)
{
    int trueDamage = damage - defense;
    health -= trueDamage;

    cout << name << " took " << trueDamage << " damage!" << endl;

    if (health <= 0)
    {
        cout << name << " has been defeated!" << endl;
    }
}

int Enemy::getExperience()
{
    return experience;
}

Character *Enemy::selectTarget(vector<Player *> possibleTargets)
{
    int lessHealth = 9999999;
    Character *target = nullptr;
    map<string, any> enemyData = getData();

    for (auto character : possibleTargets)
    {
        if (any_cast<int>(enemyData["speed"]) < lessHealth)
        {
            lessHealth = any_cast<int>(enemyData["speed"]);
            target = character;
        }
    }
    return target;
}

Action Enemy::takeAction(vector<Player *> partyMembers)
{
    Action currentAction;
    map<string, any> enemyData = getData();
    currentAction.speed = any_cast<int>(enemyData["speed"]);

    if ((any_cast<int>(enemyData["health"]) < (0.15 * any_cast<int>(enemyData["health"]))))
    {
        if (rand() % 10 <= 3)
        {
            if (!canDefend())
            {
                Character *target = selectTarget(partyMembers);
                currentAction.target = target;
                currentAction.action = [this, target]()
                {
                    doAttack(target);
                };
            }
            else
            {
                cout << (any_cast<string>(enemyData["name"])) << " ha elegido defender." << endl;
                currentAction.target = nullptr;
                currentAction.action = [this]()
                {
                    defend();
                };
            }
        }
    }
    else
    {
        Character *target = selectTarget(partyMembers);
        currentAction.target = target;
        currentAction.action = [this, target]()
        {
            doAttack(target);
        };
    }

    return currentAction;
}