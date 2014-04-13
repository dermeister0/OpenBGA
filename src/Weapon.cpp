#include ".\weapon.h"
#include ".\world.h"
#include ".\game.h"

CWorld * CWeapon::world;

CWeapon::CWeapon(int _type, int _damage, int _rel_time, int _bullets, int _speed, CPlayer *pl)
{
	damage=_damage;
	reload_time=_rel_time;
	bullets=_bullets;
	bul_speed=_speed;
	player=pl;
	type=_type;
}

CWeapon::~CWeapon(void)
{
}

// производит выстрел из оружия
void CWeapon::Fire(int plx, int ply, int angle)
{
	if (!bullets)//нет патронов
	{
		player->NextWeapon();
		return;
	}
	
	int cur_time=CL_System::get_time();
	if (cur_time-last_rel_time<reload_time)
		return;

	last_rel_time=cur_time;

	angle+=rand()%3-1;

	int sx=CMisc::Cos(angle)*bul_speed;
	int sy=CMisc::Sin(angle)*bul_speed;
	
	screate_bullet *b=new screate_bullet;
	b->x=plx;b->y=ply;
	b->ux=sx;b->uy=sy;
	b->type=0;//пока не используется
	b->shooter=player->id;

	if (player->HasFeature(BGA_ITEM_QUADDAMAGE-BGA_ITEM_BONUS1))//двойной урон
		b->damage=damage*2;
	else
		b->damage=damage;

	b->type=type;
	b->angle=angle;

	player->game->SendNetMessage(BGA_MSG_NEW_BULLET,b);
	
	bullets--;
}
