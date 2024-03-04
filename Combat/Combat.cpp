//
// Created by Victor Navarro on 19/02/24.
//

#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <map>
#include <any>

using namespace std;

bool compareSpeed(Character *a, Character *b)
{
    map<string, any> characterA = a->getData();
    map<string, any> characterB = b->getData();

    return any_cast<int>(characterA["speed"]) > any_cast<int>(characterB["speed"]);
}

Combat::Combat(vector<Character *> _participants)
{
    participants = std::move(_participants);

    for (auto participant : participants)
    {
        map<string, any> participantData = participant->getData();

        if (any_cast<bool>(participantData["isPlayer"]))
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
    map<string, any> participantData = participant->getData();

    if (any_cast<bool>(participantData["isPlayer"]))
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

// string Combat::toString()
// {
//     string result = "";
//     vector<Character *>::iterator it;
//     for (it = participants.begin(); it != participants.end(); it++)
//     {
//         result += (*it)->toString() + "\n";
//     }
//     cout << "====================" << endl;
//     return result;
// }

Character *Combat::getTarget(Character *attacker)
{
    vector<Character *>::iterator it;
    for (it = participants.begin(); it != participants.end(); it++)
    {
        map<string, any> itData = (*it)->getData();
        map<string, any> attackerData = attacker->getData();

        if (any_cast<bool>(itData["isPlayer"]) != any_cast<bool>(attackerData["isPlayer"]))
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
    int round = 1;

    while (enemies.size() > 0 && partyMembers.size() > 0)
    {
        cout << "Round " << round << endl;

        vector<Character *>::iterator it = participants.begin();
        registerActions(it);
        executeActions(it);

        round++;
    }

    if (enemies.empty())
    {
        cout << "You win!" << endl;
    }
    else
    {
        cout << "You lose!" << endl;
    }
}

void Combat::executeActions(vector<Character *>::iterator participant)
{
    while (!actionQueue.empty())
    {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        checkParticipantStatus(*participant);
        checkParticipantStatus(currentAction.target);
    }
}

void Combat::checkParticipantStatus(Character *participant)
{
    map<string, any> participantData = participant->getData();

    if (any_cast<int>(participantData["health"]) <= 0)
    {
        if (any_cast<bool>(participantData["isPlayer"]))
        {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        }
        else
        {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::registerActions(vector<Character *>::iterator participantIterator)
{
    while (participantIterator != participants.end())
    {
        map<string, any> participantData = (*participantIterator)->getData();

        if (any_cast<int>(participantData["defenseMode"]) > 0)
        {
            if (any_cast<bool>(participantData["isPlayer"]))
            {
                ((Player *)*participantIterator)->nerfDefenseMode();
            }
            else
            {
                ((Enemy *)*participantIterator)->nerfDefenseMode();
            }
        }

        if (any_cast<bool>(participantData["isPlayer"]))
        {
            Action playerAction = ((Player *)*participantIterator)->takeAction(enemies);
            actionQueue.push(playerAction);
        }
        else
        {
            Action enemyAction = ((Enemy *)*participantIterator)->takeAction(partyMembers);
            actionQueue.push(enemyAction);
        }

        participantIterator++;
    }
}