#pragma once
#include ".\main.h"
#include ".\powerups.h"

class CGame;

struct screate_item
{
	int id;//�������������
	int x,y;//����������
	int type;//��� ������
	int reload_time;//����� ���������
};

class CItem
{
	int id;//�������������
	int x,y;//����������
	int type;//��� ������
	int life;//�����
	int dir;//����������� ��������
	int reload_time;//����� ���������
	bool alive;//��� ��� ���
public:
	static CL_Sprite* powerups;
	static CL_Sprite *spr_item_place;
	static CGame* game;

	CItem(screate_item i);
	~CItem(void);
	// ������� ����� � �������� �����������
	void Draw(int ux, int uy,bool top=false);
	// ��������� ������
	void Update(void);
	// ���������� ������������� ��������� ������
	CL_Rect GetRect(void)
	{
		return CL_Rect(x,y,x+64,y+64);
	}

	// ���������� id
	int GetId(void)
	{
		return id;
	}

	// ���������� ��� ������
	int GetType(void)
	{
		return type;
	}

	// �������� ������ ������� �� �����������
	void Kill(void);

	// �����, �� ���?
	bool IsAlive(void)
	{
		return alive;
	}

	// ���������� ���������� ������
	CL_Point GetPosition(void)
	{
		return CL_Point(x,y);
	}
};
