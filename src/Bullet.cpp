#include ".\bullet.h"
#include ".\world.h"
CL_Sprite* CBullet::bullets[5];

CBullet::CBullet(screate_bullet b)
{
	type=b.type;
	x=b.x;
	y=b.y;
	ux=b.ux;
	uy=b.uy;
	angle=b.angle;
	shooter=b.shooter;
	alive=true;
	damage=b.damage;
}

CBullet::~CBullet(void)
{
}

// обновляет пулю
void CBullet::Update(scol_info i)
{
	if (i.map->IsWall(x,y,CL_Point()))
	{
		//самоуничтожение
		alive=false;

		//взрыв
		screate_explode *e=new screate_explode;
		e->x=x-ux/2;e->y=y-uy/2;
		e->vx=0;
		e->vy=0;

		e->type=BGA_EXPLODE_WEAP1+type;

		i.world->AddExplode(e);

		return;
	}
	for (int j=0;j<i.players->size();j++)
	{
		//попала в игрока
		CL_Rect t=(*i.players)[j]->GetHitRect();
		if (t.is_inside(CL_Point(x,y))&&shooter!=(*i.players)[j]->GetId())
		{
			//клиенты сами отнимают жизни своему игроку
			if ((*i.players)[j]->IsLocal())
			{
				(*i.players)[j]->DecLife(damage, shooter, type);
			}

			i.world->listener->PlaySound(BGA_SND_SHOTHIT,x,y);

			//самоуничтожение
			alive=false;
			return;
		}
	}

	x+=ux;
	y+=uy;

	//шлейф
	if (type==2 || type==4 || type==3)
	{
		screate_explode *e=new screate_explode;
		e->x=x;e->y=y;
		e->vx=-ux/7;
		e->vy=-uy/7;

		if (type==2)//ракетница
			e->type=BGA_EXPLODE_SMOKE;
		else if (type==3)//плазмаган
			e->type=BGA_EXPLODE_BLUE_RING;
		else if (type==4)//дезинтегратор
			e->type=BGA_EXPLODE_GREEN_SMOKE;

		i.world->AddExplode(e);
	}
}

// рисует пулю в условных координатах
void CBullet::Draw(int _ux, int _uy)
{
	//CL_Display::fill_rect(CL_Rect(_ux+x,_uy+y,_ux+x+10,_uy+y+10),CL_Color::blue);
	CL_Sprite *spr=bullets[type];
	spr->set_angle(angle);
	spr->draw(_ux+x,_uy+y);
}
