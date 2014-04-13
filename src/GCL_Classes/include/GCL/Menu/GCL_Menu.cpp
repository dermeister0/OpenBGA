/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_Menu'
	description: class for using menu
	author: 'Tosha'
	date: '07.10.2005'
	version: '0.0.60'
*/

#include ".\gcl_menu.h"
CL_Font * GCL_Menu::menu_font;

GCL_Menu::GCL_Menu(CL_ResourceManager *RM, CL_Font *font)
{
	resman=RM;
	GCL_MenuPage *first=new GCL_MenuPage(resman);
	pages.push_back(first);
	cur_page=0;
	menu_font=font;
}

GCL_Menu::~GCL_Menu(void)
{
	for (int i=0;i<pages.size();i++)
		delete pages[i];
}

void GCL_Menu::Draw(GCL_MenuRedrawContext context)
{
	for (int i=0;i<GCL_CONTEXT_NUM;i++)
	{
		if (context.sprites[i].spr)
			context.sprites[i].spr->draw(context.sprites[i].pos.x,context.sprites[i].pos.y);
	}	
	pages[cur_page]->Draw();
}

bool GCL_Menu::ChangePage(int new_page, bool fade, GCL_MenuRedrawContext context)
{
	if (new_page>=0 && new_page<pages.size())
	{
		if (fade)
			FadeOn(context);
		cur_page=new_page;
		pages[cur_page]->OnMouseMove(0,0);
		if (fade)
			FadeOff(context);
		return true;
	}

	return false;
}

int GCL_Menu::AddPage(void)
{
	GCL_MenuPage *temp=new GCL_MenuPage(resman);
	pages.push_back(temp);
	cur_page=pages.size()-1;
 	return cur_page;
}

int GCL_Menu::AddButton(int x, int y, bool center, string res_id)
{
	GCL_MenuPage *tp=pages[cur_page];
	int id=tp->AddButton(x,y,center,res_id);
	return id;
}

void GCL_Menu::OnMouseUp(int mx, int my)
{
	GCL_MenuPage *tp=pages[cur_page];
	tp->OnMouseUp(mx, my);
}

void GCL_Menu::OnMouseMove(int mx, int my)
{
	GCL_MenuPage *tp=pages[cur_page];
	tp->OnMouseMove(mx, my);
}

void GCL_Menu::OnMouseDown(int mx, int my)
{
	GCL_MenuPage *tp=pages[cur_page];
	tp->OnMouseDown(mx, my);
}

// добавляет плавающую кнопку
int GCL_Menu::AddButton(int x, int y, bool center, string res_id, int dx, int dy)
{
	GCL_MenuPage *tp=pages[cur_page];
	int id=tp->AddButton(x,y, center,res_id,dx,dy);
	return id;
}

// затемнение
void GCL_Menu::FadeOn(GCL_MenuRedrawContext context)
{
	for(int a=0;a<256;a+=5)
	{
		unsigned int begin_time=CL_System::get_time();
		//очищаем экран
		CL_Display::clear();

		for (int i=0;i<GCL_CONTEXT_NUM;i++)
		{
			if (context.sprites[i].spr)
				context.sprites[i].spr->draw(context.sprites[i].pos.x,context.sprites[i].pos.y);
		}		
		pages[cur_page]->Draw();
		CL_Display::fill_rect(CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()),CL_Color(0,0,0,a));
	
		//смена поверхностей
		CL_Display::flip();
		//обновление
		CL_System::keep_alive();

		while (CL_System::get_time()-begin_time<16.6);//ограничение на FPS~60

	}
}

// осветление
void GCL_Menu::FadeOff(GCL_MenuRedrawContext context)
{
	for(int a=255;a>0;a-=5)
	{
		unsigned int begin_time=CL_System::get_time();
		//очищаем экран
		CL_Display::clear();

		for (int i=0;i<GCL_CONTEXT_NUM;i++)
		{
			if (context.sprites[i].spr)
				context.sprites[i].spr->draw(context.sprites[i].pos.x,context.sprites[i].pos.y);
		}	
		pages[cur_page]->Draw();
		CL_Display::fill_rect(CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()),CL_Color(0,0,0,a));
	
		//смена поверхностей
		CL_Display::flip();
		//обновление
		CL_System::keep_alive();

		while (CL_System::get_time()-begin_time<16.6);//ограничение на FPS~60

	}
}

// устанавливает флаг для кнопки с указанным id
void GCL_Menu::SetBtnCheck(int id, bool check)
{
	pages[cur_page]->SetBtnCheck(id, check);
}

// добавляет editbox на текущую страницу
int GCL_Menu::AddEditBox(int bx, int by, string caption)
{
	GCL_MenuPage *tp=pages[cur_page];
	int id=tp->AddEditBox(bx,by,caption);
	return id;
}

// нажата клавиша
void GCL_Menu::OnKeyPress(int key)
{
	GCL_MenuPage *tp=pages[cur_page];
	tp->OnKeyPress(key);
}

// устанавливает параметр для кнопки с указанным id
void GCL_Menu::SetBtnParam(int id, int pid, string value)
{
	pages[cur_page]->SetBtnParam(id,pid,value);
}

// возвращает параметр для кнопки с указанным id
string GCL_Menu::GetBtnParam(int id, int pid)
{
	return pages[cur_page]->GetBtnParam(id,pid);
}
// возвращает параметр value указанной кнопки
string GCL_Menu::GetBtnValue(int id)
{
	return pages[cur_page]->GetBtnParam(id,GCL_EDITBOX_VALUE);
}

// устанавливает параметр value указанной кнопки
void GCL_Menu::SetBtnValue(int id,string value)
{
	pages[cur_page]->SetBtnParam(id,GCL_EDITBOX_VALUE,value);
}