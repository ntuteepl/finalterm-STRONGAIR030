#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
using namespace std;

class Character {
protected:
    static const int EXP_LV = 100;
    string name;
    int hp;
    int level;
    int exp;
    int power;
    int knowledge;
    int luck;
    int powerBoost;
    void levelUp(int hInc, int pInc, int kInc, int lInc);

public:
    Character(string n, int lv, int h, int po, int kn, int lu);
    virtual void print();
    virtual void beatMonster(int exp);
    virtual void setHP(int) = 0;
    virtual int getHP() = 0;
    virtual int getAttack() = 0;
    string getName();
    void heal(int amount) { hp += amount; if (hp > level * 100) hp = level * 100; }
    void boostPower() { powerBoost++; }
};

Character::Character(string n, int lv, int h, int po, int kn, int lu) : name(n), level(lv), exp(pow(lv - 1, 2) * EXP_LV), hp(h), power(po), knowledge(kn), luck(lu), powerBoost(0) {}

string Character::getName() {
    return this->name;
}

void Character::levelUp(int hInc, int pInc, int kInc, int lInc) {
    this->level++;
    this->hp += hInc;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;
}

void Character::print() {
    cout << this->name << ": " << this->level << " (" << this->exp << "/" << pow(this->level, 2) * EXP_LV << "), ";
    if (this->hp >= 0)
        cout << this->hp;
    else
        cout << "0";
    cout << "-" << this->power << "-" << this->knowledge << "-" << this->luck << "\n";
}

void Character::beatMonster(int exp) {
    this->exp += exp;
    while (this->exp >= pow(this->level, 2) * EXP_LV)
        this->levelUp(0, 0, 0, 0);
}

class Warrior : public Character {
private:
    static const int HP_LV = 100;
    static const int PO_LV = 10;
    static const int KN_LV = 5;
    static const int LU_LV = 5;

public:
    Warrior(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Warrior "; Character::print(); }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return power * (1 + powerBoost*0.4); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
};

class Wizard : public Character {
private:
    static const int HP_LV = 80;
    static const int PO_LV = 4;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Wizard(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Wizard "; Character::print(); }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return knowledge * (1 + powerBoost*0.4); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
};

class Monster : public Character {
private:
    static const int HP_LV = 80;
    static const int PO_LV = 5;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Monster(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Monster: HP=" << hp << ", Attack=" << power << endl; }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return power; }

};

void shop(Character* player, int& money);
void battle(Character* player, Monster* monster, int& money);
void wait();
void wait(string s);

void wait()
{
    cout << "Press enter to continue...";
    cin.ignore();
    cin.get();
}

void wait(string s)
{
    cout << s;
    cin.ignore();
    cin.get();
}

void battle(Character* player, Monster* monster, int& money) {
    bool playerWin = false;
    while (player->getHP() > 0 && monster->getHP() > 0) {

        monster->print();
        player->print();
        wait("Press enter to attack...");

        monster->setHP(monster->getHP() - player->getAttack());
        if (monster->getHP() <= 0) {
            player->beatMonster(50);
            money += 50;
            playerWin = true;
            break;
        }

        player->setHP(player->getHP() - monster->getAttack());
        if (player->getHP() <= 0) break;
    }

    cout << (playerWin ? "You win!" : "You lose!") << endl;
    wait();

}

void shop(Character* player, int& money) {
    cout << "You have " << money << " gold\n";
    cout << "Welcome to the shop!\n1. Heal (30 gold)\n2. Power Boost (50 gold)\n0. Exit shop\nChoose an option: ";
    int choice;
    cin >> choice;
    if (choice == 1 && money >= 30) {
        player->heal(50);
        money -= 30;
    } else if (choice == 2 && money >= 50) {
        player->boostPower();
        money -= 50;
    }

    if(choice != 0){
        player->print();
        cout << "You now have " << money << " gold\n";
        wait();
        cout << "\033[2J\033[1;1H";
        shop(player, money);
    }
}

int main() {
    srand(time(0));
    string name;
    cout << "Enter your character name: ";
    cin >> name;

    cout << "Choose your class:\n1. Warrior\n2. Wizard\nChoose: ";
    int choice;
    cin >> choice;
    cout << "\033[2J\033[1;1H";

    Character* player;

    if(choice == 1){
        player = new Warrior(name);
    }
    else{
        player = new Wizard(name);
    }

    player->print();
    wait("Press enter to start the game...");


    int money = 100;
    int monsterCount = 0;

    while (player->getHP() > 0) {
        int event = rand() % 3;
        cout << "\033[2J\033[1;1H";
        if (event == 0) {
            Monster monster("bird", monsterCount + 1);
            battle(player, &monster, money);
            monsterCount++;
        } else if (event == 1) {
            shop(player, money);
        } else {
            money += 50;
            cout << "You found a treasure chest! +50 gold\n";
            cout << "You now have " << money << " gold\n";
            cout << "Press enter to continue...";
            cin.ignore();
            cin.get();
        }

        if (monsterCount == 10) {
            Monster boss("Boss", 30);
            battle(player, &boss, money);
            break;
        }
    }

    delete player;
    return 0;
}
