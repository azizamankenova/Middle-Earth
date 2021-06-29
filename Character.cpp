
#include "Character.h"
#include <iostream>
#include <algorithm>

using namespace std;

Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
    this->name = _name;
    this->type = _type;
    this->attack = _attack;
    this->defense = _defense;
    this->remainingHealth = _remainingHealth;
    this->nMaxRounds = _nMaxRounds;
    this->nRoundsSinceSpecial = 0;
    this->healthHistory = new int[_nMaxRounds + 1];
    this->healthHistory[0] = _remainingHealth;

}

Character::Character(const Character& character) {

    this->name = character.name;
    this->type = character.type;
    this->attack = character.attack;
    this->defense = character.defense;
    this->remainingHealth = character.remainingHealth;
    this->nMaxRounds = character.nMaxRounds;
    this->isAlive = character.isAlive;
    this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;

    this->healthHistory = new int[this->nMaxRounds + 1];
    for (int i = 0; i < nMaxRounds + 1; i++) {
        healthHistory[i] = character.healthHistory[i];
    }
}

Character& Character::operator=(const Character& character) {

    if (this == &character) {
        return *this;
    }
    if (this->healthHistory != NULL) {
        delete[] this->healthHistory;
    }

    this->name = character.name;
    this->type = character.type;
    this->attack = character.attack;
    this->defense = character.defense;
    this->remainingHealth = character.remainingHealth;
    this->nMaxRounds = character.nMaxRounds;
    this->isAlive = character.isAlive;
    this->nRoundsSinceSpecial = character.nRoundsSinceSpecial;

    this->healthHistory = new int[this->nMaxRounds + 1];
    for (int i = 0; i < nMaxRounds + 1; i++) {
        healthHistory[i] = character.healthHistory[i];
    }
    return *this;
}

bool Character::operator<(const Character& other) {
    int min = this->name.length() < other.name.length() ? this->name.length() : other.name.length();
    for(int i = 0; i < min; i++){
        if(this->name[i] < other.name[i]){
            return true;
        }
        else if(this->name[i]  > other.name[i]){
            return false;
        }
    }
    return true;

}

Character::~Character() {

    if (this->healthHistory != NULL) {
        delete[] this->healthHistory;
    }
}

