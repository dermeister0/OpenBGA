/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_Mouse'
	description: class for translating mouse events
	author: 'Tosha'
	date: '28.07.2005'
	version: '0.0.15'
*/

#include ".\gcl_mouse.h"

GCL_Mouse::GCL_Mouse(void)
{
	for (int key=0;key<7;key++)
	{
		key_state[key].old_state=false;
		key_state[key].new_state=false;
	}
	Refresh();
}

GCL_Mouse::~GCL_Mouse(void)
{
}

// ��������� ��������� ����������
void GCL_Mouse::Refresh(void)
{
	for (int key=0;key<7;key++)
	{
		key_state[key].old_state=key_state[key].new_state;
		key_state[key].new_state=CL_Mouse::get_keycode(key);
	}
}

// ������� ������ � ������ ������
bool GCL_Mouse::KeyDown(int keycode)
{
	int n=key_state[keycode].new_state;

	return n==true;
}

// ������� ������ � ������ ������
bool GCL_Mouse::KeyUp(int keycode)
{
	int n=key_state[keycode].new_state;

	return n==false;
}

// ������� ������ ��� ������
bool GCL_Mouse::KeyPress(int keycode)
{
	int o=key_state[keycode].old_state;
	int n=key_state[keycode].new_state;

	return n==true && o==false;
}

// ������� ������ ��� ������ � ���������
bool GCL_Mouse::KeyRelease(int keycode)
{
	int o=key_state[keycode].old_state;
	int n=key_state[keycode].new_state;

	return n==false && o==true;
}

// ���������� ���������� ����
CL_Point GCL_Mouse::GetPosition(void)
{
	return CL_Point(CL_Mouse::get_x(),CL_Mouse::get_y());
}
