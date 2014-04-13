#include ".\smoke.h"

CL_Sprite *CSmoke::spr_smoke;
CL_Sprite *CSmoke::spr_green_smoke;
CL_Sprite *CSmoke::spr_blue_ring;

CSmoke::CSmoke(screate_explode e):CExplode(e)
{
    max_life_time = 3.0f;
    life_time = max_life_time;
}

CSmoke::~CSmoke(void)
{
}
