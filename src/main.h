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

#include <GCL\Keyboard\GCL_Keyboard.h> //обработка клавиатуры
#include <GCL\Mouse\GCL_Mouse.h> //обработка мыши
#include <GCL\Menu\GCL_Menu.h> //меню

#include ".\gmtypes.h" //сообщения игры
#include ".\misc.h"
#include ".\log.h" //файл лога

using namespace std;

#define SCR_W 800
#define SCR_H 600

class CBGA : public CL_ClanApplication
{
	virtual int main(int, char**);
public:
	// логотип Heavysoft
	void Intro(CL_ResourceManager *RM);
};
