
#include "stdafx.h"
#include "string"
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;
static const string  cardDest[54] = {
	"鬼","王",
	"2","3","4","5","6","7","8","9","10","J","Q","K","A",
	"2","3","4","5","6","7","8","9","10","J","Q","K","A",
	"2","3","4","5","6","7","8","9","10","J","Q","K","A",
	"2","3","4","5","6","7","8","9","10","J","Q","K","A",
};

static const string flag[15] = { "3","4","5","6","7","8","9","10","J","Q","K","A","2", "鬼","王" };

const int STATE_WAIT = 0;
const int STATE_START = 1;
const int STATE_BOSSING = 2;
const int STATE_GAMEING = 3;

class Util{
public:
	static int AC;
	static int  findAndRemove(vector<string> &dest, string str);
	static int  find(vector<string> &dest, string str);
	static int  findFlag(string str);
	static int  desc(int a, int b);
	static int  asc(int a, int b);
    static bool  compareCard(const string &carda, const string &cardb);
	static void  trim(string &s);
	static void  toUpper(string &str);
	static void setAC(int32_t ac);
	static int32_t getAC();
};

class Player
{
public:
	Player();
	stringstream msg;
	int64_t number;
	vector<string> card;
	int32_t socre;
	bool isReady;

	void sendMsg();
	void listCards();
	void breakLine();
};



class Desk {
public:

	Desk();
	string cards[54];
	int64_t number;
	vector<Player*> players;

	int state;
	int lastPlayIndex;//当前谁出得牌
	int nextPlayIndex;//该谁出牌
	int bossIndex;//谁是地主

	vector<string> lastCard;//上位玩家的牌
	string lastCardType;//上位玩家得牌类
	vector<int> *lastWeights;//上位玩家的牌

	stringstream msg;

	void join(int64_t playNum);
	void startGame();
	static void gameOver(int64_t number);
	void exit(int64_t playNum);
	void commandList();

	void shuffle();//洗牌
	void deal();//发牌
	void creataBoss();//抢地主
	void getBoss(int64_t playerNum);
	void dontBoss(int64_t playerNum);
	void sendBossCard();
	void play(int64_t playNum, string msg);
	void play(int64_t playNum, vector<string> list);//出牌
	void discard(int64_t playNum);

	void at(int64_t playNum);
	void breakLine();
	int getPlayer(int64_t number);//按qq号获得玩家得索引
	void setNextPlayerIndex();//设置下个出牌得玩家索引
	void listPlayers();
	void listPlayers(int type);
	bool isCanWin(int cardCount, vector<int> *Weights, string type);
	string getMycardType(vector<string> list, vector<int> *Weights);
	void sendMsg();
	void sendPlayerMsg();

};

class Desks {
public:
	vector<Desk*> desks;
	Desk* getOrCreatDesk(int64_t deskNum);
	static void game(int64_t deskNum, int64_t playNum, const char *msg);
	int getDesk(int64_t deskNum);
};