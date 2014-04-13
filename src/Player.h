#pragma once
#include ".\camera.h"
#include ".\controls.h"
#include ".\gamemap.h"
//#include ".\world.h"
#include ".\weapon.h"

class CWorld;
class CGame;

#define BGA_MAX_SPEED 200

//структура для создания игрока
struct screate_player
{
	int id;//код игрока
	int team;//команда игрока
	int x,y,angle;
	bool local;
	int cam_type;//если локальный, то задаем тип камеры;
				 //если нет, то CT_NOCAMERA
	char spr_id[20];//идентификатор спрайта
	char name[20];
	CL_ResourceManager* resman;//указатель на менеджер ресурсов
	int *controls;//указатель на массив кнопок
	int input_type;//тип ввода
	GCL_Keyboard * kb;//указатель на клавиатуру
	CGameMap *game_map;
	CWorld* world;
	CGame *game;
	int max_frags;//максимально набранные фраги
	int frags;//фраги
};

struct splayer_features
{
	union
	{
		struct
		{
			int haste;
			int freeze;
			int quaddamage;
			int invisibility;
			int mine;
			int regeneration;
		};
		int all[6];//доступ по индексу
	};
	
	splayer_features()
	{
		haste=0;
		freeze=0;
		quaddamage=0;
		invisibility=0;
		mine=0;
		regeneration=0;
	}
};

//структура для обновления игрока
struct supdate_player
{
	int x,y,angle;
	double ux,uy;//скорость
	double vx,vy;//ускорение
	int id;//код игрока
	splayer_features features;//состояние гамера
};

struct supdate_frags
{
	int id;//id гамера
	int target_id;//кого убил
	int weapon;//из какой пушки
	int dfrags;//изменение его фрагов (1 или -1)
	int max_frags;//макс. фраги
};

class CPlayer
{
	friend CCamera;
	friend CWeapon;

	static int last_id;//для создания игроков
	int id;//код игрока
	int team;//команда игрока

	int life;//жизни
	int armor;//броня
	int frags;//фраги
	splayer_features features;//состояние гамера
	bool no_weapon;//нет никаких патронов
	CL_SoundBuffer_Session *snd_engine;
public:
	static int max_frags;//максимально набранные фраги
private:
	string name;
	string spr_id;
	int x,y,angle;
	double pdx,pdy;//более точные координаты
	double vx,vy;//ускорения
	double ux,uy;//скорость
	double ux2, uy2;//скорость при поворотах
	bool reverse;//задний ход
	bool aimed;//нацелился

	//углы тачки
	CL_Point cr_lt;//левый верхний
	CL_Point cr_lb;//левый нижний
	CL_Point cr_rt;//правый верхний
	CL_Point cr_rb;//правый нижний
	CL_Point cr_ct;//центральный передний
	CL_Point cr_cb;//центральный задний
	CL_Point cr_c;//центр
	CL_Point cr_t;//точка перед машиной
	CL_Point cr_target;//прицел

	int clt,crt,clb,crb,cct,ccb;//состояние коллижена
	CL_Point normal_lt,normal_rt,normal_lb,normal_rb;//нормали к "толкаемым" стенам

	//характеристики для каждой тачки
	double max_speed;//максимальная скорость
	double thrust;//ускорение

	int cur_weapon;//выбранная пушка


	int htick;//время следующего уменьшения избыточных жизней
	int atick;//время следующего уменьшения избыточной брони
	int ftick;//время следующего уменьшения всех "фич"

	CCamera *cam;
	CWeapon *weapons[5];//5 пушек
	CGameMap *game_map;
	CWorld *world;
	CGame *game;

	CL_Sprite *spr_car;
	CL_ResourceManager *resman;
	CL_Font *fnt_interface;
	bool local;//локальный геймер
	int* controls;//[BGA_CTRL_COUNT];//указатель на массив кнопок
	int input_type;//тип ввода
	GCL_Keyboard * kb;//указатель на клавиатуру
public:
	enum einput_type {IT_1,IT_2,IT_3,IT_4,IT_NETWORK};
	CPlayer(screate_player pl_info);
	~CPlayer(void);
	// возвращает X-координату игрока
	int GetX(void);
	// возвращает Y-координату игрока
	int GetY(void);
	// возвращает true, если игрок локальный
	bool IsLocal(void);
	// возвращает указатель на камеру игрока
	CCamera* GetCam(void);
	// конструктор копирования, для распред. динам. памяти
	//CPlayer(const CPlayer& pl);
	// рисует игрока в условных координатах
	void Draw(int ux, int uy);
	// проверяет кнопки и отправляет сообщения
	void Input(void);
	// обновляет состояние игрока
	void Update(void);
	// возвращает информацию об игроке для нового подключившегося гамера
	screate_player GetInfo(void);
	// возвращает информацию о позиции и состоянии игрока
	supdate_player GetUpdateInfo(void);

	// возвращает идентификатор игрока
	int GetId(void)
	{
		return id;
	}
	// изменяет положение и состояние игрока
	void Modify(supdate_player pl);

	// возвращает последний идентификатор
	static int CPlayer::GetLastId(void)
	{
		return last_id;
	}
	// обновляет нелокального игрока, не учитываются многие факторы
	void UpdateRemote(void);
private:
	// возвращает 1-задний ход, 0-передний, -1 - точно определитть нельзя
	int IsReverse(void);
	// возвращает координаты точки, смещенной на заданный вектор
	CL_Point GetCorner(int dx, int dy);
public:
	// возвращает прямоугольник, при попадании в который  пуля взрывается
	CL_Rect GetHitRect(void);
private:
	// возвращает левую нижнюю точку тачки
	CL_Point GetLDCorner(void);
	// возвращает правую верхнюю точку тачки
	CL_Point GetRUCorner(void);
public:
	// уменьшает жизнь игрока на опред. кол-во хитов
	void DecLife(int dlife, int shooter, int wtype);
	// оживляет игрока в новой точке
	void Respawn(void);

	// возвращает фраги игрока
	int GetFrags(void)
	{
		return frags;
	}

	// возвращает максимально набранные фраги
	static int GetMaxFrags(void)
	{
		return max_frags;
	}

	// устанавливает фраги игрока
	void SetFrags(int f)
	{
		frags=f;
	}
	// поворачивает тачку на данный угол
	void Turn(int a);
private:
	// перемещает прицел на близжайшего врага
	void FindAim(void);
public:

	// возвращает центральную точку тачки
	CL_Point GetCenter(void)
	{
		cr_c=GetCorner(32,32);
		return cr_c;
	}
private:
	// добавляет дым из трубы
	void AddSmoke(CL_Point tube);
public:
	// если прицел поймал врага, окрашивает его в зеленый цвет
	void CheckTarget(void);

	// возвращает имя игрока
	string GetName(void)
	{
		return name;
	}
private:
	// обновляет состояние внутренних переменных столкновения
	void UpdateCollision(void);
	// ест бонус, если стоит на нем
	void EatBonus(int x, int y);
public:
	// обновляет особое состояние (special state): жизни и броня >100, различные усилители, etc.
	void UpdateSS(void);
	// выбирает следующую пушку в которой есть патроны, возвращает ее номер
	int NextWeapon(void);

	// возвращает true, если игрок подобрал такой-то бонус
	bool HasFeature(int type)
	{
		return (bool)features.all[type];
	}

	// возвращает косанду игрока
	int GetTeam(void)
	{
		return team;
	}
};
