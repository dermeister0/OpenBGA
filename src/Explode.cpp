#include ".\explode.h"

CL_Sprite *CExplode::spr_explode;
CL_Sprite *CExplode::spr_w1_explode;
CL_Sprite *CExplode::spr_w2_explode;
CL_Sprite *CExplode::spr_w3_explode;
CL_Sprite *CExplode::spr_w4_explode;
CL_Sprite *CExplode::spr_w5_explode;
CL_Sprite *CExplode::spr_respawn_explode;

CExplode::CExplode(screate_explode e)
{
	sprite=e.sprite;
	angle=rand()%360;
	alive=true;

	x=e.x;
	y=e.y;
	vx=e.vx;
	vy=e.vy;
	scale=e.scale;

	max_life_time=sprite->get_frame_count()+1;
	life_time=0;
}

CExplode::~CExplode(void)
{
}

void CExplode::Draw(int ux, int uy)
{
	int f=(int)sprite->get_frame_count()*life_time/max_life_time;
	sprite->set_frame(f);
	double s=(max_life_time/2-fabs(max_life_time/2-life_time))/max_life_time+scale;
	sprite->set_scale(s,s);
	sprite->set_angle(angle);
	sprite->draw(ux+x,uy+y);
}

void CExplode::Update(void)
{
	x+=vx;y+=vy;
	life_time+=0.3;
	if (life_time>max_life_time)
		alive=false;
}