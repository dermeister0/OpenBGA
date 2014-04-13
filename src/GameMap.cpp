#include ".\gamemap.h"
#include ".\main.h"
#include ".\game.h"

#define BGA_TILES_FILE "data/main/tiles/tiles.cfg"

CGameMap::CGameMap(CWorld *w, CGame *g)
{
	world=w;
	game=g;
	spr_tiles=new CL_Sprite("gfx/game/tiles",game->GetResman());

	//CL_InputSourceProvider *pak=g->GetPack();
	//CL_Zip_Archive *pak=(CL_Zip_Archive *)g->GetPack();
	CL_InputSource_File *file=new CL_InputSource_File(BGA_TILES_FILE);

	for (int i=0;i<168;i++)
	{
		file->read(&tile_types[i],sizeof(tile_types[i]));
		//tile_types[i]=TT_OTHER;
	}
	file->close();
	delete file;
}

CGameMap::~CGameMap(void)
{
	delete spr_tiles;
}

// загружает карту с заданным именем из файла
bool CGameMap::LoadMap(string name)
{
	LOG<<"«агрузка карты "<<name<<"\n";
	map_name=name;

	string map_file_name="data/main/maps/"+name+"/level.map";
	string level_file_name="data/main/maps/"+name+"/";

	switch (game->GetGameType())
	{
	case BGA_GAME_DEATHMATCH:
		level_file_name+="dm.cfg";
		break;
	case BGA_GAME_TEAMDEATHMATCH:
		level_file_name+="tdm.cfg";
		break;
	case BGA_GAME_CAPTURETHEFLAG:
		level_file_name+="ctf.cfg";
		break;
	}

	CL_InputSource_File *file=new CL_InputSource_File(map_file_name);
	CL_InputSource_File *lev_file=new CL_InputSource_File(level_file_name);

	//int y=file->size();

	try
	{
		file->read_string();//"BGA map file"
		mh.map_name=file->read_string();
		LOG<<" арта: "<<mh.map_name;

		mh.map_author=file->read_string();
		LOG<<" јвтор: "<<mh.map_author<<endl;

		mwidth=file->read_int32();
		mheight=file->read_int32();

		msize=mwidth*mheight;

		for (int i=0;i<msize;i++)
		{
			map_info temp;
			temp.tile=file->read_int32();
			temp.angle=file->read_int32();
			temp.shadow=file->read_int32();
			temp.item=lev_file->read_int32();
			map.push_back(temp);
			
			if (temp.item>=0)//есть бонус
			{	
				screate_item it;
				it.x=i/mwidth*64;
				it.y=i%mwidth*64;
				it.type=temp.item;
				
				int type=it.type;
				if (type==BGA_ITEM_ARMOR100 || type==BGA_ITEM_HEALTH100)
					it.reload_time=1000;
				else if (type==BGA_ITEM_QUADDAMAGE || type==BGA_ITEM_INVISIBILITY || type==BGA_ITEM_REGENERATION)
					it.reload_time=2500;
				else if (type==BGA_ITEM_HASTE || type==BGA_ITEM_FREEZE || type==BGA_ITEM_MINE)
					it.reload_time=1250;
				else
					it.reload_time=500;

				world->AddItem(&it);
			}
		}
	}
	catch (...)
	{
		return false;
	}

	file->close();
	lev_file->close();

	delete file;
	delete lev_file;

	return true;
}

// рисует карту с заданным смещеним в нужных координатах экрана
void CGameMap::Draw(int dx, int dy, CL_Rect cam,bool top)
{
	int dxt=dx/64;//переводим пикселы в тайлы
	int dyt=dy/64;//переводим пикселы в тайлы

	int x=cam.left-dx%64;//чтобы не полностью поместившиес€ тайлы
	int y=cam.top-dy%64;//тоже отрисовались

	int ht=cam.get_width()/64+2;//кол-во гор. тайлов
	int vt=cam.get_height()/64+2;//кол-во верт. тайлов

	//рисуем карту
	for (int i=dxt;i<ht+dxt;i++)
		for (int j=dyt;j<vt+dyt;j++)
		{
			if (i<0 || j<0 || i>=mwidth || j>=mheight)
				continue;

			if (!top)//рисуем нижний уровень
			{
				if (!(tile_types[map[(i)*mwidth+(j)].tile].mg==TT_ROOF))//дома - не нижний уровень
				{
					spr_tiles->set_alpha(1);
					spr_tiles->set_angle(map[(i)*mwidth+(j)].angle*90);
					spr_tiles->set_frame(map[(i)*mwidth+(j)].tile);
					spr_tiles->draw(x+(i-dxt)*64,y+(j-dyt)*64);		
				}
			}
			else 
			{
				bool draw=false;
				spr_tiles->set_alpha(0.7);
				spr_tiles->set_angle(map[(i)*mwidth+(j)].angle*90);

				if ((tile_types[map[(i)*mwidth+(j)].tile].mg==TT_ROOF))
				{
					spr_tiles->set_alpha(1);
					spr_tiles->set_frame(map[(i)*mwidth+(j)].tile);
					draw=true;
				}
				if (map[(i)*mwidth+(j)].tile==83) //деревь€
				{
					spr_tiles->set_frame(168);				
					draw=true;
				}
				if (map[(i)*mwidth+(j)].tile==106)//деревь€
				{
					spr_tiles->set_frame(169);
					draw=true;
				}

				if (draw)
					spr_tiles->draw(x+(i-dxt)*64,y+(j-dyt)*64);

				//рисуем тень
				if (map[(i)*mwidth+(j)].shadow)
				{
					if (!(tile_types[map[(i)*mwidth+(j)].tile].mg==TT_ROOF))//на домах тени нет
					{
						spr_tiles->set_angle(0);
						spr_tiles->set_frame(map[(i)*mwidth+(j)].shadow-1+170);
						spr_tiles->draw(x+(i-dxt)*64,y+(j-dyt)*64);					
					}
				}
			}
		}
}


// возвращает true, если тайл по данным координатам - стена
int CGameMap::IsWall(int x, int y, CL_Point &normal)
{
	if (x>=mwidth*64-64 || y>=mheight*64-64 || x<0 || y<0)
		return true;
	
	int tx=x/64;
	int ty=y/64;

	int dx=x%64;
	int dy=y%64;

	int dl=dx;
	int dr=64-dx;
	int dt=dy;
	int db=64-dy;
	int result=CMisc::MinNoZero(dl,dr,dt,db);

	normal.x=normal.y=0;

	int tile=map[(tx)*mwidth+(ty)].tile;
	
	if (tile_types[tile].mg==TT_ROOF)
	{
		if (result==dl)
		{
			normal.x=-10;
			return W_LEFT;
		}

		if (result==dr)
		{
			normal.x=10;
			return W_RIGHT;
		}

		if (result==dt)
		{
			normal.y=10;
			return W_TOP;
		}

		if (result==db)
		{
			normal.y=-10;
			return W_BOTTOM;
		}
	}

	return W_NONE;
}

// провер€ет столкновени€ с бонусов; возвращает id бонуса или -1
int CGameMap::ItemCollided(int x, int y)
{
	int i;
	CItem *item;
	for (i=0;i<world->items.size();i++)
	{
		item=world->items[i];
		int t=item->GetId();
		if (item->GetRect().is_inside(CL_Point(x,y)))//вз€ли бонус
		{
			return item->GetId();
		}
	}
	
	return -1;
}
