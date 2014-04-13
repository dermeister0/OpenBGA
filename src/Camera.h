#pragma once
#include ".\main.h"

class CPlayer;

class CCamera
{
	int x,y,w,h,type;
	// �������� �� ��� X
	int dx;
	// �������� �� ��� Y
	int dy;
	double vx,vy;//��������� �� X � Y
	CPlayer* pl; //��������� �� ������

public:
	static CL_Font *fnt_interface;
	static CL_Sprite* wicons[6];//������ ������
	static CL_Sprite* ficons[6];//������ �������
	static CL_Sprite* spr_armor;
	static CL_Sprite* spr_health;
	static CL_Sprite* spr_aim;
	enum ecamera_type{CT_NOCAMERA, CT_ALLSCREEN, CT_UP, CT_DOWN, CT_UPLEFT, CT_UPCENTER, CT_UPRIGHT, CT_DOWNRIGHT, CT_DOWNLEFT};
	CCamera(int _type, CPlayer *_pl);
	~CCamera(void);
	// ������������� ������� ������ ��� ������
	void SetRect(int _x, int _y, int _w, int _h);
	// ���������� ������� ������
	CL_Rect GetRect(void);
	// ���������� ��� ������
	int GetType(void);
	// ���� ������ �� �������
	void FollowPlayer(int mw, int mh);

	int GetDX(void)
	{
		return dx;
	}

	int GetDY(void)
	{
		return dy;
	}
	// ������ ��������� ��� ������
	void DrawInterface(void);
private:
	// ������� ����� � �����
	void ShadowFont(int x, int y, string text, CL_Origin alignment);
	void ShadowFont(CL_Rect dest, string text);
};
