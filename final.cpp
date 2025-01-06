#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>
#include <conio.h>
using namespace std;

#define MAX_TEAM_MEMBER 4
class Skill;
class Character;
class Warrior;
class Wizard;
class Healer;
class Monster;
class Team;

int limitChoose(int maxNum, int minNum);
void shop(Team* playerTeam, int& money);
bool battle(Character* player, Monster* monster, int& money);
bool battle(Team* playerTeam, Monster* monster, int& money);
void wait();
void wait(string s);
int getRandenNum(int maxNum, int limitNum);
int getRandenNum(int maxNum, int limitList[], int len);
int limitChoose(int maxNum, int minNum);
vector<Skill>* getskillsByClass(vector<Skill> skills, int classType);
void summon(Team* playerTeam, int& money, int& monsterCount);
void training(Team* playerTeam, int& money, vector<Skill> skills);
void printSkillList(vector<Skill> skills);

class Skill {
    private:
        string name;
        string des;
        double effect;
        int id;
        int classType; // 1: Warrior, 2: Wizard, 3: Healer
        int costMp;
        int costHp;
        int setHp;
        int setMp;
        int targetType; // 0: one enemy, 1: all enemy, 2: one ally, 3: all ally, 4: self
        int costType; // 0: costMp, 1: costHp, 2: setHp, 3: setMp

    public:
        Skill(string n, string des, int id, int cMp, int cHp, int sHp, int sMp, double effect ,int targetType, int cType, int classType);
        string getName() { return name; }
        int getId() { return id; }
        int getClassType() { return classType; }
        int getCostMp() { return costMp; }
        int getCostHp() { return costHp; }
        int getTargetType() { return targetType; }
        int getSetHp() { return setHp; }
        int getcostType() { return costType; }
        double getEffect() { return effect; }
        void operator=(Skill s);
        void print();
};

Skill::Skill(string n, string des, int id, int cMp, int cHp, int sHp, int sMp, double effect ,int targetType, int cType, int classType){
    this->id = id;
    this->des = des;
    this->classType = classType;
    this->name = n;
    this->costMp = cMp;
    this->costHp = cHp;
    this->setHp = sHp;
    this->setMp = sMp;
    this->effect = effect;
    this->targetType = targetType;
    this->costType = cType;
}

void Skill::operator=(Skill s) {
    this->id = s.id;
    this->name = s.name;
    this->costMp = s.costMp;
    this->costHp = s.costHp;
    this->setHp = s.setHp;
    this->setMp = s.setMp;
    this->costType = s.costType;
}

void Skill::print() {
    cout << name << " ";
    switch (costType) {
    case 0:
        cout << "(MP: " << costMp << ") ";
        break;
    case 1:
        cout << "(HP: " << costHp << ") ";
        break;
    case 2:
        cout << "(Set HP to " << setHp << ") ";
        break;
    case 3:
        cout << "(Set MP to " << setMp << ") ";
        break;
    }
    cout << endl;
    cout << des << endl;
}

class Character {
protected:
    static const int EXP_LV = 100;
    string name;
    char activeCode;
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
    int target;
    vector<Skill> skills;
    void levelUp(int hInc, int mInc, int pInc, int kInc, int lInc);

public:
    Character();
    Character(string n, int lv, int h, int maxMp, int po, int kn, int lu);
    virtual void print();
    virtual void beatMonster(int exp);
    virtual int getHP() { return hp; }
    virtual void setHP(int hp) { this->hp = max(hp, 0); }
    virtual int getMP() { return mp; }
    virtual void setMP(int mp) { this->mp = max(mp, 0); }
    virtual int getLuck() { return luck; }
    virtual int getAttack() = 0;
    virtual int getClass() { return 0; };
    virtual void addSkill(Skill s) { skills.push_back(s); }
    virtual int getSkillCount() { return skills.size(); }
    virtual int getTarget() { return target; }
    virtual int setTarget(int t) { target = t; }
    virtual void printActive();
    virtual void active(char activeCode,  Character* monster, Team* playerTeam) = 0;
    virtual bool useSkill(int skillIndex, Team* playerTeam);
    string getName();
    void heal(int amount) { 
        int updateHp = min(this->maxHp, this->hp + amount); 
        this->hp = updateHp;
    }
    void replyMP(int amount) { 
        int updateMp = min(this->maxMp, this->mp + amount); 
        this->mp = updateMp;
    }
    void boostPower() { powerBoost++; }
    void boostKnowledge() { knowledgeBoost++; }
};

Character::Character(){
    this->name = "No Name";
    this->level = 1;
    this->exp = 0;
    this->power = 0;
    this->knowledge = 0;
    this->luck = 0;
    this->maxHp = 0;
    this->hp = this->maxHp;
    this->maxMp = 0;
    this->mp = this->maxMp;
    this->powerBoost = 0;
    this->knowledgeBoost = 0;
    this->activeCode = '0';
    this->target = -1;
}

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
    this->activeCode = '0';
    this->target = -1;
}

string Character::getName() {
    return this->name;
}

void Character::levelUp(int hInc, int mInc, int pInc, int kInc, int lInc) {
    this->level++;
    this->maxHp += hInc;
    this->maxMp += mInc;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;

    // Reset hp and mp
    this->hp = this->maxHp;
    this->mp = this->maxMp;
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
        cout << " + " << this->powerBoost * 5 << "%";

    cout << " knowlodge: " << this->knowledge;
    if(this->knowledgeBoost > 0)
        cout << " + " << this->knowledgeBoost * 5 << "%";

    cout << " luck: " << this->luck << "\n";

    if(this->skills.size() > 0){
        cout << "skills: ";
        for(int i = 0; i < this->skills.size(); i++){
            cout << i + 1 << ". ";
            cout << this->skills[i].getName() << " ";
        }
        cout << endl;
    }
    
}

void Character::printActive() {
    cout << "( 1. Attack, 2. Defend";
    if(this->skills.size() > 0){
        char skillCount = 'A';
        for(int i = 0; i < this->skills.size(); i++){
            cout << ", " << skillCount << ". " << this->skills[i].getName();
            switch (skills[i].getcostType())
            {
                case 0:
                    cout << " [MP" << skills[i].getCostMp() << "]";
                    break;
                case 1:
                    cout << " [HP" << skills[i].getCostHp() << "]";
                    break;
                case 2:
                    cout << " [Set HP to " << skills[i].getSetHp() << "]";
                    break;
                case 3:
                    cout << " [Set MP to " << skills[i].getSetHp() << "]";
                    break;
            }
            skillCount++;
        }
    }
    cout << " ): ";
}

void Character::beatMonster(int exp) {
    this->exp += exp;
    while (this->exp >= pow(this->level, 2) * EXP_LV)
        this->levelUp(0, 0, 0, 0, 0);
}

class Warrior : public Character {
private:
    static const int HP_INIT= 60;
    static const int MP_INIT= 30;
    static const int PO_INIT= 17;
    static const int KN_INIT= 0;
    static const int LU_INIT= 5;

    static const int HP_LV = 30;
    static const int MP_LV = 20;
    static const int PO_LV = 7;
    static const int KN_LV = 0;
    static const int LU_LV = 5;
    

public:
    Warrior(string n, int lv = 1)
    : Character(n, lv, HP_INIT + (lv - 1) * HP_LV, MP_INIT + (lv - 1) * MP_LV, PO_INIT + (lv - 1) * PO_LV, KN_INIT + (lv - 1) * KN_LV, LU_INIT + (lv - 1) * LU_LV) {}
    void print() { cout << "Warrior "; Character::print(); }
    int getClass() { return 1; }
    void addSkill(Skill s) { skills.push_back(s); }
    void active(char activeCode, Character* monster);
    void active(char activeCode, Character* monster, Team* playerTeam) {active(activeCode, monster);};
    int getAttack() { return power * (1 + powerBoost*0.05); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, MP_LV, PO_LV, KN_LV, LU_LV);
    }
};

void Warrior::active(char activeCode, Character* monster) {
    if(activeCode == '1'){
        monster->setHP(monster->getHP() - this->getAttack());
        cout << this->getName() << " attacks the monster for " << this->getAttack() << " damage!\n";
    } else if (activeCode == '2') {
        cout << this->getName() << " is defending!\n";
    } else {
        int skillIndex = activeCode - 'A';
        Skill skill = this->skills[skillIndex];

        if(skill.getTargetType() == 0){
            int totalDamage = this->getAttack() * skill.getEffect();
            monster->setHP(monster->getHP() - totalDamage);
            cout << this->getName() << " uses " << skill.getName() << " attacks the monster for " << totalDamage << " damage!\n";
        }
    }
}

class Wizard : public Character {
private:
    static const int HP_INIT= 40;
    static const int MP_INIT= 60;
    static const int PO_INIT= 4;
    static const int KN_INIT= 15;
    static const int LU_INIT= 7;

    static const int HP_LV = 20;
    static const int MP_LV = 10;
    static const int PO_LV = 4;
    static const int KN_LV = 10;
    static const int LU_LV = 7;

public:
    Wizard(string n, int lv = 1) 
    : Character(n, lv, HP_INIT + (lv - 1) * HP_LV, MP_INIT + (lv - 1) * MP_LV, PO_INIT + (lv - 1) * PO_LV, KN_INIT + (lv - 1) * KN_LV, LU_INIT + (lv - 1) * LU_LV) {}
    void print() { cout << "Wizard "; Character::print(); }
    int getClass() { return 2; }
    void active(char activeCode, Character* monster);
    void active(char activeCode, Character* monster, Team* playerTeam) {active(activeCode, monster);};
    int getAttack() { return knowledge * (1 + knowledgeBoost*0.05); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, MP_LV, PO_LV, KN_LV, LU_LV);
    }
};

void Wizard::active(char activeCode, Character* monster) {
    if(activeCode == '1'){
        monster->setHP(monster->getHP() - this->getAttack());
        cout << this->getName() << " attacks the monster for " << this->getAttack() << " damage!\n";
    } else if (activeCode == '2') {
        cout << this->getName() << " is defending!\n";
    } else {
        int skillIndex = activeCode - 'A';
        Skill skill = this->skills[skillIndex];

        if(skill.getTargetType() == 0){
            int totalDamage = this->getAttack() * skill.getEffect();
            monster->setHP(monster->getHP() - totalDamage);
            cout << this->getName() << " uses " << skill.getName() << " attacks the monster for " << totalDamage << " damage!\n";
        }
    }
}

class Healer : public Character {
private:
    static const int HP_INIT= 30;
    static const int MP_INIT= 30;
    static const int PO_INIT= 1;
    static const int KN_INIT= 20;
    static const int LU_INIT= 5;

    static const int HP_LV = 20;
    static const int MP_LV = 10;
    static const int PO_LV = 1;
    static const int KN_LV = 20;
    static const int LU_LV = 7;

public:
    Healer(string n, int lv = 1) 
    : Character(n, lv, HP_INIT + (lv - 1) * HP_LV, MP_INIT + (lv - 1) * MP_LV, PO_INIT + (lv - 1) * PO_LV, KN_INIT + (lv - 1) * KN_LV, LU_INIT + (lv - 1) * LU_LV) {}
    void print() { cout << "Healer "; Character::print(); }
    int getClass() { return 3; }
    void active(char activeCode, Character* monster, Team* playerTeam);
    int getHeal() { return knowledge + (1 + knowledgeBoost*0.05); }
    int getAttack() { return power * (1 + powerBoost*0.05); }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, MP_LV, PO_LV, KN_LV, LU_LV);
    }
};



class Monster : public Character {
private:
    static const int HP_INIT= 80;
    static const int MP_INIT= 0;
    static const int PO_INIT= 5;
    static const int KN_INIT= 0;
    static const int LU_INIT= 5;

    static const int HP_LV = 80;
    static const int MP_LV = 100;
    static const int PO_LV = 5;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Monster(string n, int lv = 1);
    Monster(string n, int hp, int power) : Character(n, 10, hp, 100, power, 10, 5) {}
    void print() { cout << "Monster: HP=" << hp << ", Attack=" << power << endl; }
    int getAttack() { return power; }
    int getExp(int luck);
    int getMoney(int luck);
    void active(char activeCode, Character* monster, Team* playerTeam){};

};

Monster::Monster(string n, int lv){
    this->name = n;
    this->level = lv;
    this->power = PO_INIT + (lv - 1) * PO_LV * (1 + lv / 3);
    this->maxHp = HP_INIT + (lv - 1) * HP_LV * (1 + lv / 3);
    this->hp = this->maxHp;
}

int Monster::getExp(int luck) {
    return this->level * 400 + rand() % luck;
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
    void addHealer(string name, int lv);
    void heal(int id, int amount);
    void replyMP(int id, int amount);
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

void Team::addHealer(string name, int lv)
{
    if(memberCount < MAX_TEAM_MEMBER)
    {
        member[memberCount] = new Healer(name, lv);
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

void Team::replyMP(int id, int amount) {
    member[id - 1]->replyMP(amount);
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

bool Character::useSkill(int skillIndex, Team* playerTeam) {
    if (skillIndex < 0 || skillIndex >= skills.size()) {
        return false;
    }

    Skill skill = skills[skillIndex];
    if (skill.getcostType() == 0) {
        if (this->getMP() < skill.getCostMp()) {
            cout << "Not enough MP!\n";
            return false;
        }
        this->setMP(this->getMP() - skill.getCostMp());
    } else if (skill.getcostType() == 1) {
        if (this->getHP() < skill.getCostHp()) {
            cout << "Not enough HP!\n";
            return false;
        }
        this->setHP(this->getHP() - skill.getCostHp());
    } else if (skill.getcostType() == 2) {
        this->setHP(skill.getSetHp());
    } else if (skill.getcostType() == 3) {
        this->setMP(skill.getSetHp());
    }

    if (skill.getTargetType() == 2) {
        cout << "Choose a member to use skill: ";
        int target = limitChoose(playerTeam->getMemberCount(), 1);
        this->setTarget(target);
    }

    return true;
}

void Healer::active(char activeCode, Character* monster, Team* playerTeam){
    if(activeCode == '1'){
        monster->setHP(monster->getHP() - this->getAttack());
        cout << this->getName() << " attacks the monster for " << this->getAttack() << " damage!\n";
    } else if (activeCode == '2') {
        cout << this->getName() << " is defending!\n";
    } else {
        int skillIndex = activeCode - 'A';
        Skill skill = this->skills[skillIndex];

        if(skill.getTargetType() == 0){
            int totalDamage = this->getAttack() * skill.getEffect();
            monster->setHP(monster->getHP() - totalDamage);
            cout << this->getName() << " uses " << skill.getName() << " attacks the monster for " << totalDamage << " damage!\n";
        } else if (skill.getTargetType() == 2) {
            int totalHeal = this->getHeal() * skill.getEffect();
            playerTeam->heal(this->getTarget(), totalHeal);
            cout << this->getName() << " uses " << skill.getName() << " heal " << playerTeam->getMember(this->getTarget() - 1)->getName() << " for " << totalHeal << " HP!\n";
        } else if (skill.getTargetType() == 3) {
            int totalHeal = this->getHeal() * skill.getEffect();
            for (int i = 1; i <= playerTeam->getMemberCount(); i++) {
                playerTeam->heal(i, totalHeal);
            }
            cout << this->getName() << " uses " << skill.getName() << " heal Team for " << totalHeal << " HP!\n";
        }
    }
}

// wait for user to press enter
void wait()
{
    cout << "Press anyKey to continue..." << endl;
    getch();
}

void wait(string s)
{
    cout << s << endl;
    getch();
}

// get a random number from 0 to maxNum, except limitNum
int getRandenNum(int maxNum, int limitNum) {
    int randenNum = rand() % maxNum;
    while (randenNum == limitNum) {
        randenNum = rand() % 3;
    }
    return randenNum;
}

// get a random number from 0 to maxNum, except limitList
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

// limit the choice to a valid number
int limitChoose(int maxNum, int minNum) {
    char choice;
    cin >> choice;
    while (choice < minNum + '0' || choice > maxNum + '0') {
        cout << "Invalid choice. Choose again: ";
        cin >> choice;
    }
    return choice - '0';
}

// print the skill list
void printSkillList(vector<Skill> skills) {
    for (int i = 0; i < skills.size(); i++) {
        cout << i + 1 << ". ";
        skills[i].print();
    }
}

// get the skill list by class type
vector<Skill>* getskillsByClass(vector<Skill> skills, int classType) {
    vector<Skill>* skillList = new vector<Skill>();
    for (int i = 0; i < skills.size(); i++) {
        if (skills[i].getClassType() == classType) {
            skillList->push_back(skills[i]);
        }
    }
    return skillList;
}

// battle function
bool battle(Team* playerTeam, Monster* monster, int& money) {
    bool playerLose = false;

    int memberCount = playerTeam->getMemberCount();
    char actions[MAX_TEAM_MEMBER];
    int unableToSelect[MAX_TEAM_MEMBER];

    // Initialize actions and unableToSelect
    for (int i = 0; i < MAX_TEAM_MEMBER; ++i) {
        if (i >= memberCount || playerTeam->getMember(i) == nullptr ||
            playerTeam->getMember(i)->getHP() <= 0) {
            unableToSelect[i] = i;
            actions[i] = '!';
        } else {
            unableToSelect[i] = -1;
            actions[i] = '0';
        }
    }

    // Start battle
    cout << "A wild " << monster->getName() << " appears!\n";
    monster->print();
    playerTeam->print();
    wait("Press enter to start the battle...");
    cout << "\033[2J\033[1;1H";

    // Battle loop
    while (monster->getHP() > 0 && !playerLose) {
        // Monster's turn
        monster->print();
        int targetIndex = getRandenNum(memberCount, unableToSelect, memberCount);
        int totalDamage = monster->getAttack();

        // check if the target is defending
        if (actions[targetIndex] == '2') {
            totalDamage *= 0.2;
        }

        Character* target = playerTeam->getMember(targetIndex);
        target->setHP(target->getHP() - totalDamage);
        cout << "Monster attacks " << targetIndex + 1 << ". " << target->getName() << " for " << totalDamage << " damage!\n" << endl;

        // Check if player lost
        playerLose = true;
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (playerTeam->getMember(i)->getHP() > 0) {
                playerLose = false;
                break;
            }
        }
        if (playerLose) break;

        // Player's turn
        cout << "Choose actions for your team:\n";
        playerTeam->print();
        cout << "\n";

        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            Character* member = playerTeam->getMember(i);

            if (member->getHP() <= 0) {
                cout << member->getName() << " is down!\n";
                unableToSelect[i] = i;
                actions[i] = '!';
                continue;
            }

            cout << i + 1 << ". " << member->getName();
            member->printActive();
            cin >> actions[i];

            while (1) {      
                // if is alpha or lower case, convert to upper case
                if(isalpha(actions[i]) && islower(actions[i])) actions[i] = toupper(actions[i]);

                if (actions[i] == '1' || actions[i] == '2') break;

                int skillIndex = actions[i] - 'A';
                if (actions[i] >= 'A' && actions[i] <= 'A' + member->getSkillCount() &&
                    member->useSkill(skillIndex, playerTeam)) break;

                cout << "Invalid action. Choose again: ";
                cin >> actions[i];
            }
        }

        // Player's skill effects
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (actions[i] == '!') continue;

            Character* member = playerTeam->getMember(i);

            cout << i + 1 << ". ";

            member->active(actions[i], monster, playerTeam);
        }

        monster->print();
        wait();

        // Check if monster is defeated
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

    // Print battle result
    if (playerLose) {
        cout << "Your team has been defeated...\n";
    } else {
        cout << endl;
        cout << "Victory!\n";
        playerTeam->print();
    }

    return playerLose;
}

// shop function
void shop(Team* playerTeam, int& money) {
    const int shopItem = 3;
    int memberChoice = -1;
    cout << "Welcome to the shop!\n";

    // Shop loop
    while (true)
    {
        playerTeam->print();
        cout << "You have " << money << " gold\n";
        cout << "1. Heal Potion +50HP (20 gold)\n";
        cout << "2. MP potion +50MP (20 gold)\n";
        cout << "3. Power Boost +5%Power (50 gold)\n";
        cout << "4. knowledge Boost +5%knoledge (50 gold)\n";
        cout << "0. Exit shop\nChoose an option: ";
        int choice;
        cin >> choice;

        // Exit shop
        if (choice == 0)
        {
            cout << "Goodbye!\n";
            wait();
            break;
        }

        // Buy item
        switch (choice)
        {
        case 1:
            // Check if player has enough gold
            if (money < 20)
            {
                cout << "Not enough gold!\n";
                wait();
                continue;
            }

            cout << "\n" << endl;
            playerTeam->print();
            cout << "choose a member to heal: ";
            memberChoice = limitChoose(playerTeam->getMemberCount(), 1); // Limit the choice to a valid member

            // Heal
            playerTeam->heal(memberChoice, 50);
            cout << "Member " << memberChoice << " has been healed\n";
            playerTeam->printMember(memberChoice);

            money -= 20;

            break;

        case 2:
            // Check if player has enough gold
            if (money < 20)
            {
                cout << "Not enough gold!\n";
                wait();
                continue;
            }

            cout << "\n" << endl;
            playerTeam->print();
            cout << "choose a member to heal: ";
            memberChoice = limitChoose(playerTeam->getMemberCount(), 1); // Limit the choice to a valid member

            // restore MP
            playerTeam->replyMP(memberChoice, 50);
            cout << "Member " << memberChoice << "'s MP has been restored!\n";
            playerTeam->printMember(memberChoice);

            money -= 20;

            break;

        case 3:
            // Check if player has enough gold
            if (money < 50)
            {
                cout << "Not enough gold!\n";
                wait();
                continue;
            }

            cout << "\n" << endl;
            playerTeam->print();
            cout << "choose a member to boost poser: ";
            memberChoice = limitChoose(playerTeam->getMemberCount(), 1); // Limit the choice to a valid member

            // Boost power
            playerTeam->boostPower(memberChoice);
            cout << "Member " << memberChoice << " has been boost\n";
            playerTeam->printMember(memberChoice);

            money -= 50;

            break;

        case 4:
            // Check if player has enough gold
            if (money < 50)
            {
                cout << "Not enough gold!\n";
                wait();
                continue;
            }

            cout << "\n" << endl;
            playerTeam->print();
            cout << "choose a member to boost poser: ";
            memberChoice = limitChoose(playerTeam->getMemberCount(), 1); // Limit the choice to a valid member

            // Boost knowledge
            playerTeam->boostKnowledge(memberChoice);
            cout << "Member " << memberChoice << " has been boost\n";
            playerTeam->printMember(memberChoice);

            money -= 50;

            break;

        default:
            // Invalid choice
            cout << "Invalid choice!\n";
            wait();
            cout << "\033[2J\033[1;1H";
            continue;
        }

        cout << "You now have " << money << " gold\n";
        wait();
        cout << "\033[2J\033[1;1H";
    }

}

// summon function
void summon(Team* playerTeam, int& money, int& monsterCount) {
    const int summonCost = 50 + monsterCount * 10; // Increase cost with each monster

    cout << "You have a chance to summon hero!\n";
    cout << "You have " << money << " gold\n";
    cout << "1. Warrior (" << summonCost << " gold)\n";
    cout << "2. Wizard (" << summonCost << " gold)\n";
    cout << "3. Healer (" << summonCost << " gold)\n";
    cout << "0. Exit\n";
    cout << "Choose the hero you want to summon:\n";
    int choice = limitChoose(3, 0); // Limit the choice to 1, 2, or 3

    // Exit
    if (choice == 0) {
        return;
    }

    // Check if player has enough gold
    if (money < summonCost) {
        cout << "Not enough gold!\n";
        wait();
        return;
    }

    // Get new member name
    cout << "Enter new member name: ";
    string name;
    cin.ignore();
    getline(cin, name);

    cout << "\033[2J\033[1;1H";

    money -= summonCost; // Reduce gold

    // Add hero to team and print
    switch (choice) {
    case 1:
        playerTeam->addWarrior(name, monsterCount + 1);
        break;
    case 2:
        playerTeam->addWizard(name, monsterCount + 1);
        break;
    case 3:
        playerTeam->addHealer(name, monsterCount + 1);
        break;
    }

    playerTeam->print();
    wait();
}

// training function
void training(Team* playerTeam, int& money, vector<Skill> skills) {
    cout << "You have a chance to train your hero!\n";
    cout << "You have " << money << " gold\n";
    cout << "Do you want to learn a skill? (100 gold)\n";
    cout << "Y/N: ";
    char wantTrain;
    cin >> wantTrain;

    if (wantTrain == 'N' || wantTrain == 'n') {
        return;
    }

    if (money < 100) {
        cout << "Not enough gold!\n";
        wait();
        return;
    }

    cout << "\033[2J\033[1;1H";

    playerTeam->print();
    cout << "Choose the hero you want to train: ";
    int choice;
    choice = limitChoose(playerTeam->getMemberCount(), 1);

    Character* chosenHero = playerTeam->getMember(choice - 1);

    vector<Skill>* skillsByClass = getskillsByClass(skills, chosenHero->getClass());
    printSkillList(*skillsByClass);
    cout << "Choose the skill you want to learn: ";
    int skillChoice = limitChoose(skillsByClass->size(), 1);
    chosenHero->addSkill(skillsByClass->at(skillChoice - 1));

    money -= 100;
    cout << "You now have " << money << " gold\n";

    playerTeam->print();
    wait();

    delete skillsByClass;
}

// Initialize skills
vector<Skill> initializeSkills() {
    return {
        Skill("Warrior roar", "Deals danage equal to 200% power damage", 1, 30, 0, 0, 0, 2, 0, 0, 1),
        Skill("Last Hope", "Set HP to 0 to deal 1000% power damage", 2, 0, 0, 0, 0, 10, 0, 2, 1),
        Skill("Wizard Fireball", "Deals danage equal to 200% knowledge", 3, 30, 0, 0, 0, 2, 0, 0, 2),
        Skill("Ice Storm", "Deals danage equal to 300% knowledge", 4, 50, 0, 0, 0, 3, 0, 0, 2),
        Skill("Healer Heal", "Heal one hero equal to 300% knowledge", 5, 30, 0, 0, 0, 3, 2, 0, 3),
        Skill("Super Sanctuary", "Heal all equal to 100% knowledge", 6, 30, 0, 0, 0, 1, 3, 0, 3),
    };
}

// Main function
int main() {
    srand(time(NULL)); // Seed the random number generator
    const int EVENT_AMOUNT = 5; // Number of possible events
    string teamName, leaderName;
    const vector<Skill> skills = initializeSkills(); // Initialize skills

    // Get player team info
    cout << "Enter your Team name: ";
    getline(cin, teamName);
    cout << "Enter your Team leader name: ";
    getline(cin, leaderName);

    cout << "Choose your class:\n1. Warrior\n2. Wizard\nChoose: ";
    int choice = limitChoose(2, 1);
    cout << "\033[2J\033[1;1H";

    Team playerParty(teamName);

    // Add hero to team
    if(choice == 1){
        playerParty.addWarrior(leaderName, 1);
    }
    else{
        playerParty.addWizard(leaderName, 1);
    }

    playerParty.print();
    wait("Press anyKey to start the game...");
    cout << "\033[2J\033[1;1H";

    int money = 100;
    int monsterCount = 0;
    int eventCount = 0;
    int limitEvent[4] = {-1, -1, -1, -1}; // Limit event

    // Game loop
    while (true) {
        int event = getRandenNum(EVENT_AMOUNT, limitEvent, 4); // Get random event 

        // forced to battle without battle three times
        if(eventCount % 3 == 0 && eventCount != 0){
            event = 0;
        }

        cout << "\033[2J\033[1;1H";


        // Check if monster count has reached 10 to battle boss
        if (monsterCount >= 10) {
            Monster boss("Boss", 3600, 1000);
            bool lose = battle(&playerParty, &boss, money); 
            lose ? wait("Game Over!") : wait("You win!");
            break;
        }
        
        bool playerLose = false;
        switch (event) {
            case 0: {
                Monster monster("monster", monsterCount + 1);
                playerLose = battle(&playerParty, &monster, money);
                playerLose ? wait("Game Over!") : wait();
                eventCount = 0; // Reset event count
                for(int i = 0; i < 3; i++) limitEvent[i] = -1; // Reset limit event
                monsterCount++; // Increase monster count
                break;
            }

            // shop
            case 1: {
                shop(&playerParty, money);
                break;
            }

            // treasure
            case 2: {
                money += 50;
                cout << "You found a treasure chest! +50 gold\n";
                cout << "You now have " << money << " gold\n";
                wait();
                break;
            }

            // summon
            case 3: {
                summon(&playerParty, money, monsterCount);

                // Check if player has reached the limit
                if(playerParty.getMemberCount() >= MAX_TEAM_MEMBER){
                    limitEvent[3] = 3;
                }
                break;
            }

            // training
            case 4: {
                training(&playerParty, money, skills);
                break;
            }
        }

        if(playerLose) break; // End game if player loses

        eventCount++;
        limitEvent[eventCount] = event; // Update limit event
    }

    return 0;
}
