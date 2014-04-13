#include ".\camera.h"
#include ".\player.h"
#include ".\game.h"

CL_Sprite* CCamera::wicons[6];
CL_Sprite* CCamera::ficons[6];
CL_Sprite* CCamera::spr_armor;
CL_Sprite* CCamera::spr_health;
CL_Sprite* CCamera::spr_aim;
CL_Font *CCamera::fnt_interface;

CCamera::CCamera(int _type, CPlayer *_pl)
: dx(0), dy(0), vx(0), vy(0)
{
	type=_type;
	pl=_pl;
	switch (type)
	{
	case CT_ALLSCREEN:
		x=0;
		y=0;
		w=SCR_W;
		h=SCR_H;
		break;
	case CT_UP:
		x=0;
		y=0;
		w=SCR_W;
		h=SCR_H/2;
		break;
	case CT_UPLEFT:
		x=0;
		y=0;
		w=SCR_W/2;
		h=SCR_H/2;
		break;
	case CT_UPCENTER:
		x=SCR_W/4;
		y=0;
		w=SCR_W/2;
		h=SCR_H/2;
		break;
	case CT_UPRIGHT:
		x=SCR_W/2;
		y=0;
		w=SCR_W/2;
		h=SCR_H/2;
		break;
	case CT_DOWNLEFT:
		x=0;
		y=SCR_H/2;
		w=SCR_W/2;
		h=SCR_H/2;
		break;
	case CT_DOWNRIGHT:
		x=SCR_W/2;
		y=SCR_H/2;
		w=SCR_W/2;
		h=SCR_H/2;
		break;
	case CT_DOWN:
		x=0;
		y=SCR_H/2;
		w=SCR_W;
		h=SCR_H/2;
		break;
	}
}

CCamera::~CCamera(void)
{
}

// устанавливает область экрана для камеры
void CCamera::SetRect(int _x, int _y, int _w, int _h)
{
	x=_x;
	y=_y;
	w=_w;
	h=_h;
}

// возвращает область камеры
CL_Rect CCamera::GetRect(void)
{
	return CL_Rect(x,y,x+w,y+h);
}

// возвращает тип камеры
int CCamera::GetType(void)
{
	return type;
}

// едет следом за игроком
void CCamera::FollowPlayer(int mw, int mh)
{
	//центр камеры
	int cx=dx+w/2;
	int cy=dy+h/2;
	//мертвая зона
	//CL_Rect dz(pl->GetX()-w/4,pl->GetY()-h/4,pl->GetX()+w/4,pl->GetY()+h/4);
	//CL_Rect dz(cx-w/4,cy-h/4,cx+w/4,cy+h/4);
	//CL_Display::draw_rect(CL_Rect(dz.left-dx,dz.top-dy,dz.right-dx,dz.bottom-dy),CL_Color::red);
	//CL_Display::draw_rect(CL_Rect(dx,dy,dx+10,dy+10),CL_Color::red);


	//if (dz.is_inside(CL_Point(pl->GetX(),pl->GetY())))
	//	return;

	//перемещаем камеру
	vx=(double)(pl->cr_t.x-cx)/(double)(15);
	vy=(double)(pl->cr_t.y-cy)/(double)(15);
	dx+=(int)(vx+0.5);
	dy+=(int)(vy+0.5);

	if (dx<0)dx=0;
	if (dx+w>mw*64)dx=mw*64-w;
	if (dy<0)dy=0;
	if (dy+h>mh*64)dy=mh*64-h;
}

// рисует интерфейс для игрока
void CCamera::DrawInterface(void)
{
	int step;
	if (type==CT_ALLSCREEN || type==CT_UP || type==CT_DOWN)
	{
		//самый красивый и полный интерфейс
		//return;
		step=50;
	}
	else
		step=10;

	//интерфейс в четверть экрана

	//черный "квадрат"
	//CL_Display::fill_rect(CL_Rect(x,y,w,15),CL_Color::black);
	
	int tx=step/2;
	int ty=h-5+y;

	ShadowFont(x+tx,y+5,pl->name,origin_top_left);

	for (int i=0;i<6;i++)
	{
		if (pl->features.all[i])
			ficons[i]->draw(x+tx+i*32,y+35);
	}

	//жизни	
	tx+=fnt_interface->get_width(CL_String::from_int(999));
	ShadowFont(x+tx,ty,CL_String::from_int(pl->life),origin_bottom_right);
	spr_health->draw(x+tx,ty-30);
	//CL_Display::fill_rect(CL_Rect(x+tx,ty-30,x+tx+30,ty),CL_Color::red);
	tx+=step+20;

	//патроны
	tx+=fnt_interface->get_width(CL_String::from_int(999));
	if (!pl->no_weapon)
	{
		ShadowFont(x+tx,ty,CL_String::from_int(pl->weapons[pl->cur_weapon]->GetBullets()),origin_bottom_right);
		wicons[pl->cur_weapon]->draw(x+tx,ty-30);
	}
	tx+=step+20;
	
	/*len=fnt_interface->get_width(CL_String::from_int(pl->life));
	int len2=fnt_interface->get_width(CL_String::from_int(pl->armor));
	int step=(right-left-len-len2)/3;*/
	
	//броня
	tx+=fnt_interface->get_width(CL_String::from_int(999));
	if (pl->armor)
	{
		ShadowFont(x+tx,ty,CL_String::from_int(pl->armor),origin_bottom_right);
		spr_armor->draw(x+tx,ty-30);
	}
	//CL_Display::fill_rect(CL_Rect(x+tx,ty-30,x+tx+30,ty),CL_Color::yellow);

	//фраги
	int frlim, tlim;
	pl->game->GetLimits(frlim, tlim);

	int frw=50;
	int frh=30;
	int left=w-step-frw*3;
	ty=y+h-frh-5;

	//CL_Display::fill_rect(CL_Rect(x+left,ty,x+left+frw,ty+25),CL_Color::white);
	CL_Rect first(x+left,ty,x+left+frw,ty+frh);
	CL_Rect second(x+left+frw,ty,x+left+frw*2,ty+frh);
	CL_Rect third(x+left+frw*2,ty,x+left+frw*3,ty+frh);

	CL_Display::fill_rect(second,CL_Color::darkblue);
	CL_Display::fill_rect(third,CL_Color::darkred);

	if (frlim)
	{
		CL_Display::draw_rect(first,CL_Color::white);
		ShadowFont(first,CL_String::from_int(frlim));
	}

	CL_Display::draw_rect(second,CL_Color::white);
	CL_Display::draw_rect(third,CL_Color::white);

	int gt=pl->game->GetGameType();

	if (gt==BGA_GAME_DEATHMATCH)//фраги гамера
	{
		ShadowFont(second,CL_String::from_int(pl->frags));
		ShadowFont(third,CL_String::from_int(pl->game->GetMaxEnemyFrags(pl->id)));
	}
	if (gt==BGA_GAME_TEAMDEATHMATCH || gt==BGA_GAME_CAPTURETHEFLAG)//фраги/флаги команды
	{
		ShadowFont(second,CL_String::from_int(pl->game->GetBlueScore()));
		ShadowFont(third,CL_String::from_int(pl->game->GetRedScore()));
	}
	//лимит времени
	if (tlim)
	{
		ShadowFont(x+w/2,y+5,CL_String::from_int(pl->game->GetRemainTime())+" мин.",origin_top_center);
	}

	//прицел
	int ax=pl->cr_target.x+x-dx;
	int ay=pl->cr_target.y+y-dy;
	//CL_Display::draw_line(ax,ay-20,ax,ay+20,CL_Color::lightgreen);
	//CL_Display::draw_line(ax-20,ay,ax+20,ay,CL_Color::lightgreen);
	if (!pl->aimed)
		spr_aim->set_frame(0);
	else
		spr_aim->set_frame(1);
	spr_aim->draw(ax,ay);

	//CL_Display::draw_rect(CL_Rect(ax-100,ay-100,ax+100,ay+100),CL_Color::blueviolet);
}

// выводит текст с тенью
void CCamera::ShadowFont(int x, int y, string text, CL_Origin alignment)
{
	fnt_interface->set_alignment(alignment);
	CMisc::ShadowText(x,y,3,fnt_interface,text);
}

// выводит текст с тенью
void CCamera::ShadowFont(CL_Rect dest, string text)
{
	fnt_interface->set_alignment(origin_center);

	CMisc::ShadowText(dest,3,fnt_interface,text);
}