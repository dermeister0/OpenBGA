#include ".\killinfo.h"
#include ".\game.h"

CKillInfo::CKillInfo()
{
	fnt_kill=CMisc::fnt_small;
}

CKillInfo::~CKillInfo(void)
{
}

// добавляет информации об убийстве
void CKillInfo::Add(skill_info nfo)
{
	while (kills.size()>2)
		kills.erase(kills.begin());

	kills.push_back(nfo);
}

// обновляет дизайн панели убийств
void CKillInfo::Update(void)
{
	if (!kills.size())
		return;

	if (kills[0].life)
		kills[0].life--;
	else
		kills.erase(kills.begin());
}

// рисует панель в абсолютныйх координатах
void CKillInfo::Draw(int x, int y)
{
	for (int i=0;i<kills.size();i++)
	{
		int fx=x-fnt_kill->get_width(kills[i].killer)-CCamera::wicons[kills[i].weapon]->get_width()/2;
		fx-=fnt_kill->get_width(kills[i].dead)+15;
		int fy=y+20*i;

		fnt_kill->set_color(0,0,0,kills[i].life);
		fnt_kill->set_alpha(kills[i].life/255.0f);
		fnt_kill->draw(fx+1,fy+1,kills[i].killer);

		if (kills[i].killer_team==BGA_TEAM_BLUE)
			fnt_kill->set_color(0,0,255,kills[i].life);
		else if (kills[i].killer_team==BGA_TEAM_RED)
			fnt_kill->set_color(255,0,0,kills[i].life);
		else
			fnt_kill->set_color(255,255,255,kills[i].life);

		fnt_kill->set_alpha(kills[i].life/255.0f);
		fnt_kill->draw(fx,fy,kills[i].killer);
		//CMisc::ShadowText(fx,fy,3,fnt_kill,kills[i].killer);
		fx+=fnt_kill->get_width(kills[i].killer)+5;

		CL_Sprite temp(*CCamera::wicons[kills[i].weapon]);
		temp.set_alpha(kills[i].life/255.0f);
		temp.set_scale(0.5,0.5);
		temp.draw(fx,fy);
		fx+=CCamera::wicons[kills[i].weapon]->get_width()/2+5;

		if (kills[i].weapon==5)//суицид
			continue;
		//CMisc::ShadowText(fx,fy,3,fnt_kill,kills[i].dead);
		fnt_kill->set_color(0,0,0,kills[i].life);
		fnt_kill->set_alpha(kills[i].life/255.0f);
		fnt_kill->draw(fx+1,fy+1,kills[i].dead);

		if (kills[i].dead_team==BGA_TEAM_BLUE)
			fnt_kill->set_color(0,0,255,kills[i].life);
		else if (kills[i].dead_team==BGA_TEAM_RED)
			fnt_kill->set_color(255,0,0,kills[i].life);
		else
			fnt_kill->set_color(255,255,255,kills[i].life);

		fnt_kill->set_alpha(kills[i].life/255.0f);
		fnt_kill->draw(fx,fy,kills[i].dead);
	}
}
