#pragma once
#include ".\main.h"

class CWorld;
class CPlayer;

struct sweapon_info
{
	char name[200];
	int damage;
	int reload_time;
	int speed;
	int count;
};

class CWeapon
{
	int type;//��� �����
	int damage;//��������
	int reload_time;//����� �����������
	int bullets;//���-�� ��������
	int bul_speed;//�������� ����

	int last_rel_time;//����� ��������� �����������

	static CWorld * world;//��������� �� ������� ���
	CPlayer *player;//��������� �� ������
public:
	CWeapon(int _type, int _damage, int _rel_time, int _bullets, int _speed, CPlayer *pl);
	~CWeapon(void);
	// ���������� ������� �� ������
	void Fire(int plx, int ply, int angle);

	// ������������� ��������� �� ������� ���
	static void SetGameWorld(CWorld * w)
	{
		world=w;
	}

	// ���������� ���-�� ��������
	int GetBullets(void)
	{
		return bullets;
	}

	// ��������� ��������� ���������� �������� � �������
	void AddBullets(int count)
	{
		bullets+=count;
	}

	// ������������� ������ ���-�� �������� � �����
	void SetBullets(int count)
	{
		bullets=count;
	}
};
