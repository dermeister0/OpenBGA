#pragma once

#include "..\GCL.h"
#include "GCL_MenuPage.h"

//набор спрайтов не €вл€ющихс€ членами меню, но нуждающихс€ в отрисовке
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

	// возвращает идентификатор последней нажатой кнопки или -1
	int GetClickedButton(void)
	{
		return pages[cur_page]->GetClickedId();
	}

	// возвращает текущую страницу
	int GetCurPage(void)
	{
		return cur_page;
	}
	// добавл€ет плавающую кнопку
	int AddButton(int x, int y, bool center, string res_id, int dx, int dy);
	// затемнение
	void FadeOn(GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	// осветление
	void FadeOff(GCL_MenuRedrawContext context=GCL_MenuRedrawContext());
	// устанавливает флаг дл€ кнопки с указанным id
	void SetBtnCheck(int id, bool check);
	// добавл€ет editbox на текущую страницу
	int AddEditBox(int bx, int by, string caption);
	// нажата клавиша
	void OnKeyPress(int key);
	// устанавливает параметр дл€ кнопки с указанным id
	void SetBtnParam(int id, int pid, string value);
	// возвращает параметр дл€ кнопки с указанным id
	string GetBtnParam(int id, int pid);
	// возвращает параметр value указанной кнопки
	string GetBtnValue(int id);
	// устанавливает параметр value указанной кнопки
	void SetBtnValue(int id,string value);
	// объ€вл€ет данную кнопку радиокнопкой
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