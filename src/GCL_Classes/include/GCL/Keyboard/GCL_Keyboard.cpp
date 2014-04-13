/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_Keyboard'
	description: class for translating keyboard events
	author: 'Tosha'
	date: '7.07.2005'
	version: '0.0.8'
*/

#include ".\gcl_keyboard.h"

GCL_Keyboard::GCL_Keyboard(void)
{
	for (int key=0;key<256;key++)
	{
		key_state[key].old_state=false;
		key_state[key].new_state=false;
	}
}

GCL_Keyboard::~GCL_Keyboard(void)
{
}

// обновляет состояние клавиатуры
void GCL_Keyboard::Refresh(void)
{
	for (int key=0;key<256;key++)
	{
		key_state[key].old_state=key_state[key].new_state;
		key_state[key].new_state=CL_Keyboard::get_keycode(key);
	}
}

// клавиша нажата в данный момент
bool GCL_Keyboard::KeyDown(int keycode)
{
	int n=key_state[keycode].new_state;

	return n==true;
}

// клавиша отжата в данный момент
bool GCL_Keyboard::KeyUp(int keycode)
{
	int n=key_state[keycode].new_state;

	return n==false;
}

// клавишу только что нажали
bool GCL_Keyboard::KeyPress(int keycode)
{
	int o=key_state[keycode].old_state;
	int n=key_state[keycode].new_state;

	return n==true && o==false;
}

// клавишу только что нажали и отпустили
bool GCL_Keyboard::KeyRelease(int keycode)
{
	int o=key_state[keycode].old_state;
	int n=key_state[keycode].new_state;

	return n==false && o==true;
}
