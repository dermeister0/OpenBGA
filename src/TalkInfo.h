#pragma once
#include ".\main.h"

struct stalk_info
{
	string author;//автор
	string message;//сообщение

	stalk_info(string _author,string _message)
	{
		author=_author;
		message=_message;
		life=512;
	}
	int life;
};

class CTalkInfo
{
	vector <stalk_info> messages;
public:
	CTalkInfo(void);
	~CTalkInfo(void);
	// добавляет сообщение
	void Add(stalk_info nfo);
	// Обновляет панель сообщений
	void Update(void);
	// рисует панель сообщений в заданном месте
	void Draw(int x, int y);
};
