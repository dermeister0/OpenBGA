#pragma once
#include ".\main.h"
#include ".\soundtypes.h"

struct splay_sound
{
	int id,x,y;
};

class CListener
{
	int x,y;
	int volume;//громкость звука в игре
	CL_SoundBuffer *sounds[BGA_SOUND_COUNT];
public:
	static CL_SoundBuffer *snd_engine;
	CListener(CL_ResourceManager *RM);
	~CListener(void);
	// устанавливает позицию слушателя
	void SetPosition(int _x, int _y);
	// проигрывает звук с указанным id в заданном месте
	void PlaySound(int id, int px, int py);
	// заставляет остановить все звуки
	void ShutUp(void);
	// изменяет звуковую сессию в соответствии с координатами
	void ModifySound(CL_SoundBuffer_Session* buf, int px, int py);
};
