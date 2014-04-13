/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_MenuPage'
	description: class for using different menu screens
	author: 'Tosha'
	date: '07.10.2005'
	version: '0.0.52'
*/

#include ".\gcl_menupage.h"
#include <iostream>
#include <windows.h>

int GCL_MenuPage::ClickedBtn=-1;

GCL_MenuPage::GCL_MenuPage(CL_ResourceManager * RM)
{
	resman=RM;
}

GCL_MenuPage::~GCL_MenuPage(void)
{
	for (int i=0;i<buttons.size();i++)
		delete buttons[i];
}

int GCL_MenuPage::AddButton(int x, int y, bool center, string res_id)
{
	GCL_Button *temp=new GCL_Button(x,y,center,buttons.size(),res_id,resman);
	buttons.push_back(temp);
	return buttons.size()-1;
}

void GCL_MenuPage::Draw(void)
{
	for (int i=0;i<buttons.size();i++)
		buttons[i]->Draw();
}

void GCL_MenuPage::OnMouseUp(int mx, int my)
{
	GCL_Button *btn;
	for (int i=0;i<buttons.size();i++)
	{
		btn=buttons[i];
		if (btn->HavePoint(mx,my))
		{
			ClickedBtn=btn->GetId();
		}
	}
}

void GCL_MenuPage::OnMouseDown(int mx, int my)
{
	GCL_Button *btn;
	for (int i=0;i<buttons.size();i++)
	{
		btn=buttons[i];
		if (btn->HavePoint(mx,my))
		{
			btn->SetFrame(GCL_Button::GCL_BUTTON_FRAME_DOWN);
		}
	}
}

void GCL_MenuPage::OnMouseMove(int mx, int my)
{
	GCL_Button *btn;
	for (int i=0;i<buttons.size();i++)
	{
		btn=buttons[i];
		if (btn->HavePoint(mx,my))
			btn->SetFrame(GCL_Button::GCL_BUTTON_FRAME_ON);
		else
			btn->SetFrame(GCL_Button::GCL_BUTTON_FRAME_OFF);
	}
}

// добавляет плавающую кнопку
int GCL_MenuPage::AddButton(int x, int y, bool center, string res_id, int dx, int dy)
{
	GCL_Button *temp=new GCL_MovingButton(x,y,center,buttons.size(),res_id,resman,dx,dy);
	buttons.push_back(temp);
	return buttons.size()-1;
}

// устанавливает флаг для кнопки с указанным id
void GCL_MenuPage::SetBtnCheck(int id, bool check)
{
	buttons[id]->SetChecked(check);
}

// устанавливает параметр для кнопки с указанным id
void GCL_MenuPage::SetBtnParam(int id, int pid, string value)
{
	buttons[id]->SetParam(pid,value);
}

// возвращает параметр для кнопки с указанным id
string GCL_MenuPage::GetBtnParam(int id, int pid)
{
	return buttons[id]->GetParam(pid);
}

// добавлет editbox на страницу
int GCL_MenuPage::AddEditBox(int bx, int by, string res_id)
{
	GCL_Button *temp=new GCL_EditBox(bx,by,buttons.size(),res_id,resman);
	buttons.push_back(temp);
	return buttons.size()-1;
}

// нажата клавиша
void GCL_MenuPage::OnKeyPress(int key)
{
	GCL_Button *btn;
	for (int i=0;i<buttons.size();i++)
	{
		btn=buttons[i];
		if (btn->IsChecked())
		{
			btn->OnKeyPress(key);
		}
	}
}
