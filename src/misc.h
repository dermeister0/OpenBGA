#pragma once
#include <math.h>
#include ".\main.h"
char * toRUS(char * str);

class CMisc
{
	// ����� ��
	static const double N_PI;
public:
	static CL_Font *fnt_small;
	static CL_Font *fnt_menu;
	static CL_Font *fnt_about;
	CMisc(void);
	~CMisc(void);
	// ������������ ������� � �������
	static double DegToRad(double angle);
	// ���������� 1, ���� ����� �������., � ����. ������ -1
	static int Sign(double num);
	// ��������� ����� � ������� ���������� ��� �����, ������� 90
	static double Sin(int angle);
	// ��������� ������� � ������� ���������� ��� �����, ������� 90
	static double Cos(int angle);
	// ���������� true, ���� ����� ���������� �����, ��� ���� �� ��� ����� 0
	static bool EqualSignOrZero(double a, double b);
	// ��������� ���-�� ������ � �������
	static int GetFPS(void);
	// ���������� �����, ������� �����
	static CL_Point LeftPoint(CL_Point a, CL_Point b);
	// ���������� �����, ������� ����
	static CL_Point UpPoint(CL_Point a, CL_Point b);
	// ���������� �����, ������� ����
	static CL_Point DownPoint(CL_Point a, CL_Point b);
	// ���������� �����, ������� ������
	static CL_Point RightPoint(CL_Point a, CL_Point b);
	// ������� ����� � ����� � ������ �����������, ������ ������� � ��������
	static void ShadowText(int x, int y, int w, CL_Font * font, string text);
	static void ShadowText(CL_Rect dest, int w, CL_Font * font, string text);
	static void ShadowText(int x, int y, int w, CL_Font * font, CL_Color color, string text);
	// ���������� ���� ����� ����� �������
	static int Get2PAngle(CL_Point a, CL_Point b);
	// ������������ ������� � ����
	static int RadToDeg(double angle);
	// ���������� ������� �� ������� �����
	static int Min(int a, int b, int c, int d);
	// ���������� ���������� ��������� ����� �� ������� ����� 
	static int MinNoZero(int a, int b, int c, int d);
};
