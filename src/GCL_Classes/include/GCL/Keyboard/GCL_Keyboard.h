#pragma once

#include "..\GCL.h"

class GCL_Keyboard
{
	struct 
	{
		bool old_state;
		bool new_state;
	} key_state[256];//массив состояний клавиш
public:
	GCL_Keyboard(void);
	~GCL_Keyboard(void);

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
};

#ifndef _DEBUG
	#pragma comment(lib, "GCL_Keyboard.lib")
#else
	#pragma comment(lib, "GCL_Keyboard_d.lib")
#endif