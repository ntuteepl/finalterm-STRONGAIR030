#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <conio.h>
using namespace std;

#define MAX_TEAM_MEMBER 3

class Character {
protected:
    static const int EXP_LV = 100;
    string name;
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    int level;
    int exp;
    int power;
    int knowledge;
    int luck;
    int powerBoost;
    int knowledgeBoost;
    void levelUp(int hInc, int mInc, int pInc, int kInc, int lInc);

public:
    Character(string n, int lv, int h, int maxMp, int po, int kn, int lu);
    virtual void print();
    virtual void beatMonster(int exp);
    virtual int getHP() { return hp; }
    virtual void setHP(int hp) { this->hp = min(hp, this->maxHp); }
    virtual int getMP() { return mp; }
    virtual void setMP(int mp) { this->mp = min(mp, this->maxMp); }
    virtual int getLuck() { return luck; }
    virtual int getAttack() = 0;
    string getName();
    void heal(int amount) { this->setHP(min(this->maxHp, this->hp + amount)); }
    void boostPower() { powerBoost++; }
    void boostKnowledge() { knowledgeBoost++; }
};

Character::Character(string n, int lv, int h, int maxMp, int po, int kn, int lu){
    this->name = n;
    this->level = lv;
    this->exp = pow(this->level - 1, 2) * EXP_LV;
    this->power = po;
    this->knowledge = kn;
    this->luck = lu;
    this->maxHp = h;
    this->hp = this->maxHp;
    this->maxMp = maxMp;
    this->mp = this->maxMp;
    this->powerBoost = 0;
    this->knowledgeBoost = 0;
}

string Character::getName() {
    return this->name;
}

void Character::levelUp(int hInc, int mInc, int pInc, int kInc, int lInc) {
    this->level++;
    this->maxHp += hInc;
    this->maxMp += mInc;
    this->hp = this->maxHp;
    this->mp = this->maxMp;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;
}

void Character::print() {
    cout << this->name << ": " << "LV " << this->level << " (" << this->exp << "/" << pow(this->level, 2) * EXP_LV << "), ";

    cout << "HP: ";
    if (this->hp >= 0)
        cout << this->hp << "/" << this->maxHp;
    else
        cout << "0" << "/" << this->maxHp;
    
    cout << " MP: ";
    if(this->mp > 0)
        cout << this->mp << "/" << this->maxMp;
    else
        cout << "0" << "/" << this->maxMp;

    cout << " power: " << this->power; 
    if(this->powerBoost > 0)
        cout << " + " << this->powerBoost * 40 << "%";

    cout << " knowlodge: " << this->knowledge;
    if(this->knowledgeBoost > 0)
        cout << " + " << this->knowledgeBoost * 40 << "%";
    
    cout << " luck: " << this->luck << "\n";
}

void Character::beatMonster(int exp) {
    this->exp += exp;
    while (this->exp >= pow(this->level, 2) * EXP_LV)
        this->levelUp(0, 0, 0, 0, 0);
}

class Warrior : public Character {
private:
    static const int HP_LV = 50;
    static const int MP_LV = 30;
    static const int PO_LV = 10;
    static const int KN_LV = 5;
    static const int LU_LV = 5;

public:
    Warrior(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Warrior "; Character::print(); }
    int getAttack() { return power * (1 + powerBoost*0.4); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, MP_LV, PO_LV, KN_LV, LU_LV);
    }
};

class Wizard : public Character {
private:
    static const int HP_LV = 30;
    static const int MP_LV = 100;
    static const int PO_LV = 4;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Wizard(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Wizard "; Character::print(); }
    int getAttack() { return knowledge * (1 + powerBoost*0.4); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, MP_LV, PO_LV, KN_LV, LU_LV);
    }
};

class Monster : public Character {
private:
    static const int HP_LV = 80;
    static const int MP_LV = 100;
    static const int PO_LV = 5;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Monster(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV * (1 + (lv - 1) * 0.2), lv * KN_LV, lv * LU_LV) {}
    void print() { cout << "Monster: HP=" << hp << ", Attack=" << power << endl; }
    int getAttack() { return power; }
    int getExp(int luck);
    int getMoney(int luck);
};

int Monster::getExp(int luck) {
    return this->level * 300 + rand() % luck;
}

int Monster::getMoney(int luck) {
    return this->level * 50 + rand() % luck;
}

class Team
{
protected:
    string name;
	int memberCount;
	Character* member[MAX_TEAM_MEMBER];
public:
	Team(string name);
	~Team();
    Character* operator[] (int i) { return member[i]; }
	void addWarrior(string name, int lv);
	void addWizard(string name, int lv);
    void heal(int id, int amount);
    void boostPower(int id);
    void boostKnowledge(int id);
    void printMember(int id);
	void memberBeatMonster(string name, int exp);
    int getMemberCount() { return memberCount; }
    Character* getMember(int i) { return member[i]; }
	void print();
    
};

Team::Team(string name)
{
    this->name = name;
	this->memberCount = 0;
	for(int i = 0; i < MAX_TEAM_MEMBER; i++)
		this->member[i] = nullptr;
}

Team::~Team()
{
	for(int i = 0; i < memberCount; i++)
		delete member[i];
}

void Team::addWarrior(string name, int lv)
{
	if(memberCount < MAX_TEAM_MEMBER)
	{
		member[memberCount] = new Warrior(name, lv);
		memberCount++;
	}
}

void Team::addWizard(string name, int lv)
{
	if(memberCount < MAX_TEAM_MEMBER)
	{
		member[memberCount] = new Wizard(name, lv);
		memberCount++;
	}
}

void Team::memberBeatMonster(string name, int exp)
{
    int avgExp = exp / memberCount;
	for(int i = 0; i < memberCount; i++)
	{
        member[i]->beatMonster(avgExp);
	}
}

void Team::heal(int id, int amount) {
    member[id - 1]->heal(amount);
}

void Team::boostPower(int id) {
    member[id - 1]->boostPower();
}

void Team::boostKnowledge(int id) {
    member[id - 1]->boostKnowledge();
}

void Team::print()
{
    cout << "Team: " << name << endl;
	for(int i = 0; i < memberCount; i++) {
        cout << i + 1 << ". ";
		member[i]->print();
    }
}

void Team::printMember(int id) {
    member[id - 1]->print();
}


void shop(Character* player, int& money);
bool battle(Character* player, Monster* monster, int& money);
bool battle(Team* playerTeam, Monster* monster, int& money);
void wait();
void wait(string s);
int getRandenNum(int maxNum, int limitNum);
int getRandenNum(int maxNum, int limitList[], int len);
int chooseMember(Team* playerTeam);

void wait()
{
    cout << "Press enter to continue..." << endl;
    getch();
}

void wait(string s)
{
    cout << s << endl;
    getch();
}

int getRandenNum(int maxNum, int limitNum) {
    int randenNum = rand() % maxNum;
    while (randenNum == limitNum) {
        randenNum = rand() % 3;
    }
    return randenNum;
}

int getRandenNum(int maxNum, int limitList[], int len){
    while (1) {
        int randenNum = rand() % maxNum;
        bool isLimit = false;
        for (int i = 0; i < len; i++) {
            if (randenNum == limitList[i]) {
                isLimit = true;
                break;
            }
        }
        if (!isLimit) {
            return randenNum;
        }
    }
}


int chooseMember(Team* playerTeam) {
    int choice;
    cin >> choice;
    while (choice < 1 || choice > playerTeam->getMemberCount()) {
        cout << "Invalid choice! Choose again: ";
        cin >> choice;
    }
    return choice;
}

bool battle(Team* playerTeam, Monster* monster, int& money) {
    bool playerLose = false;
    int memberCount = playerTeam->getMemberCount();
    int actions[MAX_TEAM_MEMBER];
    int unableToSelect[MAX_TEAM_MEMBER];

    for(int i = 0; i < MAX_TEAM_MEMBER; i++){
        if(i >= memberCount || playerTeam->getMember(i) == nullptr || playerTeam->getMember(i)->getHP() <= 0){
            unableToSelect[i] = i;
            actions[i] = -1;
        } else {
            unableToSelect[i] = -1;
            actions[i] = 0;
        }

    }

    cout << "A wild monster appears!\n";
    monster->print();
    playerTeam->print();
    wait("Press enter to start the battle...");
    cout << "\033[2J\033[1;1H";

    while (monster->getHP() > 0 && !playerLose) {
        monster->print();
        int targetIndex = getRandenNum(memberCount, unableToSelect, memberCount);
        int totalDamage = monster->getAttack();
        if(actions[targetIndex] == 2){
            totalDamage *= 0.2;
        }
        Character* target = playerTeam->getMember(targetIndex);
        target->setHP(target->getHP() - totalDamage);
        cout << "Monster attacks " << targetIndex + 1 << ". " << target->getName() << " for " << totalDamage << " damage!\n" << endl;

        // ?查??是否全部?亡
        playerLose = true;
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (playerTeam->getMember(i)->getHP() > 0) {
                playerLose = false;
                break;
            }
        }
        if (playerLose) break;

        cout << "Choose actions for your team:\n";
        playerTeam->print();
        cout << "\n";
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (playerTeam->getMember(i)->getHP() <= 0) {
                cout << playerTeam->getMember(i)->getName() << " is down!\n";
                unableToSelect[i] = i;
                actions[i] = -1;
                continue;
            }

            cout << i + 1 << ". " << playerTeam->getMember(i)->getName() << " (1: Attack, 2: Defend, 0: Default): ";
            cin >> actions[i];
            while (actions[i] < 0 || actions[i] > 2) {
                cout << "Invalid action. Choose again: ";
                cin >> actions[i];
            }
        }

        // ?行玩家行?
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (actions[i] == -1) continue; // 跳??亡的??

            Character* member = playerTeam->getMember(i);

            cout << i + 1 << ". " << member->getName() << " ";

            if (actions[i] == 1) { // 攻?
                monster->setHP(monster->getHP() - member->getAttack());
                cout << "attacks the monster for " << member->getAttack() << " damage!\n";

            } else if (actions[i] == 2) { // 防御
                cout << "is defending!\n";

            } else if (actions[i] == 0) { // 默?攻?
                monster->setHP(monster->getHP() - member->getAttack());
                cout << "attacks the monster for " << member->getAttack() << " damage!\n";
            }
        }

        monster->print();
        wait();


        // ?查怪物是否被??
        if (monster->getHP() <= 0) {
            cout << "Monster defeated!\n";
            int totalExp = monster->getExp(playerTeam->getMember(0)->getLuck());
            money += monster->getMoney(playerTeam->getMember(0)->getLuck());
            cout << "You gained " << totalExp << " EXP and " << monster->getMoney(playerTeam->getMember(0)->getLuck()) << " gold!\n";
            playerTeam->memberBeatMonster(monster->getName(), totalExp);
            break;
        }

        cout << "\033[2J\033[1;1H";
    }

    if (playerLose) {
        cout << "Your team has been defeated...\n";
    } else {
        cout << "Victory!\n";
    }

    return playerLose;
}

void shop(Team* playerTeam, int& money) {
    const int shopItem = 3;
    cout << "You have " << money << " gold\n";
    cout << "Welcome to the shop!\n1. Heal (30 gold)\n2. Power Boost (50 gold)\n3. knowledge Boost (50 gold)\n0. Exit shop\nChoose an option: ";
    int choice, memberChoice;
    bool canBuy = true;
    cin >> choice;

    switch (choice)
    {
    case 1:
        if (money < 30) {
            canBuy = false;
            break;
        }

        cout << "\n" << endl;
        playerTeam->print();
        cout << "choose a member to heal: ";
        memberChoice = chooseMember(playerTeam);
        playerTeam->heal(memberChoice, 50);
        cout << "Member " << memberChoice << " has been healed\n";
        playerTeam->printMember(memberChoice);

        money -= 30;

        break;
    
    case 2:
        if (money < 50) {
            canBuy = false;
            break;
        }

        cout << "\n" << endl;
        playerTeam->print();
        cout << "choose a member to boost poser: ";
        memberChoice = chooseMember(playerTeam);
        playerTeam->boostPower(memberChoice);
        cout << "Member " << memberChoice << " has been boost\n";
        playerTeam->printMember(memberChoice);
        money -= 50;

        break;

    case 3:
        if (money < 50) {
            canBuy = false;
            break;
        }
        
        cout << "\n" << endl;
        playerTeam->print();
        cout << "choose a member to boost poser: ";
        memberChoice = chooseMember(playerTeam);
        playerTeam->boostKnowledge(memberChoice);
        cout << "Member " << memberChoice << " has been boost\n";
        playerTeam->printMember(memberChoice);
        money -= 50;

        break;
    
    }

    if(choice != 0 && choice <= shopItem && canBuy){
        cout << "You now have " << money << " gold\n";
        wait();
        cout << "\033[2J\033[1;1H";
        shop(playerTeam, money);
    } else if (!canBuy) {
        cout << "Not enough gold!\n";
        wait();
        cout << "\033[2J\033[1;1H";
        shop(playerTeam, money);
    } else if (choice > shopItem) {
        cout << "Invalid choice!\n";
        wait();
        cout << "\033[2J\033[1;1H";
        shop(playerTeam, money);
    } else if (choice == 0) {
        cout << "Goodbye!\n";
        wait();
    }

}

void summon(Team* playerTeam, int& money, int& monsterCount) {
    cout << "You have a chance to summon hero!\n";
    cout << "You have " << money << " gold\n";
    cout << "1. Warrior (" << 50 + monsterCount * 10 << " gold)\n";
    cout << "2. Wizard (" << 50 + monsterCount * 10 << " gold)\n";
    cout << "0. Exit\n";
    cout << "Choose the hero you want to summon:\n";
    int choice;
    cin >> choice;

    cout << "Enter new member name: ";
    string name;
    cin.ignore();
    getline(cin, name);

    cout << "\033[2J\033[1;1H";

    if (money < 50 + monsterCount * 10) {
        cout << "Not enough gold!\n";
        wait();
        return;
    }

    if(choice == 1){
        playerTeam->addWarrior(name, monsterCount + 1);
    } else if (choice == 2) {
        playerTeam->addWizard(name, monsterCount + 1);
    } else if (choice == 0) {
        return;
    } else {
        cout << "Invalid choice!\n";
        wait();
        summon(playerTeam, money, monsterCount);
    }
    playerTeam->print();
    wait();
}

int main() {
    srand(time(0));
    const int EVENT_AMOUNT = 4;
    string teamName, leaderName;

    cout << "Enter your Team name: ";
    getline(cin, teamName);
    cout << "Enter your Team leader name: ";
    getline(cin, leaderName);

    cout << "Choose your class:\n1. Warrior\n2. Wizard\nChoose: ";
    int choice;
    cin >> choice;
    cout << "\033[2J\033[1;1H";

    Team playerParty(teamName);

    if(choice == 1){
        playerParty.addWarrior(leaderName, 1);
    }
    else{
        playerParty.addWizard(leaderName, 1);
    }


    playerParty.print();
    wait("Press enter to start the game...");

    int money = 100;
    int monsterCount = 0;
    int eventCount = 0;
    int limitEvent[2] = {-1, -1};

    while (1) {
        int event = getRandenNum(EVENT_AMOUNT, limitEvent, EVENT_AMOUNT);
        if(eventCount % 3 == 0 && eventCount != 0){
            event = 0;
        }

        cout << "\033[2J\033[1;1H";
        if (event == 0) {
            Monster monster("bird", monsterCount + 1);
            bool lose = battle(&playerParty, &monster, money);
            lose ? wait("Game Over!") : wait();
            if(lose) break;
            eventCount = 0;
            monsterCount++;
        } else if (event == 1) {
            shop(&playerParty, money);
        } else if (event == 2) {
            money += 50;
            cout << "You found a treasure chest! +50 gold\n";
            cout << "You now have " << money << " gold\n";
            wait();
        } else if (event == 3) {
            summon(&playerParty, money, monsterCount);
            if(playerParty.getMemberCount() == 3){
                limitEvent[1] = 3;
            }
        }

        if (monsterCount == 10) {
            Monster boss("Boss", 30);
            bool lose = battle(&playerParty, &boss, money); 
            lose ? wait("Game Over!") : wait();
            if(lose) break;
        }

        eventCount++;
        limitEvent[0] = event;
    }

    return 0;
}
