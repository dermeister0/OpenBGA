#pragma once
#include ".\main.h"

#define BGA_EXPLODE_NORMAL 1 //обычный взрыв
#define BGA_EXPLODE_SMOKE  2 //дым из трубы
#define BGA_EXPLODE_BONUS  3 //взрыв бонуса

#define BGA_EXPLODE_WEAP1  4 //взрыв пули пулемета
#define BGA_EXPLODE_WEAP2  5 //взрыв пули дробовика
#define BGA_EXPLODE_WEAP3  6 //взрыв пули ракетницы
#define BGA_EXPLODE_WEAP4  7 //взрыв пули плазмагана
#define BGA_EXPLODE_WEAP5  8 //взрыв пули дезинтегратора

#define BGA_EXPLODE_GREEN_SMOKE  9 //шлейф дезинтегратора
#define BGA_EXPLODE_BLUE_RING  10 //шлейф плазмагана

#define BGA_EXPLODE_RESPAWN  11 //выход из телеТорта

struct screate_explode
{
	int x,y;//координаты взрыва
	int vx,vy;//скорость
	int type;//тип
	double scale;//размеры спрайта
	CL_Sprite *sprite;//устанавливается локальным компом
	screate_explode()
	{
		scale=1.0;
	}
};

class CExplode
{
protected:
	CL_Sprite *sprite;

	int angle;
	int x,y;
	int vx,vy;
	double scale;
	bool alive;
	float max_life_time;
	float life_time;
public:
	static CL_Sprite *spr_explode;
	static CL_Sprite *spr_w1_explode;
	static CL_Sprite *spr_w2_explode;
	static CL_Sprite *spr_w3_explode;
	static CL_Sprite *spr_w4_explode;
	static CL_Sprite *spr_w5_explode;
	static CL_Sprite *spr_respawn_explode;
	CExplode(screate_explode e);
	~CExplode(void);
	virtual void Draw(int ux, int uy);
	virtual void Update();
	bool IsAlive()
	{
		return alive;
	}
};
