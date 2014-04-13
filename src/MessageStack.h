#pragma once
#include <queue>
using namespace std;

struct GameMessage
{
	int type;//тип
	void *info1;//информация
	int info2;//доп. информация
};

//очередь сообщений
class CMessageQueue
{
	queue<GameMessage> mqueue;
public:
	CMessageQueue(void);
	~CMessageQueue(void);
	// добавляет сообщение в очередь
	void AddMessage(GameMessage& msg);
	//передает первое сообщение из очереди, возвращает false, если очередь пуста
	bool GetGameMessage(GameMessage*msg);
	// добавляет сообщение с одним параметром
	void AddMessage(int type, void *info);
	// добавляет сообщение в локальную очередь и отправляет по сети
	//void SendMessage(int type, void* info);
};
