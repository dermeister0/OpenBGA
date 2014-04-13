#include ".\item.h"
#include ".\explode.h"
#include ".\game.h"

CL_Sprite* CItem::powerups;
CL_Sprite *CItem::spr_item_place;
CGame* CItem::game;

CItem::CItem(screate_item i)
{
	x=i.x;
	y=i.y;
	type=i.type;

	reload_time=i.reload_time;
	life=rand()%reload_time;

	dir=rand()%2-1;
	dir=CMisc::Sign(dir);

	alive=true;
	id=i.id;
}

CItem::~CItem(void)
{
}

// ������� ����� � �������� �����������
void CItem::Draw(int ux, int uy,bool top)
{
	if (!alive)
		return;

	//��������� �����
	if (type>=BGA_ITEM_SPCUSTOM && type<=BGA_ITEM_SPBLUE)
		return;
	
	if (!top)
	{
		powerups->set_angle(life);
		powerups->set_frame(type);
		powerups->draw(ux+x+32,uy+y+32);
	}
	else
	{
		spr_item_place->set_alpha(0.4);
		spr_item_place->set_scale(1,1);
		spr_item_place->set_angle(360-life);
		//spr_item_place->set_color(0,0,255,0.4);
		spr_item_place->draw(ux+x+32,uy+y+32);
	}
}

// ��������� ������
void CItem::Update(void)
{
	if (life)
		life+=dir;
	else
	{
		life=360;
		if (!alive)
			alive=true;
	}
}

// �������� ������ ������� �� �����������
void CItem::Kill(void)
{
	alive=false;
	life=reload_time;
	dir=-1;


	//"�����" ������
	screate_explode *e=new screate_explode;
	e->x=x+32;e->y=y+32;
	e->vx=0;
	e->vy=0;
	e->type=BGA_EXPLODE_BONUS;

	game->SendNetMessage(BGA_MSG_NEW_EXPLODE,e);
}