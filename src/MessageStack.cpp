#include ".\messagestack.h"

CMessageQueue::CMessageQueue(void)
{
}

CMessageQueue::~CMessageQueue(void)
{
}

// добавляет сообщение в очередь
void CMessageQueue::AddMessage(GameMessage& msg)
{
	mqueue.push(msg);
}

//передает первое сообщение из очереди, возвращает false, если очередь пуста
bool CMessageQueue::GetGameMessage(GameMessage*msg)
{
	if (mqueue.empty())
		return false;

	*msg=mqueue.front();
	mqueue.pop();

	return true;
}

// добавляет сообщение с одним параметром
void CMessageQueue::AddMessage(int type, void *info)
{
	GameMessage msg;
	memset(&msg,0,sizeof(GameMessage));
	msg.type=type;
	msg.info1=info;
	mqueue.push(msg);
}

/*// добавляет сообщение в локальную очередь и отправляет по сети
void CMessageQueue::SendMessage(int type, void* info)
{
	GameMessage msg;
	memset(&msg,0,sizeof(GameMessage));
	msg.type=type;
	msg.info1=info;
	mqueue.push(msg);

	//здесь шлем по сети
}*/
