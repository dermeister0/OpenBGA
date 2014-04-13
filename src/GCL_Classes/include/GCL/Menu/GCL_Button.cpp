/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_Button'
	description: class for using buttons
	author: 'Tosha'
	date: '16.10.2005'
	version: '0.0.30'
*/

#include ".\gcl_button.h"

GCL_Button::GCL_Button(int bx, int by, bool center, int bid, string res_id, CL_ResourceManager *RM)
{
	x=bx;
	y=by;

	id=bid;

	check=false;
	radio=false;

	if (!RM)
	{
		sprite=NULL;
		return;
	}


	sprite=new CL_Sprite(res_id,RM);

	w=sprite->get_width();
	h=sprite->get_height();

	if (center)
	{
		x-=w/2;
		y-=h/2;
	}

}

GCL_Button::~GCL_Button()
{
	if (sprite)
		delete sprite;
}

int GCL_Button::GetId(void)
{
	return id;
}

void GCL_Button::SetFrame(int _frame)
{
	if (!radio)
		frame=_frame;
	else if (radio==GCL_BUTTON_RADIO && check)
		frame=GCL_BUTTON_FRAME_DOWN;
	else if (radio==GCL_BUTTON_RADIO && !check)
		frame=_frame;
}

bool GCL_Button::HavePoint(int px, int py)
{
	if (px>=x-10 && px<=x+w+10 && py>=y && py<=y+h)
		return true;

	return false;
}

void GCL_Button::OnKeyPress(int key)
{
}

// устанавливает параметры кнопки
void GCL_Button::SetParam(int id, string value)
{
}

// возвращает параметры кнопки
string GCL_Button::GetParam(int id)
{
	return "";
}