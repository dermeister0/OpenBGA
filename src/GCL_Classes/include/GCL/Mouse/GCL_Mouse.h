#pragma once

#include "..\GCL.h"

class GCL_Mouse
{
	struct 
	{
		bool old_state;
		bool new_state;
	} key_state[7];//массив состояний кнопок мыши
public:
	GCL_Mouse(void);
	~GCL_Mouse(void);

	// обновляет состояние клавиатуры
	void Refresh(void);
	// клавиша нажата в данный момент
	bool KeyDown(int keycode);
	// клавиша отжата в данный момент
	bool KeyUp(int keycode);
	// клавишу только что нажали
	bool KeyPress(int keycode);
	// клавишу только что нажали и отпустили
	bool KeyRelease(int keycode);
	// возвращает координаты мыши
	CL_Point GetPosition(void);
};

#ifndef _DEBUG
	#pragma comment(lib, "GCL_Mouse.lib")
#else
	#pragma comment(lib, "GCL_Mouse_d.lib")
#endif