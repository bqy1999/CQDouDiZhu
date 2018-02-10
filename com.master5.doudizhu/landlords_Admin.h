#pragma once

#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tchar.h>  
#include <regex> 

#include "landlords_classes.h"
using namespace std;

int64_t Admin::readAdmin()
{
	wstring model = L"admin";
	wstring key = L"admin";
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
}

bool Admin::isAdmin(int64_t playNum)
{
	if (playNum == Admin::readAdmin()) {
		return true;
	}
	else {
		wstring msg;
		msg = L"������Ͳ��ǹ���Ա��";
		Util::sendPrivateMsg(playNum, Util::wstring2string(msg).data());
	}
}

wstring Admin::readString() {
	WCHAR tmp[15];
	GetPrivateProfileString(L"admin", L"admin", L"", tmp, 15, CONFIG_PATH.c_str());
	return wstring(tmp);
}

bool Admin::allotScoreTo(wstring msg, int64_t playNum)
{
	//��������
	int score;
	int64_t playerNum;

	wsmatch mr;
	wstring::const_iterator src_it = msg.begin(); // ��ȡ��ʼλ��
	wstring::const_iterator src_end = msg.end(); // ��ȡ����λ��
	regex_search(src_it, src_end, mr, numberReg);
	wstringstream ss;
	ss << mr[0].str();
	ss >> playerNum;
	ss.str(L"");
	src_it = mr[0].second;
	regex_search(src_it, src_end, mr, numberReg);
	wstringstream scoress;
	scoress << mr[0].str();
	scoress >> score;
	scoress.str(L"");

	return Admin::isAdmin(playNum) && Admin::writeScore(playerNum, score+500000000);
}

bool Admin::allotScoreTo2(wstring msg, int64_t playNum)
{
	//���为��
	int score;
	int64_t playerNum;

	wsmatch mr;
	wstring::const_iterator src_it = msg.begin(); // ��ȡ��ʼλ��
	wstring::const_iterator src_end = msg.end(); // ��ȡ����λ��
	regex_search(src_it, src_end, mr, numberReg);
	wstringstream ss;
	ss << mr[0].str();
	ss >> playerNum;
	ss.str(L"");
	src_it = mr[0].second;
	regex_search(src_it, src_end, mr, numberReg);
	wstringstream scoress;
	scoress << mr[0].str();
	scoress >> score;
	score = -score;
	scoress.str(L"");

	return Admin::isAdmin(playNum) && Admin::writeScore(playerNum, score+500000000);
}

bool Admin::gameOver(wstring msg, int64_t playNum)
{
	if (Admin::isAdmin(playNum)) {
		wstringstream ss;
		ss << msg.substr(4, msg.size());
		int64_t destNum;
		ss >> destNum;
		ss.str(L"");
		casino.gameOver(destNum);
		return true;
	}
	return false;
}

bool Admin::writeAdmin(int64_t playerNum)
{
	wstring model = L"admin";
	wstring key = L"admin";
	wstringstream ss;
	ss << playerNum;
	wstring value = ss.str();
	ss.str(L"");
	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());

}

int64_t Admin::readScore(int64_t playerNum)
{
	wstring model = L"score";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");

	//���Ӹ��ֹ��ܣ����ֵ��5�ڷ֣�������������δ�ҵ�ʱ���ص�Ĭ��ֵ��
	//����ֱ�������bug�����������Ҫʹ��desk�е�readScore������
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 500000000, CONFIG_PATH.c_str());
}

bool Admin::getScore(int64_t playerNum)
{
	wstring model = L"time";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	int64_t lastGetScoreTime = GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
	time_t rawtime;
	int64_t now = time(&rawtime);

	if (now / (24 * 60 * 60) > lastGetScoreTime / (24 * 60 * 60)) {
		Admin::addScore(playerNum, CONFIG_INIT_SCORE);
		ss << now;
		wstring value = ss.str();
		ss.str(L"");
		return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
	}
	return false;
}

bool Admin::writeScore(int64_t playerNum, int64_t score)
{
	wstring model = L"score";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	ss << score;
	wstring value = ss.str();
	ss.str(L"");

	//�������ݿ�汾
	Admin::writeVersion();

	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
}

bool Admin::addScore(int64_t playerNum, int score) {
	int64_t hasScore = Admin::readScore(playerNum); //����ʹ��desk��ĺ���
	hasScore += score;
	if (hasScore < 0) {
		hasScore = 0;
	}
	else if (hasScore > 1000000000) {
		hasScore = 1000000000;
	}
	return  Admin::writeScore(playerNum, hasScore);
}

int64_t Admin::readWin(int64_t playerNum)
{
	wstring model = L"win";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
}

int64_t Admin::readLose(int64_t playerNum)
{
	wstring model = L"lose";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
}

bool Admin::writeWin(int64_t playerNum, int64_t win)
{
	wstring model = L"win";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	ss << win;
	wstring value = ss.str();
	ss.str(L"");
	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
}

bool Admin::writeLose(int64_t playerNum, int64_t lose)
{
	wstring model = L"lose";
	wstringstream ss;
	ss << playerNum;
	wstring key = ss.str();
	ss.str(L"");
	ss << lose;
	wstring value = ss.str();
	ss.str(L"");
	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
}

bool Admin::addWin(int64_t playerNum)
{
	int64_t hasWin = Admin::readWin(playerNum) + 1;
	return Admin::writeWin(playerNum, hasWin);
}

bool Admin::addLose(int64_t playerNum)
{
	int64_t hasLose = Admin::readLose(playerNum) + 1;
	return Admin::writeLose(playerNum, hasLose);
}

wstring Admin::readDataType() {
	wstring model = L"type";
	wstring key = L"isofficial";
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str()) ? L"��ʽ����" : L"��������";
}

bool Admin::writeDataType() {
	wstring model = L"type";
	wstring key = L"isofficial";
	wstringstream ss;
	ss.str(L"");
	ss << 1 - GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
	wstring value = ss.str();
	ss.str(L"");
	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
}

int64_t Admin::readVersion()
{
	wstring model = L"version";
	wstring key = L"version";
	return GetPrivateProfileInt(model.c_str(), key.c_str(), 0, CONFIG_PATH.c_str());
}

bool Admin::writeVersion()
{
	wstring model = L"version";
	wstring key = L"version";

	time_t rawtime = time(0);

	wstringstream ss;
	ss.str(L"");

	char tmp[64];
	strftime(tmp, sizeof(tmp), "%y%m%d%H%M", localtime(&rawtime));
	ss << tmp;
	wstring value = ss.str();
	ss.str(L"");
	return WritePrivateProfileString(model.c_str(), key.c_str(), value.c_str(), CONFIG_PATH.c_str());
}

bool Admin::IAmAdmin(int64_t playerNum)
{
	return Admin::readAdmin() == 0 && Admin::writeAdmin(playerNum);
}

bool Admin::resetGame(int64_t playNum)
{
	return playNum == Admin::readAdmin() && DeleteFile(CONFIG_PATH.c_str());
}

//˽�˲�ѯ��Ϣ
void Admin::getPlayerInfo(int64_t playNum) {
	wstringstream msg;
	msg << Admin::readDataType() << L" " << Admin::readVersion() << L" UTC\r\n";
	msg << L"[CQ:at,qq=" << playNum << L"]��"
		<< Admin::readWin(playNum) << L"ʤ"
		<< Admin::readLose(playNum) << L"����" 
		<< L"����";

	int64_t hasScore = Admin::readScore(playNum);
	hasScore -= 500000000;
	if (hasScore >= 0) {
		msg << hasScore;
	}
	else {
		msg << L"-" << -hasScore;
	}
	msg << "\r\n";

	wstring tmp = msg.str();
	if (tmp.empty()) {
		msg.str(L"");
		return;
	}
	int length = tmp.length();
	if (tmp[length - 2] == '\r' && tmp[length - 1] == '\n') {
		tmp = tmp.substr(0, length - 2);
	}

	Util::sendPrivateMsg(playNum, Util::wstring2string(tmp).data());
}

bool Admin::backupData(int64_t playNum) {
	if (!isAdmin(playNum)) {
		return false;
	}

	wstring msg;
	ifstream in;
	ofstream out;

	char *sourceFile = ".\\app\\com.auntspecial.doudizhu\\config.ini";
	char *targetFile_1 = ".\\app\\com.auntspecial.doudizhu\\config_";
	char *targetFile_2 = ".ini.bak";
	char targetFile[80] = {0};

	time_t rawtime = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%y%m%d%H%M", localtime(&rawtime));
	strcpy(targetFile, targetFile_1);
	strcat(targetFile, tmp);
	strcat(targetFile, targetFile_2);

	in.open(sourceFile, ios::binary);//��Դ�ļ�
	if (in.fail())//��Դ�ļ�ʧ��
	{
		cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		msg = L"Դ�ļ���ʧ�ܡ�";
		Util::sendPrivateMsg(playNum, Util::wstring2string(msg).data());
	}
	out.open(targetFile, ios::binary);//����Ŀ���ļ� 
	if (out.fail())//�����ļ�ʧ��
	{
		cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		msg = L"Ŀ���ļ���ʧ�ܡ�";
		Util::sendPrivateMsg(playNum, Util::wstring2string(msg).data());
	}
	else//�����ļ�
	{
		out << in.rdbuf();
		out.close();
		in.close();

		msg = L"�ļ��ѱ����� " + Util::string2wstring(targetFile);
		Util::sendPrivateMsg(playNum, Util::wstring2string(msg).data());
	}
}