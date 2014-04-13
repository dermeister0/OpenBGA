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

	// возвращает идентификатор последней нажатой кнопки или -1
	static int GetClickedId(void)
	{
		int t=ClickedBtn;
		ClickedBtn=-1;
		return t;
	}
	// добавл€ет плавающую кнопку
	int AddButton(int x, int y, bool center, string res_id, int dx, int dy);
	// устанавливает флаг дл€ кнопки с указанным id
	void SetBtnCheck(int id, bool check);
	// добавлет editbox на страницу
	int AddEditBox(int bx, int by, string res_id);
	// нажата клавиша
	void OnKeyPress(int key);
	// устанавливает параметр дл€ кнопки с указанным id
	void SetBtnParam(int id, int pid, string value);
	// возвращает параметр дл€ кнопки с указанным id
	string GetBtnParam(int id, int pid);
	// объ€вл€ет данную кнопку радиокнопкой
	void MakeRadio(int id, int type)
	{
		buttons[id]->MakeRadio(type);
	}
};
