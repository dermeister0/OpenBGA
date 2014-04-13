#include ".\game.h"

// �������� ������
int CGame::RunServer(void)
{
	try
	{
		//� ������� ������ id==0
		comp_id=0;
		is_server=true;
		NS=new CL_NetSession("BGA");
		NS->start_listen(BGA_SERVER_PORT);

		slot_message_receive=NS->sig_netpacket_receive(BGA_MESSAGE).connect(this,&CGame::on_message_receive);
		slot_client_connected=NS->sig_computer_connected().connect(this,&CGame::on_client_connected);
	}
	catch (CL_Error err)
	{
		return 0;
	}
}

// �������� ������
int CGame::RunClient(void)
{
	try
	{
		//�������� ���������� id �������
		comp_id=rand()%2000;
		is_server=false;
		NS=new CL_NetSession("BGA");

		CL_IPAddress adr("localhost",BGA_SERVER_PORT);
		server_comp=NS->connect(adr);
		list_received=false;

		slot_message_receive=NS->sig_netpacket_receive(BGA_MESSAGE).connect(this,&CGame::on_message_receive);

		//�������� ����
		while (!list_received)
		{
			CL_System::keep_alive();
		}

		//������� �������
		ProcessMessages();
	}
	catch (CL_Error)
	{
		return 0;
	}
}

// �������, ���������� ��� ��������� ��������� �� ����
void CGame::on_message_receive(CL_NetPacket & packet, CL_NetComputer & computer)
{
	if (is_server)
		NS->get_all().send(BGA_MESSAGE,packet);
	
	int type=packet.input.read_int32();
	//id ����������� ���������
	int id=packet.input.read_int32();

	screate_player *pl=NULL;//����� �����
	sworld_info *w=NULL;//����� ������� ���
	supdate_player *upl=NULL;//�������� �����
	screate_bullet *b=NULL;//����� ����
	screate_explode *e=NULL;//����� �����
	supdate_frags *fr=NULL;//��������� �����
	splay_sound *ps=NULL;//������������� ����

	//��������� ���� �
	if (id==comp_id)
		return;

	switch(type)
	{
	case BGA_MSG_EXIST_REMOTE_PLAYER:
	case BGA_MSG_NEW_REMOTE_PLAYER:
		pl=new screate_player;
		packet.input.read(pl,sizeof(screate_player));
		CreateRemotePlayer(pl);
		break;
	case BGA_MSG_WORLD_INFO:
		w=new sworld_info;
		packet.input.read(w,sizeof(sworld_info));
		MQ.AddMessage(BGA_MSG_WORLD_INFO,w);
		break;
	case BGA_MSG_END_OF_PLAYERS_LIST:
		list_received=true;
		break;
	case BGA_MSG_UPDATE_PLAYER:
		upl=new supdate_player;
		packet.input.read(upl,sizeof(supdate_player));
		MQ.AddMessage(BGA_MSG_UPDATE_PLAYER, upl);
		break;
	case BGA_MSG_NEW_BULLET:
		b=new screate_bullet;
		packet.input.read(b,sizeof(screate_bullet));
		MQ.AddMessage(BGA_MSG_NEW_BULLET, b);
		break;
	//�� ����� ���������� ������ ������ ������
	case BGA_MSG_NEW_EXPLODE:
		e=new screate_explode;
		packet.input.read(e,sizeof(screate_explode));
		MQ.AddMessage(BGA_MSG_NEW_EXPLODE, e);
		break;
	case BGA_MSG_UPDATE_FRAGS:
		fr=new supdate_frags;
		packet.input.read(fr,sizeof(supdate_frags));
		MQ.AddMessage(BGA_MSG_UPDATE_FRAGS, fr);
		break;
	case BGA_MSG_PLAY_SOUND:
		ps=new splay_sound;
		packet.input.read(fr,sizeof(splay_sound));
		MQ.AddMessage(BGA_MSG_PLAY_SOUND, ps);
		break;
	}
}

// �������������� ����, ��������� �����������
int CGame::InitNetwork(void)
{
	last_net_upd=0;
	if (!RunServer())
	{
		int result=RunClient();
		return result;
	}
	else
		return 1;
}

// ������ ����������� � �������
void CGame::on_client_connected(CL_NetComputer& comp)
{
	screate_player *pl;
	
	sworld_info *info=new sworld_info;
	*info=game_world->GetWorldInfo();
	SendNetMessage(BGA_MSG_WORLD_INFO, info, &comp);

	for (int i=0;i<players->size();i++)
	{
		pl=new screate_player;
		*pl=(*players)[i]->GetInfo();
		SendNetMessage(BGA_MSG_EXIST_REMOTE_PLAYER,pl,&comp);
	}
	SendNetMessage(BGA_MSG_END_OF_PLAYERS_LIST,NULL,&comp);
}

// ���������� ��������� �� ����
void CGame::SendNetMessage(int type, void* info, CL_NetComputer* comp)
{
	CL_NetPacket packet;
	packet.output.write_int32(type);
	//�������� id �����������
	packet.output.write_int32(comp_id);

	screate_player *pl=NULL;
	sworld_info *w=NULL;
	supdate_player *upl=NULL;//�������� �����
	screate_bullet *b=NULL;//����� ����
	screate_explode *e=NULL;//����� �����
	supdate_frags *fr=NULL;//��������� �����
	splay_sound *ps=NULL;//������������� ����

	switch(type)
	{
	case BGA_MSG_EXIST_REMOTE_PLAYER:
		pl=(screate_player *)info;
		packet.output.write(pl,sizeof(screate_player));
		delete pl;
		break;
	case BGA_MSG_NEW_REMOTE_PLAYER:
		pl=(screate_player *)info;
		packet.output.write(pl,sizeof(screate_player));
		//������ pl � ������� CreateLocalPlayers
		break;
	case BGA_MSG_WORLD_INFO:
		w=(sworld_info *)info;
		packet.output.write(w,sizeof(sworld_info));
		delete w;
		break;
	case BGA_MSG_UPDATE_PLAYER:
		upl=(supdate_player *)info;
		packet.output.write(upl,sizeof(supdate_player));
		delete upl;
		break;
	case BGA_MSG_NEW_BULLET:
		b=(screate_bullet *)info;
		packet.output.write(b,sizeof(screate_bullet));
		MQ.AddMessage(BGA_MSG_NEW_BULLET, b);
		//b ������� � ������� ���������
		break;
	//�� ����� ���������� ������ ������ ������
	case BGA_MSG_NEW_EXPLODE:
		e=(screate_explode *)info;
		packet.output.write(e,sizeof(screate_explode));
		MQ.AddMessage(BGA_MSG_NEW_EXPLODE, e);
		//e ������� � ������� ���������
		break;
	case BGA_MSG_UPDATE_FRAGS:
		fr=(supdate_frags *)info;
		packet.output.write(fr,sizeof(supdate_frags));
		MQ.AddMessage(BGA_MSG_UPDATE_FRAGS, fr);
		break;
	case BGA_MSG_PLAY_SOUND:
		ps=(splay_sound *)info;
		packet.output.write(ps,sizeof(splay_sound));
		MQ.AddMessage(BGA_MSG_PLAY_SOUND, ps);
		break;
	}

	if (is_server)
	{
		//���� ����
		if (!comp)
			NS->get_all().send(BGA_MESSAGE,packet);
		else
			comp->send(BGA_MESSAGE,packet);
	}
	else
		server_comp.send(BGA_MESSAGE,packet);
}