//
// Created by Victor Navarro on 19/02/24.
//

#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;

bool compareSpeed(Character *a, Character *b)
{
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants)
{
    participants = std::move(_participants);
    for (auto participant : participants)
    {
        if (participant->getIsPlayer())
        {
            partyMembers.push_back((Player *)participant);
        }
        else
        {
            enemies.push_back((Enemy *)participant);
        }
    }
}

Combat::Combat(vector<Player *> _partyMembers, vector<Enemy *> _enemies)
{
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character *>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat()
{
    participants = vector<Character *>();
}

void Combat::addParticipant(Character *participant)
{
    participants.push_back(participant);
    if (participant->getIsPlayer())
    {
        partyMembers.push_back((Player *)participant);
    }
    else
    {
        enemies.push_back((Enemy *)participant);
    }
}

void Combat::combatPrep()
{
    sort(participants.begin(), participants.end(), compareSpeed);
}

string Combat::toString()
{
    string result = "";
    vector<Character *>::iterator it;
    for (it = participants.begin(); it != participants.end(); it++)
    {
        result += (*it)->toString() + "\n";
    }
    cout << "====================" << endl;
    return result;
}

Character *Combat::getTarget(Character *attacker)
{
    vector<Character *>::iterator it;
    for (it = participants.begin(); it != participants.end(); it++)
    {
        if ((*it)->getIsPlayer() != attacker->getIsPlayer())
        {
            return *it;
        }
    }
    return nullptr;
}

void Combat::doCombat()
{
    cout << "Inicio del combate" << endl;
    combatPrep();

    while (participants.size() > 1)
    {
        vector<Character *>::iterator it = participants.begin();

        while (it != participants.end())
        {
            Character *target = nullptr;

            if ((*it)->getDefenseMode() > 0)
            {
                (*it)->nerfDefenseMode();
            }

            if ((*it)->getIsPlayer())
            {
                int action;

                cout << "Es el turno de " << (*it)->getName() << endl;

                cout << "¿Qué acción deseas realizar? 1 - Atacar | 2 - Defender: ";
                cin >> action;

                while (action)
                {
                    if (action == 1)
                    {
                        target = ((Player *)*it)->selectTarget(enemies);
                        (*it)->doAttack(target);
                    }
                    else if (action == 2)
                    {
                        if (!(*it)->defend())
                        {
                            cout << "Ya te has defendido, es hora de atacar." << endl;
                            (*it)->doAttack(target);
                        }
                        cout << (*it)->getName() << " ha elegido defender." << endl;
                    }
                    else
                    {
                        cout << "Accion invalida. Intentalo de nuevo." << endl;
                    }

                    cout << "¿Que accion deseas realizar? 1 - Atacar | 2 - Defender: ";
                    cin >> action;
                }
            }
            else
            {
                while (true)
                {
                    target = ((Enemy *)*it)->selectTarget(partyMembers);

                    if ((*it)->getHealth() < 0.15 * (*it)->getMaxHealth())
                    {
                        if (rand() % 10 <= 3)
                        {
                            if (!(*it)->defend())
                            {
                                (*it)->doAttack(target);
                                break;
                            }
                            else
                            {
                                cout << (*it)->getName() << " ha elegido defender." << endl;
                                break;
                            }
                        }
                    }
                    else
                    {
                        (*it)->doAttack(target);
                        cout << (*it)->getName() << " ataco a " << target->getName() << " por " << (*it)->getAttack() << " puntos." << endl;
                        break;
                    }
                }
            }

            if (target->getHealth() <= 0)
            {
                it = participants.erase(remove(participants.begin(), participants.end(), target), participants.end());

                if (target->getIsPlayer())
                {
                    partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), target), partyMembers.end());
                }
                else
                {
                    cout << "You killed enemy " << target->getName() << endl;
                    enemies.erase(remove(enemies.begin(), enemies.end(), target), enemies.end());
                }
            }
            else
            {
                it++;
            }
        }
    }

    if (participants.size() == 1)
    {
        Character *remainingCharacter = participants[0];
        if (remainingCharacter->getIsPlayer())
        {
            cout << "Game Over" << endl;
        }
        else
        {
            cout << "Victory" << endl;
        }
    }
}
