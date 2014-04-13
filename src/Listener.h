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
	int volume;//��������� ����� � ����
	CL_SoundBuffer *sounds[BGA_SOUND_COUNT];
public:
	static CL_SoundBuffer *snd_engine;
	CListener(CL_ResourceManager *RM);
	~CListener(void);
	// ������������� ������� ���������
	void SetPosition(int _x, int _y);
	// ����������� ���� � ��������� id � �������� �����
	void PlaySound(int id, int px, int py);
	// ���������� ���������� ��� �����
	void ShutUp(void);
	// �������� �������� ������ � ������������ � ������������
	void ModifySound(CL_SoundBuffer_Session* buf, int px, int py);
};
