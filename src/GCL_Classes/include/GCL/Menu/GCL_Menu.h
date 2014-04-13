#pragma once

#include "..\GCL.h"
#include "GCL_MenuPage.h"

//����� �������� �� ���������� ������� ����, �� ����������� � ���������
#define GCL_CONTEXT_NUM 3
struct GCL_MenuRedrawContext
{
	struct 
	{	
		CL_Sprite *spr;
		CL_Point pos;
	}sprites[3];

	GCL_MenuRedrawContext()
	{
		for (int i=0;i<GCL_CONTEXT_NUM;i++)
		{
			sprites[i].spr=NULL;
			sprites[i].pos=CL_Point(0,0);
		}
	}
};

class GCL_Menu
{
	vector<GCL_MenuPage*> pages;
	int cur_page;
	CL_ResourceManager *resman;
public:
	static CL_Font * menu_font;
	GCL_Menu(CL_ResourceManager *RM, CL_Font *font);
	~GCL_Menu(void);
	void Draw(GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	bool ChangePage(int new_page, bool fade=false, GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	int AddPage(void);
	int AddButton(int x, int y, bool center, string res_id);
	void OnMouseUp(int mx, int my);
	void OnMouseMove(int mx, int my);
	void OnMouseDown(int mx, int my);

	// ���������� ������������� ��������� ������� ������ ��� -1
	int GetClickedButton(void)
	{
		return pages[cur_page]->GetClickedId();
	}

	// ���������� ������� ��������
	int GetCurPage(void)
	{
		return cur_page;
	}
	// ��������� ��������� ������
	int AddButton(int x, int y, bool center, string res_id, int dx, int dy);
	// ����������
	void FadeOn(GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	// ����������
	void FadeOff(GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	// ������������� ���� ��� ������ � ��������� id
	void SetBtnCheck(int id, bool check);
	// ��������� editbox �� ������� ��������
	int AddEditBox(int bx, int by, string caption);
	// ������ �������
	void OnKeyPress(int key);
	// ������������� �������� ��� ������ � ��������� id
	void SetBtnParam(int id, int pid, string value);
	// ���������� �������� ��� ������ � ��������� id
	string GetBtnParam(int id, int pid);
	// ���������� �������� value ��������� ������
	string GetBtnValue(int id);
	// ������������� �������� value ��������� ������
	void SetBtnValue(int id,string value);
	// ��������� ������ ������ ������������
	void MakeRadio(int id, int type)
	{
		pages[cur_page]->MakeRadio(id, type);
	}
};

#ifndef _DEBUG
	#pragma comment(lib, "GCL_Menu.lib")
#else
	#pragma comment(lib, "GCL_Menu_d.lib")
#endif