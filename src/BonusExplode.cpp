#include ".\bonusexplode.h"

CBonusExplode::CBonusExplode(screate_explode e):CExplode(e)
{
    max_life_time = 3.0f;
    life_time = max_life_time;
}

CBonusExplode::~CBonusExplode(void)
{
}
