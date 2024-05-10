#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"
#include <fstream>
#include <sstream>

using namespace std;

void savePlayerData(const Character &player, int nivel)
{
    std::ofstream archivo("player_data.txt");
    if (archivo.is_open())
    {
        // Obtener los datos del jugador usando la función getData
        map<string, any> playerData = player.getData();

        // Guardar el nivel
        archivo << "CombatLevel: " << nivel << endl;

        // Guardar los datos del jugador
        for (const auto &[key, value] : playerData)
        {
            archivo << key << ": ";
            if (value.type() == typeid(string))
            {
                archivo << any_cast<string>(value);
            }
            else if (value.type() == typeid(int))
            {
                archivo << any_cast<int>(value);
            }
            else if (value.type() == typeid(bool))
            {
                archivo << (any_cast<bool>(value) ? "true" : "false");
            }
            // Agrega más casos según los tipos de datos que esperas manejar

            archivo << endl;
        }

        archivo.close();
    }
    else
    {
        cout << "Sin datos guardados" << endl;
    }
}

int loadPlayerData(Player &player)
{
    ifstream archivo("player_data.txt");
    if (archivo.is_open())
    {
        string linea;
        int level = 0;
        map<string, any> data;

        while (getline(archivo, linea))
        {
            istringstream iss(linea);
            string clave, valor;

            if (getline(iss, clave, ':') && getline(iss, valor))
            {
                // Eliminar espacios en blanco al principio y al final de la clave y el valor
                clave.erase(0, clave.find_first_not_of(" \t\n\r\f\v"));
                clave.erase(clave.find_last_not_of(" \t\n\r\f\v") + 1);
                valor.erase(0, valor.find_first_not_of(" \t\n\r\f\v"));
                valor.erase(valor.find_last_not_of(" \t\n\r\f\v") + 1);

                if (clave == "CombatLevel")
                {
                    level = std::stoi(valor);
                }
                else
                {
                    if (clave == "health" || clave == "attack" || clave == "defense" || clave == "speed" || clave == "defenseMode")
                    {
                        // Asigna el valor a la clave en el mapa de datos
                        data[clave] = std::stoi(valor);
                    }
                    else if (clave == "isPlayer")
                    {
                        data[clave] = (valor == "true");
                    }
                    else
                    {
                        data[clave] = valor;
                    }
                }
            }
        }

        // Asignar los datos cargados al jugador
        player.setData(data);

        archivo.close();

        return level;
    }
    else
    {
        cout << "Sin datos" << endl;
        return 0; // Retorna 0 para indicar un error
    }
}

int main()
{
    Player player("Goku", 100, 800, 80, 100);
    Enemy enemy("Freezer", 50, 6, 2, 5);
    Enemy enemy2("Cell", 50, 6, 2, 5);

    int combatLevel = 1;

    int levelSaved = loadPlayerData(player);

    if (levelSaved > 1)
    {
        combatLevel = levelSaved;
    }

    vector<Character *> participants;

    if (combatLevel > 1)
    {
        enemy.buffEnemy(combatLevel);
        enemy2.buffEnemy(combatLevel);
    }

    participants.push_back(&player);
    participants.push_back(&enemy);
    participants.push_back(&enemy2);

    Combat combat(participants, combatLevel);
    combatLevel = combat.doCombat();

    savePlayerData(player, combatLevel);

    cout << "Nivel de combate: " << combatLevel << endl;

    return 0;
}

// RUN CODE:
// g++ main.cpp Player/Player.cpp Enemy/Enemy.cpp Combat/Combat.cpp Combat/Action.cpp Character/Character.cpp -o main ; ./main.exe