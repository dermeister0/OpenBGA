#pragma once
#include ".\main.h"

class CPlayer;

class CCamera
{
	int x,y,w,h,type;
	// смещение по оси X
	int dx;
	// смещение по оси Y
	int dy;
	double vx,vy;//ускорения по X и Y
	CPlayer* pl; //указатель на игрока

public:
	static CL_Font *fnt_interface;
	static CL_Sprite* wicons[6];//иконки оружия
	static CL_Sprite* ficons[6];//иконки бонусов
	static CL_Sprite* spr_armor;
	static CL_Sprite* spr_health;
	static CL_Sprite* spr_aim;
	enum ecamera_type{CT_NOCAMERA, CT_ALLSCREEN, CT_UP, CT_DOWN, CT_UPLEFT, CT_UPCENTER, CT_UPRIGHT, CT_DOWNRIGHT, CT_DOWNLEFT};
	CCamera(int _type, CPlayer *_pl);
	~CCamera(void);
	// устанавливает область экрана для камеры
	void SetRect(int _x, int _y, int _w, int _h);
	// возвращает область камеры
	CL_Rect GetRect(void);
	// возвращает тип камеры
	int GetType(void);
	// едет следом за игроком
	void FollowPlayer(int mw, int mh);

	int GetDX(void)
	{
		return dx;
	}

	int GetDY(void)
	{
		return dy;
	}
	// рисует интерфейс для игрока
	void DrawInterface(void);
private:
	// выводит текст с тенью
	void ShadowFont(int x, int y, string text, CL_Origin alignment);
	void ShadowFont(CL_Rect dest, string text);
};
