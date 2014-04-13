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
	string pkname;//имя файла для расшифровки архива
	string pak0;//имя файла для расшифровки архива
	CMessageQueue MQ;//очередь сообщений
	CL_NetSession *NS;//сетевая сессия
	//CL_Socket * //здесь будет автопоиск сервака
	unsigned int comp_id;//id компа

	GCL_Keyboard game_kb; //клавиатура
	GCL_Mouse game_mouse; //мышь
	CL_Zip_Archive *PakFile;//файл архива
	CL_ResourceManager *resman;

	CL_Slot slot_message_receive;
	CL_Slot slot_client_connected;
	bool is_server;//я-сервак!
	bool list_received;//список игроков получен клиентом
	CL_NetComputer server_comp;//для клиента:комп сервака
	unsigned int last_net_upd;//последнее обновление сетки

	CWorld *game_world;//все объекты игры
	vector<CPlayer*> *players;//указатель на всех игроков

	int local_players;//кол-во игроков на этом компе
	string level_name;//название уровня
	int controls[4][BGA_CTRL_COUNT];//клавиши управления

	CFragInfo *frag_info;//панель фрагов
public:
	sweapon_info weapons[5];//оружие
	CGame(void);
	~CGame(void);
	// конструктор с указанием кол_ва игроков
	CGame(int np);
	// основной цикл игры
	void MainLoop(void);
	// обработка сообщений игры
	void ProcessMessages(void);
	// загружает управление для каждого геймера
	void LoadControls(void);
private:
	// создает всех локальных игроков
	void CreateLocalPlayers(void);

	string local_names[4];//имена лок. игроков
	int local_commands[4];//команды лок. игроков

public:
	// стартует сервер
	int RunServer(void);
	// стартует клиент
	int RunClient(void);
	// функция, вызываемая при получении сообщения по сети
	void on_message_receive(CL_NetPacket & packet, CL_NetComputer & computer);
	// инициализирует сеть, выполняет подключение
	int InitNetwork(void);
	// клиент подключился к серваку
	void on_client_connected(CL_NetComputer& comp);
	// отправляет сообщение по сети
	void SendNetMessage(int type, void* info, CL_NetComputer* comp=NULL);
	// создает удаленного игрока
	void CreateRemotePlayer(screate_player *pl);

	// возвращает указатель на менеджер ресурсов
	CL_ResourceManager * GetResman(void)
	{
		return resman;
	}
private:
	// загружает характеристики оружия
	void LoadWeapons(void);
public:

	// возвращает указатель на архив с ресурсами
	CL_InputSourceProvider * GetPack(void)
	{
		return PakFile;
	}
	// загружает ресурсы для интерфейса
	void LoadInterface(void);
	// загружает частицы
	void LoadParticles(void);

	// добавляет сообщение в очередь
	void AddMqMessage(int type, void* info)
	{
		MQ.AddMessage(type,info);
	}
	// возвращает максимальные чужие фраги для указанного игрока
	int GetMaxEnemyFrags(int id);
private:

	//ПЕРЕМЕННЫЕ ХОДА ИГРЫ
	int game_type;//тип игры (десматч, тим десматч...)
	int frag_limit;//лимит фрагов
	int time_limit;//лимит времени
	int red_frags;//фраги красной команды
	int blue_frags;//фраги синей команды
	unsigned int gamebegin_time;//время начала игры
	bool end_game;//флаг конца игры
public:

	// устанавливает ограничения для игры
	void SetLimits(int frag, int time)
	{
		frag_limit=frag;
		time_limit=time;
	}

	// возвращает ограничения для игры
	void GetLimits(int& frag, int& time)
	{
		frag=frag_limit;
		time=time_limit;
	}
	// возвращает время до конца игры в минутах
	int GetRemainTime(void)
	{
		return time_limit-(CL_System::get_time()-gamebegin_time)/60000;
	}
	// уничтожает старый игровой мир, инициализирует переменные, создает новый мир
	void NewGame(int gametype, int lplayers, string level_name);
	// выводит/обновляет меню паузы
	void IngameMenu(void);
private:
	// загрузка бонусов
	void LoadPowerups(void);
public:
	// устанавливает имена локальных игроков
	void SetLocalNames(string name1, string name2, string name3, string name4);
	// устанавливает команды локальных игроков
	void SetLocalCommands(int cmd1, int cmd2, int cmd3, int cmd4);

	// возвращает тип игры
	int GetGameType(void)
	{
		return game_type;
	}
private:
	// обновляет фраги команд
	void UpdateTeamFrags(void);
public:

	// возвращает фраги/флаги красной команды
	int GetRedScore(void)
	{
		return red_frags;
	}
	// возвращает фраги/флаги синей команды
	int GetBlueScore(void)
	{
		return blue_frags;
	}
};
