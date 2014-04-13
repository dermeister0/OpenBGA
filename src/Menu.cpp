#include ".\menu.h"
#include <iostream>

CGameMenu::CGameMenu(CGame* _game)
{
	pointer=new CL_Sprite("menu/pointer",_game->GetResman());
	title=new CL_Sprite("menu/title",_game->GetResman());
	darktitle=new CL_Sprite("menu/darktitle",_game->GetResman());
	devby=new CL_Sprite("menu/devby",_game->GetResman());
	copyright=new CL_Sprite("intro/copyright", _game->GetResman());

	fnt_about=CMisc::fnt_about;
	about_str=new string;
	*about_str=CL_String::load("about", _game->GetResman());

	context_main.sprites[0].spr=darktitle;
	context_main.sprites[0].pos=CL_Point(150,0);
	context_main.sprites[1].spr=devby;
	context_main.sprites[1].pos=CL_Point(20,510);

	context_about.sprites[0].spr=copyright;
	context_about.sprites[0].pos=CL_Point(200,0);
	copyright->set_color(CL_Color(101,101,101));

	context_current=&context_main;

	curhandler=&CGameMenu::MenuMain;

	game=_game;
	MainMenu=new GCL_Menu(game->GetResman(),CMisc::fnt_menu);

	//btn_game=MainMenu->AddButton(SCR_W/2,150,true,"menu/game",SCR_W/4,10);
	btn_game_server=MainMenu->AddButton(SCR_W/2,70,true,"menu/createserver",SCR_W/6,10);
	btn_game_client=MainMenu->AddButton(SCR_W/2,150,true,"menu/connectserver",SCR_W/6,10);

	btn_options=MainMenu->AddButton(SCR_W/2,230,true,"menu/options",SCR_W/4,10);
	btn_help=MainMenu->AddButton(SCR_W/2,310,true,"menu/help",SCR_W/4,10);
	btn_about=MainMenu->AddButton(SCR_W/2,390,true,"menu/about",SCR_W/4,10);
	btn_bglist=MainMenu->AddButton(SCR_W/2,470,true,"menu/bglist",SCR_W/4,10);
	btn_exit=MainMenu->AddButton(SCR_W/2,550,true,"menu/exit",SCR_W/4,10);

	CL_SoundBuffer theme("menu/menu_theme", game->GetResman());
	menu_theme=new CL_SoundBuffer_Session; 
	*menu_theme=theme.play();

	page_main=0;

	/*page_game=MainMenu->AddPage();
	btn_game_back=MainMenu->AddButton(50,500,false,"menu/back",10,10);
	btn_game_server=MainMenu->AddButton(SCR_W/2,200,true,"menu/createserver",SCR_W/6,10);
	btn_game_client=MainMenu->AddButton(SCR_W/2,300,true,"menu/connectserver",SCR_W/6,10);*/

	page_gametype=MainMenu->AddPage();
	btn_gametype_back=MainMenu->AddButton(50,500,false,"menu/back",10,10);
	btn_gametype_deathmatch=MainMenu->AddButton(100,50,false,"menu/deathmatch");
	MainMenu->SetBtnCheck(btn_gametype_deathmatch,true);
	MainMenu->MakeRadio(btn_gametype_deathmatch,GCL_Button::GCL_BUTTON_RADIO);
	btn_gametype_teamdeathmatch=MainMenu->AddButton(100,120,false,"menu/teamdeathmatch");
	MainMenu->MakeRadio(btn_gametype_teamdeathmatch,GCL_Button::GCL_BUTTON_RADIO);
	btn_gametype_next=MainMenu->AddButton(600,500,false,"menu/next",10,10);
	
	page_gameparams=MainMenu->AddPage();
	team1=team4=BGA_TEAM_RED;
	team2=team3=BGA_TEAM_BLUE;

	btn_gameparams_back=MainMenu->AddButton(50,500,false,"menu/back",10,10);
	btn_gameparams_next=MainMenu->AddButton(600,500,false,"menu/next",10,10);

	ebx_gameparams_name1=MainMenu->AddEditBox(50,300,"Игрок 1");
	MainMenu->SetBtnParam(ebx_gameparams_name1,GCL_EDITBOX_MAXLEN,"15");
	MainMenu->SetBtnValue(ebx_gameparams_name1,"Игрок 1");
	btn_gameparams_cmd1=MainMenu->AddButton(20,307,false,"menu/switchteam");
	MainMenu->MakeRadio(btn_gameparams_cmd1,GCL_Button::GCL_BUTTON_CHECKBOX);
	MainMenu->SetBtnCheck(btn_gameparams_cmd1,team1-1);

	ebx_gameparams_name2=MainMenu->AddEditBox(450,300,"Игрок 2");
	MainMenu->SetBtnParam(ebx_gameparams_name2,GCL_EDITBOX_MAXLEN,"15");
	MainMenu->SetBtnValue(ebx_gameparams_name2,"Игрок 2");
	btn_gameparams_cmd2=MainMenu->AddButton(420,307,false,"menu/switchteam");
	MainMenu->MakeRadio(btn_gameparams_cmd2,GCL_Button::GCL_BUTTON_CHECKBOX);
	MainMenu->SetBtnCheck(btn_gameparams_cmd2,team2-1);

	ebx_gameparams_name3=MainMenu->AddEditBox(50,350,"Игрок 3");
	MainMenu->SetBtnParam(ebx_gameparams_name3,GCL_EDITBOX_MAXLEN,"15");
	MainMenu->SetBtnValue(ebx_gameparams_name3,"Игрок 3");
	btn_gameparams_cmd3=MainMenu->AddButton(20,357,false,"menu/switchteam");
	MainMenu->MakeRadio(btn_gameparams_cmd3,GCL_Button::GCL_BUTTON_CHECKBOX);
	MainMenu->SetBtnCheck(btn_gameparams_cmd3,team3-1);

	ebx_gameparams_name4=MainMenu->AddEditBox(450,350,"Игрок 4");
	MainMenu->SetBtnParam(ebx_gameparams_name4,GCL_EDITBOX_MAXLEN,"15");
	MainMenu->SetBtnValue(ebx_gameparams_name4,"Игрок 4");
	btn_gameparams_cmd4=MainMenu->AddButton(420,357,false,"menu/switchteam");
	MainMenu->MakeRadio(btn_gameparams_cmd4,GCL_Button::GCL_BUTTON_CHECKBOX);
	MainMenu->SetBtnCheck(btn_gameparams_cmd4,team4-1);

	ebx_gameparams_localgamers=MainMenu->AddEditBox(20,250,"Локальные игроки");
	MainMenu->SetBtnParam(ebx_gameparams_localgamers,GCL_EDITBOX_MAXLEN,"1");
	MainMenu->SetBtnParam(ebx_gameparams_localgamers,GCL_EDITBOX_ONLYNUM,"true");
	MainMenu->SetBtnValue(ebx_gameparams_localgamers ,"2");

	ebx_gameparams_fraglimit=MainMenu->AddEditBox(20,400,"Лимит фрагов");
	MainMenu->SetBtnParam(ebx_gameparams_fraglimit,GCL_EDITBOX_MAXLEN,"3");
	MainMenu->SetBtnParam(ebx_gameparams_fraglimit,GCL_EDITBOX_ONLYNUM,"true");
	MainMenu->SetBtnValue(ebx_gameparams_fraglimit,"20");

	ebx_gameparams_timelimit=MainMenu->AddEditBox(420,400,"Лимит времени");
	MainMenu->SetBtnParam(ebx_gameparams_timelimit,GCL_EDITBOX_MAXLEN,"3");
	MainMenu->SetBtnParam(ebx_gameparams_timelimit,GCL_EDITBOX_ONLYNUM,"true");
	MainMenu->SetBtnValue(ebx_gameparams_timelimit,"0");


	game_type=BGA_GAME_DEATHMATCH;


	page_about=MainMenu->AddPage();
	btn_about_back=MainMenu->AddButton(400,565,true,"menu/back",SCR_W/4,10);

	MainMenu->ChangePage(page_main);
}

CGameMenu::~CGameMenu(void)
{
	delete MainMenu;
	delete pointer;
	delete title;
	delete darktitle;
	delete devby;
	delete menu_theme;
	delete copyright;
	delete about_str;
}

// выводит меню
void CGameMenu::Draw()
{
	MainMenu->Draw(*context_current);
	pointer->draw(CL_Mouse::get_x()-pointer->get_width()/2,CL_Mouse::get_y()-pointer->get_width()/2);
}

// заставляет меню проверить состояние мыши и обновить все кнопки
void CGameMenu::Update(GCL_Mouse mouse, GCL_Keyboard kb)
{
	int x=mouse.GetPosition().x;
	int y=mouse.GetPosition().y;

	MainMenu->OnMouseMove(x,y);

	if (mouse.KeyDown(CL_MOUSE_LEFT))
		MainMenu->OnMouseDown(x,y);

	if (mouse.KeyRelease(CL_MOUSE_LEFT))
		MainMenu->OnMouseUp(x,y);

	for (int i=0;i<255;i++)
		if (kb.KeyPress(i))
			MainMenu->OnKeyPress(i);

	Navigate();
}

// проверяет нажатую кнопку и выполняет соответствующие действия
void CGameMenu::Navigate(void)
{
	int btn=MainMenu->GetClickedButton();
	//ничего не нажато
	if (btn==-1)
		return;

	int page=MainMenu->GetCurPage();
	
	if (page==page_main)
	{
		/*if (btn==btn_game)
		{
			MainMenu->FadeOn(context_main);
			MainMenu->ChangePage(page_game);
			context_current=&context_null;
			MainMenu->FadeOff();
		}*/
		if (btn==btn_game_server)
		{
			MainMenu->FadeOn(context_main);
			MainMenu->ChangePage(page_gametype);
			context_current=&context_null;
			MainMenu->FadeOff();
		}
		if (btn==btn_about)
		{
			MainMenu->FadeOn(context_main);
			MainMenu->ChangePage(page_about);
			context_current=&context_about;
			MainMenu->FadeOff(context_about);
			y=650;r=g=b=101;ch=dif=1;
			copyright->set_color(CL_Color(101,101,101));
			curhandler=&CGameMenu::MenuAbout;
		}
		if (btn==btn_exit)
		{
			std::cout<<"Exit button clicked.\n";
			MainMenu->FadeOn(context_main);
			quit=true;
		}
	}
	if (page==page_about)
	{
		if (btn==btn_about_back)
		{
			MainMenu->FadeOn(context_about);
			MainMenu->ChangePage(page_main);

			fnt_about->set_color(101,101,101);
			copyright->set_color(101,101,101);

			context_current=&context_main;
			MainMenu->FadeOff(context_main);
			curhandler=&CGameMenu::MenuMain;
		}
	}
	/*if (page==page_game)
	{
		if (btn==btn_game_back)
		{
			MainMenu->FadeOn();
			MainMenu->ChangePage(page_main);
			context_current=&context_main;
			MainMenu->FadeOff(context_main);
		}

	}*/
	if (page==page_gametype)
	{
		if (btn==btn_gametype_back)
		{
			MainMenu->FadeOn();
			MainMenu->ChangePage(page_main);
			context_current=&context_main;
			MainMenu->FadeOff(context_main);
		}
		if (btn==btn_gametype_next)
		{
			MainMenu->FadeOn();
			MainMenu->ChangePage(page_gameparams);
			MainMenu->FadeOff();
		}
		if (btn==btn_gametype_deathmatch)
		{
			MainMenu->SetBtnCheck(btn_gametype_deathmatch,true);
			MainMenu->SetBtnCheck(btn_gametype_teamdeathmatch,false);
			game_type=BGA_GAME_DEATHMATCH;
		}
		if (btn==btn_gametype_teamdeathmatch)
		{
			MainMenu->SetBtnCheck(btn_gametype_teamdeathmatch,true);
			MainMenu->SetBtnCheck(btn_gametype_deathmatch,false);
			game_type=BGA_GAME_TEAMDEATHMATCH;
		}
	}
	if (page==page_gameparams)
	{
		if (btn==btn_gameparams_back)
		{
			MainMenu->FadeOn();
			MainMenu->ChangePage(page_gametype);
			MainMenu->FadeOff();
		}
		if (btn==btn_gameparams_next)
		{
			MainMenu->FadeOn();
			menu_theme->stop();
			int fr_lim,t_lim,loc_p;
			fr_lim=CL_String::to_int(MainMenu->GetBtnValue(ebx_gameparams_fraglimit));
			t_lim=CL_String::to_int(MainMenu->GetBtnValue(ebx_gameparams_timelimit));
			loc_p=CL_String::to_int(MainMenu->GetBtnValue(ebx_gameparams_localgamers));

			if (loc_p<1)
				loc_p=1;
			else if (loc_p>4)
				loc_p=4;

			game->SetLocalNames(MainMenu->GetBtnValue(ebx_gameparams_name1),MainMenu->GetBtnValue(ebx_gameparams_name2),
				MainMenu->GetBtnValue(ebx_gameparams_name3),MainMenu->GetBtnValue(ebx_gameparams_name4));

			game->SetLocalCommands(team1,team2,team3,team4);

			string mapname="platetown";
			LOG<<LOG.Time()<<"Запуск игры (карта - "<<mapname<<", лимит фрагов - "<<fr_lim<<", лимит времени - "<<t_lim<<" мин.)\n";

			game->SetLimits(fr_lim,t_lim);
			game->NewGame(game_type,loc_p,mapname);
			game->MainLoop();
			menu_theme->play();
			MainMenu->ChangePage(page_main);
			context_current=&context_main;
			MainMenu->FadeOff(context_main);
		}
		if (btn==ebx_gameparams_name1)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,true);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_name2)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,true);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_name3)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,true);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_name4)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,true);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_fraglimit)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,true);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_timelimit)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,true);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,false);
		}
		if (btn==ebx_gameparams_localgamers)
		{
			MainMenu->SetBtnCheck(ebx_gameparams_name1,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name2,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name3,false);
			MainMenu->SetBtnCheck(ebx_gameparams_name4,false);
			MainMenu->SetBtnCheck(ebx_gameparams_fraglimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_timelimit,false);
			MainMenu->SetBtnCheck(ebx_gameparams_localgamers,true);
		}
		
		if (btn==btn_gameparams_cmd1)
		{
			team1=(team1==1 ? 2 : 1);
			MainMenu->SetBtnCheck(btn_gameparams_cmd1,team1-1);
		}
		if (btn==btn_gameparams_cmd2)
		{
			team2=(team2==1 ? 2 : 1);
			MainMenu->SetBtnCheck(btn_gameparams_cmd2,team2-1);
		}
		if (btn==btn_gameparams_cmd3)
		{
			team3=(team3==1 ? 2 : 1);
			MainMenu->SetBtnCheck(btn_gameparams_cmd3,team3-1);
		}
		if (btn==btn_gameparams_cmd4)
		{
			team4=(team4==1 ? 2 : 1);
			MainMenu->SetBtnCheck(btn_gameparams_cmd4,team4-1);
		}
	}
}

// главный цикл меню
void CGameMenu::MainLoop(void)
{
	GCL_Mouse mouse;
	GCL_Keyboard kb;

	LOG<<LOG.Time()<<"Загружено меню\n";
	unsigned int begin_time;
	quit=false;
	//главный цикл
	while(!quit)//(CL_Keyboard::get_keycode(CL_KEY_ESCAPE)||
	{
		begin_time=CL_System::get_time();
		//очищаем экран
		CL_Display::clear();
		
		(this->*curhandler)();//обработчик текущей страницы меню

		Draw();
		Update(mouse,kb);

		//смена поверхностей
		CL_Display::flip();
		//обновление
		CL_System::keep_alive();
		mouse.Refresh();
		kb.Refresh();

		while (CL_System::get_time()-begin_time<16.6);//ограничение на FPS~60

	}
}

void CGameMenu::MenuMain()
{
	//darktitle->draw(150,0);
	//devby->draw(350,450);
	//devby->draw(20,510);
}

void CGameMenu::MenuAbout()
{
	unsigned int LastUpdate=0;
	CL_Rect rect;
	rect.left=0; rect.right=800;
	rect.top=0; rect.bottom=130;
	
	//обновление
	//int rd=1,gd=1,bd=1;
	CL_Color color(r,g,b);

	if (CL_System::get_time()-LastUpdate<15)
		return;

	LastUpdate=CL_System::get_time();

	color.set_color(r,g,b);
	copyright->set_color(color);

	fnt_about->set_color(color);

	fnt_about->draw(72,y,*about_str);

	CL_Display::fill_rect(rect,CL_Color::black);
	CL_Display::fill_rect(CL_Rect(0,510,800,600),CL_Color::black);
	copyright->draw(200,0);
	
	if (y<-fnt_about->get_height(*about_str))
		y=650;
	else
		y--;

	//--------------
	if (ch==1)
	{
		r+=dif;
		if (r==255 || r==50)
			ch=2;
	}

	if (ch==2)
	{
		g+=dif;
		if (g==255 || g==50)
			ch=3;
	}

	if (ch==3)
	{
		b+=dif;
		if (b==255)
		{
			ch=1;
			dif=-1;
		}
		if (b==50)
		{
			ch=1;
			dif=1;
		}
	}
}
// заставка игры
void CGameMenu::Intro(void)
{
	LOG<<LOG.Time()<<"Заставка BGA\n";
	unsigned int LastUpdate=0;

	//звук
	CL_SoundBuffer logo("menu/logo", game->GetResman());
	CL_SoundBuffer_Session snd1=logo.prepare();//.play();
	//CL_SoundBuffer_Session snd2=logo_2.prepare();
	//CL_SoundBuffer_Session snd3=logo_3.prepare();

	GCL_Keyboard kb;
	kb.Refresh();
	kb.Refresh();

	double y=0;int time=1;
	int key;
	double speed=0.0;
	bool done=false;
	snd1.play();
	while(!done)
	{
		for (key=0;key<255;key++)
			if(kb.KeyPress(key))
			{
              	done=1;
				snd1.stop();
				//snd2.stop();
				//snd3.stop();
			}

		if (CL_System::get_time()-LastUpdate<10 //если прошло достаточно времени...
			|| LastUpdate>CL_System::get_time())//...или мы искусственно задержались
			continue;

		LastUpdate=CL_System::get_time();

		CL_Display::clear();

		title->draw(150,y);
		CL_Display::flip();
		CL_System::keep_alive();
		kb.Refresh();
		
		if (time==1)
		{
			if (y<300)
			{
				y+=speed;
				speed+=0.05;
			}
			else
			{
				time=2;
				speed=3;
				//snd2.play();
			}
		}
		if (time==2)
		{
			if (speed>0)//y>200)
			{
				y-=speed;
				speed-=0.05;
			}
			else
			{
				time=3;
				speed=0;
			}
		}
		if (time==3)
		{
			if (y<300)
			{
				y+=speed;
				speed+=0.05;
			}
			else
			{
				//snd3.play();
				time=4;
				speed=2;
				LastUpdate+=500;
			}
		}
		if (time==4)
		{
			if (y>0)
			{
				y-=speed;
				speed+=0.02;
			}
			else
				break;
		}
	}
	CL_System::sleep(1000);
	int r=255;
	CL_Color color(r,r,r);
	while(!done && r!=9)
	{
		for (key=0;key<255;key++)
			if(CL_Keyboard::get_keycode(key))
			{
				done=1;
				snd1.stop();
			}

		if (CL_System::get_time()-LastUpdate<10)
			continue;

		LastUpdate=CL_System::get_time();
	
		CL_Display::clear();
		color.set_color(r,r,r);
		title->set_color(color);

		title->draw(150,0);
		CL_Display::flip();
		CL_System::keep_alive();
		r--;
	}
	MainMenu->FadeOff(context_main);
}
