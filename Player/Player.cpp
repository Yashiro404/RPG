//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>
#include <map>
#include <any>
#include <limits>

using namespace std;

Player::Player(string _name, int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _attack, _defense, _speed, true, 0)
{
    level = 1;
    experience = 0;
}

void Player::doAttack(Character *target)
{
    target->takeDamage(attack);
}

void Player::takeDamage(int damage)
{
    int trueDamage = damage - defense;

    health -= trueDamage;

    cout << name << " took " << trueDamage << " damage!" << endl;

    if (health <= 0)
    {
        cout << name << " has been defeated!" << endl;
    }
}

void Player::levelUp()
{
    level++;
}

void Player::gainExperience(int exp)
{
    experience += exp;

    if (experience >= 100)
    {
        levelUp();
        experience = 100 - experience;
    }
}

Character *Player::selectTarget(vector<Enemy *> possibleTargets)
{
    int selectedTarget = 0;

    for (int i = 0; i < possibleTargets.size(); i++)
    {
        map<string, any> targetData = possibleTargets[i]->getData();

        cout << i << ". " << any_cast<string>(targetData["name"]) << endl;
    }

    while (true)
    {
        cout << "Enter the number corresponding to your target: ";
        cin >> selectedTarget;

        if (cin.fail() || selectedTarget < 0 || selectedTarget >= possibleTargets.size())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number." << endl;
        }
        else
        {
            break;
        }
    }

    return possibleTargets[selectedTarget];
}

Action Player::takeAction(vector<Enemy *> enemies)
{
    Action currentAction;
    Character *target = nullptr;
    map<string, any> playerData = getData();
    
    currentAction.speed = any_cast<int>(playerData["speed"]);

    do
    {
        int action = 0;

        cout << "Select an action: " << endl
             << "1. Attack" << endl
             << "2. Defend" << endl;
        cin >> action;

        switch (action)
        {
        case 1:
            target = selectTarget(enemies);
            currentAction.target = target;
            currentAction.action = [this, target]()
            {
                doAttack(target);
            };

            break;
        case 2:
            if (!canDefend())
            {
                cout << "Ya te has defendido, es hora de atacar." << endl;
                target = selectTarget(enemies);
                currentAction.target = target;
                currentAction.action = [this, target]()
                {
                    doAttack(target);
                };
            }
            else
            {
                cout << "Has elegido defender." << endl;
                currentAction.action = [this]()
                {
                    defend();
                };
            }
            break;
        default:
            cout << "Invalid action. Please enter a valid number." << endl;
            continue;
        }

        break;
    } while (true);

    return currentAction;
}
