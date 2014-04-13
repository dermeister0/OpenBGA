#pragma once

#include "..\GCL.h"

class GCL_Mouse
{
	struct 
	{
		bool old_state;
		bool new_state;
	} key_state[7];//������ ��������� ������ ����
public:
	GCL_Mouse(void);
	~GCL_Mouse(void);

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
	// ���������� ���������� ����
	CL_Point GetPosition(void);
};

#ifndef _DEBUG
	#pragma comment(lib, "GCL_Mouse.lib")
#else
	#pragma comment(lib, "GCL_Mouse_d.lib")
#endif