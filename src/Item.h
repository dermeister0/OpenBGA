#pragma once
#include ".\main.h"
#include ".\powerups.h"

class CGame;

struct screate_item
{
	int id;//идентификатор
	int x,y;//координаты
	int type;//тип бонуса
	int reload_time;//время оживления
};

class CItem
{
	int id;//идентификатор
	int x,y;//координаты
	int type;//тип бонуса
	int life;//жизни
	int dir;//направление вращения
	int reload_time;//время оживления
	bool alive;//жив или нет
public:
	static CL_Sprite* powerups;
	static CL_Sprite *spr_item_place;
	static CGame* game;

	CItem(screate_item i);
	~CItem(void);
	// выводит бонус в условных координатах
	void Draw(int ux, int uy,bool top=false);
	// обновляет объект
	void Update(void);
	// возвращает прямоугольник коллижина бонуса
	CL_Rect GetRect(void)
	{
		return CL_Rect(x,y,x+64,y+64);
	}

	// возвращает id
	int GetId(void)
	{
		return id;
	}

	// возвращает тип бонуса
	int GetType(void)
	{
		return type;
	}

	// посылает бонусу команду на уничтожение
	void Kill(void);

	// бонус, ты жив?
	bool IsAlive(void)
	{
		return alive;
	}

	// возвращает координаты бонуса
	CL_Point GetPosition(void)
	{
		return CL_Point(x,y);
	}
};
