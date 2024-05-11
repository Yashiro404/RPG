#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"
#include <fstream>
#include <sstream>

using namespace std;

void savePlayerData(const Character &player, int nivel)
{
    ofstream dataFile("player_data.txt");
    if (dataFile.is_open())
    {
        map<string, any> playerData = player.getData();

        dataFile << "CombatLevel: " << nivel << endl;

        for (const auto &[key, value] : playerData)
        {
            dataFile << key << ": ";
            if (value.type() == typeid(string))
            {
                dataFile << any_cast<string>(value);
            }
            else if (value.type() == typeid(int))
            {
                dataFile << any_cast<int>(value);
            }
            else if (value.type() == typeid(bool))
            {
                dataFile << (any_cast<bool>(value) ? "true" : "false");
            }

            dataFile << endl;
        }

        dataFile.close();
    }
    else
    {
        cout << "No saved data, creating new game" << endl;
        cout << " " << endl;
    }
}

int loadPlayerData(Player &player)
{
    ifstream dataFile("player_data.txt");
    if (dataFile.is_open())
    {
        string line;
        int level = 0;
        map<string, any> data;

        while (getline(dataFile, line))
        {
            istringstream iss(line);
            string key, value;

            if (getline(iss, key, ':') && getline(iss, value))
            {
                key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));
                key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
                value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
                value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

                if (key == "CombatLevel")
                {
                    level = stoi(value);
                }
                else
                {
                    if (key == "health" || key == "attack" || key == "defense" || key == "speed" || key == "defenseMode")
                    {
                        data[key] = stoi(value);
                    }
                    else if (key == "isPlayer")
                    {
                        data[key] = (value == "true");
                    }
                    else
                    {
                        data[key] = value;
                    }
                }
            }
        }

        player.setData(data);

        dataFile.close();

        return level;
    }
    else
    {
        cout << "No saved data, creating new game" << endl;
        cout << " " << endl;

        return 0;
    }
}

int main()
{
    int combatLevel = 1;
    vector<Character *> participants;

    Player player("Goku", 1000, 80, 80, 100);
    int levelSaved = loadPlayerData(player);

    participants.push_back(&player);

    vector<Enemy> enemiesData = {
        Enemy("Saibaman", 40, 30, 5, 50),
        Enemy("Vegeta", 95, 110, 9, 110),
        Enemy("Piccolo", 85, 100, 9, 80),
        Enemy("Gohan", 90, 105, 8, 90),
        Enemy("Krillin", 75, 90, 7, 70),
        Enemy("Yamcha", 70, 85, 6, 75),
        Enemy("Tien", 80, 95, 7, 85),
        Enemy("Chiaotzu", 60, 80, 5, 60),
        Enemy("Master Roshi", 65, 75, 6, 55),
        Enemy("Bulma", 50, 70, 4, 65),
        Enemy("Trunks", 92, 115, 9, 105),
        Enemy("Goten", 88, 110, 8, 100),
        Enemy("Android 18", 95, 105, 9, 95),
        Enemy("Android 17", 95, 105, 9, 95),
        Enemy("Frieza", 110, 125, 10, 120),
        Enemy("Cell", 105, 120, 10, 115),
        Enemy("Majin Buu", 120, 130, 15, 105),
        Enemy("Beerus", 150, 140, 10, 150),
        Enemy("Whis", 160, 150, 10, 160),
        Enemy("Jiren", 200, 200, 20, 200)};

    if (levelSaved > 1)
    {
        combatLevel = levelSaved;
    }

    while (true)
    {
        srand(time(nullptr));

        for (int i = 0; i < 4; ++i)
        {
            int randomIndex = rand() % enemiesData.size();
            Enemy *enemy = new Enemy(enemiesData[randomIndex]);

            if (combatLevel > 1)
            {
                enemy->buffEnemy(combatLevel);
            }

            participants.push_back(enemy);
        }

        Combat combat(participants, combatLevel);
        combatLevel = combat.doCombat();

        if (combatLevel == 0)
        {
            break;
        }

        savePlayerData(player, combatLevel);
    }

    return 0;
}

// RUN CODE:
// g++ main.cpp Player/Player.cpp Enemy/Enemy.cpp Combat/Combat.cpp Combat/Action.cpp Character/Character.cpp -o main ; ./main.exe