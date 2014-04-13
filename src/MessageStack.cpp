#include ".\messagestack.h"

CMessageQueue::CMessageQueue(void)
{
}

CMessageQueue::~CMessageQueue(void)
{
}

// ��������� ��������� � �������
void CMessageQueue::AddMessage(GameMessage& msg)
{
	mqueue.push(msg);
}

//�������� ������ ��������� �� �������, ���������� false, ���� ������� �����
bool CMessageQueue::GetGameMessage(GameMessage*msg)
{
	if (mqueue.empty())
		return false;

	*msg=mqueue.front();
	mqueue.pop();

	return true;
}

// ��������� ��������� � ����� ����������
void CMessageQueue::AddMessage(int type, void *info)
{
	GameMessage msg;
	memset(&msg,0,sizeof(GameMessage));
	msg.type=type;
	msg.info1=info;
	mqueue.push(msg);
}

/*// ��������� ��������� � ��������� ������� � ���������� �� ����
void CMessageQueue::SendMessage(int type, void* info)
{
	GameMessage msg;
	memset(&msg,0,sizeof(GameMessage));
	msg.type=type;
	msg.info1=info;
	mqueue.push(msg);

	//����� ���� �� ����
}*/
