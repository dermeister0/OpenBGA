#include ".\listener.h"

CL_SoundBuffer *CListener::snd_engine;

CListener::CListener(CL_ResourceManager *RM)
{
	LOG<<"Загрузка звуков\n";
	sounds[BGA_SND_SHOT1]=new CL_SoundBuffer("sfx/weapons/1",RM);
	sounds[BGA_SND_SHOT2]=new CL_SoundBuffer("sfx/weapons/2",RM);
	sounds[BGA_SND_SHOT3]=new CL_SoundBuffer("sfx/weapons/3",RM);
	sounds[BGA_SND_SHOT4]=new CL_SoundBuffer("sfx/weapons/4",RM);
	sounds[BGA_SND_SHOT5]=new CL_SoundBuffer("sfx/weapons/5",RM);

	sounds[BGA_SND_SHOTHIT]=new CL_SoundBuffer("sfx/misc/hit",RM);
	sounds[BGA_SND_RESPAWN]=new CL_SoundBuffer("sfx/misc/respawn",RM);
	sounds[BGA_SND_ENGINE]=new CL_SoundBuffer("sfx/misc/engine",RM);

	sounds[BGA_SND_EXPLODE]=new CL_SoundBuffer("sfx/effects/explode",RM);

	sounds[BGA_SND_HASTE]=new CL_SoundBuffer("sfx/items/haste",RM);
	sounds[BGA_SND_QUADDAMAGE]=new CL_SoundBuffer("sfx/items/quaddamage",RM);
	sounds[BGA_SND_INVISIBILITY]=new CL_SoundBuffer("sfx/items/invisibility",RM);
	sounds[BGA_SND_REGENERATION]=new CL_SoundBuffer("sfx/items/regeneration",RM);

	sounds[BGA_SND_AMMO]=new CL_SoundBuffer("sfx/items/ammo",RM);
	sounds[BGA_SND_ARMOR]=new CL_SoundBuffer("sfx/items/armor",RM);
	sounds[BGA_SND_HEALTH]=new CL_SoundBuffer("sfx/items/health",RM);

	x=0,y=0;
	snd_engine=sounds[BGA_SND_ENGINE];

#ifdef _DEBUG
	volume=0;
#else
	volume=1; 
#endif
}

CListener::~CListener(void)
{
	LOG<<"Уничтожение звуков\n";

	for (int i=0;i<BGA_SOUND_COUNT;i++)
		delete sounds[i];
	
	ShutUp();
}

// устанавливает позицию слушателя
void CListener::SetPosition(int _x, int _y)
{
	x=_x;
	y=_y;
}

// проигрывает звук с указанным id в заданном месте
void CListener::PlaySound(int id, int px, int py)
{
	CL_Point listener(x,y);
	//псевдо-3d звук
	float vol=listener.distance(CL_Point(px,py));
	if (vol>2000)
		vol=1;
	else
		vol/=2000.f;
	sounds[id]->set_volume((1-vol)*volume);

	sounds[id]->play();
}

// заставляет остановить все звуки
void CListener::ShutUp(void)
{
}

// изменяет звуковую сессию в соответствии с координатами
void CListener::ModifySound(CL_SoundBuffer_Session* buf, int px, int py)
{
	CL_Point listener(x,y);
	//псевдо-3d звук
	float vol=listener.distance(CL_Point(px,py));
	if (vol>2000)
		vol=1;
	else
		vol/=2000.f;
	buf->set_volume((1-vol)*volume);

}
