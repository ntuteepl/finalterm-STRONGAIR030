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
void summon(Team* playerTeam, int& money, int& monsterCount);
void training(Team* playerTeam, int& money, vector<Skill> skills);
void printSkillList(vector<Skill> skills);
vector<Skill>* getskillsByClass(vector<Skill> skills, int classType);

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
        Skill(string n, string des, int id, int cMp, int cHp, int sHp, int sMp, double effect ,int targetType, int cType, int classType) {
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
        string getName() { return name; }
        void operator=(Skill s) {
            this->id = s.id;
            this->name = s.name;
            this->costMp = s.costMp;
            this->costHp = s.costHp;
            this->setHp = s.setHp;
            this->setMp = s.setMp;
            this->costType = s.costType;
        }
        int getId() { return id; }
        int getClassType() { return classType; }
        int getCostMp() { return costMp; }
        int getCostHp() { return costHp; }
        int getTargetType() { return targetType; }
        int getSetHp() { return setHp; }
        int getcostType() { return costType; }
        double getEffect() { return effect; }
        void print() {
            cout << name << " " ;
            if(costType == 0){
                cout << "(MP: " << costMp << ") ";
            } else if (costType == 1) {
                cout << "(HP: " << costHp << ") ";
            } else if (costType == 2) {
                cout << "(Set HP to " << setHp << ") ";
            } else if (costType == 3) {
                cout << "(Set MP to " << setMp << ") ";
            }

            cout << endl;
            cout << des << endl;
        }
};

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
    static const int HP_LV = 50;
    static const int MP_LV = 30;
    static const int PO_LV = 10;
    static const int KN_LV = 5;
    static const int LU_LV = 5;
    

public:
    Warrior(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
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
    static const int HP_LV = 30;
    static const int MP_LV = 100;
    static const int PO_LV = 4;
    static const int KN_LV = 15;
    static const int LU_LV = 7;

public:
    Wizard(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
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
    static const int HP_LV = 30;
    static const int MP_LV = 80;
    static const int PO_LV = 1;
    static const int KN_LV = 10;
    static const int LU_LV = 7;

public:
    Healer(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * MP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {}
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
    void active(char activeCode, Character* monster, Team* playerTeam){};

};

int Monster::getExp(int luck) {
    return this->level * 500 + rand() % luck;
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


int limitChoose(int maxNum, int minNum) {
    int choice;
    cin >> choice;
    while (choice < minNum || choice > maxNum) {
        cout << "Invalid choice. Choose again: ";
        cin >> choice;
    }
    return choice;
}

void printSkillList(vector<Skill> skills) {
    for (int i = 0; i < skills.size(); i++) {
        cout << i + 1 << ". ";
        skills[i].print();
    }
}

vector<Skill>* getskillsByClass(vector<Skill> skills, int classType) {
    vector<Skill>* skillList = new vector<Skill>();
    for (int i = 0; i < skills.size(); i++) {
        if (skills[i].getClassType() == classType) {
            skillList->push_back(skills[i]);
        }
    }
    return skillList;
}

bool battle(Team* playerTeam, Monster* monster, int& money) {
    bool playerLose = false;
    int memberCount = playerTeam->getMemberCount();
    char actions[MAX_TEAM_MEMBER];
    int unableToSelect[MAX_TEAM_MEMBER];

    for(int i = 0; i < MAX_TEAM_MEMBER; i++){
        if(i >= memberCount || playerTeam->getMember(i) == nullptr || playerTeam->getMember(i)->getHP() <= 0){
            unableToSelect[i] = i;
            actions[i] = '!';
        } else {
            unableToSelect[i] = -1;
            actions[i] = '0';
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
        if(actions[targetIndex] == '2'){
            totalDamage *= 0.2;
        }
        Character* target = playerTeam->getMember(targetIndex);
        target->setHP(target->getHP() - totalDamage);
        cout << "Monster attacks " << targetIndex + 1 << ". " << target->getName() << " for " << totalDamage << " damage!\n" << endl;

        // ?嚙範??嚙瞌嚙稻嚙踝蕭嚙踝蕭?嚙窯
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
                int skillIndex = actions[i] - 'A';
                if (actions[i] == '1' || actions[i] == '2') break;
                if (actions[i] >= 'A' && actions[i] <= 'A' + member->getSkillCount() && member->useSkill(skillIndex, playerTeam)) break;
                
                cout << "Invalid action. Choose again: ";
                cin >> actions[i];
            }
        }

        // ?嚙賣玩嚙窮嚙踝蕭?
        for (int i = 0; i < playerTeam->getMemberCount(); ++i) {
            if (actions[i] == '!') continue; 

            Character* member = playerTeam->getMember(i);

            cout << i + 1 << ". ";

            member->active(actions[i], monster, playerTeam);
        }

        monster->print();
        wait();

        // ?嚙範嚙褒迎蕭嚙瞌嚙稻嚙瞋??
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
        cout << endl;
        cout << "Victory!\n";
        playerTeam->print();
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
        memberChoice = limitChoose(playerTeam->getMemberCount(), 1);
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
        memberChoice = limitChoose(playerTeam->getMemberCount(), 1);
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
        memberChoice = limitChoose(playerTeam->getMemberCount(), 1);
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
    cout << "3. Healer (" << 50 + monsterCount * 10 << " gold)\n";
    cout << "0. Exit\n";
    cout << "Choose the hero you want to summon:\n";
    int choice = limitChoose(3, 0);

    if (money < 50 + monsterCount * 10) {
        cout << "Not enough gold!\n";
        wait();
        return;
    }

    if(choice == 0){
        return;
    }

    cout << "Enter new member name: ";
    string name;
    cin.ignore();
    getline(cin, name);

    cout << "\033[2J\033[1;1H";

    money -= 50 + monsterCount * 10;

    if(choice == 1){
        playerTeam->addWarrior(name, monsterCount + 1);
    } else if (choice == 2) {
        playerTeam->addWizard(name, monsterCount + 1);
    } else if (choice == 3) {
        playerTeam->addHealer(name, monsterCount + 1);
    }
    playerTeam->print();
    wait();
}

void training(Team* playerTeam, int& money, vector<Skill> skills) {
    cout << "You have a chance to train your hero!\n";
    cout << "You have " << money << " gold\n";
    cout << "Do yout want learn some skill? (100 gold)\n";
    cout << "Y/N: ";
    char wantTrain;
    cin >> wantTrain;

    if (wantTrain == 'N' || wantTrain == 'n') {
        return;
    }

    if(money < 100){
        cout << "Not enough gold!\n";
        wait();
        return;
    }

    playerTeam->print();
    cout << "Choose the hero you want to train: ";
    int choice;
    choice = limitChoose(playerTeam->getMemberCount(), 1);
    cout << "\033[2J\033[1;1H";

    Character* choosedHero = playerTeam->getMember(choice - 1);
    
    vector<Skill>* skillsByClass = getskillsByClass(skills, choosedHero->getClass());
    printSkillList(*skillsByClass);
    cout << "Choose the skill you want to learn:\n";
    int skillChoice = limitChoose(skillsByClass->size(), 1);
    choosedHero->addSkill(skillsByClass->at(skillChoice - 1));

    money -= 100;
    cout << "You now have " << money << " gold\n";

    playerTeam->print();
    wait();

    delete skillsByClass;
}

vector<Skill> initializeSkills() {
    return {
        Skill("Warrior roar", "Deals danage equal to 200% max HP", 1, 30, 0, 0, 0, 2, 0, 0, 1),
        Skill("Last Hope", "Set HP to 1 to deal 1000% power damage", 2, 0, 0, 1, 0, 10, 0, 2, 1),
        Skill("Wizard Fireball", "Deals danage equal to 200% knowledge", 3, 30, 0, 0, 0, 2, 0, 0, 2),
        Skill("Ice Storm", "Deals danage equal to 300% knowledge", 4, 50, 0, 0, 0, 3, 0, 0, 2),
        Skill("Healer Heal", "Heal one hero equal to 300% knowledge", 5, 30, 0, 0, 0, 3, 2, 0, 3),
        Skill("Super Sanctuary", "Heal all equal to 100% knowledge", 6, 30, 0, 0, 0, 1, 3, 0, 3),
    };
}

int main() {
    srand(time(0));
    const int EVENT_AMOUNT = 5;
    string teamName, leaderName;
    const vector<Skill> skills = initializeSkills();

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
        playerParty.addWarrior(leaderName, 10);
    }
    else{
        playerParty.addWizard(leaderName, 10);
    }

    playerParty.addWarrior("tool1", 10);
    playerParty.addHealer("tool2", 10);
    playerParty.addHealer("tool3", 10);

    
    playerParty.getMember(0)->addSkill(skills[2]);
    playerParty.getMember(1)->addSkill(skills[1]);
    playerParty.getMember(2)->addSkill(skills[4]);
    playerParty.getMember(2)->addSkill(skills[5]);
    playerParty.getMember(3)->addSkill(skills[4]);
    playerParty.getMember(3)->addSkill(skills[5]);
    


    playerParty.print();
    wait("Press enter to start the game...");
    cout << "\033[2J\033[1;1H";

    int money = 1000;
    int monsterCount = 10;
    int eventCount = 0;
    int limitEvent[2] = {-1, -1};

    while (1) {
        int event = getRandenNum(EVENT_AMOUNT, limitEvent, EVENT_AMOUNT);
        if(eventCount % 3 == 0 && eventCount != 0){
            event = 0;
        }

        cout << "\033[2J\033[1;1H";

        if (monsterCount >= 10) {
            Monster boss("Boss", 30);
            bool lose = battle(&playerParty, &boss, money); 
            lose ? wait("Game Over!") : wait("You win!");
            break;
        }

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
            if(playerParty.getMemberCount() >= MAX_TEAM_MEMBER){
                limitEvent[1] = 3;
            }
        } else if (event == 4) {
            training(&playerParty, money, skills);
        }


        eventCount++;
        limitEvent[0] = event;
    }

    return 0;
}
