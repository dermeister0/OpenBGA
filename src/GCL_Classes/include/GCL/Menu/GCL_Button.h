#pragma once

#include "..\GCL.h"

class GCL_Button
{
protected:
	CL_Sprite * sprite;

	int x,y,w,h;
	int dx,dy;

	int frame, id;

	bool check;
	int radio;
public:
	GCL_Button(int bx, int by, bool center, int bid, string res_id, CL_ResourceManager *RM);
	virtual ~GCL_Button();
	int GetId(void);

	const static int GCL_BUTTON_FRAME_OFF=0;
	const static int GCL_BUTTON_FRAME_ON=1;
	const static int GCL_BUTTON_FRAME_DOWN=2;

	const static int GCL_BUTTON_RADIO=1;
	const static int GCL_BUTTON_CHECKBOX=2;

	void SetFrame(int _frame);
	virtual void Draw(){sprite->set_frame(frame);sprite->draw(x,y);}
	bool HavePoint(int px, int py);

	// устанавливает состо€ние кнопки
	void SetChecked(bool new_check)
	{
		check=new_check;
		if (check)
			frame=GCL_BUTTON_FRAME_DOWN;
		else
			frame=GCL_BUTTON_FRAME_OFF;
	}

	// возвращает состо€ние кнопки
	bool IsChecked()
	{
		return check;
	}
	// посылаютс€ сообщени€ о нажатой клавише
	virtual void OnKeyPress(int key);

	// устанавливает параметры кнопки
	virtual void SetParam(int id, string value);
	// возвращает параметры кнопки
	virtual string GetParam(int id);

	// объ€вл€ет данную кнопку радиокнопкой
	void MakeRadio(int type)
	{
		radio=type;
	}
};