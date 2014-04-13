//main.h
#pragma once

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/network.h>
#include <ClanLib/sound.h>
#include <ClanLib/vorbis.h>
#include <fstream>

#include <GCL\Keyboard\GCL_Keyboard.h> //��������� ����������
#include <GCL\Mouse\GCL_Mouse.h> //��������� ����
#include <GCL\Menu\GCL_Menu.h> //����

#include ".\gmtypes.h" //��������� ����
#include ".\misc.h"
#include ".\log.h" //���� ����

using namespace std;

#define SCR_W 800
#define SCR_H 600

class CBGA : public CL_ClanApplication
{
	virtual int main(int, char**);
public:
	// ������� Heavysoft
	void Intro(CL_ResourceManager *RM);
};
