#pragma once
#include ".\main.h"
#include ".\world.h"

class CFragInfo
{
	CWorld * world;
	CGame * game;
	struct sfrag_info
	{
		string name;
		int frags;
		int command;
	};
	vector <sfrag_info> frags;
public:
	CFragInfo(CWorld *w, CGame * g);
	~CFragInfo(void);
	// ������� ���������� � ������
	void Draw(void);
	// ��������� � ��������� ���������� � ������
	void Update(void);
};
