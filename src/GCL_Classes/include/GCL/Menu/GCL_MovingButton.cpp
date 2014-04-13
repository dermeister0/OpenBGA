/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_MovingButton'
	description: class for using moving buttons
	author: 'Tosha'
	date: '02.08.2005'
	version: '0.0.40'
*/

#include ".\gcl_movingbutton.h"

GCL_MovingButton::GCL_MovingButton(int bx, int by, bool center, int bid, string res_id, CL_ResourceManager *RM,int dx, int dy)
									:GCL_Button(bx,by,center,bid,res_id,RM)
{
	this->dx=dx;
	this->dy=dy;	

	moving_rect.left=x-dx;
	moving_rect.right=x+w+dx;
	moving_rect.top=y-dy;
	moving_rect.bottom=y+dy+h;

	rx=x;ry=y;
	next_move=0;

	ChangeDir();
}

GCL_MovingButton::~GCL_MovingButton(void)
{
//	delete sprite;
}

// изменяет направление движения кнопки
void GCL_MovingButton::ChangeDir(void)
{
	double d=(double)dx/(double)dy*2;
	//uy=((double)dy/(double)dx);
	
	ux=rand()%7-3;
	uy=rand()%7-3;

	uy/=d;

	if (ux<1)
		ux++;
	if (uy<1)
		uy++;
	if (ux>3)
		ux=3;
	if (uy>3)
		uy=3;

	next_change=CL_System::get_time()+rand()%5000+3000;
}

// непосредственно движение кнопки
void GCL_MovingButton::Move(void)
{
	int t=CL_System::get_time();
	if (CL_System::get_time()>=next_move)
		next_move=CL_System::get_time()+50;
	else
		return;

	//движение по X
	if (ux<0)
	{
		if (rx>moving_rect.left)
			rx+=ux;
		else
			ChangeXDir();
	}
	else
	{
		if (rx+w<moving_rect.right)
			rx+=ux;
		else
			ChangeXDir();
	}
	
	//движение по оси Y
	if (uy<0)
	{
		if (ry>moving_rect.top)
			ry+=uy;
		else
			ChangeYDir();
	}
	else
	{
		if (ry+h<moving_rect.bottom)
			ry+=uy;
		else
			ChangeYDir();
	}

	//округляем
	x=(int)rx+0.5;
	y=(int)ry+0.5;

	if (CL_System::get_time()>=next_change)
		ChangeDir();
}

void GCL_MovingButton::Draw()
{
	sprite->set_frame(frame);
	sprite->draw(x,y);
	Move();
}

// изменяет направление по оси X
void GCL_MovingButton::ChangeXDir(void)
{
	if (ux<0)
		ux=rand()%4;
	else
		ux=rand()%4-3;
}

// изменяет направление по оси Y
void GCL_MovingButton::ChangeYDir(void)
{
	if (uy<0)
		uy=rand()%4;
	else
		uy=rand()%4-3;
}
