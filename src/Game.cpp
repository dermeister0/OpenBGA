#include ".\game.h"
#include ".\item.h"

CGame::CGame(void)
{
	resman=new CL_ResourceManager("data/main/resources.xml");
	CL_ResourceManager pak0res("data/pak0/resources.xml");
	resman->add_resources(pak0res);

	LoadControls();//загружаем управление
	LoadWeapons();//загружаем оружие
	LoadInterface();//загружаем ресурсы для отрисовки интерфейса
	LoadParticles();//загружаем частицы
	LoadPowerups();//загружает бонусы и особые точки

	frag_limit=0;
	time_limit=0;
	SetLocalCommands(1,2,1,2);

	game_world=NULL;//обнуляем, чтобы NewGame его не удалила
	NS=NULL;
	frag_info=NULL;
}

CGame::~CGame(void)
{
	LOG<<"Освобождение игровых ресурсов";

	delete game_world;
	//delete PakFile;
	delete frag_info;
	
	for (int i=0;i<6;i++)
	{
		delete CCamera::wicons[i];
		delete CCamera::ficons[i];

		if (i==5)
			continue;
		delete CBullet::bullets[i];
	}
	delete CCamera::spr_armor;
	delete CCamera::spr_health;
	delete CCamera::spr_aim;

	delete CMisc::fnt_small;
	delete CMisc::fnt_menu;
	delete CMisc::fnt_about;

	delete CSmoke::spr_smoke;
	delete CSmoke::spr_green_smoke;
	delete CSmoke::spr_blue_ring;

    delete CExplode::spr_explode;
	delete CExplode::spr_w1_explode;
	delete CExplode::spr_w2_explode;
	delete CExplode::spr_w3_explode;
	delete CExplode::spr_w4_explode;
	delete CExplode::spr_w5_explode;
	delete CExplode::spr_respawn_explode;

	/*for (int i=0;i<BGA_ITEM_COUNT;i++)
	{
		delete CItem::powerups[i];
	}*/
	delete CItem::powerups;
	delete CItem::spr_item_place;

	delete resman;
	//CL_Zip_Archive z(pkname);
}

// конструктор с указанием кол-ва игроков
CGame::CGame(int np)
{
	local_players=np;
}

// основной цикл игры
void CGame::MainLoop(void)
{
	//int i;
	unsigned int begin_time;
	int fps=0;
	gamebegin_time=CL_System::get_time();

	while (!end_game)
	{
		begin_time=CL_System::get_time();
		//ввод
		game_world->Input();
		//обработка сообщений
		ProcessMessages();

		//обновление
		game_world->Update();
		fps=CMisc::GetFPS();
		
		if (begin_time-last_net_upd>BGA_NET_REFRESH_TIME)
		{
			game_world->UpdateNetwork();
			last_net_upd=begin_time;
		}
		//отрисовка
		CL_Display::clear();

		game_world->Draw();

		if (game_kb.KeyDown(CL_KEY_TAB))
			frag_info->Draw();

		//рисуем FPS
		CMisc::fnt_small->set_color(255,255,255,255);
		CMisc::fnt_small->draw(0,0,"FPS:"+CL_String::from_int(fps));

		CL_Display::flip();

		//обновление ввода
		CL_System::keep_alive(10);
		game_kb.Refresh();
		game_mouse.Refresh();

		//ПРОВЕРКА ОКОНЧАНИЯ ИГРЫ
		if (game_kb.KeyPress(CL_KEY_ESCAPE))
			IngameMenu();

		if (frag_limit && CPlayer::GetMaxFrags()==frag_limit)
			end_game=true;

		if (time_limit && !GetRemainTime())
			end_game=true;

		while (CL_System::get_time()-begin_time<16.6);//ограничение на FPS~60
	}

	LOG<<LOG.Time()<<"Раунд завершен\n";
	//восстанавливает область отсечения
	CL_Display::set_cliprect(CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()));

	delete game_world;
	game_world=NULL;

	game_kb.Refresh();
}

// обработка сообщений игры
void CGame::ProcessMessages(void)
{
	GameMessage msg;
	screate_player *pl_info=NULL;//новый игрок
	sworld_info *w=NULL;//новый игровой мир
	supdate_player *upl=NULL;//обновлен игрок
	screate_bullet *b=NULL;//новая пуля
	screate_explode *e=NULL;//новый взрыв
	supdate_frags *fr=NULL;//обновляем фраги
	splay_sound *ps=NULL;//воспроизводим звук

	//для обновления фрагов
	skill_info nfo;
	CPlayer *pl;

	while (MQ.GetGameMessage(&msg))
	{
		switch (msg.type)
		{
		case BGA_MSG_NEW_LOCAL_PLAYER:
		case BGA_MSG_NEW_REMOTE_PLAYER:
			pl_info=(screate_player *)msg.info1;
			game_world->AddPlayer(*pl_info);
			frag_info->Update();
//			delete pl_info;
			break;
		case BGA_MSG_WORLD_INFO:
			w=(sworld_info *)msg.info1;
			game_world->NewWorld(*w);
			delete w;
			break;
		case BGA_MSG_UPDATE_PLAYER:
			upl=(supdate_player *)msg.info1;
			game_world->UpdatePlayer(*upl);
			delete upl;
			break;
		case BGA_MSG_NEW_BULLET:
			b=(screate_bullet *)msg.info1;
			game_world->AddBullet(b);
			delete b;
			break;
		case BGA_MSG_NEW_EXPLODE:
			e=(screate_explode *)msg.info1;
			game_world->AddExplode(e);
			delete e;
			break;
		case BGA_MSG_UPDATE_FRAGS:
			fr=(supdate_frags *)msg.info1;

			pl=game_world->GetPlayer(fr->id);
			pl->SetFrags(pl->GetFrags()+fr->dfrags);
			if (fr->max_frags>CPlayer::max_frags)
				CPlayer::max_frags=fr->max_frags;

			nfo.killer=game_world->GetPlayer(fr->id)->GetName();
			nfo.dead=game_world->GetPlayer(fr->target_id)->GetName();
			if (game_type==BGA_GAME_TEAMDEATHMATCH || game_type==BGA_GAME_CAPTURETHEFLAG)
			{
				nfo.killer_team=game_world->GetPlayer(fr->id)->GetTeam();
				nfo.dead_team=game_world->GetPlayer(fr->target_id)->GetTeam();
				UpdateTeamFrags();
			}
			else
			{
				nfo.killer_team=0;
				nfo.dead_team=0;
			}
			nfo.weapon=fr->weapon;
			game_world->kill_info.Add(nfo);
			frag_info->Update();
			delete fr;
			break;
		case BGA_MSG_PLAY_SOUND:
			ps=(splay_sound *)msg.info1;
			game_world->listener->PlaySound(ps->id,ps->x,ps->y);
			delete ps;
			break;
		}
	}
}

// загружает управление для каждого геймера
void CGame::LoadControls(void)
{
	LOG<<"Загрузка управления\n";

	controls[0][BGA_CTRL_LEFT]=CL_KEY_LEFT;
	controls[0][BGA_CTRL_RIGHT]=CL_KEY_RIGHT;
	controls[0][BGA_CTRL_UP]=CL_KEY_UP;
	controls[0][BGA_CTRL_DOWN]=CL_KEY_DOWN;
	controls[0][BGA_CTRL_FIRE]=CL_KEY_RCONTROL;
	controls[0][BGA_CTRL_CHANGE_WEAPON]=CL_KEY_RSHIFT;

	controls[1][BGA_CTRL_LEFT]=CL_KEY_A;
	controls[1][BGA_CTRL_RIGHT]=CL_KEY_D;
	controls[1][BGA_CTRL_UP]=CL_KEY_W;
	controls[1][BGA_CTRL_DOWN]=CL_KEY_S;
	controls[1][BGA_CTRL_FIRE]=CL_KEY_R;
	controls[1][BGA_CTRL_CHANGE_WEAPON]=CL_KEY_T;

	controls[2][BGA_CTRL_LEFT]=CL_KEY_K;
	controls[2][BGA_CTRL_RIGHT]=VK_OEM_1;//;:
	controls[2][BGA_CTRL_UP]=CL_KEY_O;
	controls[2][BGA_CTRL_DOWN]=CL_KEY_L;
	controls[2][BGA_CTRL_FIRE]=VK_OEM_4;// [{
	controls[2][BGA_CTRL_CHANGE_WEAPON]=VK_OEM_6;// ]}

	controls[3][BGA_CTRL_LEFT]=CL_KEY_NUMPAD4;
	controls[3][BGA_CTRL_RIGHT]=CL_KEY_NUMPAD6;
	controls[3][BGA_CTRL_UP]=CL_KEY_NUMPAD8;
	controls[3][BGA_CTRL_DOWN]=CL_KEY_NUMPAD5;
	controls[3][BGA_CTRL_FIRE]=CL_KEY_RETURN;//!!!!!!!!!!!!!
	controls[3][BGA_CTRL_CHANGE_WEAPON]=VK_ADD;// +
}

// создает всех локальных игроков
void CGame::CreateLocalPlayers(void)
{
	int i;
	screate_player *pl;
	int ct[4];//типы камер
	if (local_players==1)
	{
		ct[0]=CCamera::CT_ALLSCREEN;
	}
	else if(local_players==2)
	{
		ct[0]=CCamera::CT_UP;
		ct[1]=CCamera::CT_DOWN;
	}
	else if(local_players==3)
	{
		ct[0]=CCamera::CT_UPCENTER;
		ct[1]=CCamera::CT_DOWNLEFT;
		ct[2]=CCamera::CT_DOWNRIGHT;
	}
	else
	{
		ct[0]=CCamera::CT_UPLEFT;
		ct[1]=CCamera::CT_UPRIGHT;
		ct[2]=CCamera::CT_DOWNLEFT;
		ct[3]=CCamera::CT_DOWNRIGHT;
	}

	int id=CPlayer::GetLastId();
	for (i=0;i<local_players;i++)
	{
		pl=new screate_player;
		pl->id=id;
		id++;

		pl->frags=0;
		pl->max_frags=0;
		
		CL_Point p=game_world->GetStartPoint();
		pl->x=p.x;
		pl->y=p.y;
		pl->angle=rand()%360;

		//в командной игре спрайт указывается явно, а в десматче - рандомом
		//strcpy(pl->spr_id,"gfx/cars/red");
		string spr="gfx/cars/";
		
		if (game_type==BGA_GAME_TEAMDEATHMATCH || game_type==BGA_GAME_CAPTURETHEFLAG)
		{
			pl->team=local_commands[i];
			if (pl->team==BGA_TEAM_BLUE)
				spr+="blue";
			else
				spr+="red";
		}
		else //(game_type==BGA_GAME_DEATHMATCH)
		{
			pl->team=pl->id;
			spr+=CL_String::from_int(rand()%6);
		}
		
		strcpy(pl->spr_id,spr.c_str());

		string nm=local_names[i];
		LOG<<LOG.Time()<<"Подключен локальный игрок с именем "<<nm<<endl;
			//"Tosha"+CL_String::from_int(rand()%1000);
		strcpy(pl->name,nm.c_str());
		pl->resman=resman;
		pl->kb=&game_kb;
		pl->world=game_world;
		pl->game=this;


		pl->local=false;
		pl->input_type=CPlayer::IT_NETWORK;
		pl->cam_type=CCamera::CT_NOCAMERA;

		SendNetMessage(BGA_MSG_NEW_REMOTE_PLAYER,pl);

		pl->local=true;
		pl->input_type=i;//CPlayer::IT_1;
		pl->cam_type=ct[i];
		pl->controls=controls[pl->input_type];

		MQ.AddMessage(BGA_MSG_NEW_LOCAL_PLAYER,pl);
	}
	

}

// создает удаленного игрока
void CGame::CreateRemotePlayer(screate_player *pl)
{
	pl->resman=resman;
	pl->world=game_world;
	pl->game=this;
	//strcpy(pl->spr_id,"gfx/cars/r_monster");
	MQ.AddMessage(BGA_MSG_NEW_REMOTE_PLAYER,pl);
}
// загружает характеристики оружия
void CGame::LoadWeapons(void)
{
	LOG<<"Загрузка оружия\n";	
	//заполняем файл значениями
	/*FILE *ofile=fopen("_data/configs/weapons.cfg","w");

	for (int i=0;i<5;i++)
	{
		strcpy(weapons[i].name,"gun");
		weapons[i].damage=10;
		weapons[i].reload_time=10;
		weapons[i].speed=10;

		fprintf(ofile,"%s\n%d\n%d\n%d\n",weapons[i].name,weapons[i].damage,weapons[i].reload_time,weapons[i].speed);
	}	
	fclose(ofile);*/

	//PakFile->save();

	CL_InputSource_File *file=new CL_InputSource_File("data/main/configs/weapons.cfg");
	char *temp=new char[file->size()];
	file->read(temp,file->size());
	temp[file->size()-1]=0;

	char *t=temp;

	for (int i=0;i<5;i++)
	{
		int count=0;
		int res=sscanf(t,"%s\n%d\n%d\n%d\n%d\n%n",&weapons[i].name,&weapons[i].damage,&weapons[i].reload_time,&weapons[i].speed,&weapons[i].count,&count);
 		t+=count;
		//загружает спрайты пуль
		CBullet::bullets[i]=new CL_Sprite("gfx/bullets/"+CL_String::from_int(i+1),resman);
		//загружает иконки оружия
		CCamera::wicons[i]=new CL_Sprite("gfx/wicons/"+CL_String::from_int(i+1),resman);
	}	
	//самоубийство
	CCamera::wicons[5]=new CL_Sprite("gfx/wicons/6",resman);

	file->close();
	delete file;
	delete [] temp;
}

// загружает ресурсы для интерфейса
void CGame::LoadInterface(void)
{
	LOG<<"Загрузка интерфейса\n";

	CCamera::spr_armor=new CL_Sprite("gfx/interface/armor",resman);
	CCamera::spr_health=new CL_Sprite("gfx/interface/health",resman);
	CCamera::spr_aim=new CL_Sprite("gfx/sprites/aim",resman);

	CMisc::fnt_small=new CL_Font("fonts/misc",resman);
	CMisc::fnt_menu=new CL_Font("fonts/menu",resman);
	CMisc::fnt_about=new CL_Font("fonts/about",resman);
	CCamera::fnt_interface=new CL_Font("fonts/interface",resman);

	for (int i=0;i<6;i++)
	{
		//загружает иконки бонусов
		CCamera::ficons[i]=new CL_Sprite("gfx/ficons/"+CL_String::from_int(i+1),resman);
		CCamera::ficons[i]->set_alpha(0.5);
	}
}

// загружает частицы
void CGame::LoadParticles(void)
{
	LOG<<"Загрузка частиц\n";

	CSmoke::spr_smoke=new CL_Sprite("gfx/particles/smoke",resman);
	CSmoke::spr_green_smoke=new CL_Sprite("gfx/particles/green_smoke",resman);
	CSmoke::spr_blue_ring=new CL_Sprite("gfx/particles/blue_ring",resman);

	CExplode::spr_explode=new CL_Sprite("gfx/particles/explode",resman);

	CExplode::spr_w1_explode=new CL_Sprite("gfx/particles/w1_explode",resman);
	CExplode::spr_w2_explode=new CL_Sprite("gfx/particles/w2_explode",resman);
	CExplode::spr_w3_explode=new CL_Sprite("gfx/particles/w3_explode",resman);
	CExplode::spr_w4_explode=new CL_Sprite("gfx/particles/w4_explode",resman);
	CExplode::spr_w5_explode=new CL_Sprite("gfx/particles/w5_explode",resman);
	
	CExplode::spr_respawn_explode=new CL_Sprite("gfx/particles/respawn_explode",resman);
	CExplode::spr_respawn_explode->set_color(0,0,1,0.3);
}

// возвращает максимальные чужие фраги для указанного игрока
int CGame::GetMaxEnemyFrags(int id)
{
	int max=-100000;
	int frags=0;
	for (int i=0;i<players->size();i++)
	{
		frags=(*players)[i]->GetFrags();
		if (frags>max && (*players)[i]->GetId()!=id)
			max=frags;
	}
	return max;
}


// уничтожает старый игровой мир, инициализирует переменные, создает новый мир
void CGame::NewGame(int gametype, int lplayers, string level_name)
{
	LOG<<LOG.Time()<<"Начата новая игра\n";

	end_game=false;
	game_type=gametype;
	local_players=lplayers;

	red_frags=0;
	blue_frags=0;

	if (game_world)
		delete game_world;

	if (NS)//сетевая сессия
		delete NS;

	if (frag_info)
		delete frag_info;

	game_world=new CWorld(this);
	frag_info=new CFragInfo(game_world,this);
	frag_info->Update();
	game_world->talk_info.Add(stalk_info("","Добро пожаловать в BGA v0.22 !"));

	sworld_info i;
	strcpy(i.level_name,level_name.c_str());
	game_world->NewWorld(i);

	InitNetwork();

	CreateLocalPlayers();
	players=game_world->GetPlayers();

}

// выводит/обновляет меню паузы
void CGame::IngameMenu(void)
{
	unsigned int begin_time;
	bool quit=false;

	GCL_Menu pmenu(resman,CMisc::fnt_menu);
	int btn_continue=pmenu.AddButton(SCR_W/2,SCR_H/2-35,true,"menu/next");
	int btn_exit=pmenu.AddButton(SCR_W/2,SCR_H/2+35,true,"menu/exit");
	CL_Sprite pointer("menu/pointer",resman);

	//главный цикл меню
	while(!quit)
	{
		begin_time=CL_System::get_time();
		//обработка сообщений
		ProcessMessages();

		//обновление
		game_world->Update();
		
		//отрисовка
		CL_Display::clear();

		game_world->Draw();

		CL_Display::fill_rect(CL_Rect(SCR_W/2-150,SCR_H/2-150,SCR_W/2+150,SCR_H/2+150),CL_Color(0,0,0,192));
		pmenu.Draw();

		int x=game_mouse.GetPosition().x;
		int y=game_mouse.GetPosition().y;

		pointer.draw(x-pointer.get_width()/2,y-pointer.get_width()/2);

		CL_Display::flip();

		//обновление ввода
		CL_System::keep_alive(10);
		game_kb.Refresh();
		game_mouse.Refresh();

		//обновление меню
		if (game_kb.KeyPress(CL_KEY_ESCAPE))
			quit=true;

		pmenu.OnMouseMove(x,y);

		if (game_mouse.KeyDown(CL_MOUSE_LEFT))
			pmenu.OnMouseDown(x,y);

		if (game_mouse.KeyRelease(CL_MOUSE_LEFT))
			pmenu.OnMouseUp(x,y);

		int btn=pmenu.GetClickedButton();

		if (btn==btn_continue)
			quit=true;

		if (btn==btn_exit)
		{
			quit=true;
			end_game=true;
		}

		while (CL_System::get_time()-begin_time<16.6);//ограничение на FPS~60
	}
}

// загрузка бонусов
void CGame::LoadPowerups(void)
{
	LOG<<"Загрузка бонусов\n";

	CItem::powerups=new CL_Sprite("gfx/powerups/items",resman);
	CItem::powerups->set_alpha(0.7);

	CItem::spr_item_place=new CL_Sprite("gfx/sprites/item_place",resman);
	CItem::spr_item_place->set_alpha(0.4);

	CItem::game=this;

/*	CItem::powerups[BGA_ITEM_AMMO1]=new CL_Sprite("gfx/powerups/ammo1",resman);
	CItem::powerups[BGA_ITEM_AMMO2]=new CL_Sprite("gfx/powerups/ammo2",resman);
	CItem::powerups[BGA_ITEM_AMMO3]=new CL_Sprite("gfx/powerups/ammo3",resman);
	CItem::powerups[BGA_ITEM_AMMO4]=new CL_Sprite("gfx/powerups/ammo4",resman);
	CItem::powerups[BGA_ITEM_AMMO5]=new CL_Sprite("gfx/powerups/ammo5",resman);

	CItem::powerups[BGA_ITEM_ARMOR25]=new CL_Sprite("gfx/powerups/armor25",resman);
	CItem::powerups[BGA_ITEM_ARMOR100]=new CL_Sprite("gfx/powerups/armor100",resman);

	CItem::powerups[BGA_ITEM_HEALTH5]=new CL_Sprite("gfx/powerups/health5",resman);
	CItem::powerups[BGA_ITEM_HEALTH10]=new CL_Sprite("gfx/powerups/health10",resman);
	CItem::powerups[BGA_ITEM_HEALTH25]=new CL_Sprite("gfx/powerups/health25",resman);
	CItem::powerups[BGA_ITEM_HEALTH100]=new CL_Sprite("gfx/powerups/health100",resman);
*/
}

// устанавливает имена локальных игроков
void CGame::SetLocalNames(string name1, string name2, string name3, string name4)
{
	local_names[0]=name1;
	local_names[1]=name2;
	local_names[2]=name3;
	local_names[3]=name4;
}

// устанавливает команды локальных игроков
void CGame::SetLocalCommands(int cmd1, int cmd2, int cmd3, int cmd4)
{
	local_commands[0]=cmd1;
	local_commands[1]=cmd2;
	local_commands[2]=cmd3;
	local_commands[3]=cmd4;
}
// обновляет фраги команд
void CGame::UpdateTeamFrags(void)
{
	red_frags=0,blue_frags=0;
	int f,t;

	for (int i=0;i<(*game_world->GetPlayers()).size();i++)
	{
		f=(*game_world->GetPlayers())[i]->GetFrags();
		t=(*game_world->GetPlayers())[i]->GetTeam();
		
		if (t==BGA_TEAM_RED)
			red_frags+=f;
		else
			blue_frags+=f;
	}
}

