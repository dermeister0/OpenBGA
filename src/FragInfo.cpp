#include ".\fraginfo.h"
#include ".\game.h"

CFragInfo::CFragInfo(CWorld *w, CGame * g)
{
	world=w;
	game=g;
}

CFragInfo::~CFragInfo()
{
}

// выводит информацию о фрагах
void CFragInfo::Draw(void)
{
	CL_Font *fnt=CCamera::fnt_interface;
	
	CL_Display::fill_rect(CL_Rect(150,50,650,550),CL_Color(0,0,0,100));
	fnt->set_alignment(origin_top_left);
	CMisc::ShadowText(200,80,3,fnt,"Имя");
	fnt->set_alignment(origin_top_right);
	CMisc::ShadowText(600,80,3,fnt,"Фраги");

	if (game->GetGameType()==BGA_GAME_TEAMDEATHMATCH || game->GetGameType()==BGA_GAME_CAPTURETHEFLAG)
	{
		fnt->set_alignment(origin_top_left);
		CMisc::ShadowText(420,80,3,fnt,CL_Color::red,CL_String::from_int(game->GetRedScore()));
		fnt->set_alignment(origin_top_center);
		CMisc::ShadowText(400,80,3,fnt,"|");
		fnt->set_alignment(origin_top_right);
		CMisc::ShadowText(380,80,3,fnt,CL_Color::blue,CL_String::from_int(game->GetBlueScore()));
	}

	int gt=game->GetGameType();
	CL_Color col;

	for (int i=0;i<frags.size();i++)
	{
		fnt->set_alignment(origin_top_left);
		if (gt==BGA_GAME_TEAMDEATHMATCH || gt==BGA_GAME_CAPTURETHEFLAG)
		{
			if (frags[i].command==BGA_TEAM_BLUE)
				col=CL_Color::blue;
			else
				col=CL_Color::red;
		}
		else
			col=CL_Color::white;

		CMisc::ShadowText(200,130+i*30,3,fnt,col,frags[i].name);
		fnt->set_alignment(origin_top_right);
		CMisc::ShadowText(600,130+i*30,3,fnt,col,CL_String::from_int(frags[i].frags));
	}
}

// обновляет и сортирует информацию о фрагах
void CFragInfo::Update(void)
{
	frags.clear();
	sfrag_info nfo;
	for (int i=0;i<(*world->GetPlayers()).size();i++)
	{
		nfo.name=(*world->GetPlayers())[i]->GetName();
		nfo.frags=(*world->GetPlayers())[i]->GetFrags();
		nfo.command=(*world->GetPlayers())[i]->GetTeam();
		frags.push_back(nfo);
	}

	if (frags.size()<2)
		return;

	//сортируем
	for (int i=0;i<frags.size()-1;i++)
	{
		for (int j=1;j<frags.size();j++)
		{
			if (frags[j].frags>frags[i].frags)
			{
				sfrag_info temp=frags[i];
				frags[i]=frags[j];
				frags[j]=temp;
			}
		}
	}
}
