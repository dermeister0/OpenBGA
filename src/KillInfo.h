#pragma once
#include ".\main.h"
#include ".\camera.h"

struct skill_info
{	
	string killer;//������
	int killer_team;
	string dead;//������
	int dead_team;
	int weapon;
	skill_info()
	{
		life=512;
	}
	int life;
};

class CKillInfo
{
	vector <skill_info> kills;
	CL_Font * fnt_kill;
public:
	CKillInfo();
	~CKillInfo(void);
	// ��������� ���������� �� ��������
	void Add(skill_info nfo);
	// ��������� ������ ������ �������
	void Update(void);
	// ������ ������ � ����������� �����������
	void Draw(int x, int y);
};
