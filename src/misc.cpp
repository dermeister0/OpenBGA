#include <windows.h>
#include ".\misc.h"
#include ".\main.h"

char * toRUS_buf=new char[255];

CLog* CLog::instance = NULL;

const double CMisc::N_PI=3.14159;//число ПИ
CL_Font *CMisc::fnt_small;
CL_Font *CMisc::fnt_menu;
CL_Font *CMisc::fnt_about;

char * toRUS(char * str)
{
	CharToOem(str, toRUS_buf); 
	return toRUS_buf;
}

CMisc::CMisc(void)
{
}

CMisc::~CMisc(void)
{
}

// конвертирует градусы в радианы
double CMisc::DegToRad(double angle)
{
	return angle*N_PI/(double)180;
}

// возвращает 1, если число неотриц., в прот. случае -1
int CMisc::Sign(double num)
{
	if (num==0)
        return 1;
	else
		return num/fabs(num);
}

// вычисляет синус с точными значениями для углов, кратных 90
double CMisc::Sin(int angle)
{
	if (angle==0)
		return 0;
	if (angle==90)
		return 1;
	if (angle==180)
		return 0;
	if (angle==270)
		return -1;

	return sin(DegToRad(angle));
}

// вычисляет косинус с точными значениями для углов, кратных 90
double CMisc::Cos(int angle)
{
	if (angle==0)
		return 1;
	if (angle==90)
		return 0;
	if (angle==180)
		return -1;
	if (angle==270)
		return 0;

	return cos(DegToRad(angle));
}

// возвращает true, если знаки параметров равны, или один из них равен 0
bool CMisc::EqualSignOrZero(double a, double b)
{
	if (a==0 || b==0)
		return true;

	return (Sign(a)==Sign(b));
}

// возращает кол-во кадров в секунду
int CMisc::GetFPS(void)
{
	// Calculate time since last update
	static int start_time = CL_System::get_time();
	
	int cur_time = CL_System::get_time();
	int delta_time = cur_time - start_time;
	start_time = cur_time;

	//fps = calc_fps(delta_time);
	static int fps_result = 0;
	static int fps_counter = 0;
	static int total_time = 0;
	
	total_time += delta_time;
	if(total_time >= 1000)	// One second has passed
	{
		fps_result = fps_counter + 1;
		fps_counter = total_time = 0;
	}
	fps_counter++;	// Increase fps
	//CPlayer::fps=fps_result;
	return fps_result;
}

// возвращает точку, которая левее
CL_Point CMisc::LeftPoint(CL_Point a, CL_Point b)
{
	if (a.x<b.x)
		return a;
	else
		return b;
}

// возвращает точку, которая выше
CL_Point CMisc::UpPoint(CL_Point a, CL_Point b)
{
	if (a.y<b.y)
		return a;
	else
		return b;
}

// возвращает точку, которая ниже
CL_Point CMisc::DownPoint(CL_Point a, CL_Point b)
{
	if (a.y>b.y)
		return a;
	else
		return b;
}

// возвращает точку, которая правее
CL_Point CMisc::RightPoint(CL_Point a, CL_Point b)
{
	if (a.x>b.x)
		return a;
	else
		return b;
}

// выводит текст с тенью в данных координатах, нужным шрифтом и толщиной
void CMisc::ShadowText(int x, int y, int w, CL_Font * font, string text)
{
	font->set_color(CL_Color::black);
	font->draw(x+w,y+w,text);
	font->set_color(CL_Color::white);
	font->draw(x,y,text);
}

// выводит текст с тенью в данных координатах, нужным шрифтом и толщиной
void CMisc::ShadowText(int x, int y, int w, CL_Font * font, CL_Color color, string text)
{
	font->set_color(CL_Color::black);
	font->draw(x+w,y+w,text);
	font->set_color(color);
	font->draw(x,y,text);
}

void CMisc::ShadowText(CL_Rect dest, int w, CL_Font * font, string text)
{
	CL_Rect t=dest;
	t.left+=w;t.right+=w;
	t.top+=w;t.bottom+=w;

	font->set_color(CL_Color::black);
	font->draw(t,text);
	font->set_color(CL_Color::white);
	font->draw(dest,text);
}
// возвращает угол между двумя точками
int CMisc::Get2PAngle(CL_Point a, CL_Point b)
{
	int x1=a.x;
	int x2=b.x;
	int y1=a.y;
	int y2=b.y;
	
	int x3=x2;
	int y3=y1;

	double cb=y3-y2;
	double ca=x3-x1;

	double rad=atan(cb/ca);
	int angle=360-RadToDeg(rad);

	if (x2<x1 && y1<y2)//вторая четверть
		angle+=180;
	else if (x2<x1 && y1>=y2) //III четверть
		angle+=180;
	return angle;
}

// конвертирует радианы в углы
int CMisc::RadToDeg(double angle)
{
	return angle*(double)180/N_PI;
}

// возвращает меньшее из четырех чисел
int CMisc::Min(int a, int b, int c, int d)
{
	int t=a;

	if (b<t)
		t=b;
	if (c<t)
		t=c;
	if (d<t)
		t=d;

	return t;
}

// возвращает наименьшее ненулевое число из четырех чисел 
int CMisc::MinNoZero(int a, int b, int c, int d)
{
	int t=100000;

	if (a<t && a!=0)
		t=a;
	if (b<t && b!=0)
		t=b;
	if (c<t && c!=0)
		t=c;
	if (d<t && d!=0)
		t=d;

	return t;
}