#pragma once
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <fstream>
using namespace std;

//тайлы
#define TILE_BORDER 0
#define TILE_ROOF 1
#define TILE_PAVEMENT 3
#define TILE_ROAD 3
#define TILE_HORROAD 4
#define TILE_VERROAD 5
#define TILE_GRASS 6
#define TILE_SAND 7
#define TILE_PAVEMENT2 8
#define TILE_TREE 9
#define TILE_SIDEWALK 10

//типы игры
#define GAME_DM 1
#define GAME_TDM 2
#define GAME_CTF 3


class projectX : public CL_ClanApplication
{
	virtual int main(int, char**);
};

struct stile_info
{
	int tile;
	int color;
	int angle;
	int special;//напр. гор. или верт. дорога
};

#define BGA_TILES_FILE "_data/main/tiles/tiles.cfg"

class CEditor
{
	CL_Sprite *spr_tiles;
	CL_Sprite *spr_items;
	CL_Font * fnt_misc;
	int dx,dy;//смещения карты
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
	int brushes[12];//кисти
	int brush_angles[12];//повороты кистей
	int lb,tb;//границы

	//углы копируемого прямоугольника
	CL_Point top_left;
	CL_Point right_bottom;

	struct map_header
	{
		string map_name;
		string map_author;
	}mh;
	string map_file_name;

	struct 
	{
		int tile;
		int angle;
	};

public:
	CEditor(void);
	~CEditor(void);
	void tile_editor(void);
	void tile_grouper(void);
	// возвращает индекс следующего тайла в заданной группе
	int NextInGroup(int begin_index, int group, int mg);
	void map_editor(void);
	// загоняет в массив кисти из определенной группы и подгруппы
	void UpdateBrushes(int mg, int sg);
	void EditTile(int x, int y, int brush, int angle);
	void GetBrush(int x, int y, int &brush, int &angle);

	void EditItem(int x, int y, int brush);
	void GetItem(int x, int y, int &brush);

	// сохраняет карту в файл (вместе с dm-уровнем)
	void SaveMap(void);
	// сохраняет карту в файл в формате второй версии
	void SaveMapV2(void);
	// сохраняет уровень в файл в формате второй версии
	void SaveLevel(int game_type);
	// загружает уровень в файл в формате второй версии
	void LoadLevel(int game_type);


	// загружает карту из файла
	int LoadMap(string mname);
	// копирование прямоугольной области тайлов
	void Copy(void);
	// рисует карту и сетку
	void Draw();
	// вставляет скопированный кусок в указанное место
	void Paste(void);
	// ждет, пока нажата кнопка мыши
	void WaitMouse(void);
	// генерирует карту по скетчу
	void GenerateMap(void);
	// добавляет тень к нужному тайлу
	void SetShadow(int x, int y, int type);
	// редактор бонусов/особых точек
	void item_editor(void);
private:
	// рисует объекты
	void DrawItems(void);
};
