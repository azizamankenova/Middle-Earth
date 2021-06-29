#include "Character.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
/*
creates a team of 5 characters by reading input from an input file 
*/
void createCharacters(vector<Character>* team,ifstream& infile, int num){
    for(int i = 0; i < 5; i++){
        string name, type;
        int attack, defense, health;
        infile >> name >> type >> attack >> defense >> health;
        Character a = Character(name,type,attack, defense, health, num);
        team->push_back(a);

    }
}
/*
sorts a vector of characters alphabetically and returns a new vector of chacracter pointers
*/
vector<Character*> sortAlphabetically(vector<Character>* team){

    vector< Character* > t;
    bool flagged[5] = {true,true,true,true,true};
    for(int j = 0; j < 5; j++) {
        Character *a;
        int i;
        for (i = 0; i < 5; ++i) {
            if(flagged[i]){
                a= &team->at(i);
                break;
            }
        }
        int index = i;
        for (int k = 0; k < team->size(); k++) {
            if (index != k && flagged[k] && (team->at(k)) < *a) {
                a = &team->at(k);
                index = k;
            }
        }
        t.push_back(a);
        flagged[index] = false;
    }
    return t;
}
/*
gets a specific character from a team and returns a character pointer
*/
Character* getCharacterFromTeam( vector<Character*>& team, const string& name){
    Character* character_ptr;
    int index;
    for (int j = 0; j < 5; j++) {
        if (team[j]->name == name) {
            character_ptr = team[j];
            index = j;
            break;
        }
    }
    //if the character is dead, searches for the next name-wise alphabetically alive character
    if (!character_ptr->isAlive) {
        if (index != 4) {
            for (int j = (index + 1); j < 5; j++) {
                if (team[j]->isAlive) {
                    character_ptr = team[j];
                    break;
                }
            }
        }
    }
    //If there is no alphabetically next alive character, then it is replaced by the alphabetically previous alive character.
    if (!character_ptr->isAlive) {
        if (index != 0) {
            for (int i = (index - 1); i >= 0; i--) {
                if (team[i]->isAlive) {
                    character_ptr = team[i];
                    break;
                }
            }
        }
    }
    return character_ptr;
}
/*
updates health history of each member of a team in each round
*/
void updateHealthHistory(vector<Character>* team, int index){
    for(int i = 0; i < 5; i++){
        team->at(i).healthHistory[index] = team->at(i).remainingHealth;
    }
}
/*
updates the number of rounds passed after using a special skill for each member of a team
*/
void updateNRoundsSinceSpecial(vector<Character>* team){
    for (int i = 0; i < 5; i++) {
        if(team->at(i).isAlive) {
            team->at(i).nRoundsSinceSpecial++;
        }
        else{
            team->at(i).nRoundsSinceSpecial = 0;
        }
    }

}
/*
finds the index of a hobbit in the team vector and returns an integer
*/
int indexHobbit(vector<Character>* team){

    for (int i = 0; i < 5; i++) {
        if(team->at(i).type == "Hobbit"){
            return i;
        }
    }
}
/*
the main function of the program, reads the input file and prints out the output into the output file
*/
int main(int argc, char* argv[]) {
    ifstream infile;
    infile.open(argv[1]);
    int numRounds;
    infile >> numRounds;
    //create two different vectorof characters for each team
    vector<Character> team1;
    createCharacters(&team1, infile, numRounds);
    vector<Character> team2;
    createCharacters(&team2, infile, numRounds);
    //store the same characters in a separate vector in alphabetical order
    vector<Character*> team1Sorted = sortAlphabetically(&team1);
    vector<Character*> team2Sorted = sortAlphabetically(&team2);
    ofstream outfile;
    outfile.open(argv[2]);

    //keep track of killed characters in each team
    Character* lastKilled1 = nullptr;
    Character* lastKilled2 = nullptr;
    //finds the index of hobbit in a vector of each team
    int indexHobbit1 = indexHobbit(&team1);
    int indexHobbit2 = indexHobbit(&team2);
    //count of the rounds passed
    int roundsPassed = 0;

    //Simulation of the game
    for(int i = 0; i < numRounds; i++) {

        string attacker, defender, special;
        bool ifSpecial;
        infile >> attacker >> defender >> special;
        if (special == "NO-SPECIAL") {
            ifSpecial = false;
        } else {
            ifSpecial = true;
        }
        //creates two character pointers for an attacker and a defender
        Character* attackerPtr;
        Character* defenderPtr;
        //damage of an attack
        int damage = 0;
        //depending on the round, finding the attacker and defender from the corresponding team
        if (i % 2 == 0) {
            attackerPtr = getCharacterFromTeam( team1Sorted, attacker);
            defenderPtr = getCharacterFromTeam( team2Sorted, defender);
        }
        else {
            attackerPtr = getCharacterFromTeam( team2Sorted, attacker);
            defenderPtr = getCharacterFromTeam( team1Sorted, defender);
        }
        //calculates the damage on the defender
        damage = attackerPtr->attack - defenderPtr->defense;
        //checks for the special rounds
        if(ifSpecial){
            //Elves convey half of their health to the hobbit of its community per 10 rounds.
            if(attackerPtr->type == "Elves" && attackerPtr->nRoundsSinceSpecial > 10){
                for(int j = 0; j < 5; j++){
                    if(team1.at(j).type == "Hobbit"){
                        team1.at(j).remainingHealth += (attackerPtr->remainingHealth/2);
                        attackerPtr->remainingHealth /= 2;
                        attackerPtr->nRoundsSinceSpecial = 0;
                        break;
                    }
                }
            }
            //Dwarfs special skill doubles the damage on the opponent per 20 rounds
            else if(attackerPtr->type == "Dwarfs" && attackerPtr->nRoundsSinceSpecial > 20){
                damage = 2*(attackerPtr->attack - defenderPtr->defense);
                attackerPtr->nRoundsSinceSpecial = 0;

            }
            //Wizards turn the last killed community member back to life with the initial health of the killed member, per 50 rounds.
            else if(attackerPtr->type == "Wizards" && attackerPtr->nRoundsSinceSpecial > 50){
                 if(i % 2 == 0){
                     if(lastKilled1 != nullptr){
                         if(!lastKilled1->isAlive){
                             lastKilled1->nRoundsSinceSpecial = 0;
                             lastKilled1->isAlive = true;
                         }

                         lastKilled1->remainingHealth = lastKilled1->healthHistory[0];
                         attackerPtr->nRoundsSinceSpecial = 0;
                     }
                 }
                 else{
                     if(lastKilled2 != nullptr){
                         if(!lastKilled2->isAlive){
                             lastKilled2->nRoundsSinceSpecial = 0;
                             lastKilled2->isAlive = true;
                         }
                         lastKilled2->remainingHealth = lastKilled2->healthHistory[0];
                         attackerPtr->nRoundsSinceSpecial = 0;
                     }
                 }


            }

        }

        //changes the remaining health of the defender if the damage is above 0
        if(damage > 0) {
            int currentHealth = defenderPtr->remainingHealth - damage;
            if(currentHealth>0) {
                defenderPtr->remainingHealth = currentHealth;
            }
            else{
                defenderPtr->remainingHealth = 0;
                defenderPtr->isAlive = false;
                defenderPtr->nRoundsSinceSpecial = 0;
                if (i % 2 == 0){
                    lastKilled2 = defenderPtr;
                }
                else{
                    lastKilled1 = defenderPtr;
                }
                }
            }
        //updates health history and number of rounds passed since the special skill of each member of two teams
        updateHealthHistory(&team1, i+1);
        updateHealthHistory(&team2, i+1);
        updateNRoundsSinceSpecial(&team1);
        updateNRoundsSinceSpecial(&team2);

        roundsPassed++;
        //checks for the end conditions, when Hobbit has died
        if(i % 2 == 0 && !team2.at(indexHobbit2).isAlive){
            outfile << "Community-1" << endl;
            break;
        }
        else if(i % 2 == 1 && !team1.at(indexHobbit1).isAlive){
            outfile << "Community-2" << endl;
            break;
        }
        //checks for the other end condition, when Hobbit is left defenseless 
        else if(!defenderPtr->isAlive){
            int countCasualties = 0;
            if(i % 2 == 0){
                for(int j = 0; j < 5; j++){
                    if(team2.at(j).type != "Hobbit" && !team2.at(j).isAlive){
                        countCasualties++;
                    }
                    else if(team2.at(j).type != "Hobbit" && team2.at(j).isAlive){
                        break;
                    }

                }
                if(countCasualties == 4){
                    outfile << "Community-1" << endl;
                    break;
                }
            }
            else{
                for(int j = 0; j < 5; j++){
                    if(team1.at(j).type != "Hobbit" && !team1.at(j).isAlive){
                        countCasualties++;
                    }
                    else if(team1.at(j).type != "Hobbit" && team1.at(j).isAlive){
                        break;
                    }
                }
                if(countCasualties == 4){
                    outfile << "Community-2" << endl;
                    break;
                }
            }


        }
        //condition for the Draw in the game
        if(i == ( numRounds - 1 )){
            outfile << "Draw" << endl;
        }
    }

    infile.close();

    outfile << roundsPassed << endl;
    //counts the total number of casualties of each team
    int numCasualties = 0;
    for (int i = 0; i < 5; ++i) {
        if(!team1.at(i).isAlive){
            numCasualties++;
        }
    }
    for (int i = 0; i < 5; ++i) {
        if(!team2.at(i).isAlive){
            numCasualties++;
        }
    }

    outfile << numCasualties << endl;

    //prints the health history of each member of two teams
    for (int i = 0; i < 5; i++) {
        outfile << team1.at(i).name << " ";
        for (int j = 0; j <= roundsPassed; j++) {
            outfile << team1[i].healthHistory[j] << " ";
        }
        outfile << endl;

    }
    for (int i = 0; i < 5; i++) {
        outfile << team2.at(i).name << " ";
        for (int j = 0; j <= roundsPassed; j++) {
            outfile << team2[i].healthHistory[j] << " ";
        }
        outfile << endl;

    }

    outfile.close();

    return 0;
}