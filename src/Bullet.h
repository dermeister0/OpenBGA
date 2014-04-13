#pragma once
#include ".\main.h"

//���������� � ������ � ������� ��� ���������
struct scol_info;

struct screate_bullet
{
	int x,y;//����������
	int ux,uy;//��������
	int type;//��� ����
	int shooter;//id �������
	int damage;//����
	int angle;//����
};

class CBullet
{
	int type;//���
	int damage;//��������
	//������� � �.�. � �.�.

	int x,y;//����������
	int ux,uy;//�����������
	int angle;//����
	int shooter;//id �������
	bool alive;//���� ����
public:
	static CL_Sprite* bullets[5];
	CBullet(screate_bullet b);
	~CBullet(void);
	// ��������� ����
	void Update(scol_info i);
	// ������ ���� � �������� �����������
	void Draw(int _ux, int _uy);

	// ���������� true, ���� ���� ����
	bool IsAlive(void)
	{
		return alive;
	}
};
