#pragma once
#include ".\player.h"
#include ".\gamemap.h"
#include ".\bullet.h"
#include ".\explode.h"
#include ".\killinfo.h"
#include ".\talkinfo.h"
#include ".\listener.h"
#include ".\item.h"
#include <vector>
using namespace std;

class CGame;

struct sworld_info
{
	char level_name[20];
};

//информация о стенах и игроках для коллижена
struct scol_info
{
	CGameMap *map;
	vector<CPlayer*> *players;
	CWorld *world;
};

class CWorld
{
	vector<CPlayer*> players;//все игроки
	vector<CBullet*> bullets;//все пули
	vector<CExplode*> explodes;//все взрывы и дым из трубы
	vector<CItem*> items;//все бонусы и особые точки
	int start_points[32];//идентификаторы стартовых точек
	int sp_count;//кол-во стартовых точек
	int last_id;//id нового бонуса

	CGameMap *Map;//карта
	CGame *game;

	friend class CGameMap;
public:
	CListener *listener;//слушатель
	CKillInfo kill_info;//панель убийств
	CTalkInfo talk_info;//панель сообщений

	CWorld(CGame* _game);
	~CWorld(void);
	// рисует игровой мир
	void Draw(void);
	// добавляет нового игрока
	void AddPlayer(screate_player pl_info);
	// обрабатывает ввод всех локальный игроков
	void Input(void);
	// обновляет игровой мир
	void Update();
	// возвращает рандомную точку появления игрока
	CL_Point GetStartPoint(void);

	// возвращает указатель на игроков
	vector<CPlayer*> * GetPlayers(void)
	{
		return &players;
	}
	// возвращает общую информация об игровом мире
	sworld_info GetWorldInfo(void);
	// стирает текущий игровой мир и создает новый
	int NewWorld(sworld_info info);
	// посылает информацию о состоянии локальных объектов
	void UpdateNetwork(void);
	// обновляет конкретного игрока
	void UpdatePlayer(supdate_player pl);
	// возвращает индекс игрока с определенным id
	int FindPlayer(int id);
	// добавляет пулю в игровой мир
	void AddBullet(screate_bullet *b);
	// удаляет "мертвые" пули
	void KillDeadBullets(void);
	// возвращает адрес на игрока с заданным id
	CPlayer * GetPlayer(int id);
	// добавляет взрыв в игровой мир
	void AddExplode(screate_explode* e);
	// удаляет "мертвые" взрывы
	void KillDeadExplodes(void);
	// добавляет бонус в игровой мир
	void AddItem(screate_item *i);
	// возвращает указатель на бонус с заданным id
	CItem* GetItem(int id);
	// посылает сообщение о воспроизведении звука
	void PlaySound(int id, int x, int y, bool local);

	// возвращает указатель на слушателя
	CListener* GetListener(void)
	{
		return listener;
	}
};
