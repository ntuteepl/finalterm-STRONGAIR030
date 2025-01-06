[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/_v8RbUGg)

# 遊戲說明

***

## 如何執行遊戲

編譯final.cpp 然後執行，就是這麼簡單!!

***

## 遊戲方式

輸入隊伍名稱、選則初始職業後開始遊戲->
隨機事件直到打完10隻怪->
打boss->遊戲結束

***

## 職業

有3種職業
1. Warrior(戰士)
2. Wizard(法師)
3. Healer(補師)

### 1. Warrior(戰士)

#### 初始數值:
- HP: 60
- MP: 30
- POWER: 17
- KNOWLEDGE: 0
- LUCKY: 5

#### 每等級加成
- HP: 30
- MP: 20
- POWER: 7
- KNOWLEDGE: 0
- LUCKY: 5

#### 普通攻擊
Warrior的普通傷害是1倍的POWER

#### 技能
1. Warrior roar (MP30)  
造成200%POWER的傷害
3. Last Hope (Set HP to 0)  
犧牲所有HP來造成10倍POWER的傷害

### 2. Wizard(法師)

#### 初始數值:
- HP: 40
- MP: 60
- POWER: 4
- KNOWLEDGE: 15
- LUCKY: 7

#### 每等級加成
- HP: 20
- MP: 10
- POWER: 4
- KNOWLEDGE: 10
- LUCKY: 7

#### 普通攻擊
Wizard的普通傷害是1倍的KNOWLEDGE

#### 技能
1. Wizard Fireball (MP30)  
造成200%KNOWLEDGE的傷害
3. Ice Storm (MP50)  
造成300%KNOWLEDGE的傷害

### 3. Healer(補師)



#### 初始數值:
- HP: 30
- MP: 30
- POWER: 1
- KNOWLEDGE: 20
- LUCKY: 5

#### 每等級加成
- HP: 20
- MP: 10
- POWER: 1
- KNOWLEDGE: 20
- LUCKY: 7

#### 普通攻擊
Healer的普通傷害是1倍的POWER

#### 技能
1. Healer Heal (MP30)  
選擇一位隊友(可以是自己)造成2倍KNOWLEDGE的治療量
3. Super Sanctuary (MP50)    
對所有隊友(包括自己)造成1倍KNOWLEDGE的治療量

***

## 隨機事件

有5種隨機事件:
1. 商店
2. 召喚英雄
3. 訓練
4. 撿到錢
5. 打怪

你不會在三個隨機事件中遇到相同的隨機事件(除了打怪)

### 1. 商店

商品種類&效果:
1. Heal Potion +50HP (20gold)
2. MP Potion +50MP (20gold)
3. Power Boost +5%POWER (50gold)
4. Knowledge Boost +5%KNOWLEDGE (50gold)

你可以重複買東西直到你退出商店

### 2. 召喚英雄

你可以花 50 + 打怪數量 $\times$ 10 的金幣來召喚英雄  
英雄等級會是打怪數量+1的等級

有3種職業
1. Warrior(戰士)
2. Wizard(法師)
3. Healer(補師)

### 3. 訓練

你可以花100的金幣來訓練英雄也就是獲得技能  
所有的技能都必須由訓練來得到，只能獲得要訓練的英雄他自己職業的技能

技能請看職業說明

### 4. 撿到錢

對就是撿到錢!!  
+50金幣，就是這麼簡單

### 5. 打怪

如果三個隨機事件內沒有打怪會強制打怪
每次遇到的怪物都會變強

#### 怪物數值

初始數值:
- HP: 80
- ATTACK: 5

遇到次數數值加成
- HP: 80 + (打怪次數/3)%
- ATTACK: 5 + (打怪次數/3)%

#### 戰鬥方式

怪物先攻
怪物隨機對一位英雄造成1倍ATTACK的傷害

之後可以對HP沒有歸零的英雄操作

操作方式有:
1. Attack 普通攻擊
2. Defend 防禦 如果下回合被攻擊到減傷80%
3. 技能 可以使用有學習過的技能

戰鬥會一直持續直到怪物死亡或所有英雄HP歸零

如果所有英雄HP歸零 GAME OVER!!  
如果怪物死亡將會獲得:  
400 $\times$ 打怪次數的exp 和 50 $\times$ 打怪次數+1的gold
exp會平分給所有英雄
英雄升級將會回復所有HP和MP

***

## Boss戰

當你打完10隻怪後會直接進入Boss戰

### Boss數值
初始數值:
- HP: 3600
- ATTACK: 1020

戰鬥方式和隨機事件中的打怪一樣


***




  




