#pragma once

#include "..\GCL.h"
#include "GCL_Button.h"
#include "GCL_MovingButton.h"
#include "GCL_EditBox.h"

class GCL_MenuPage
{
	vector<GCL_Button*> buttons;
	vector<GCL_Button*>::iterator btn_iter;
	CL_ResourceManager *resman;
	static int ClickedBtn;
public:
	GCL_MenuPage(CL_ResourceManager * RM);
	~GCL_MenuPage(void);
	int AddButton(int x, int y, bool center, string res_id);
	void Draw(void);
	void OnMouseUp(int mx, int my);
	void OnMouseDown(int mx, int my);
	void OnMouseMove(int mx, int my);

	// ���������� ������������� ��������� ������� ������ ��� -1
	static int GetClickedId(void)
	{
		int t=ClickedBtn;
		ClickedBtn=-1;
		return t;
	}
	// ��������� ��������� ������
	int AddButton(int x, int y, bool center, string res_id, int dx, int dy);
	// ������������� ���� ��� ������ � ��������� id
	void SetBtnCheck(int id, bool check);
	// �������� editbox �� ��������
	int AddEditBox(int bx, int by, string res_id);
	// ������ �������
	void OnKeyPress(int key);
	// ������������� �������� ��� ������ � ��������� id
	void SetBtnParam(int id, int pid, string value);
	// ���������� �������� ��� ������ � ��������� id
	string GetBtnParam(int id, int pid);
	// ��������� ������ ������ ������������
	void MakeRadio(int id, int type)
	{
		buttons[id]->MakeRadio(type);
	}
};
