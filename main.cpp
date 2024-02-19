#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"

int main()
{
    while (true)
    {
        Player *player = new Player("Victor", 100, 50, 4, 10);
        Enemy *enemy = new Enemy("Estudiante de Redes", 50, 5, 2, 5, 10);

        cout << player->toString() << endl;
        cout << enemy->toString() << endl;
        cout << "=====================\n"
             << endl;

        while (player->isAlive() && enemy->isAlive())
        {
            player->doAttack(enemy);
            int playerDamage = player->getAttack();
            cout << "¡El jugador hizo " << playerDamage << " puntos de ataque al enemigo!\n";

            if (!enemy->isAlive())
                break;

            enemy->doAttack(player);
            int enemyDamage = enemy->getAttack();
            cout << "¡El enemigo hizo " << enemyDamage << " puntos de ataque al jugador!\n";
        }

        cout << player->toString() << endl;
        cout << enemy->toString() << endl;

        if (!player->isAlive())
        {
            cout << "El jugador ha muerto. Fin del juego.\n"
                 << endl;
        }
        else if (!enemy->isAlive())
        {
            cout << "El enemigo ha muerto. Continuamos con el siguiente enemigo.\n"
                 << endl;
        }

        delete player;
        delete enemy;

        cout << ">> Desea continuar la batalla? (s/n): \n";
        char choice;
        cin >> choice;

        if (choice != 's' && choice != 'S')
        {
            cout << "Fin del juego." << endl;
            break;
        }
    }

    return 0;
}

// Ejecutar código: g++ main.cpp Player/Player.cpp Enemy/Enemy.cpp Character/Character.cpp -o main ; .\main.exe
