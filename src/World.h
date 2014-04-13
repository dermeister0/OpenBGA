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

//���������� � ������ � ������� ��� ���������
struct scol_info
{
	CGameMap *map;
	vector<CPlayer*> *players;
	CWorld *world;
};

class CWorld
{
	vector<CPlayer*> players;//��� ������
	vector<CBullet*> bullets;//��� ����
	vector<CExplode*> explodes;//��� ������ � ��� �� �����
	vector<CItem*> items;//��� ������ � ������ �����
	int start_points[32];//�������������� ��������� �����
	int sp_count;//���-�� ��������� �����
	int last_id;//id ������ ������

	CGameMap *Map;//�����
	CGame *game;

	friend class CGameMap;
public:
	CListener *listener;//���������
	CKillInfo kill_info;//������ �������
	CTalkInfo talk_info;//������ ���������

	CWorld(CGame* _game);
	~CWorld(void);
	// ������ ������� ���
	void Draw(void);
	// ��������� ������ ������
	void AddPlayer(screate_player pl_info);
	// ������������ ���� ���� ��������� �������
	void Input(void);
	// ��������� ������� ���
	void Update();
	// ���������� ��������� ����� ��������� ������
	CL_Point GetStartPoint(void);

	// ���������� ��������� �� �������
	vector<CPlayer*> * GetPlayers(void)
	{
		return &players;
	}
	// ���������� ����� ���������� �� ������� ����
	sworld_info GetWorldInfo(void);
	// ������� ������� ������� ��� � ������� �����
	int NewWorld(sworld_info info);
	// �������� ���������� � ��������� ��������� ��������
	void UpdateNetwork(void);
	// ��������� ����������� ������
	void UpdatePlayer(supdate_player pl);
	// ���������� ������ ������ � ������������ id
	int FindPlayer(int id);
	// ��������� ���� � ������� ���
	void AddBullet(screate_bullet *b);
	// ������� "�������" ����
	void KillDeadBullets(void);
	// ���������� ����� �� ������ � �������� id
	CPlayer * GetPlayer(int id);
	// ��������� ����� � ������� ���
	void AddExplode(screate_explode* e);
	// ������� "�������" ������
	void KillDeadExplodes(void);
	// ��������� ����� � ������� ���
	void AddItem(screate_item *i);
	// ���������� ��������� �� ����� � �������� id
	CItem* GetItem(int id);
	// �������� ��������� � ��������������� �����
	void PlaySound(int id, int x, int y, bool local);

	// ���������� ��������� �� ���������
	CListener* GetListener(void)
	{
		return listener;
	}
};
