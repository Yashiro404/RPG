//
// Created by Victor Navarro on 15/02/24.
//

#include "Character.h"
#include <map>
#include <any>

using namespace std;

Character::Character(string _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer, int _defenseMode)
{
    name = _name;
    health = _health;
    attack = _attack;
    defense = _defense;
    speed = _speed;
    isPlayer = _isPlayer;
    defenseMode = _defenseMode;
}

map<string, any> Character::getData()
{
    map<string, any> data;
    data["name"] = name;
    data["health"] = health;
    data["attack"] = attack;
    data["defense"] = defense;
    data["speed"] = speed;
    data["isPlayer"] = isPlayer;
    data["defenseMode"] = defenseMode;

    return data;
}

bool Character::flee(Character *target)
{
    if (this->speed > target->speed)
        return true;

    int chance = rand() % 100;
    return chance > 30;
}

bool Character::canDefend()
{
    if (defenseMode == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Character::defend()
{
    defenseMode = 2;
    defense = static_cast<int>(defense * 1.2);
}

void Character::nerfDefenseMode()
{
    if (defenseMode > 0)
    {
        defenseMode -= 1;

        if (defenseMode == 0)
        {
            defense = static_cast<int>(defense * .8);
        }
    }
}