#pragma once
#include ".\main.h"

struct stalk_info
{
	string author;//�����
	string message;//���������

	stalk_info(string _author,string _message)
	{
		author=_author;
		message=_message;
		life=512;
	}
	int life;
};

class CTalkInfo
{
	vector <stalk_info> messages;
public:
	CTalkInfo(void);
	~CTalkInfo(void);
	// ��������� ���������
	void Add(stalk_info nfo);
	// ��������� ������ ���������
	void Update(void);
	// ������ ������ ��������� � �������� �����
	void Draw(int x, int y);
};
