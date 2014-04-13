#pragma once
#include "explode.h"

class CSmoke :
	public CExplode
{
	float size;
public:
	static CL_Sprite *spr_smoke;
	static CL_Sprite *spr_green_smoke;
	static CL_Sprite *spr_blue_ring;

	CSmoke(screate_explode e);
	~CSmoke(void);

	// рисует дым
	void Draw(int ux, int uy)
	{
		sprite->set_alpha((life_time/max_life_time) * 0.3);
		float scale=0.2f + ((1 - life_time/max_life_time)) * 0.7f;
		sprite->set_scale(scale,scale);
		sprite->set_angle(angle);
		//int pitch=sprite->get_width()*scale/2;
		sprite->draw(ux+x,uy+y);
	}
	// обновляет дым
	void Update(void)
	{
		x+=vx;y+=vy;
		life_time-=0.05;
		if (life_time<0)
			alive=false;
	}
};
