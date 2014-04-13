#pragma once
#include ".\main.h"
#include ".\game.h"

class CGameMenu
{
	//������ ������ � �������
	int btn_game;
	int btn_help;
	int btn_about;
	int btn_exit;
	int btn_options;
	int btn_bglist;
	int btn_help_back;
	int btn_about_back;
	int btn_options_back;

	int btn_game_back;
	int btn_game_server;
	int btn_game_client;

	int btn_gametype_back;
	int btn_gametype_deathmatch;
	int btn_gametype_teamdeathmatch;
	int btn_gametype_next;

	int btn_gameparams_back;
	int btn_gameparams_next;
	int ebx_gameparams_localgamers;
	int ebx_gameparams_name1;
	int ebx_gameparams_name2;
	int ebx_gameparams_name3;
	int ebx_gameparams_name4;
	int btn_gameparams_cmd1;
	int btn_gameparams_cmd2;
	int btn_gameparams_cmd3;
	int btn_gameparams_cmd4;
	int ebx_gameparams_fraglimit;
	int ebx_gameparams_timelimit;

	int page_main;
	int page_about;
	int page_help;
	int page_game;
	int page_gametype;
	int page_gameparams;

	bool quit;//���� ������

	GCL_Menu * MainMenu;
	GCL_MenuRedrawContext context_main;
	GCL_MenuRedrawContext context_about;
	GCL_MenuRedrawContext context_null;
	GCL_MenuRedrawContext *context_current;
	CGame *game;//��������� �� ����
	CL_Sprite *pointer;
	CL_Sprite *title;
	CL_Sprite *darktitle;
	CL_Sprite *devby;
	CL_Sprite *copyright;
	CL_Font *fnt_about;
	string *about_str;
	void (CGameMenu::*curhandler)();//��������� �� ������� ����������� ����
	void MenuMain();
	void MenuAbout();

	//��� "� ���������"
	int y,r,g,b,ch,dif;

	int game_type;
	int team1,team2,team3,team4;

	CL_SoundBuffer_Session *menu_theme;
public:
	CGameMenu(CGame* _game);
	~CGameMenu(void);
	// ������� ����
	void Draw(void);
	// ���������� ���� ��������� ��������� ���� � �������� ��� ������
	void Update(GCL_Mouse mouse, GCL_Keyboard kb);
private:
	// ��������� ������� ������ � ��������� ��������������� ��������
	void Navigate(void);
public:
	// ������� ���� ����
	void MainLoop(void);
	// �������� ����
	void Intro(void);
};
