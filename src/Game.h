#pragma once
#include ".\main.h"
#include ".\messagestack.h"
#include ".\world.h"
#include ".\controls.h"
#include ".\weapon.h"
#include ".\smoke.h"
#include ".\fraginfo.h"
#include ".\gameflow.h"


#define BGA_SERVER_PORT "70000"
#define BGA_MESSAGE "bga_message"
#define BGA_NET_REFRESH_TIME 200

class CGame
{
	string pkname;//��� ����� ��� ����������� ������
	string pak0;//��� ����� ��� ����������� ������
	CMessageQueue MQ;//������� ���������
	CL_NetSession *NS;//������� ������
	//CL_Socket * //����� ����� ��������� �������
	unsigned int comp_id;//id �����

	GCL_Keyboard game_kb; //����������
	GCL_Mouse game_mouse; //����
	CL_Zip_Archive *PakFile;//���� ������
	CL_ResourceManager *resman;

	CL_Slot slot_message_receive;
	CL_Slot slot_client_connected;
	bool is_server;//�-������!
	bool list_received;//������ ������� ������� ��������
	CL_NetComputer server_comp;//��� �������:���� �������
	unsigned int last_net_upd;//��������� ���������� �����

	CWorld *game_world;//��� ������� ����
	vector<CPlayer*> *players;//��������� �� ���� �������

	int local_players;//���-�� ������� �� ���� �����
	string level_name;//�������� ������
	int controls[4][BGA_CTRL_COUNT];//������� ����������

	CFragInfo *frag_info;//������ ������
public:
	sweapon_info weapons[5];//������
	CGame(void);
	~CGame(void);
	// ����������� � ��������� ���_�� �������
	CGame(int np);
	// �������� ���� ����
	void MainLoop(void);
	// ��������� ��������� ����
	void ProcessMessages(void);
	// ��������� ���������� ��� ������� �������
	void LoadControls(void);
private:
	// ������� ���� ��������� �������
	void CreateLocalPlayers(void);

	string local_names[4];//����� ���. �������
	int local_commands[4];//������� ���. �������

public:
	// �������� ������
	int RunServer(void);
	// �������� ������
	int RunClient(void);
	// �������, ���������� ��� ��������� ��������� �� ����
	void on_message_receive(CL_NetPacket & packet, CL_NetComputer & computer);
	// �������������� ����, ��������� �����������
	int InitNetwork(void);
	// ������ ����������� � �������
	void on_client_connected(CL_NetComputer& comp);
	// ���������� ��������� �� ����
	void SendNetMessage(int type, void* info, CL_NetComputer* comp=NULL);
	// ������� ���������� ������
	void CreateRemotePlayer(screate_player *pl);

	// ���������� ��������� �� �������� ��������
	CL_ResourceManager * GetResman(void)
	{
		return resman;
	}
private:
	// ��������� �������������� ������
	void LoadWeapons(void);
public:

	// ���������� ��������� �� ����� � ���������
	CL_InputSourceProvider * GetPack(void)
	{
		return PakFile;
	}
	// ��������� ������� ��� ����������
	void LoadInterface(void);
	// ��������� �������
	void LoadParticles(void);

	// ��������� ��������� � �������
	void AddMqMessage(int type, void* info)
	{
		MQ.AddMessage(type,info);
	}
	// ���������� ������������ ����� ����� ��� ���������� ������
	int GetMaxEnemyFrags(int id);
private:

	//���������� ���� ����
	int game_type;//��� ���� (�������, ��� �������...)
	int frag_limit;//����� ������
	int time_limit;//����� �������
	int red_frags;//����� ������� �������
	int blue_frags;//����� ����� �������
	unsigned int gamebegin_time;//����� ������ ����
	bool end_game;//���� ����� ����
public:

	// ������������� ����������� ��� ����
	void SetLimits(int frag, int time)
	{
		frag_limit=frag;
		time_limit=time;
	}

	// ���������� ����������� ��� ����
	void GetLimits(int& frag, int& time)
	{
		frag=frag_limit;
		time=time_limit;
	}
	// ���������� ����� �� ����� ���� � �������
	int GetRemainTime(void)
	{
		return time_limit-(CL_System::get_time()-gamebegin_time)/60000;
	}
	// ���������� ������ ������� ���, �������������� ����������, ������� ����� ���
	void NewGame(int gametype, int lplayers, string level_name);
	// �������/��������� ���� �����
	void IngameMenu(void);
private:
	// �������� �������
	void LoadPowerups(void);
public:
	// ������������� ����� ��������� �������
	void SetLocalNames(string name1, string name2, string name3, string name4);
	// ������������� ������� ��������� �������
	void SetLocalCommands(int cmd1, int cmd2, int cmd3, int cmd4);

	// ���������� ��� ����
	int GetGameType(void)
	{
		return game_type;
	}
private:
	// ��������� ����� ������
	void UpdateTeamFrags(void);
public:

	// ���������� �����/����� ������� �������
	int GetRedScore(void)
	{
		return red_frags;
	}
	// ���������� �����/����� ����� �������
	int GetBlueScore(void)
	{
		return blue_frags;
	}
};
