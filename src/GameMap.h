#pragma once
#include <vector>
#include ".\main.h"
using namespace std;

class CWorld;
class CGame;

class CGameMap
{
	struct
	{
		char mg;//main group
		char sg;//secondary group
	}tile_types[168];
	enum {TT_ROAD, TT_GRASS, TT_ROOF, TT_WATER, TT_SAND, TT_OTHER};
	struct map_info
	{
		int tile;
		int angle;
		int shadow;
		int item;
	};

	int mwidth,mheight,msize;//размер карты
	vector<map_info> map;

	struct map_header
	{
		string map_name;
		string map_author;
	}mh;
	string map_name;
	CWorld *world;
	CGame *game;
	CL_Sprite *spr_tiles;
	CL_ResourceManager *resman;
public:
	enum {W_NONE,W_LEFT,W_RIGHT,W_TOP,W_BOTTOM};//тип стены

	CGameMap(CWorld *w, CGame *g);
	~CGameMap(void);
	// загружает карту с заданным именем из файла
	bool LoadMap(string name);
	// рисует карту с заданным смещеним в нужных координатах экрана
	void Draw(int dx, int dy, CL_Rect cam,bool top=false);

	// возвращает ширину карты в тайлах
	int GetWidth(void)
	{
		return mwidth;
	}
	// возвращает высоту карты в тайлах
	int GetHeight(void)
	{
		return mheight;
	}

	// возвращает название карты
	string CGameMap::GetName(void)
	{
		return map_name;
	}
	// возвращает true, если тайл по данным координатам - стена
	int IsWall(int x, int y, CL_Point &normal);
	// проверяет столкновения с бонусов; возвращает id бонуса или -1
	int ItemCollided(int x, int y);
};
