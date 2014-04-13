#pragma once
#include "gcl_button.h"
#include "..\GCL.h"

class GCL_MovingButton :
	public GCL_Button
{
	CL_Rect moving_rect;
	double ux,uy;//��������
	double rx,ry;//������ ����������
	unsigned int next_change;//����� ���������� ��������
	unsigned int next_move;//����� ���������� �����������
public:
	GCL_MovingButton(int bx, int by, bool center, int bid, string res_id, CL_ResourceManager *RM,int dx, int dy);
	~GCL_MovingButton();
	void Draw();
private:
	// �������� ����������� �������� ������
	void ChangeDir(void);
	// ��������������� �������� ������
	void Move(void);
public:
	// �������� ����������� �� ��� X
	void ChangeXDir(void);
	// �������� ����������� �� ��� Y
	void ChangeYDir(void);
};
