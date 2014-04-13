#include ".\player.h"
#include ".\world.h"
#include ".\game.h"

int CPlayer::last_id=0;
int CPlayer::max_frags=0;

CPlayer::CPlayer(screate_player pl_info)
{
	id=pl_info.id;
	team=pl_info.team;
	name.assign(pl_info.name);
	x=pl_info.x;
	y=pl_info.y;
	pdx=x;pdy=y;
	local=pl_info.local;

	angle=pl_info.angle;
	vx=0.0;
	vy=0.0;
	ux=0.0;
	uy=0.0;
	ux2=uy2=0.0;
	frags=0;

	life=100;
	armor=100;

	max_speed=BGA_MAX_SPEED;
	thrust=0.2;

	game_map=pl_info.game_map;
	world=pl_info.world;
	game=pl_info.game;
	resman=pl_info.resman;

	cam=new CCamera(pl_info.cam_type,this);

	spr_car=new CL_Sprite(pl_info.spr_id,pl_info.resman);
	fnt_interface=new CL_Font("fonts/misc",pl_info.resman);
	spr_id=pl_info.spr_id;

	controls=pl_info.controls;
	input_type=pl_info.input_type;
	kb=pl_info.kb;

	last_id=id+1;

	//инициализируем пушки
	CWeapon::SetGameWorld(world);
	for (int w=0;w<5;w++)
		weapons[w]=new CWeapon(w,game->weapons[w].damage,game->weapons[w].reload_time*10,100,game->weapons[w].speed*3,this);

	cur_weapon=0;

	if (pl_info.max_frags>max_frags)
		max_frags=pl_info.max_frags;
	frags=pl_info.frags;

	//звук двигателя
	snd_engine=new CL_SoundBuffer_Session();
	*snd_engine=CListener::snd_engine->prepare();
	snd_engine->set_looping(true);
	//snd_engine->play();//по просбе AVA12 с forum.sources.ru звук отключен

	Respawn();
}

CPlayer::~CPlayer(void)
{
	delete cam;
	delete spr_car;
	delete fnt_interface;
	for (int w=0;w<5;w++)
		delete weapons[w];

	snd_engine->stop();
	delete snd_engine;
}

// возвращает X-координату игрока
int CPlayer::GetX(void)
{
	return x;
}

// возвращает Y-координату игрока
int CPlayer::GetY(void)
{
	return y;
}

// возвращает true, если игрок локальный
bool CPlayer::IsLocal(void)
{
	return local;
}

// возвращает указатель на камеру игрока
CCamera* CPlayer::GetCam(void)
{
	return cam;
}

// конструктор копирования, для распред. динам. памяти
/*CPlayer::CPlayer(const CPlayer& pl)
{
	x=pl.x;
	y=pl.y;
	local=pl.local;

	

	cam=new CCamera(pl.cam->GetType(),this);
	spr_car=new CL_Sprite(*pl.spr_car);
	fnt_interface=new CL_Font(*pl.fnt_interface);

	controls=pl.controls;
	input_type=pl.input_type;
	kb=pl.kb;

	angle=pl.angle;
	vx=pl.vx;vy=pl.vy;
	ux=pl.ux;uy=pl.uy;
	max_speed=pl.max_speed;
	pdx=pl.pdx;
	pdy=pl.pdy;
	ux2=pl.ux2;
	uy2=pl.uy2;
}*/

// рисует игрока в условных координатах
void CPlayer::Draw(int ux, int uy)
{
	spr_car->set_angle(angle);

	if (!HasFeature(BGA_ITEM_INVISIBILITY-BGA_ITEM_BONUS1))//невидимость
	{
		//тень
		spr_car->set_color(0,0,0,0.5);
		spr_car->draw(x+ux+5,y+uy+5);//тень только у видимой тачки
		spr_car->set_color(255,255,255,1);
	}
	else
		spr_car->set_color(255,255,255,0.3);

	//машина
	spr_car->draw(x+ux,y+uy);

	//двойной урон
	if (HasFeature(BGA_ITEM_QUADDAMAGE-BGA_ITEM_BONUS1))
	{
		spr_car->set_color(0,0,255,0.4);
		spr_car->draw(x+ux,y+uy);
	}

	//регенерация
	if (HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
	{
		spr_car->set_color(255,0,0,0.4);
		spr_car->draw(x+ux,y+uy);
	}

	//маркеры
	/*CL_Display::fill_rect(CL_Rect(x-5+ux,y-5+uy,x+5+ux,y+5+uy),CL_Color::red);
	CL_Display::fill_rect(CL_Rect(cr_t.x-5+ux,cr_t.y-5+uy,cr_t.x+5+ux,cr_t.y+5+uy),CL_Color::red);

	CL_Display::fill_rect(CL_Rect(cr_lt.x-5+ux,cr_lt.y-5+uy,cr_lt.x+5+ux,cr_lt.y+5+uy),CL_Color::green);
	CL_Display::fill_rect(CL_Rect(cr_rt.x-5+ux,cr_rt.y-5+uy,cr_rt.x+5+ux,cr_rt.y+5+uy),CL_Color::green);
	CL_Display::fill_rect(CL_Rect(cr_lb.x-5+ux,cr_lb.y-5+uy,cr_lb.x+5+ux,cr_lb.y+5+uy),CL_Color::blue);
	CL_Display::fill_rect(CL_Rect(cr_rb.x-5+ux,cr_rb.y-5+uy,cr_rb.x+5+ux,cr_rb.y+5+uy),CL_Color::blue);*/

	//CL_Display::draw_quad(CL_Quad(cr_lt.x+ux,cr_lt.y+uy,cr_rt.x+ux,cr_rt.y+uy,cr_rb.x+ux,cr_rb.y+uy,cr_lb.x+ux,cr_lb.y+uy),CL_Color::beige);
	CL_Rect hr=GetHitRect();
	hr.left+=ux;hr.right+=ux;
	hr.top+=uy;hr.bottom+=uy;
	//CL_Display::draw_rect(hr,CL_Color::darksalmon);
	//angle++;
	//fnt_interface->draw(0,0,CL_String::from_int(angle));
	//fnt_interface->draw(cam->GetRect().left, cam->GetRect().top, " x speed: "+CL_String::from_int((int)this->ux)+
	//	" y speed: "+CL_String::from_int((int)this->uy));

	CMisc::ShadowText(x+ux,y+uy,2,fnt_interface,name);
}

// проверяет кнопки и отправляет сообщения
void CPlayer::Input(void)
{
	//vx=0.0;vy=0.0;
	double rad_angle=CMisc::DegToRad(angle);
	double sa=CMisc::Sin(angle);
	double ca=CMisc::Cos(angle);

	if (kb->KeyDown(controls[BGA_CTRL_UP]))
	{
		vx=ca*thrust;
		vy=sa*thrust;
	}
	if (kb->KeyDown(controls[BGA_CTRL_DOWN]))
	{
		vx=-ca*thrust;
		vy=-sa*thrust;
	}
	if (kb->KeyDown(controls[BGA_CTRL_RIGHT]))
	{
		Turn(4);
	}
	if (kb->KeyDown(controls[BGA_CTRL_LEFT]))
	{
		Turn(-4);
	}
	if (kb->KeyDown(controls[BGA_CTRL_FIRE]))
	{
		CL_Point front=GetCorner(32,0);
		weapons[cur_weapon]->Fire(front.x,front.y,CMisc::Get2PAngle(cr_c,cr_target));
	}
	if (kb->KeyPress(controls[BGA_CTRL_CHANGE_WEAPON]))
	{
		NextWeapon();
	}

	if (angle>359)
		angle=angle-360;
	if (angle<0)
		angle=360+angle;

	//проверяем, задний ли ход
	int revtest=IsReverse();
	if (revtest!=-1)
		reverse=revtest;
}

// обновляет состояние игрока
void CPlayer::Update(void)
{
	//скорость
	ux+=vx;
	uy+=vy;
	vx=vy=0;

	//ограничение скорости
	while (ux*ux+uy*uy>max_speed)
	{
		ux*=0.95;
		uy*=0.95;
	}

	int speed=ux*ux+uy*uy;

	//сила трения
	double fx=-ux;
	double fy=-uy;

	double friction=0.19;//пока трение одинаково на люб. поверхности
	double length_fr=fx*fx+fy*fy;
	//если едем
	if (length_fr>1)
	{
		fx=fx*friction/length_fr;
		fy=fy*friction/length_fr;
	}
	else
		fx=fy=0.0;

	//трение
	ux+=fx;
	uy+=fy;
	double sm=1.0;
	bool movex=true;
	bool movey=true;
	if (ux<sm&&ux>0 || ux>-sm&&ux<0)
		movex=false;
	if (uy<sm&&uy>0 || uy>-sm&&uy<0)
		movey=false;

	
	double tx=pdx,ty=pdy;
	//tx+=ux;ty+=uy;

	//перемещение
	if (movex)
		pdx+=ux;
	if (movey)
		pdy+=uy;

	//округляем
	x=(int)pdx+0.5;

	bool xcol=false,ycol=false;

	UpdateCollision();
	if (clt || crt || clb || crb || cct || ccb)
	{
		pdx=tx;
		//округляем
		x=(int)pdx+0.5;
		xcol=true;
	}

	y=(int)pdy+0.5;

	UpdateCollision();
	if (clt || crt || clb || crb || cct || ccb)
	{
		pdy=ty;
		//округляем
		y=(int)pdy+0.5;
		ycol=true;
	}

	if (xcol)// && !ycol)
	{
		if (abs(ux)>2)
			ux-=CMisc::Sign(ux)*thrust*2;
	}
	if(ycol)// && !xcol)
	{
		if (abs(uy)>2)
			uy-=CMisc::Sign(uy)*thrust*2;
	}

	//если коллижинимся
	
	/*int ccc;
	if (clt || crt || clb || crb)
	{
		if (clt)
			ccc=clt;
		else if (crt)
			ccc=crt;
		else if (clb)
			ccc=clb;
		else
			ccc=crb;


		//возврат к предыдущей позиции
		pdx=tx;
		pdy=ty;
		//округляем
		x=(int)pdx+0.5;
		y=(int)pdy+0.5;		

		if (ux*ux+uy*uy>50)
		{
			ux-=ux*0.7;
			uy-=uy*0.7;
		}
	}

	int scm=0;//скалярное произведение
	int dir=0;//0-вправо, 1-влево

	if (clt)
	{
		scm=CMisc::Sign(ux)*normal_lt.x+CMisc::Sign(uy)*normal_lt.y;
	}
	if (crt)
	{
		dir=1;
		scm=CMisc::Sign(ux)*normal_rt.x+CMisc::Sign(uy)*normal_rt.y;
	}
	if (clb)
	{
		dir=1;
		scm=CMisc::Sign(ux)*normal_lb.x+CMisc::Sign(uy)*normal_lb.y;
	}
	if (crb)
	{
		scm=CMisc::Sign(ux)*normal_rb.x+CMisc::Sign(uy)*normal_rb.y;
	}

		/* //зацепились левым передним углом или правым задним
		if ((clt && !crt ||	crb && !clb)&&speed>2)
		{
			Turn(3);
			//ux=-ux;
			//uy=-uy;
		}

		//зацепились правым передним углом или левым задним
		if ((!clt && crt || !crb && clb)&speed>2)
		{
			Turn(-3);
		}

		/*if (ccc==CGameMap::W_LEFT || ccc==CGameMap::W_RIGHT)
		{
			ux=-ux;
		}
		if (ccc==CGameMap::W_TOP || ccc==CGameMap::W_BOTTOM)
		{
			uy=-uy;
		}* /

	if (scm)//>0)//острый угол
	{
		if (!dir)
			Turn(8);
		else
			Turn(-8);

		if (speed>2)
		{
			ux*=0.7;
			uy*=0.7;
		}

		int fcol_num=0;
		if (clt)
			fcol_num++;
		if (crt)
			fcol_num++;
		if (clb)
			fcol_num++;
		if (crb)
			fcol_num++;

		clt=game_map->IsWall(cr_lt.x,cr_lt.y,normal_lt);
		crt=game_map->IsWall(cr_rt.x,cr_rt.y,normal_rt);
		clb=game_map->IsWall(cr_lb.x,cr_lb.y,normal_lb);
		crb=game_map->IsWall(cr_rb.x,cr_rb.y,normal_rb);

		int col_num=0;
		if (clt)
			col_num++;
		if (crt)
			col_num++;
		if (clb)
			col_num++;
		if (crb)
			col_num++;

		if (col_num>fcol_num)
		{
			if (!dir)
				Turn(-8);
			else
				Turn(8);
		}
	}*/

	//находим углы
	int free=8;
	cr_c=GetCorner(32,32);
	cr_lt=GetCorner(free,0);
	cr_lb=GetCorner(free,64);
	cr_rt=GetCorner(64-free,0);
	cr_rb=GetCorner(64-free,64);

	//точка перед тачкой, чтобы смотреть камерой
	cr_t.x=x+ux*25;
	cr_t.y=y+uy*25;

	//обновляем индивидуальные особенности
	UpdateSS();

	//едим бонусы
	EatBonus(cr_lt.x, cr_lt.y);
	EatBonus(cr_lb.x, cr_lb.y);
	EatBonus(cr_rt.x, cr_rt.y);
	EatBonus(cr_rb.x, cr_rb.y);

	//ищем цель
	FindAim();
	//попаду в кого-нибудь?
	CheckTarget();
	//перемещаем камеру
	cam->FollowPlayer(game_map->GetWidth(),game_map->GetHeight());

	//дым из трубы
	if (!HasFeature(BGA_ITEM_INVISIBILITY-BGA_ITEM_BONUS1)&&!HasFeature(BGA_ITEM_FREEZE-BGA_ITEM_BONUS1))
	{
		AddSmoke(GetCorner(24,64));
		AddSmoke(GetCorner(40,64));
	}

	//звук двигателя
	world->GetListener()->ModifySound(snd_engine,x,y);
}

// возвращает информацию об игроке для нового подключившегося гамера
screate_player CPlayer::GetInfo(void)
{
	screate_player i;

	i.id=id;
	i.x=x;
	i.y=y;
	i.angle=angle;

	i.input_type=CPlayer::IT_NETWORK;
	i.cam_type=CCamera::CT_NOCAMERA;
	i.controls=NULL;
	i.local=false;

	i.frags=frags;
	i.max_frags=max_frags;
	strcpy(i.name,name.c_str());
	strcpy(i.spr_id,spr_id.c_str());

	return i;
}

// возвращает информацию о позиции и состоянии игрока
supdate_player CPlayer::GetUpdateInfo(void)
{
	supdate_player pl;
	pl.x=x;
	pl.y=y;
	pl.angle=angle;
	pl.id=id;
	pl.ux=ux;
	pl.uy=uy;
	pl.vx=vx;
	pl.vy=vy;
	pl.features=features;
	return pl;
}

// изменяет положение и состояние игрока
void CPlayer::Modify(supdate_player pl)
{
	x=pl.x;
	y=pl.y;
	angle=pl.angle;
	ux=pl.ux;
	uy=pl.uy;
	pdx=x;
	pdy=y;
	features=pl.features;
}


// обновляет нелокального игрока, не учитываются многие факторы
void CPlayer::UpdateRemote(void)
{
	//vx=0;vy=0;
	Update();
}

// возвращает 1-задний ход, 0-передний, -1 - точно определить нельзя
int CPlayer::IsReverse(void)
{
	double ca=CMisc::Cos(angle);
	double sa=CMisc::Sin(angle);

	if (CMisc::Sign(ca)==CMisc::Sign(ux) && CMisc::Sign(sa)==CMisc::Sign(uy))
	{
		//сто пудов передний ход    
		return 0;
	}
	if (CMisc::Sign(ca)!=CMisc::Sign(ux) && CMisc::Sign(sa)!=CMisc::Sign(uy))
	{
		//сто пудов задний ход    
		return 1;
	}

	//не знаю
	return -1;
}

// возвращает координаты точки, смещенной на заданный вектор
CL_Point CPlayer::GetCorner(int dx, int dy)
{
	CL_Point p;
	p.x=dx;
	p.y=dy;

	p=p.rotate(CL_Point(32,32),angle+90);
	
	p.x+=x;
	p.y+=y;
	return p;
}

// возвращает прямоугольник, при попадании в который  пуля взрывается
CL_Rect CPlayer::GetHitRect(void)
{
	CL_Point LD=GetLDCorner();
	CL_Point RU=GetRUCorner();
	CL_Rect t(LD.x,LD.y,RU.x,RU.y);
	t.normalize();
	return t;
}

// возвращает левую нижнюю точку тачки
CL_Point CPlayer::GetLDCorner(void)
{
	CL_Point l=cr_lt;
	l=CMisc::LeftPoint(l,cr_rt);
	l=CMisc::LeftPoint(l,cr_lb);
	l=CMisc::LeftPoint(l,cr_rb);

	CL_Point d=cr_lt;
	d=CMisc::DownPoint(d,cr_rt);
	d=CMisc::DownPoint(d,cr_lb);
	d=CMisc::DownPoint(d,cr_rb);

	return CL_Point(l.x,d.y);
}

// возвращает правую верхнюю точку тачки
CL_Point CPlayer::GetRUCorner(void)
{
	CL_Point l=cr_lt;
	l=CMisc::RightPoint(l,cr_rt);
	l=CMisc::RightPoint(l,cr_lb);
	l=CMisc::RightPoint(l,cr_rb);

	CL_Point d=cr_lt;
	d=CMisc::UpPoint(d,cr_rt);
	d=CMisc::UpPoint(d,cr_lb);
	d=CMisc::UpPoint(d,cr_rb);

	return CL_Point(l.x,d.y);
}

// уменьшает жизнь игрока на опред. кол-во хитов
void CPlayer::DecLife(int dlife, int shooter, int wtype)
{
	//отнимает жизни и броню
	double t=(double)dlife*0.75;
	if (armor>=t)
	{
		armor-=(int)t;
		life-=(int)dlife*0.25;
	}
	else
	{
		int arml=armor;
		armor=0;
        life-=dlife+arml;
	}

	if (life<=0)
	{
		CPlayer *pl=world->GetPlayer(shooter);
		int f=pl->GetFrags()+1;

		if (game->GetGameType()==BGA_GAME_DEATHMATCH)
		{
			if (f>max_frags)
				max_frags=f;
		}
		if (game->GetGameType()==BGA_GAME_TEAMDEATHMATCH || game->GetGameType()==BGA_GAME_CAPTURETHEFLAG)
		{
			if (pl->GetTeam()==BGA_TEAM_BLUE)
			{
                if (game->GetBlueScore()+1>max_frags)
					max_frags=game->GetBlueScore()+1;
			}
			else
			{
                if (game->GetRedScore()+1>max_frags)
					max_frags=game->GetRedScore()+1;
			}
		}

		supdate_frags *fr=new supdate_frags;
		fr->id=shooter;
		fr->target_id=id;//меня убил
		fr->weapon=wtype;//из этой пушки
		
		if (team!=pl->GetTeam())
			fr->dfrags=1;//добавляем фраг
		else //самойбийство
			fr->dfrags=-1;//убираем фраг

		fr->max_frags=max_frags;
		game->SendNetMessage(BGA_MSG_UPDATE_FRAGS, fr);
		
		//взрыв
		screate_explode *e=new screate_explode;
		e->x=cr_c.x;e->y=cr_c.y;
		e->vx=0;
		e->vy=0;

		e->type=BGA_EXPLODE_NORMAL;

		world->AddExplode(e);

		Respawn();
	}
}

// оживляет игрока в новой точке
void CPlayer::Respawn(void)
{
	life=100;
	armor=0;

	htick=atick=ftick=0;

	CL_Point t=world->GetStartPoint();
	angle=rand()%4*90;
	pdx=t.x;
	pdy=t.y;
	x=(int)pdx,y=(int)pdy;
	ux=uy=0;

	reverse=false;
	aimed=false;
	cr_target=GetCorner(32,-200);
	
	max_speed=BGA_MAX_SPEED;
	for (int i=0;i<6;i++)
		features.all[i]=0;

	cur_weapon=0;
	//патроны
	for (int i=0;i<5;i++)
		weapons[i]->SetBullets(0);
	weapons[0]->SetBullets(100);
	no_weapon=false;

	world->PlaySound(BGA_SND_RESPAWN,t.x,t.y,true);

	//облако телеторта
	screate_explode *e=new screate_explode;
	e->x=t.x+32;e->y=t.y+32;
	e->vx=0;
	e->vy=0;

	e->type=BGA_EXPLODE_RESPAWN;

	world->AddExplode(e);
}

// поворачивает тачку на данный угол
void CPlayer::Turn(int a)
{
	double l=sqrt(ux*ux+uy*uy);
	double sa=CMisc::Sin(angle);
	double ca=CMisc::Cos(angle);
	//если едем
	if (l>1.0)
	{
		//задний ход
		/*if (!(CMisc::EqualSignOrZero(ux+ca/5,ca)) || !(CMisc::EqualSignOrZero(uy+sa/5,sa)))
		{
			reverse=true;
		}*/

		if (!reverse)
			angle+=a;
		else
			angle-=a;

		ux2=CMisc::Cos(angle);
		uy2=CMisc::Sin(angle);

		if (reverse)
		{
			ux2=-ux2;
			uy2=-uy2;
		}

		//плавный поворот
		ux=ux+(l*ux2-ux)/5;
		uy=uy+(l*uy2-uy)/5;
	}
}

// перемещает прицел на ближайшего врага
void CPlayer::FindAim(void)
{
	vector<CPlayer*> *enemy=world->GetPlayers();
	CL_Point temp;
	CL_Point front=GetCorner(32,-200);
	bool found=false;
	double dist=1;
	for (int i=0;i<enemy->size();i++)
	{
		//это ж Я!
		if ((*enemy)[i]==this)
			continue;
		
		temp=(*enemy)[i]->GetCenter();

		dist=front.distance(temp);
		if (dist==0)
			dist=1;
		if (dist<150)
		{
			found=true;
			break;
		}

	}

	if (!found)
	{
		temp=front;
		dist=cr_target.distance(temp);
		if (dist==0)
			dist=1;
	}

	//подъезжаем
	double dx=(temp.x-cr_target.x)/10;//dist;
	double dy=(temp.y-cr_target.y)/10;//dist;
	cr_target.x+=(int)(dx+0.5);
	cr_target.y+=(int)(dy+0.5);
}

// добавляет дым из трубы
void CPlayer::AddSmoke(CL_Point tube)
{
	screate_explode *e=new screate_explode;
	e->x=tube.x;e->y=tube.y;
	double ac=CMisc::Cos(angle);
	double as=CMisc::Sin(angle);
	e->vx=-ac*3.0f;
	e->vy=-as*3.0f;

	if (IsReverse()==1)
	{
		e->vx-=(int)abs(ux)*CMisc::Sign(ac)*1.1;
		e->vy-=(int)abs(uy)*CMisc::Sign(as)*1.1;
	}

	e->type=BGA_EXPLODE_SMOKE;

	game->AddMqMessage(BGA_MSG_NEW_EXPLODE,e);
}

// если прицел поймал врага, окрашивает его в зеленый цвет
void CPlayer::CheckTarget(void)
{
	vector<CPlayer*> *enemy=world->GetPlayers();
	CL_Rect temp;

	aimed=false;
	for (int i=0;i<enemy->size();i++)
	{
		//это ж Я!
		if ((*enemy)[i]==this)
			continue;
		
		temp=(*enemy)[i]->GetHitRect();
		CL_Point s=(*enemy)[i]->GetCenter();

		//нацелился
		if (temp.is_inside(cr_target))
		{
			aimed=true;
			break;
		}

	}
}

// обновляет состояние внутренних переменных столкновения
void CPlayer::UpdateCollision(void)
{
	//находим углы
	int free=8;
	cr_lt=GetCorner(free,0);
	cr_lb=GetCorner(free,64);
	cr_rt=GetCorner(64-free,0);
	cr_rb=GetCorner(64-free,64);
	cr_ct=GetCorner(32,0);
	cr_cb=GetCorner(32,64);
	
	CL_Point t;

	clt=game_map->IsWall(cr_lt.x,cr_lt.y,normal_lt);
	crt=game_map->IsWall(cr_rt.x,cr_rt.y,normal_rt);
	clb=game_map->IsWall(cr_lb.x,cr_lb.y,normal_lb);
	crb=game_map->IsWall(cr_rb.x,cr_rb.y,normal_rb);
	cct=game_map->IsWall(cr_ct.x,cr_ct.y,t);
	ccb=game_map->IsWall(cr_cb.x,cr_cb.y,t);
}

// ест бонус, если стоит на нем
void CPlayer::EatBonus(int x, int y)
{
	int id=game_map->ItemCollided(x,y);
	
	if (id==-1)//а есть то нечего :(
		return;

	CItem *item=world->GetItem(id);

	if (!item->IsAlive())//так он же дохлый!
		return;

	int type=item->GetType();
	
	bool ate=false;//съел?

	int dh=0;//прибавка к здоровью
	int bul_count=0;//кол-во патронов

	switch (type)
	{
	case BGA_ITEM_AMMO1:
	case BGA_ITEM_AMMO2:
	case BGA_ITEM_AMMO3:
	case BGA_ITEM_AMMO4:
	case BGA_ITEM_AMMO5:
		bul_count=game->weapons[type-BGA_ITEM_AMMO1].count;
		weapons[type-BGA_ITEM_AMMO1]->AddBullets(bul_count);
		if (no_weapon)
			cur_weapon=type-BGA_ITEM_AMMO1;
		no_weapon=false;
		ate=true;
		world->PlaySound(BGA_SND_AMMO,x,y,false);
		break;

	case BGA_ITEM_ARMOR25://броня до 200
		if (armor<200)
		{
			armor+=25;
			if (armor>200)
				armor=200;
			ate=true;
			world->PlaySound(BGA_SND_ARMOR,x,y,false);
		}
		break;

	case BGA_ITEM_ARMOR100://броня до 100
		if (armor<100)
		{
			armor+=100;
			if (armor>100)
				armor=100;
			ate=true;
			world->PlaySound(BGA_SND_ARMOR,x,y,false);
		}
		break;

	case BGA_ITEM_HEALTH5://жизни до 200
		dh=5;
	case BGA_ITEM_HEALTH25:
		if (!dh)
			dh=25;
		if (life<200)
		{
			life+=dh;
			if (life>200)
				life=200;
			ate=true;
			world->PlaySound(BGA_SND_HEALTH,x,y,false);
		}
		break;

	case BGA_ITEM_HEALTH10://жизни до 100
		dh=10;
	case BGA_ITEM_HEALTH100:
		if (!dh)
			dh=100;
		if (life<100)
		{
			life+=dh;
			if (life>100)
				life=100;
			ate=true;
			world->PlaySound(BGA_SND_HEALTH,x,y,false);
		}
		break;

	case BGA_ITEM_BONUS1:
	case BGA_ITEM_BONUS2:
	case BGA_ITEM_BONUS3:
	case BGA_ITEM_BONUS4:
	case BGA_ITEM_BONUS5:
	case BGA_ITEM_BONUS6:
		if (type==BGA_ITEM_REGENERATION)
            features.all[type-BGA_ITEM_BONUS1]=15;
		else if (type==BGA_ITEM_HASTE)
            features.all[type-BGA_ITEM_BONUS1]=8;
		else if (type==BGA_ITEM_MINE)
			features.all[type-BGA_ITEM_BONUS1]=0;		
		else
			features.all[type-BGA_ITEM_BONUS1]=5;
		ate=true;


		if (type==BGA_ITEM_HASTE)//ускорение
		{
			max_speed=BGA_MAX_SPEED*2;
			world->PlaySound(BGA_SND_HASTE,x,y,false);
		}
		if (type==BGA_ITEM_QUADDAMAGE)//двойной урон
		{
			world->PlaySound(BGA_SND_QUADDAMAGE,x,y,false);
		}
		if (type==BGA_ITEM_INVISIBILITY)//невидимость
		{
			world->PlaySound(BGA_SND_INVISIBILITY,x,y,false);
		}
		if (type==BGA_ITEM_REGENERATION)//регенерация
		{
			world->PlaySound(BGA_SND_REGENERATION,x,y,false);
		}
		if (type==BGA_ITEM_FREEZE)//заморозка
		{
			max_speed=1;
		}
		if (type==BGA_ITEM_MINE)//мина
		{
			DecLife(100,this->id,5);

			//взрыв
			screate_explode *e=new screate_explode;
			e->x=x+32;e->y=y+32;
			e->vx=0;
			e->vy=0;

			e->type=BGA_EXPLODE_NORMAL;

			world->AddExplode(e);
		}
		break;
	}

	if (ate)
		item->Kill();
}

// обновляет особое состояние (special state): жизни и броня >100, различные усилители, etc.
void CPlayer::UpdateSS(void)
{
	unsigned int time=CL_System::get_time();

	if (htick<time)//худеем
	{
		if (!HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
		{
			if (life>100)
				life--;
		}
		else if (life<200)//регенерация
			life++;
	
		htick=time+2000;
	}

	if (atick<time)//изнашивается броня
	{
		if (!HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
		{
			if (armor>100)
				armor--;
		}
		else if (armor<200)//регенерация
			armor++;
		atick=time+2000;
	}

	int i;
	bool ok=false;
	for (i=0;i<6;i++)
	{
		if (features.all[i]>0 && ftick<time)//кончается энергия тюнинга
		{
			if ((i==BGA_ITEM_HASTE-BGA_ITEM_BONUS1 || i==BGA_ITEM_FREEZE-BGA_ITEM_BONUS1)
				&& features.all[i]==1)//ускорение или замедление
			{
				max_speed=BGA_MAX_SPEED;
			}

			features.all[i]--;
			ok=true;
		}
	}

	if (ok)
		ftick=time+2000;
}

// выбирает следующую пушку в которой есть патроны, возвращает ее номер
int CPlayer::NextWeapon(void)
{
	int i;
	//до конца
	for (i=cur_weapon+1;i<5;i++)
		if (weapons[i]->GetBullets())
		{
			cur_weapon=i;
			return i;
		}
	
	//до начальной пушки
	for (i=0;i<cur_weapon;i++)
		if (weapons[i]->GetBullets())
		{
			cur_weapon=i;
			return i;
		}
	
	//в других пушках нет патронов
	
	if (!weapons[cur_weapon]->GetBullets())//в этой тоже нет :)
		no_weapon=true;

	return cur_weapon;
}
