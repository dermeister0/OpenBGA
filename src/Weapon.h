#pragma once
#include ".\main.h"

class CWorld;
class CPlayer;

struct sweapon_info
{
	char name[200];
	int damage;
	int reload_time;
	int speed;
	int count;
};

class CWeapon
{
	int type;//тип пушки
	int damage;//мощность
	int reload_time;//время перезарядки
	int bullets;//кол-во патронов
	int bul_speed;//скорость пуль

	int last_rel_time;//время последней перезарядки

	static CWorld * world;//указатель на игровой мир
	CPlayer *player;//указатель на игрока
public:
	CWeapon(int _type, int _damage, int _rel_time, int _bullets, int _speed, CPlayer *pl);
	~CWeapon(void);
	// производит выстрел из оружия
	void Fire(int plx, int ply, int angle);

	// устанавливает указатель на игровой мир
	static void SetGameWorld(CWorld * w)
	{
		world=w;
	}

	// возвращает кол-во патронов
	int GetBullets(void)
	{
		return bullets;
	}

	// добавляет указанное количество патронов в магазин
	void AddBullets(int count)
	{
		bullets+=count;
	}

	// устанавливает данное кол-во патронов в пушке
	void SetBullets(int count)
	{
		bullets=count;
	}
};
