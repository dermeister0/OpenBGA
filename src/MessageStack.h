#pragma once
#include <queue>
using namespace std;

struct GameMessage
{
	int type;//���
	void *info1;//����������
	int info2;//���. ����������
};

//������� ���������
class CMessageQueue
{
	queue<GameMessage> mqueue;
public:
	CMessageQueue(void);
	~CMessageQueue(void);
	// ��������� ��������� � �������
	void AddMessage(GameMessage& msg);
	//�������� ������ ��������� �� �������, ���������� false, ���� ������� �����
	bool GetGameMessage(GameMessage*msg);
	// ��������� ��������� � ����� ����������
	void AddMessage(int type, void *info);
	// ��������� ��������� � ��������� ������� � ���������� �� ����
	//void SendMessage(int type, void* info);
};
