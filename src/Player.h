#pragma once
#include ".\camera.h"
#include ".\controls.h"
#include ".\gamemap.h"
//#include ".\world.h"
#include ".\weapon.h"

class CWorld;
class CGame;

#define BGA_MAX_SPEED 200

//��������� ��� �������� ������
struct screate_player
{
	int id;//��� ������
	int team;//������� ������
	int x,y,angle;
	bool local;
	int cam_type;//���� ���������, �� ������ ��� ������;
				 //���� ���, �� CT_NOCAMERA
	char spr_id[20];//������������� �������
	char name[20];
	CL_ResourceManager* resman;//��������� �� �������� ��������
	int *controls;//��������� �� ������ ������
	int input_type;//��� �����
	GCL_Keyboard * kb;//��������� �� ����������
	CGameMap *game_map;
	CWorld* world;
	CGame *game;
	int max_frags;//����������� ��������� �����
	int frags;//�����
};

struct splayer_features
{
	union
	{
		struct
		{
			int haste;
			int freeze;
			int quaddamage;
			int invisibility;
			int mine;
			int regeneration;
		};
		int all[6];//������ �� �������
	};
	
	splayer_features()
	{
		haste=0;
		freeze=0;
		quaddamage=0;
		invisibility=0;
		mine=0;
		regeneration=0;
	}
};

//��������� ��� ���������� ������
struct supdate_player
{
	int x,y,angle;
	double ux,uy;//��������
	double vx,vy;//���������
	int id;//��� ������
	splayer_features features;//��������� ������
};

struct supdate_frags
{
	int id;//id ������
	int target_id;//���� ����
	int weapon;//�� ����� �����
	int dfrags;//��������� ��� ������ (1 ��� -1)
	int max_frags;//����. �����
};

class CPlayer
{
	friend CCamera;
	friend CWeapon;

	static int last_id;//��� �������� �������
	int id;//��� ������
	int team;//������� ������

	int life;//�����
	int armor;//�����
	int frags;//�����
	splayer_features features;//��������� ������
	bool no_weapon;//��� ������� ��������
	CL_SoundBuffer_Session *snd_engine;
public:
	static int max_frags;//����������� ��������� �����
private:
	string name;
	string spr_id;
	int x,y,angle;
	double pdx,pdy;//����� ������ ����������
	double vx,vy;//���������
	double ux,uy;//��������
	double ux2, uy2;//�������� ��� ���������
	bool reverse;//������ ���
	bool aimed;//���������

	//���� �����
	CL_Point cr_lt;//����� �������
	CL_Point cr_lb;//����� ������
	CL_Point cr_rt;//������ �������
	CL_Point cr_rb;//������ ������
	CL_Point cr_ct;//����������� ��������
	CL_Point cr_cb;//����������� ������
	CL_Point cr_c;//�����
	CL_Point cr_t;//����� ����� �������
	CL_Point cr_target;//������

	int clt,crt,clb,crb,cct,ccb;//��������� ���������
	CL_Point normal_lt,normal_rt,normal_lb,normal_rb;//������� � "���������" ������

	//�������������� ��� ������ �����
	double max_speed;//������������ ��������
	double thrust;//���������

	int cur_weapon;//��������� �����


	int htick;//����� ���������� ���������� ���������� ������
	int atick;//����� ���������� ���������� ���������� �����
	int ftick;//����� ���������� ���������� ���� "���"

	CCamera *cam;
	CWeapon *weapons[5];//5 �����
	CGameMap *game_map;
	CWorld *world;
	CGame *game;

	CL_Sprite *spr_car;
	CL_ResourceManager *resman;
	CL_Font *fnt_interface;
	bool local;//��������� ������
	int* controls;//[BGA_CTRL_COUNT];//��������� �� ������ ������
	int input_type;//��� �����
	GCL_Keyboard * kb;//��������� �� ����������
public:
	enum einput_type {IT_1,IT_2,IT_3,IT_4,IT_NETWORK};
	CPlayer(screate_player pl_info);
	~CPlayer(void);
	// ���������� X-���������� ������
	int GetX(void);
	// ���������� Y-���������� ������
	int GetY(void);
	// ���������� true, ���� ����� ���������
	bool IsLocal(void);
	// ���������� ��������� �� ������ ������
	CCamera* GetCam(void);
	// ����������� �����������, ��� �������. �����. ������
	//CPlayer(const CPlayer& pl);
	// ������ ������ � �������� �����������
	void Draw(int ux, int uy);
	// ��������� ������ � ���������� ���������
	void Input(void);
	// ��������� ��������� ������
	void Update(void);
	// ���������� ���������� �� ������ ��� ������ ��������������� ������
	screate_player GetInfo(void);
	// ���������� ���������� � ������� � ��������� ������
	supdate_player GetUpdateInfo(void);

	// ���������� ������������� ������
	int GetId(void)
	{
		return id;
	}
	// �������� ��������� � ��������� ������
	void Modify(supdate_player pl);

	// ���������� ��������� �������������
	static int CPlayer::GetLastId(void)
	{
		return last_id;
	}
	// ��������� ������������ ������, �� ����������� ������ �������
	void UpdateRemote(void);
private:
	// ���������� 1-������ ���, 0-��������, -1 - ����� ����������� ������
	int IsReverse(void);
	// ���������� ���������� �����, ��������� �� �������� ������
	CL_Point GetCorner(int dx, int dy);
public:
	// ���������� �������������, ��� ��������� � �������  ���� ����������
	CL_Rect GetHitRect(void);
private:
	// ���������� ����� ������ ����� �����
	CL_Point GetLDCorner(void);
	// ���������� ������ ������� ����� �����
	CL_Point GetRUCorner(void);
public:
	// ��������� ����� ������ �� �����. ���-�� �����
	void DecLife(int dlife, int shooter, int wtype);
	// �������� ������ � ����� �����
	void Respawn(void);

	// ���������� ����� ������
	int GetFrags(void)
	{
		return frags;
	}

	// ���������� ����������� ��������� �����
	static int GetMaxFrags(void)
	{
		return max_frags;
	}

	// ������������� ����� ������
	void SetFrags(int f)
	{
		frags=f;
	}
	// ������������ ����� �� ������ ����
	void Turn(int a);
private:
	// ���������� ������ �� ����������� �����
	void FindAim(void);
public:

	// ���������� ����������� ����� �����
	CL_Point GetCenter(void)
	{
		cr_c=GetCorner(32,32);
		return cr_c;
	}
private:
	// ��������� ��� �� �����
	void AddSmoke(CL_Point tube);
public:
	// ���� ������ ������ �����, ���������� ��� � ������� ����
	void CheckTarget(void);

	// ���������� ��� ������
	string GetName(void)
	{
		return name;
	}
private:
	// ��������� ��������� ���������� ���������� ������������
	void UpdateCollision(void);
	// ��� �����, ���� ����� �� ���
	void EatBonus(int x, int y);
public:
	// ��������� ������ ��������� (special state): ����� � ����� >100, ��������� ���������, etc.
	void UpdateSS(void);
	// �������� ��������� ����� � ������� ���� �������, ���������� �� �����
	int NextWeapon(void);

	// ���������� true, ���� ����� �������� �����-�� �����
	bool HasFeature(int type)
	{
		return (bool)features.all[type];
	}

	// ���������� ������� ������
	int GetTeam(void)
	{
		return team;
	}
};
