#include ".\talkinfo.h"
#include ".\misc.h"

CTalkInfo::CTalkInfo(void)
{
}

CTalkInfo::~CTalkInfo(void)
{
}

// добавляет сообщение
void CTalkInfo::Add(stalk_info nfo)
{
	while (messages.size()>2)
		messages.erase(messages.begin());

	messages.push_back(nfo);
}

// Обновляет панель сообщений
void CTalkInfo::Update(void)
{
	if (!messages.size())
		return;

	if (messages[0].life)
		messages[0].life--;
	else
		messages.erase(messages.begin());
}

// рисует панель сообщений в заданном месте
void CTalkInfo::Draw(int x, int y)
{
	CL_Font *fnt=CMisc::fnt_small;
	for (int i=0;i<messages.size();i++)
	{
		int fx,fy;
		fx=x;
		fy=y+i*20;

		if (!messages[i].author.empty())
		{
			fnt->set_color(255,255,255);
			fnt->set_alpha(messages[i].life/255.0f);
			fnt->draw(fx+1,fy+1,messages[i].author+":");

			fnt->set_color(0,0,0);
			fnt->set_alpha(messages[i].life/255.0f);
			fnt->draw(fx,fy,messages[i].author+":");

			fx+=fnt->get_width(messages[i].author+":W");
		}

		fnt->set_color(0,0,0);
		fnt->set_alpha(messages[i].life/255.0f);
		fnt->draw(fx+1,fy+1,messages[i].message);

		fnt->set_color(255,255,255);
		fnt->set_alpha(messages[i].life/255.0f);
		fnt->draw(fx,fy,messages[i].message);

	}
}
