#pragma once

#include "..\GCL.h"

class GCL_Keyboard
{
	struct 
	{
		bool old_state;
		bool new_state;
	} key_state[256];//������ ��������� ������
public:
	GCL_Keyboard(void);
	~GCL_Keyboard(void);

	// ��������� ��������� ����������
	void Refresh(void);
	// ������� ������ � ������ ������
	bool KeyDown(int keycode);
	// ������� ������ � ������ ������
	bool KeyUp(int keycode);
	// ������� ������ ��� ������
	bool KeyPress(int keycode);
	// ������� ������ ��� ������ � ���������
	bool KeyRelease(int keycode);
};

#ifndef _DEBUG
	#pragma comment(lib, "GCL_Keyboard.lib")
#else
	#pragma comment(lib, "GCL_Keyboard_d.lib")
#endif