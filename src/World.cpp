#include ".\world.h"
#include ".\game.h"
#include ".\player.h"
#include ".\bonusexplode.h"

CWorld::CWorld(CGame* _game)
{
	Map=NULL;//new CGameMap(this);
	game=_game;
	listener=new CListener(game->GetResman());
	//Map->LoadMap("1");
}

CWorld::~CWorld()
{
	LOG<<"����������� �������� ����\n";
	if (Map)
		delete Map;

	CPlayer *pl;
	int s;
	s=players.size();
	for (int j=0;j<s;j++)
	{			
		pl=players[j];
		delete pl;
		players.pop_back();
	}

	CBullet *b;
	s=bullets.size();
	for (int j=0;j<s;j++)
	{			
		b=bullets[j];
		delete b;
		bullets.pop_back();
	}

	delete listener;
}

// ������ ������� ���
void CWorld::Draw(void)
{
	int i,j;
	CL_Rect camr;

	for (i=0;i<players.size();i++)
	{
		//�����
		if (players[i]->IsLocal())
		{
			camr=players[i]->GetCam()->GetRect();
			CL_Display::set_cliprect(camr);
			int dx=players[i]->GetCam()->GetDX();
			int dy=players[i]->GetCam()->GetDY();
			Map->Draw(dx,dy,camr);

			//������
			for (j=0;j<items.size();j++)
			{			
				items[j]->Draw(camr.left-dx,camr.top-dy);
			}			

			//������
			for (j=0;j<players.size();j++)
			{		
				if (players[j]->HasFeature(BGA_ITEM_INVISIBILITY-BGA_ITEM_BONUS1) && i!=j) //�����������
					continue;

				players[j]->Draw(camr.left-dx,camr.top-dy);
			}

			//����
			for (j=0;j<bullets.size();j++)
			{			
				bullets[j]->Draw(camr.left-dx,camr.top-dy);
			}

			//������
			for (j=0;j<explodes.size();j++)
			{			
				explodes[j]->Draw(camr.left-dx,camr.top-dy);
			}

			//��������� �������
			for (j=0;j<items.size();j++)
			{			
				items[j]->Draw(camr.left-dx,camr.top-dy,true);
			}	
			Map->Draw(dx,dy,camr,true);//������� �������
				
			//���������
			CL_Display::draw_rect(camr,CL_Color::white);
			players[i]->GetCam()->DrawInterface();
			
			//������ �������
			kill_info.Draw(players[i]->GetCam()->GetRect().right,players[i]->GetCam()->GetRect().top+5);
			//������ ���������
			talk_info.Draw(players[i]->GetCam()->GetRect().left+5,players[i]->GetCam()->GetRect().bottom-90);
		}
	}
	//��������������� ������� ���������
	CL_Display::set_cliprect(CL_Rect(0,0,CL_Display::get_width(),CL_Display::get_height()));
}

// ��������� ������ ������
void CWorld::AddPlayer(screate_player pl_info)
{
	pl_info.game_map=Map;
	CPlayer *pl=new CPlayer(pl_info);
	players.push_back(pl);
}

// ������������ ���� ���� ��������� �������
void CWorld::Input(void)
{
	for (int i=0;i<players.size();i++)
		if (players[i]->IsLocal())
			players[i]->Input();
}

// ��������� ������� ���
void CWorld::Update()
{
	scol_info info;
	info.map=Map;
	info.players=&players;
	info.world=this;

	//������
	int lx=0,ly=0,lc=0;
	for (int i=0;i<players.size();i++)
		if (players[i]->IsLocal())
		{
			players[i]->Update();
			lx+=players[i]->GetX();
			ly+=players[i]->GetY();
			lc++;
		}
		else
			players[i]->UpdateRemote();

	//��������� ���������
	lx/=lc;
	ly/=lc;
	listener->SetPosition(lx,ly);

	//����
	for (int i=0;i<bullets.size();i++)
			bullets[i]->Update(info);

	//������
	for (int i=0;i<explodes.size();i++)
			explodes[i]->Update();

	//������
	for (int i=0;i<items.size();i++)
			items[i]->Update();

	//������ �������
	kill_info.Update();
	//������ ���������
	talk_info.Update();

	KillDeadBullets();
	KillDeadExplodes();

}

// ���������� ��������� ����� ��������� ������
CL_Point CWorld::GetStartPoint(void)
{
	/*int x,y;
	do
	{
		x=rand()%((Map->GetWidth()-1)*64);
		y=rand()%((Map->GetHeight()-1)*64);
	} while(Map->IsWall(x,y,CL_Point()));*/

	int id=start_points[rand()%sp_count];
    
	return items[id]->GetPosition();
}
// ���������� ����� ���������� �� ������� ����
sworld_info CWorld::GetWorldInfo(void)
{
	sworld_info i;
	strcpy(i.level_name,Map->GetName().c_str());
	return i;
}

// ������� ������� ������� ��� � ������� �����
int CWorld::NewWorld(sworld_info info)
{
	LOG<<LOG.Time()<<"�������� �������� ����\n";
	sp_count=0;//���-�� ��������� �����
	last_id=0;

	//������� �����
	if (Map)
		delete Map;

	Map=new CGameMap(this, game);
	Map->LoadMap(info.level_name);

	return 1;
}

// �������� ���������� � ��������� ��������� ��������
void CWorld::UpdateNetwork(void)
{
	supdate_player *pl;
	for (int i=0;i<players.size();i++)
		if (players[i]->IsLocal())
		{
			pl=new supdate_player;
			*pl=players[i]->GetUpdateInfo();
			game->SendNetMessage(BGA_MSG_UPDATE_PLAYER, pl);
		}
}

// ��������� ����������� ������
void CWorld::UpdatePlayer(supdate_player pl)
{
	int idx=FindPlayer(pl.id);
	if (idx==-1)
		return;
	if (!players[idx]->IsLocal())
		players[idx]->Modify(pl);
}

// ���������� ������ ������ � ������������ id
int CWorld::FindPlayer(int id)
{
	for (int i=0;i<players.size();i++)
		if (players[i]->GetId()==id)
			return i;

	return -1;
}

// ��������� ���� � ������� ���
void CWorld::AddBullet(screate_bullet *b)
{
	CBullet *bul=new CBullet(*b);
	PlaySound(b->type,b->x,b->y,true);
	bullets.push_back(bul);
}

// ������� "�������" ����
void CWorld::KillDeadBullets(void)
{
	vector<CBullet*>::iterator it;
	for (it=bullets.begin();it!=bullets.end();it++)
		if (!(*it)->IsAlive())
		{
			delete (*it);
			bullets.erase(it);
			it--;
		}
}

// ���������� ����� �� ������ � �������� id
CPlayer * CWorld::GetPlayer(int id)
{
	int idx=FindPlayer(id);
	return players[idx];
}

// ��������� ����� � ������� ���
void CWorld::AddExplode(screate_explode* e)
{
	CExplode *expl;
	if (e->type==BGA_EXPLODE_NORMAL)
	{
		e->sprite=CExplode::spr_explode;
		e->scale=1.5;
		PlaySound(BGA_SND_EXPLODE,e->x,e->y,true);
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_SMOKE)
	{
		e->sprite=CSmoke::spr_smoke;
		expl=new CSmoke(*e);
	}
	else if(e->type==BGA_EXPLODE_BONUS)
	{
		e->sprite=CItem::spr_item_place;
		expl=new CBonusExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_WEAP1)
	{
		e->sprite=CExplode::spr_w1_explode;
		e->scale=0;
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_WEAP2)
	{
		e->sprite=CExplode::spr_w2_explode;
		e->scale=0.1;
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_WEAP3)
	{
		e->sprite=CExplode::spr_w3_explode;
		e->scale=0.1;
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_WEAP4)
	{
		e->sprite=CExplode::spr_w4_explode;
		e->scale=0.3;
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_WEAP5)
	{
		e->sprite=CExplode::spr_w5_explode;
		e->scale=0.3;
		expl=new CExplode(*e);
	}
	else if(e->type==BGA_EXPLODE_GREEN_SMOKE)
	{
		e->sprite=CSmoke::spr_green_smoke;
		expl=new CSmoke(*e);
	}
	else if(e->type==BGA_EXPLODE_BLUE_RING)
	{
		e->sprite=CSmoke::spr_blue_ring;
		expl=new CSmoke(*e);
	}
	else if (e->type==BGA_EXPLODE_RESPAWN)
	{
		e->sprite=CExplode::spr_respawn_explode;
		e->scale=1.5;
		expl=new CExplode(*e);
	}
	explodes.push_back(expl);

}

// ������� "�������" ������
void CWorld::KillDeadExplodes(void)
{
	vector<CExplode*>::iterator it;
	for (it=explodes.begin();it!=explodes.end();it++)
		if (!(*it)->IsAlive())
		{
			delete (*it);
			explodes.erase(it);
			it--;
		}
}

// ��������� ����� � ������� ���
void CWorld::AddItem(screate_item *i)
{
	i->id=last_id++;
	CItem *item=new CItem(*i);
	items.push_back(item);

	if (i->type==BGA_ITEM_SPCUSTOM)//��������� �����
	{
		start_points[sp_count]=i->id;
		sp_count++;
	}
}

// ���������� ��������� �� ����� � �������� id
CItem* CWorld::GetItem(int id)
{
	for (int i=0;i<items.size();i++)
		if (items[i]->GetId()==id)
			return items[i];

}

// �������� ��������� � ��������������� �����
void CWorld::PlaySound(int id, int x, int y, bool local)
{
	splay_sound *ps=new splay_sound;
	ps->id=id;
	ps->x=x;
	ps->y=y;

	if (local)
		game->AddMqMessage(BGA_MSG_PLAY_SOUND,ps);
	else
		game->SendNetMessage(BGA_MSG_PLAY_SOUND,ps);
}
