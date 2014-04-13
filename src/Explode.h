#pragma once
#include ".\main.h"

#define BGA_EXPLODE_NORMAL 1 //������� �����
#define BGA_EXPLODE_SMOKE  2 //��� �� �����
#define BGA_EXPLODE_BONUS  3 //����� ������

#define BGA_EXPLODE_WEAP1  4 //����� ���� ��������
#define BGA_EXPLODE_WEAP2  5 //����� ���� ���������
#define BGA_EXPLODE_WEAP3  6 //����� ���� ���������
#define BGA_EXPLODE_WEAP4  7 //����� ���� ����������
#define BGA_EXPLODE_WEAP5  8 //����� ���� ��������������

#define BGA_EXPLODE_GREEN_SMOKE  9 //����� ��������������
#define BGA_EXPLODE_BLUE_RING  10 //����� ����������

#define BGA_EXPLODE_RESPAWN  11 //����� �� ���������

struct screate_explode
{
	int x,y;//���������� ������
	int vx,vy;//��������
	int type;//���
	double scale;//������� �������
	CL_Sprite *sprite;//��������������� ��������� ������
	screate_explode()
	{
		scale=1.0;
	}
};

class CExplode
{
protected:
	CL_Sprite *sprite;

	int angle;
	int x,y;
	int vx,vy;
	double scale;
	bool alive;
	float max_life_time;
	float life_time;
public:
	static CL_Sprite *spr_explode;
	static CL_Sprite *spr_w1_explode;
	static CL_Sprite *spr_w2_explode;
	static CL_Sprite *spr_w3_explode;
	static CL_Sprite *spr_w4_explode;
	static CL_Sprite *spr_w5_explode;
	static CL_Sprite *spr_respawn_explode;
	CExplode(screate_explode e);
	~CExplode(void);
	virtual void Draw(int ux, int uy);
	virtual void Update();
	bool IsAlive()
	{
		return alive;
	}
};
