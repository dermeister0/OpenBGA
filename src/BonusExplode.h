#pragma once
#include ".\explode.h"

class CBonusExplode :
	public CExplode
{
	float size;
public:
	CBonusExplode(screate_explode e);
	~CBonusExplode(void);

	// рисует взрыв
	void Draw(int ux, int uy)
	{
		sprite->set_alpha((life_time/max_life_time) * 0.3+0.1);
		float scale=1.0f - ((max_life_time - life_time)/max_life_time) * 0.7f;
		sprite->set_scale(scale,scale);
		sprite->set_angle(angle);
		sprite->draw(ux+x,uy+y);
	}
	// обновляет взрыв
	void Update(void)
	{
		life_time-=0.05;
		angle++;
		if (life_time<0)
			alive=false;
	}
};
