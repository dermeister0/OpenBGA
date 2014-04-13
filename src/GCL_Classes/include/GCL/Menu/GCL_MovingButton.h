#pragma once
#include "gcl_button.h"
#include "..\GCL.h"

class GCL_MovingButton :
	public GCL_Button
{
	CL_Rect moving_rect;
	double ux,uy;//скорость
	double rx,ry;//точные координаты
	unsigned int next_change;//время следующего поворота
	unsigned int next_move;//время следующего перемещения
public:
	GCL_MovingButton(int bx, int by, bool center, int bid, string res_id, CL_ResourceManager *RM,int dx, int dy);
	~GCL_MovingButton();
	void Draw();
private:
	// изменяет направление движения кнопки
	void ChangeDir(void);
	// непосредственно движение кнопки
	void Move(void);
public:
	// изменяет направление по оси X
	void ChangeXDir(void);
	// изменяет направление по оси Y
	void ChangeYDir(void);
};
