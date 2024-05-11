//
// Created by Victor Navarro on 29/02/24.
//

#ifndef RPG_ACTION_H
#define RPG_ACTION_H
#include <functional>
#include "../Character/Character.h"

using namespace std;

class Action {
public:
    int speed = 0;

    Character* target = nullptr;
    Character* subscriber = nullptr;

    function<void(void)> action = nullptr;
    Action(int _speed, function<void(void)> _action, Character* _subscriber, Character* _target);
    Action();

    bool operator<(const Action& other) const;
};


#endif //RPG_ACTION_H
