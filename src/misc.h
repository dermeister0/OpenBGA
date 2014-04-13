#pragma once
#include <math.h>
#include ".\main.h"
char * toRUS(char * str);

class CMisc
{
	// число ПИ
	static const double N_PI;
public:
	static CL_Font *fnt_small;
	static CL_Font *fnt_menu;
	static CL_Font *fnt_about;
	CMisc(void);
	~CMisc(void);
	// конвертирует градусы в радианы
	static double DegToRad(double angle);
	// возвращает 1, если число неотриц., в прот. случае -1
	static int Sign(double num);
	// вычисляет синус с точными значениями для углов, кратных 90
	static double Sin(int angle);
	// вычисляет косинус с точными значениями для углов, кратных 90
	static double Cos(int angle);
	// возвращает true, если знаки параметров равны, или один из них равен 0
	static bool EqualSignOrZero(double a, double b);
	// возращает кол-во кадров в секунду
	static int GetFPS(void);
	// возвращает точку, которая левее
	static CL_Point LeftPoint(CL_Point a, CL_Point b);
	// возвращает точку, которая выше
	static CL_Point UpPoint(CL_Point a, CL_Point b);
	// возвращает точку, которая ниже
	static CL_Point DownPoint(CL_Point a, CL_Point b);
	// возвращает точку, которая правее
	static CL_Point RightPoint(CL_Point a, CL_Point b);
	// выводит текст с тенью в данных координатах, нужным шрифтом и толщиной
	static void ShadowText(int x, int y, int w, CL_Font * font, string text);
	static void ShadowText(CL_Rect dest, int w, CL_Font * font, string text);
	static void ShadowText(int x, int y, int w, CL_Font * font, CL_Color color, string text);
	// возвращает угол между двумя точками
	static int Get2PAngle(CL_Point a, CL_Point b);
	// конвертирует радианы в углы
	static int RadToDeg(double angle);
	// возвращает меньшее из четырех чисел
	static int Min(int a, int b, int c, int d);
	// возвращает наименьшее ненулевое число из четырех чисел 
	static int MinNoZero(int a, int b, int c, int d);
};
