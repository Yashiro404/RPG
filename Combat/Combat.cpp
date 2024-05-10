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

Combat::Combat(vector<Character *> _participants, int combatLevel)
{
    level = combatLevel;
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

int Combat::doCombat()
{
    cout << "----LEVEL " << level << "----" << endl;
    combatPrep();

    int round = 1;
    int expTotal = 0;

    while (enemies.size() > 0 && partyMembers.size() > 0)
    {
        cout << "==================" << endl;
        cout << "Round " << round << " >>>" << endl;

        vector<Character *>::iterator it = participants.begin();
        registerActions(it);
        cout << "------------------" << endl;
        expTotal += executeActions(it);

        if (enemies.empty() || partyMembers.empty())
        {
            break;
        }

        round++;
    }

    if (enemies.empty())
    {
        cout << "You win!" << endl;
        level++;

        for (auto player : partyMembers)
        {
            player->buffPlayer(expTotal);
        }

        return level;
    }
    else
    {
        cout << "You lose!" << endl;

        return 0;
    }
}

int Combat::executeActions(vector<Character *>::iterator participant)
{
    int expWin = 0;

    while (!actionQueue.empty())
    {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        if (currentAction.target != nullptr)
        {
            expWin = checkParticipantStatus(currentAction.target);
        }

        checkParticipantStatus(*participant);

        if (enemies.empty() || partyMembers.empty())
        {
            break;
        }
    }

    return expWin;
}

int Combat::checkParticipantStatus(Character *participant)
{
    map<string, any> participantData = participant->getData();
    int expGift = 0;

    if (any_cast<int>(participantData["health"]) <= 0)
    {
        if (any_cast<bool>(participantData["isPlayer"]))
        {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        }
        else
        {
            Enemy *enemyParticipant = dynamic_cast<Enemy *>(participant);
            expGift = enemyParticipant->getExpGift();

            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }

    return expGift;
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