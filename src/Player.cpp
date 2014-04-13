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

	//�������������� �����
	CWeapon::SetGameWorld(world);
	for (int w=0;w<5;w++)
		weapons[w]=new CWeapon(w,game->weapons[w].damage,game->weapons[w].reload_time*10,100,game->weapons[w].speed*3,this);

	cur_weapon=0;

	if (pl_info.max_frags>max_frags)
		max_frags=pl_info.max_frags;
	frags=pl_info.frags;

	//���� ���������
	snd_engine=new CL_SoundBuffer_Session();
	*snd_engine=CListener::snd_engine->prepare();
	snd_engine->set_looping(true);
	//snd_engine->play();//�� ������ AVA12 � forum.sources.ru ���� ��������

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

// ���������� X-���������� ������
int CPlayer::GetX(void)
{
	return x;
}

// ���������� Y-���������� ������
int CPlayer::GetY(void)
{
	return y;
}

// ���������� true, ���� ����� ���������
bool CPlayer::IsLocal(void)
{
	return local;
}

// ���������� ��������� �� ������ ������
CCamera* CPlayer::GetCam(void)
{
	return cam;
}

// ����������� �����������, ��� �������. �����. ������
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

// ������ ������ � �������� �����������
void CPlayer::Draw(int ux, int uy)
{
	spr_car->set_angle(angle);

	if (!HasFeature(BGA_ITEM_INVISIBILITY-BGA_ITEM_BONUS1))//�����������
	{
		//����
		spr_car->set_color(0,0,0,0.5);
		spr_car->draw(x+ux+5,y+uy+5);//���� ������ � ������� �����
		spr_car->set_color(255,255,255,1);
	}
	else
		spr_car->set_color(255,255,255,0.3);

	//������
	spr_car->draw(x+ux,y+uy);

	//������� ����
	if (HasFeature(BGA_ITEM_QUADDAMAGE-BGA_ITEM_BONUS1))
	{
		spr_car->set_color(0,0,255,0.4);
		spr_car->draw(x+ux,y+uy);
	}

	//�����������
	if (HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
	{
		spr_car->set_color(255,0,0,0.4);
		spr_car->draw(x+ux,y+uy);
	}

	//�������
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

// ��������� ������ � ���������� ���������
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

	//���������, ������ �� ���
	int revtest=IsReverse();
	if (revtest!=-1)
		reverse=revtest;
}

// ��������� ��������� ������
void CPlayer::Update(void)
{
	//��������
	ux+=vx;
	uy+=vy;
	vx=vy=0;

	//����������� ��������
	while (ux*ux+uy*uy>max_speed)
	{
		ux*=0.95;
		uy*=0.95;
	}

	int speed=ux*ux+uy*uy;

	//���� ������
	double fx=-ux;
	double fy=-uy;

	double friction=0.19;//���� ������ ��������� �� ���. �����������
	double length_fr=fx*fx+fy*fy;
	//���� ����
	if (length_fr>1)
	{
		fx=fx*friction/length_fr;
		fy=fy*friction/length_fr;
	}
	else
		fx=fy=0.0;

	//������
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

	//�����������
	if (movex)
		pdx+=ux;
	if (movey)
		pdy+=uy;

	//���������
	x=(int)pdx+0.5;

	bool xcol=false,ycol=false;

	UpdateCollision();
	if (clt || crt || clb || crb || cct || ccb)
	{
		pdx=tx;
		//���������
		x=(int)pdx+0.5;
		xcol=true;
	}

	y=(int)pdy+0.5;

	UpdateCollision();
	if (clt || crt || clb || crb || cct || ccb)
	{
		pdy=ty;
		//���������
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

	//���� ������������
	
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


		//������� � ���������� �������
		pdx=tx;
		pdy=ty;
		//���������
		x=(int)pdx+0.5;
		y=(int)pdy+0.5;		

		if (ux*ux+uy*uy>50)
		{
			ux-=ux*0.7;
			uy-=uy*0.7;
		}
	}

	int scm=0;//��������� ������������
	int dir=0;//0-������, 1-�����

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

		/* //���������� ����� �������� ����� ��� ������ ������
		if ((clt && !crt ||	crb && !clb)&&speed>2)
		{
			Turn(3);
			//ux=-ux;
			//uy=-uy;
		}

		//���������� ������ �������� ����� ��� ����� ������
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

	if (scm)//>0)//������ ����
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

	//������� ����
	int free=8;
	cr_c=GetCorner(32,32);
	cr_lt=GetCorner(free,0);
	cr_lb=GetCorner(free,64);
	cr_rt=GetCorner(64-free,0);
	cr_rb=GetCorner(64-free,64);

	//����� ����� ������, ����� �������� �������
	cr_t.x=x+ux*25;
	cr_t.y=y+uy*25;

	//��������� �������������� �����������
	UpdateSS();

	//���� ������
	EatBonus(cr_lt.x, cr_lt.y);
	EatBonus(cr_lb.x, cr_lb.y);
	EatBonus(cr_rt.x, cr_rt.y);
	EatBonus(cr_rb.x, cr_rb.y);

	//���� ����
	FindAim();
	//������ � ����-������?
	CheckTarget();
	//���������� ������
	cam->FollowPlayer(game_map->GetWidth(),game_map->GetHeight());

	//��� �� �����
	if (!HasFeature(BGA_ITEM_INVISIBILITY-BGA_ITEM_BONUS1)&&!HasFeature(BGA_ITEM_FREEZE-BGA_ITEM_BONUS1))
	{
		AddSmoke(GetCorner(24,64));
		AddSmoke(GetCorner(40,64));
	}

	//���� ���������
	world->GetListener()->ModifySound(snd_engine,x,y);
}

// ���������� ���������� �� ������ ��� ������ ��������������� ������
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

// ���������� ���������� � ������� � ��������� ������
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

// �������� ��������� � ��������� ������
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


// ��������� ������������ ������, �� ����������� ������ �������
void CPlayer::UpdateRemote(void)
{
	//vx=0;vy=0;
	Update();
}

// ���������� 1-������ ���, 0-��������, -1 - ����� ���������� ������
int CPlayer::IsReverse(void)
{
	double ca=CMisc::Cos(angle);
	double sa=CMisc::Sin(angle);

	if (CMisc::Sign(ca)==CMisc::Sign(ux) && CMisc::Sign(sa)==CMisc::Sign(uy))
	{
		//��� ����� �������� ���    
		return 0;
	}
	if (CMisc::Sign(ca)!=CMisc::Sign(ux) && CMisc::Sign(sa)!=CMisc::Sign(uy))
	{
		//��� ����� ������ ���    
		return 1;
	}

	//�� ����
	return -1;
}

// ���������� ���������� �����, ��������� �� �������� ������
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

// ���������� �������������, ��� ��������� � �������  ���� ����������
CL_Rect CPlayer::GetHitRect(void)
{
	CL_Point LD=GetLDCorner();
	CL_Point RU=GetRUCorner();
	CL_Rect t(LD.x,LD.y,RU.x,RU.y);
	t.normalize();
	return t;
}

// ���������� ����� ������ ����� �����
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

// ���������� ������ ������� ����� �����
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

// ��������� ����� ������ �� �����. ���-�� �����
void CPlayer::DecLife(int dlife, int shooter, int wtype)
{
	//�������� ����� � �����
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
		fr->target_id=id;//���� ����
		fr->weapon=wtype;//�� ���� �����
		
		if (team!=pl->GetTeam())
			fr->dfrags=1;//��������� ����
		else //������������
			fr->dfrags=-1;//������� ����

		fr->max_frags=max_frags;
		game->SendNetMessage(BGA_MSG_UPDATE_FRAGS, fr);
		
		//�����
		screate_explode *e=new screate_explode;
		e->x=cr_c.x;e->y=cr_c.y;
		e->vx=0;
		e->vy=0;

		e->type=BGA_EXPLODE_NORMAL;

		world->AddExplode(e);

		Respawn();
	}
}

// �������� ������ � ����� �����
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
	//�������
	for (int i=0;i<5;i++)
		weapons[i]->SetBullets(0);
	weapons[0]->SetBullets(100);
	no_weapon=false;

	world->PlaySound(BGA_SND_RESPAWN,t.x,t.y,true);

	//������ ���������
	screate_explode *e=new screate_explode;
	e->x=t.x+32;e->y=t.y+32;
	e->vx=0;
	e->vy=0;

	e->type=BGA_EXPLODE_RESPAWN;

	world->AddExplode(e);
}

// ������������ ����� �� ������ ����
void CPlayer::Turn(int a)
{
	double l=sqrt(ux*ux+uy*uy);
	double sa=CMisc::Sin(angle);
	double ca=CMisc::Cos(angle);
	//���� ����
	if (l>1.0)
	{
		//������ ���
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

		//������� �������
		ux=ux+(l*ux2-ux)/5;
		uy=uy+(l*uy2-uy)/5;
	}
}

// ���������� ������ �� ���������� �����
void CPlayer::FindAim(void)
{
	vector<CPlayer*> *enemy=world->GetPlayers();
	CL_Point temp;
	CL_Point front=GetCorner(32,-200);
	bool found=false;
	double dist=1;
	for (int i=0;i<enemy->size();i++)
	{
		//��� � �!
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

	//����������
	double dx=(temp.x-cr_target.x)/10;//dist;
	double dy=(temp.y-cr_target.y)/10;//dist;
	cr_target.x+=(int)(dx+0.5);
	cr_target.y+=(int)(dy+0.5);
}

// ��������� ��� �� �����
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

// ���� ������ ������ �����, ���������� ��� � ������� ����
void CPlayer::CheckTarget(void)
{
	vector<CPlayer*> *enemy=world->GetPlayers();
	CL_Rect temp;

	aimed=false;
	for (int i=0;i<enemy->size();i++)
	{
		//��� � �!
		if ((*enemy)[i]==this)
			continue;
		
		temp=(*enemy)[i]->GetHitRect();
		CL_Point s=(*enemy)[i]->GetCenter();

		//���������
		if (temp.is_inside(cr_target))
		{
			aimed=true;
			break;
		}

	}
}

// ��������� ��������� ���������� ���������� ������������
void CPlayer::UpdateCollision(void)
{
	//������� ����
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

// ��� �����, ���� ����� �� ���
void CPlayer::EatBonus(int x, int y)
{
	int id=game_map->ItemCollided(x,y);
	
	if (id==-1)//� ���� �� ������ :(
		return;

	CItem *item=world->GetItem(id);

	if (!item->IsAlive())//��� �� �� ������!
		return;

	int type=item->GetType();
	
	bool ate=false;//����?

	int dh=0;//�������� � ��������
	int bul_count=0;//���-�� ��������

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

	case BGA_ITEM_ARMOR25://����� �� 200
		if (armor<200)
		{
			armor+=25;
			if (armor>200)
				armor=200;
			ate=true;
			world->PlaySound(BGA_SND_ARMOR,x,y,false);
		}
		break;

	case BGA_ITEM_ARMOR100://����� �� 100
		if (armor<100)
		{
			armor+=100;
			if (armor>100)
				armor=100;
			ate=true;
			world->PlaySound(BGA_SND_ARMOR,x,y,false);
		}
		break;

	case BGA_ITEM_HEALTH5://����� �� 200
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

	case BGA_ITEM_HEALTH10://����� �� 100
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


		if (type==BGA_ITEM_HASTE)//���������
		{
			max_speed=BGA_MAX_SPEED*2;
			world->PlaySound(BGA_SND_HASTE,x,y,false);
		}
		if (type==BGA_ITEM_QUADDAMAGE)//������� ����
		{
			world->PlaySound(BGA_SND_QUADDAMAGE,x,y,false);
		}
		if (type==BGA_ITEM_INVISIBILITY)//�����������
		{
			world->PlaySound(BGA_SND_INVISIBILITY,x,y,false);
		}
		if (type==BGA_ITEM_REGENERATION)//�����������
		{
			world->PlaySound(BGA_SND_REGENERATION,x,y,false);
		}
		if (type==BGA_ITEM_FREEZE)//���������
		{
			max_speed=1;
		}
		if (type==BGA_ITEM_MINE)//����
		{
			DecLife(100,this->id,5);

			//�����
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

// ��������� ������ ��������� (special state): ����� � ����� >100, ��������� ���������, etc.
void CPlayer::UpdateSS(void)
{
	unsigned int time=CL_System::get_time();

	if (htick<time)//������
	{
		if (!HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
		{
			if (life>100)
				life--;
		}
		else if (life<200)//�����������
			life++;
	
		htick=time+2000;
	}

	if (atick<time)//������������ �����
	{
		if (!HasFeature(BGA_ITEM_REGENERATION-BGA_ITEM_BONUS1))
		{
			if (armor>100)
				armor--;
		}
		else if (armor<200)//�����������
			armor++;
		atick=time+2000;
	}

	int i;
	bool ok=false;
	for (i=0;i<6;i++)
	{
		if (features.all[i]>0 && ftick<time)//��������� ������� �������
		{
			if ((i==BGA_ITEM_HASTE-BGA_ITEM_BONUS1 || i==BGA_ITEM_FREEZE-BGA_ITEM_BONUS1)
				&& features.all[i]==1)//��������� ��� ����������
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

// �������� ��������� ����� � ������� ���� �������, ���������� �� �����
int CPlayer::NextWeapon(void)
{
	int i;
	//�� �����
	for (i=cur_weapon+1;i<5;i++)
		if (weapons[i]->GetBullets())
		{
			cur_weapon=i;
			return i;
		}
	
	//�� ��������� �����
	for (i=0;i<cur_weapon;i++)
		if (weapons[i]->GetBullets())
		{
			cur_weapon=i;
			return i;
		}
	
	//� ������ ������ ��� ��������
	
	if (!weapons[cur_weapon]->GetBullets())//� ���� ���� ��� :)
		no_weapon=true;

	return cur_weapon;
}
