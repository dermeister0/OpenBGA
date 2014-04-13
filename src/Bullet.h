#pragma once
#include ".\main.h"

//информация о стенах и игроках для коллижена
struct scol_info;

struct screate_bullet
{
	int x,y;//координаты
	int ux,uy;//скорость
	int type;//тип пули
	int shooter;//id стрелка
	int damage;//урон
	int angle;//угол
};

class CBullet
{
	int type;//тип
	int damage;//мощность
	//разброс и т.д. и т.п.

	int x,y;//координаты
	int ux,uy;//перемещение
	int angle;//угол
	int shooter;//id стрелка
	bool alive;//пуля жива
public:
	static CL_Sprite* bullets[5];
	CBullet(screate_bullet b);
	~CBullet(void);
	// обновляет пулю
	void Update(scol_info i);
	// рисует пулю в условных координатах
	void Draw(int _ux, int _uy);

	// возвращает true, если пуля жива
	bool IsAlive(void)
	{
		return alive;
	}
};
