/*
	name: 'Bad Guys on Autos'
	author: 'Tosha'
	begin date: '7.07.2005'
*/

#define VERSION "0.24.90"

#include "main.h"
#include "misc.h"
#include "game.h"
#include "menu.h"
#include "main.h"


int CBGA::main(int argc, char **argv)
{
#ifdef _DEBUG
	// создаём консоль
	CL_ConsoleWindow console("BGA Console");
	console.redirect_stdio();
#endif

	try
	{
		//инициализация
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();
		CL_SetupNetwork::init();
		CL_SetupSound::init();
		CL_SetupVorbis::init();
		CL_SoundOutput output(44100);

		LOG<<"Bad Guys on Autos\nВерсия "<<VERSION<<endl;		
		LOG<<LOG.Time()<<"Старт игры\n";

		LOG<<"Установка  графического режима...\n";

		//окно
#ifdef _DEBUG
		CL_DisplayWindow window("BGA v"VERSION, SCR_W, SCR_H, false, true);
#else
		CL_DisplayWindow window("BGA v"VERSION, SCR_W, SCR_H, true);
#endif
		window.hide_cursor();

		//получаем текущий режим
		DEVMODE cur_devmode;
		EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &cur_devmode);

		LOG<<"Установлен графический режим "<<cur_devmode.dmPelsWidth<<"x"<<cur_devmode.dmPelsHeight<<"x";
		LOG<<cur_devmode.dmBitsPerPel<<"@"<<cur_devmode.dmDisplayFrequency<<endl;

		//контекст
		CL_GraphicContext *gc = window.get_gc();

		srand(CL_System::get_time());
		
		CGame Game;
#ifndef _DEBUG
		Intro(Game.GetResman());

		CGameMenu game_menu(&Game);
		game_menu.Intro();
		game_menu.MainLoop();
#else
		//CGameMenu game_menu(&Game);
		//game_menu.MainLoop();
		Game.SetLimits(3,0);
		Game.SetLocalNames("Tosha","Hitman","Crash","Doom");
		Game.NewGame(BGA_GAME_DEATHMATCH,2,"platetown");
		Game.MainLoop();
#endif


		LOG<<LOG.Time()<<"Завершение игры\n";

		//деинициализация
		CL_SetupVorbis::deinit();
		CL_SetupSound::deinit();
		CL_SetupNetwork::deinit();
	 	CL_SetupCore::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupGL::deinit();
	}
	catch (CL_Error err)
	{
		std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		LOG<<endl<<LOG.Time()<<"ОШИБКА! "<< err.message.c_str() <<endl;

#ifdef _DEBUG
		// Display console close message and wait for a key
		console.display_close_message();
#endif
	}
	catch (...)
	{
		LOG<<endl<<LOG.Time()<<"ОШИБКА! Необработанное исключение!"<<endl;
	}

	LOG<<"\n\n\nThe time of Bad Guys will come soon...";
	//console.display_close_message();
	return 0;
} 

CBGA app;

// логотип Heavysoft
void CBGA::Intro(CL_ResourceManager *RM)
{
	LOG<<LOG.Time()<<"Заставка Heavysoft\n";
	CL_Surface img_copyright("intro/heavysoft", RM);
	
	int  key=0; int done=0; 
	int r=0;

	unsigned int LastUpdate=0;

	//даем видюхе время сменить разрешение
	CL_System::sleep(1000);

	//звук
	CL_SoundBuffer intro_snd("intro/intro", RM);
	CL_SoundBuffer_Session snd=intro_snd.play();

	CL_Color color(r,r,r);
	while(!done && r!=255)
	{
		for (key=0;key<255;key++)
			if(CL_Keyboard::get_keycode(key))
			{
				done=1;
				snd.stop();
			}

		if (CL_System::get_time()-LastUpdate<10)
			continue;

		LastUpdate=CL_System::get_time();
	
		CL_Display::clear();
		color.set_color(r,r,r);
		img_copyright.set_color(color);

		img_copyright.draw(0,0);
		CL_Display::flip();
		CL_System::keep_alive();
		r++;
	}
	CL_System::sleep(1000);
	while(!done && r!=9)
	{
		for (key=0;key<255;key++)
			if(CL_Keyboard::get_keycode(key))
			{
				done=1;
				snd.stop();
			}

		if (CL_System::get_time()-LastUpdate<10)
			continue;

		LastUpdate=CL_System::get_time();
	
		CL_Display::clear();
		color.set_color(r,r,r);
		img_copyright.set_color(color);

		img_copyright.draw(0,0);
		CL_Display::flip();
		CL_System::keep_alive();
		r--;
	}

	
}
