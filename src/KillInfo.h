#pragma once
#include ".\main.h"
#include ".\camera.h"

struct skill_info
{	
	string killer;//убийца
	int killer_team;
	string dead;//жертва
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
	// добавляет информации об убийстве
	void Add(skill_info nfo);
	// обновляет дизайн панели убийств
	void Update(void);
	// рисует панель в абсолютныйх координатах
	void Draw(int x, int y);
};
