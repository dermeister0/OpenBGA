#include ".\editor.h"
#include "GCL\Keyboard\GCL_Keyboard.h"
#include "GCL\Mouse\GCL_Mouse.h"
#include ".\bitmaploader.h"

//version 0.0.305

CEditor::CEditor(void)
{
	CL_ResourceManager resman("data/main/resources.xml");//,new CL_Zip_Archive("data"),true);

	spr_tiles=new CL_Sprite("gfx/game/tiles",&resman);
	spr_items=new CL_Sprite("gfx/powerups/items",&resman);
	fnt_misc=new CL_Font("fonts/misc",&resman);

	ifstream file_tiles(BGA_TILES_FILE);

	for (int i=0;i<168;i++)
	{
		file_tiles.read((char*)&tile_types[i],sizeof(tile_types[i]));
		//tile_types[i]=TT_OTHER;
	} 

	file_tiles.close();

	if(!LoadMap("platetown"))
	{
		mwidth=32;
		mheight=32;
		msize=mwidth*mheight;

		for (int i=0;i<msize;i++)
		{
			map_info temp;
			temp.tile=8;//rand()%169;
			temp.angle=0;
			temp.shadow=0;
			temp.item=-1;
			map.push_back(temp);
		}

		mh.map_name="Замухрыжка";
		mh.map_author="Tosha";
		map_file_name="data\\main\\maps\\"+CL_String::from_int(rand()%100)+".map";
	}

}

CEditor::~CEditor(void)
{
	/*ofstream file_tiles(BGA_TILES_FILE);

	for (int i=0;i<168;i++)
		file_tiles.write((char*)&tile_types[i],sizeof(tile_types[i]));

	file_tiles.close();	*/

	delete spr_tiles;
	delete spr_items;
	delete fnt_misc;
}

void CEditor::tile_editor(void)
{
	GCL_Keyboard my_kb;
	int t_num=0, t_type=0;
	int fr;

	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();

		fr=spr_tiles->get_current_frame();
		t_num=fr;
		t_type=tile_types[t_num].mg;

		spr_tiles->draw(100,100);
		fnt_misc->draw(50,200,CL_String::format("Tile num: %1\nType: %2",t_num+1,t_type+1));
		fnt_misc->draw(50,230,"1-дорога, 2-трава, 3-крыша, 4-вода, 5-песок, 6-другое");

		if (my_kb.KeyPress(CL_KEY_RIGHT)||my_kb.KeyPress(CL_KEY_SPACE))
			fr++;
		if (my_kb.KeyPress(CL_KEY_LEFT))
			fr--;
		if (my_kb.KeyPress(CL_KEY_NEXT))
			fr+=10;
		if (my_kb.KeyPress(CL_KEY_PRIOR))
			fr-=10;

		for (int key=CL_KEY_1;key<=CL_KEY_6;key++)
			if (my_kb.KeyPress(key))
			{
				t_type=key-CL_KEY_1;
				tile_types[t_num].mg=t_type;
			}

		if (fr>=spr_tiles->get_frame_count())
			fr=fr-spr_tiles->get_frame_count();
		if (fr<0)
			fr=spr_tiles->get_frame_count()+fr;
		spr_tiles->set_frame(fr);

		CL_Display::flip();
		CL_System::keep_alive();
		my_kb.Refresh();
	}
}	

void CEditor::tile_grouper(void)
{
	GCL_Keyboard my_kb;
	int t_num=0, t_type=0;
	int fr=0;
	int pointer=0,point_num=0;
	int cur_group=0;

	point_num=NextInGroup(0,cur_group,fr);//pointer++;

	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();

		int x[9],lx=0,ly=0,y=0,i,count=0;

		for (i=0;i<9;i++)
			x[i]=0;

		for (i=0;i<168;i++)
			if (tile_types[i].mg==fr)
			{
				spr_tiles->set_frame(i);
				y=tile_types[i].sg*64;
				spr_tiles->draw(x[y/64],y);
				fnt_misc->draw(x[y/64],y,CL_String::from_int(y/64));

				if (i==point_num)//count==pointer)
				{
					lx=x[y/64];ly=y;
					//point_num=i;
				}
				count++;
				x[y/64]+=64;
				/*if (x>800)
				{
					x=0;y+=64;
				}*/
			}
		
		CL_Display::draw_line(lx,ly,lx+64,ly+64,CL_Color::red);
		CL_Display::draw_line(lx+64,ly,lx,ly+64,CL_Color::red);

		/*fr=spr_tiles->get_current_frame();
		t_num=fr;*/
		t_type=tile_types[point_num].sg;

		/*spr_tiles->draw(100,100);*/
		fnt_misc->draw(50,580,CL_String::format("Tile num: %1   Group: %2",point_num+1,t_type+1));
		//fnt_misc->draw(50,230,"1-дорога, 2-трава, 3-крыша, 4-вода, 5-песок, 6-другое");*/

		if (my_kb.KeyPress(CL_KEY_SPACE))
			fr++;
		if (my_kb.KeyPress(CL_KEY_RIGHT))
			point_num=NextInGroup(point_num,cur_group,fr);//pointer++;
		if (my_kb.KeyPress(CL_KEY_LEFT))
			pointer--;
		if (my_kb.KeyPress(CL_KEY_DOWN))
		{
			cur_group++;
			point_num=NextInGroup(0,cur_group,fr);//pointer++;
		}
		if (my_kb.KeyPress(CL_KEY_UP))
		{
			cur_group--;
			point_num=NextInGroup(0,cur_group,fr);//pointer++;
		}

		for (int key=CL_KEY_1;key<=CL_KEY_9;key++)
			if (my_kb.KeyPress(key))
			{
				t_type=key-CL_KEY_1;
				tile_types[point_num].sg=t_type;
			}

		if (fr>=6)//spr_tiles->get_frame_count())
			fr=0;//fr-spr_tiles->get_frame_count();
		int tile_num=count;
		if (pointer>=tile_num)
			pointer=0;
		if (pointer<0)
			pointer=tile_num-1;
		if (cur_group>=9)
			cur_group=0;
		if (cur_group<0)
			cur_group=8;

		CL_Display::flip();
		CL_System::keep_alive();
		my_kb.Refresh();
	}
}

// возвращает индекс следующего тайла в заданной группе
int CEditor::NextInGroup(int begin_index, int group, int mg)
{
	int i;
	for (i=begin_index+1;i<168;i++)
	{
		if (tile_types[i].sg==group&&tile_types[i].mg==mg)
			return i;
	}

	//не нашли с середины
	for (i=0;i<=begin_index;i++)
	{
		if (tile_types[i].sg==group&&tile_types[i].mg==mg)
			return i;
	}
	return -1;
}

void CEditor::map_editor(void)
{
	int cur_group=0;//тип кисти
	int cur_line=0;//подгруппа кисти
	int cur_brush=0;//кисть
	dx=0,dy=0;
	int x,y,i,j;
	lb=20,tb=90;//границы

	GCL_Keyboard my_kb;
	GCL_Mouse my_mouse;
	fnt_misc->set_color(CL_Color::aquamarine);

	//абсолютные значения
	int gl_brush=0,gl_angle=0;

	for(i=0;i<12;i++)
	{
		brushes[i]=0;
		brush_angles[i]=0;
	}

	while (!my_kb.KeyPress(CL_KEY_ESCAPE))
	{
		Draw();

		//рисуем кисти
		for (i=0;i<12;i++)
		{
			spr_tiles->set_angle(brush_angles[i]*90);
			spr_tiles->set_frame(brushes[i]);
			spr_tiles->draw(lb+i*64,0);
		}
		CL_Display::draw_rect(CL_Rect(lb+cur_brush*64,0,lb+cur_brush*64+64,64),CL_Color::red);


		/*for (x=0;x<mwidth;x++)
			for (y=0;y<mheight;y++)
			{
				//spr_tiles->set_frame(map[x*mwidth+y].tile);
				//spr_tiles->draw(lb+x*64,tb+y*64);
			}*/

		if (my_kb.KeyPress(CL_KEY_RIGHT)&&(dx+12<mwidth))
			dx++;
		if (my_kb.KeyPress(CL_KEY_LEFT)&&(dx>0))
			dx--;
		if (my_kb.KeyPress(CL_KEY_DOWN)&&(dy+8<mheight))
			dy++;
		if (my_kb.KeyPress(CL_KEY_UP)&&(dy>0))
			dy--;
		if (my_kb.KeyPress(CL_KEY_NEXT))
			cur_line++;
		if (my_kb.KeyPress(CL_KEY_PRIOR))
			cur_line--;
		if (my_kb.KeyPress(CL_KEY_SPACE))
			cur_group++;
		if (my_kb.KeyPress(VK_OEM_PERIOD))
		{
			brush_angles[cur_brush]++;
			if (brush_angles[cur_brush]>3) 
				brush_angles[cur_brush]=0;
			gl_angle=brush_angles[cur_brush];
		}
		if (my_kb.KeyPress(CL_KEY_C))
		{
			Copy();
			WaitMouse();
			my_kb.Refresh();
			my_mouse.Refresh();
			continue;
		}
		if (my_kb.KeyPress(CL_KEY_P))
		{
			Paste();
			WaitMouse();
			my_kb.Refresh();
			my_mouse.Refresh();
			continue;
		}
		if (my_mouse.KeyDown(CL_MOUSE_LEFT))
			EditTile(CL_Mouse::get_x(),CL_Mouse::get_y(),gl_brush,gl_angle);

		//пипетка
		if (my_mouse.KeyDown(CL_MOUSE_RIGHT))
			GetBrush(CL_Mouse::get_x(),CL_Mouse::get_y(),gl_brush,gl_angle);

		if (cur_line==-1)
			cur_line=8;
		else if (cur_line==9)
			cur_line=0;
		if (cur_group==-1)
			cur_group=TT_OTHER;
		else if (cur_group>TT_OTHER)
			cur_group=TT_ROAD;

		UpdateBrushes(cur_group,cur_line);

		for (int key=CL_KEY_F1;key<=CL_KEY_F12;key++)
			if (my_kb.KeyPress(key))
			{
				cur_brush=key-CL_KEY_F1;
				gl_brush=brushes[cur_brush];
			}

		CL_Display::flip();
		CL_System::keep_alive();
		my_kb.Refresh();
		my_mouse.Refresh();
	}
	SaveMap();

}

// загоняет в массив кисти из определенной группы и подгруппы
void CEditor::UpdateBrushes(int mg, int sg)
{
	int i,j;
	for (i=0,j=0;i<168 && j<12;i++)
		if (tile_types[i].mg==mg && tile_types[i].sg==sg)
			brushes[j++]=i;

	for (;j<12;j++)
		brushes[j]=0;
}

void CEditor::EditTile(int x, int y, int brush, int angle)
{
	int mx,my;
	mx=(x-lb)/64+dx;
	my=(y-tb)/64+dy;

	map[mx*mwidth+my].tile=brush;
	map[mx*mwidth+my].angle=angle;
}

// сохраняет карту в файл
void CEditor::SaveMap(void)
{
	SaveMapV2();
	return;

	CL_OutputSource_File ofile(map_file_name);

	ofile.write_string("BGA map file");
	ofile.write_string(mh.map_name);
	ofile.write_string(mh.map_author);

	ofile.write_int32(mwidth);
	ofile.write_int32(mheight);

	int i;
	for (i=0;i<msize;i++)
	{
		ofile.write_int32(map[i].tile);
		ofile.write_int32(map[i].angle);
		ofile.write_int32(map[i].shadow);
		ofile.write_int32(map[i].item);
	}
}

// сохраняет карту в файл
void CEditor::SaveMapV2(void)
{
	mh.map_name="platetown";
	mh.map_author="Heavysoft";
	map_file_name="data\\main\\maps\\"+mh.map_name+"\\level.map";

	CL_OutputSource_File ofile(map_file_name);

	ofile.write_string("BGA map file");
	ofile.write_string(mh.map_name);
	ofile.write_string(mh.map_author);

	ofile.write_int32(mwidth);
	ofile.write_int32(mheight);

	int i;
	for (i=0;i<msize;i++)
	{
		ofile.write_int32(map[i].tile);
		ofile.write_int32(map[i].angle);
		ofile.write_int32(map[i].shadow);
		//ofile.write_int32(map[i].item);
	}
}

// сохраняет уровень в файл в формате второй версии
void CEditor::SaveLevel(int game_type)
{
	mh.map_name="platetown";
	string gt;

	switch (game_type)
	{
	case GAME_DM:
		gt="dm";
		break;
	case GAME_TDM:
		gt="tdm";
		break;
	case GAME_CTF:
		gt="ctf";
		break;
	}

	map_file_name="data\\main\\maps\\"+mh.map_name+"\\"+gt+".cfg";

	CL_OutputSource_File ofile(map_file_name);

	int i;
	for (i=0;i<msize;i++)
	{
		//ofile.write_int32(map[i].tile);
		//ofile.write_int32(map[i].angle);
		//ofile.write_int32(map[i].shadow);
		ofile.write_int32(map[i].item);
	}
}

// загружает уровень в файл в формате второй версии
void CEditor::LoadLevel(int game_type)
{
	mh.map_name="platetown";
	string gt;

	switch (game_type)
	{
	case GAME_DM:
		gt="dm";
		break;
	case GAME_TDM:
		gt="tdm";
		break;
	case GAME_CTF:
		gt="ctf";
		break;
	}

	map_file_name="data\\main\\maps\\"+mh.map_name+"\\"+gt+".cfg";

	CL_InputSource_File ifile(map_file_name);

	int i;
	for (i=0;i<msize;i++)
	{
		//ofile.write_int32(map[i].tile);
		//ofile.write_int32(map[i].angle);
		//ofile.write_int32(map[i].shadow);
		map[i].item=ifile.read_int32();
	}
}

// загружает карту из файла
int CEditor::LoadMap(string mname)
{
	map_file_name="data\\main\\maps\\"+mname+"\\level.map";
	try
	{
		CL_InputSource_File ifile(map_file_name);

		ifile.read_string();//"BGA map file"
		mh.map_name=ifile.read_string();
		mh.map_author=ifile.read_string();

		mwidth=ifile.read_int32();
		mheight=ifile.read_int32();

		msize=mwidth*mheight;


		for (int i=0;i<msize;i++)
		{
			map_info temp;
			temp.tile=ifile.read_int32();
			//temp.tile=if2.read_int32();
			temp.angle=ifile.read_int32();
			//temp.angle=if2.read_int32();
			temp.shadow=ifile.read_int32();
			//temp.shadow=if2.read_int32();
			//temp.item=ifile.read_int32();
			map.push_back(temp);
		}
	}
	catch (...)
	{
		return 0;
	}

	LoadLevel(GAME_DM);

	return 1;
}

int projectX::main(int argc, char **argv)
{
	// создаём консоль
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	try
	{
		//инициализация
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();

		//окно
		CL_DisplayWindow window("BGA Editor", 800, 600, false, true);
		//контекст
		CL_GraphicContext *gc = window.get_gc();

		srand(CL_System::get_time());

		CEditor editor;
		
		//CEditor Editor;
		//editor.tile_editor();
		//Editor.tile_grouper();
		//editor.map_editor();
		
		//editor.GenerateMap();
		//editor.map_editor();
		
		editor.item_editor();
		//editor.SaveMap();

		//editor.SaveMapV2();
		//editor.SaveLevel(GAME_DM);

		//деинициализация
	 	CL_SetupCore::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupGL::deinit();
	}
	catch (CL_Error err)
	{
		std::cout << "Exception caught: " << err.message.c_str() << std::endl;

		// Display console close message and wait for a key
		console.display_close_message();
	}

	return 0;
} 


void CEditor::GetBrush(int x, int y, int &brush, int &angle)
{
	int mx,my;
	mx=(x-lb)/64+dx;
	my=(y-tb)/64+dy;

	brush=map[mx*mwidth+my].tile;
	angle=map[mx*mwidth+my].angle;
}

void CEditor::Draw()
{
	int x,y,j;	
	CL_Display::clear();
		CL_Display::draw_line(0,67,800,67,CL_Color::aqua);

		//рисуем карту
		for (x=0;x<12;x++)
			for (y=0;y<12;y++)
			{
				spr_tiles->set_angle(map[(dx+x)*mwidth+(dy+y)].angle*90);
				spr_tiles->set_frame(map[(dx+x)*mwidth+(dy+y)].tile);
				spr_tiles->set_alpha(1);
				spr_tiles->draw(lb+x*64,tb+y*64);

				//рисуем тень
				if (map[(dx+x)*mwidth+(dy+y)].shadow)
				{
					spr_tiles->set_frame(map[(dx+x)*mwidth+(dy+y)].shadow-1+170);
					spr_tiles->set_angle(0);
					//spr_tiles->set_alpha(0.7);
					spr_tiles->draw(lb+x*64,tb+y*64);
				}
			}

		//рисуем вертикальную сетку
		for (x=lb,j=0;x<800;x+=64,j++)
		{
			CL_Display::draw_line(x,tb-20,x,600,CL_Color::aqua);
			fnt_misc->draw(lb+30+j*64,tb-20,CL_String::from_int(dx+j+1));
		}
		//рисуем горизонтальную сетку
		for (y=tb,j=0;y<600;y+=64,j++)
		{
			CL_Display::draw_line(0,y,800,y,CL_Color::aqua);
			fnt_misc->draw(8,tb+30+j*64,CL_String::from_int(dy+j+1));
		}
}

// копирование прямоугольной области тайлов
void CEditor::Copy(void)
{
	GCL_Mouse my_mouse;
	GCL_Keyboard my_kb;

	while (!my_mouse.KeyPress(CL_MOUSE_LEFT))
	{
		CL_Display::clear();
		Draw();
		if (my_kb.KeyPress(CL_KEY_RIGHT)&&(dx+12<mwidth)) 
			dx++;
		if (my_kb.KeyPress(CL_KEY_LEFT)&&(dx>0))
			dx--;
		if (my_kb.KeyPress(CL_KEY_DOWN)&&(dy+8<mheight))
			dy++;
		if (my_kb.KeyPress(CL_KEY_UP)&&(dy>0))
			dy--;

		CL_Display::flip();
		CL_System::keep_alive();
		my_mouse.Refresh();
		my_kb.Refresh();
	}
	top_left.x=(my_mouse.GetPosition().x-lb)/64+dx;
	top_left.y=(my_mouse.GetPosition().y-tb)/64+dy;
	my_mouse.Refresh();
	while (!my_mouse.KeyPress(CL_MOUSE_LEFT))
	{
		CL_Display::clear();
		Draw();
		if (my_kb.KeyPress(CL_KEY_RIGHT)&&(dx+12<mwidth))
			dx++;
		if (my_kb.KeyPress(CL_KEY_LEFT)&&(dx>0))
			dx--;
		if (my_kb.KeyPress(CL_KEY_DOWN)&&(dy+8<mheight))
			dy++;
		if (my_kb.KeyPress(CL_KEY_UP)&&(dy>0))
			dy--;

		CL_Display::flip();
		CL_System::keep_alive();
		my_mouse.Refresh();
		my_kb.Refresh();
	}
	right_bottom.x=(my_mouse.GetPosition().x-lb)/64+dx;
	right_bottom.y=(my_mouse.GetPosition().y-tb)/64+dy;
}

projectX app;

// вставляет скопированный кусок в указанное место
void CEditor::Paste(void)
{
	GCL_Mouse my_mouse;
	GCL_Keyboard my_kb;

	while (!my_mouse.KeyPress(CL_MOUSE_LEFT))
	{
		CL_Display::clear();
		Draw();
		if (my_kb.KeyPress(CL_KEY_RIGHT)&&(dx+12<mwidth))
			dx++;
		if (my_kb.KeyPress(CL_KEY_LEFT)&&(dx>0))
			dx--;
		if (my_kb.KeyPress(CL_KEY_DOWN)&&(dy+8<mheight))
			dy++;
		if (my_kb.KeyPress(CL_KEY_UP)&&(dy>0))
			dy--;

		CL_Display::flip();
		CL_System::keep_alive();
		my_mouse.Refresh();
		my_kb.Refresh();
	}
	CL_Point plt;
	plt.x=(my_mouse.GetPosition().x-lb)/64+dx;
	plt.y=(my_mouse.GetPosition().y-tb)/64+dy;

	my_mouse.Refresh();
	CL_System::keep_alive();

	int cl=top_left.x;
	int ct=top_left.y;
	int cr=right_bottom.x;
	int cb=right_bottom.y;

	int ci,cj;

	for (int i=plt.x, ci=cl;i<plt.x+cr-cl+1;i++,ci++)
		for (int j=plt.y, cj=ct;j<plt.y+cb-ct+1;j++,cj++)
		{
			if (i>=mwidth || j>=mheight)
				continue;

			map[i*mwidth+j].tile=map[ci*mwidth+cj].tile;
			map[i*mwidth+j].angle=map[ci*mwidth+cj].angle;			
		}
}

// ждет, пока нажата кнопка мыши
void CEditor::WaitMouse(void)
{
	while (CL_Mouse::get_keycode(CL_MOUSE_LEFT))
	{
		CL_System::keep_alive();
	}
}

// генерирует карту по скетчу
void CEditor::GenerateMap(void)
{
	int roof_t[7][26]=
	{
		//черепица
		{
			11,0, 12,0, 33,0,
			92,90, 18,0, 91,270,//-1
			15,0, 16,0, 34,0,
			85,0, 86,0, 89,0, 90,0
		},
		//рыжие стены
		{
			40,180, 37,180, 40,270,
			37,90, 124,0, 37,270,
			40,90, 37,0, 40,0,
			36,180, 36,270, 36,90, 36,0
		},
		//зеленые стены
		{
			65,180, 66,180, 65,270,
			66,90, 67,0, 66,270,
			65,90, 66,0, 65,0,
			64,90, 64,180, 64,0, 64,270
		},
		//синие стены
		{
			14,180, 17,180, 14,270,
			17,90, 20,0, 17,270,
			14,90, 17,0, 14,0,
			100,90, 100,180, 100,0, 100,270
		},
		//каменные стены
		{
			42,180, 71,180, 42,270,
			71,90, 46,0, 71,270,
			42,90, 71,0, 42,0,
			99,180, 99,270, 99,90, 99,0
		},
		//коричневые стены
		{
			26,180, 27,180, 26,270,
			27,90, 31,0, 27,270,
			26,90, 27,0, 26,0,
			103,90, 103,180, 103,0, 103,270
		},
		//дорога
		{
			23,0, 1,90, 23,90,
			1,0, 84,0, 1,180,
			23,270, 1,270, 23,180,
			25,180, 25,270, 25,90, 25,0
		}
	};

	BITMAP_FILE sketch;
	sketch.buffer=0;
	int res=Load_Bitmap_File(&sketch,"data/main/maps/sketch.bmp");
	
	int w=sketch.bitmapinfoheader.biWidth;
	int h=sketch.bitmapinfoheader.biHeight;
	
	stile_info *temp=new stile_info[w*h];

	//ofstream f("temp.txt");
	for (int i=0;i<w;i++)
	{
		for (int j=0;j<h;j++)
		{
			/*int a='0';
			if (sketch.buffer[i+j*h]==0)
				f<<"B ";
			if (sketch.buffer[i+j*h]==1)
				f<<"R ";
			if (sketch.buffer[i+j*h]==2)
				f<<"G ";*/
			
			temp[i+j*h].tile=sketch.buffer[i+j*h];

			if (temp[i+j*h].tile==TILE_HORROAD)
			{
				temp[i+j*h].special=TILE_HORROAD;
				temp[i+j*h].tile=TILE_ROAD;
			}
			else if(temp[i+j*h].tile==TILE_VERROAD)
			{
				temp[i+j*h].special=TILE_VERROAD;
				temp[i+j*h].tile=TILE_ROAD;
			}
			else
				temp[i+j*h].special=0;

			temp[i+j*h].angle=0;
			temp[i+j*h].color=-1;
			


			//f<<sketch.buffer[i+j*h]+'0'<<" ";
		}
		//f<<endl;
	}

	map.clear();
	mwidth=w;mheight=h;
	msize=w*h;

	for (int i=0;i<msize;i++)
	{
		map_info temp;
		temp.tile=8;//rand()%169;
		temp.angle=0;
		temp.shadow=0;
		temp.item=-1;
		map.push_back(temp);
	}

	Unload_Bitmap_File(&sketch);

	bool left=false,right=false,top=false,down=false;
	bool lefttop=false,rightdown=false;
	bool righttop=false,leftdown=false;
	int color=0,type=0;
	for (int j=0;j<w;j++)
	{
		for (int i=0;i<h;i++)
		{
			type=0;
			if (temp[i+j*h].tile==TILE_ROOF || temp[i+j*h].tile==TILE_BORDER || temp[i+j*h].tile==TILE_ROAD)//крыша или бордер или дорога
			{
				int curt=temp[i+j*h].tile;

				bool noshd=false;
				if (curt==3)
					noshd=true;
				
				if (i==0)
					left=false;
				else
					left=temp[i-1+j*h].tile==curt;

				if (i==h-1)
					right=false;
				else
					right=temp[i+1+j*h].tile==curt;

				if (j==0)
					top=false;
				else
					top=temp[i+(j-1)*h].tile==curt;

				if (j==w-1)
					down=false;
				else
					down=temp[i+(j+1)*h].tile==curt;

				if (i==0 || j==0)
					lefttop=false;
				else
					lefttop=temp[i-1+(j-1)*h].tile==curt;

				if (i==h-1 || j==0)
					righttop=false;
				else
					righttop=temp[i+1+(j-1)*h].tile==curt;

				if (i==0 || j==w-1)
					leftdown=false;
				else
					leftdown=temp[i-1+(j+1)*h].tile==curt;

				if (i==h-1 || j==w-1)
					rightdown=false;
				else
					rightdown=temp[i+1+(j+1)*h].tile==curt;

				if (!left && !top)
				{
					color=rand()%6;
					type=0;//лев-верх
				}
				if (left && !top &&right)
				{
					color=temp[i-1+j*h].color;
					type=1;//верх
				}
				if (left && !top && !right)
				{
					color=temp[i-1+j*h].color;
					type=2;//прав-верх
					if (!noshd)
						SetShadow(i+1,j,1);
				}
				if (!left && top)
				{
					color=temp[i+(j-1)*h].color;
					type=3;//лев
				}
				if (left && top && right)
				{
					color=temp[i-1+j*h].color;
					type=4;//сер
				}
				if (left && top && down && !right)
				{
					color=temp[i-1+j*h].color;
					type=5;//прав
					if (!noshd)
						SetShadow(i+1,j,1);
				}
				if (!left && top && !down)
				{
					color=temp[i+(j-1)*h].color;
					type=6;//низ-лев
					if (!noshd)
						SetShadow(i,j+1,2);
				}
				if (left && top && !down && right)
				{
					color=temp[i-1+j*h].color;
					type=7;//низ
					if (!noshd)
						SetShadow(i,j+1,2);
				}
				if (left && top && !down && !right)
				{
					color=temp[i-1+j*h].color;
					type=8;//низ-прав
					if (!noshd)
					{
						SetShadow(i,j+1,2);
						SetShadow(i+1,j,1);
						SetShadow(i+1,j+1,3);
					}
				}
				if (left && top && !lefttop)
				{
					color=temp[i-1+j*h].color;
					type=9;//угл лев-верх
				}
				if (right && top && !righttop)
				{
					color=temp[i+(j-1)*h].color;
					type=10;//угл прав-верх
				}
				if (left && down && !leftdown)
				{
					color=temp[i-1+j*h].color;
					type=11;//угл лев-низ
				}
				if (right && down && !rightdown)
				{
					color=temp[i-1+j*h].color;
					type=12;//угл прав-низ
					if (!noshd)
						SetShadow(i+1,j+1,4);
				}

				if (temp[i+j*h].tile==3)//дорога
					color=6;
				
				if (temp[i+j*h].tile==0)//бордер
				{
					color=5;
					/*if (!i && j>0 && j<h-1)
					{
						type=5;
						SetShadow(i+1,j,1);
					}
					if (!j && i>0 && i<w-1)
					{
						type=7;				
						SetShadow(i,j+1,2);
					}
					if (!i && !j)
						SetShadow(i+1,j+1,4);*/
				}
				temp[i+j*h].color=color;
				int t=roof_t[color][type*2];
				map[j+i*h].tile=roof_t[color][type*2];
				int a=roof_t[color][type*2+1];
				map[j+i*h].angle=roof_t[color][type*2+1]/90;

			}
			else if (temp[i+j*h].tile==TILE_SIDEWALK)//тротуар
			{
				map[j+i*h].tile=0;
				int curt=temp[i+j*h].tile;
		
				if (i==0)
					left=false;
				else
					left=temp[i-1+j*h].tile==curt;

				if (i==h-1)
					right=false;
				else
					right=temp[i+1+j*h].tile==curt;

				if (j==0)
					top=false;
				else
					top=temp[i+(j-1)*h].tile==curt;

				if (j==w-1)
					down=false;
				else
					down=temp[i+(j+1)*h].tile==curt;

				/*if (i==0 || j==0)
					lefttop=false;
				else
					lefttop=temp[i-1+(j-1)*h].tile==curt;

				if (i==h-1 || j==0)
					righttop=false;
				else
					righttop=temp[i+1+(j-1)*h].tile==curt;

				if (i==0 || j==w-1)
					leftdown=false;
				else
					leftdown=temp[i-1+(j+1)*h].tile==curt;

				if (i==h-1 || j==w-1)
					rightdown=false;
				else
					rightdown=temp[i+1+(j+1)*h].tile==curt;*/

				// |--
				// |
				if (!left && !top && down && right)
				{
					map[j+i*h].tile=21;
					map[j+i*h].angle=0;
				}
				// --|
				//   |
				if (left && !top && down && !right)
				{
					map[j+i*h].tile=21;
					map[j+i*h].angle=1;
				}
				//   |
				// --|
				if (left && top && !down && !right)
				{
					map[j+i*h].tile=21;
					map[j+i*h].angle=2;
				}
				// |
				// |--
				if (!left && top && !down && right)
				{
					map[j+i*h].tile=21;
					map[j+i*h].angle=3;
				}

				// ^
				// |
				if (!left && !top && down && !right)
				{
					map[j+i*h].tile=165;
					map[j+i*h].angle=2;
				}
				// |
				// v
				if (!left && top && !down && !right)
				{
					map[j+i*h].tile=165;
					map[j+i*h].angle=0;
				}
				// <-
				if (!left && !top && !down && right)
				{
					map[j+i*h].tile=165;
					map[j+i*h].angle=1;
				}
				// ->
				if (left && !top && !down && !right)
				{
					map[j+i*h].tile=165;
					map[j+i*h].angle=3;
				}

				// -- 
				// --
				if (left && !top && !down && right)
				{
					map[j+i*h].tile=58;
					map[j+i*h].angle=1;
				}
				// ||
				// ||
				if (!left && top && down && !right)
				{
					map[j+i*h].tile=58;
					map[j+i*h].angle=0;
				}

				if (!map[j+i*h].tile)//ставим дерево
					map[j+i*h].tile=83;

			}
			else if (temp[i+j*h].tile==TILE_GRASS)
			{
				map[j+i*h].tile=93;
				map[j+i*h].angle=0;
			}
			else if (temp[i+j*h].tile==TILE_SAND)
			{
				map[j+i*h].tile=111;
				map[j+i*h].angle=0;
			}
			else if (temp[i+j*h].tile==TILE_PAVEMENT2)
			{
				map[j+i*h].tile=109;
				map[j+i*h].angle=0;
			}
			else if (temp[i+j*h].tile==TILE_TREE)
			{
				if (rand()%2==0)
                    map[j+i*h].tile=83;
				else
					map[j+i*h].tile=106;
				map[j+i*h].angle=0;
			}
			else //if (temp[i+j*h].tile==2)//земля
			{
				map[j+i*h].tile=8;
				map[j+i*h].angle=0;
			}

			if (temp[i+j*h].special==TILE_HORROAD)
			{
				map[j+i*h].tile=76;
				map[j+i*h].angle=1;
			}
			else if (temp[i+j*h].special==TILE_VERROAD)
			{
				map[j+i*h].tile=76;
				map[j+i*h].angle=0;
			}
		}
	}

	delete [] temp;

}

// добавляет тень к нужному тайлу
void CEditor::SetShadow(int x, int y, int type)
{
	if (x>=mwidth)
		return;
	if (y>=mheight)
		return;

	int h=mheight;
	int s=map[y+x*h].shadow;
	if (s==3 || s==0)
		map[y+x*h].shadow=type;
	else
	{
		if (type==3)
			return;
		
		map[y+x*h].shadow=4; 
	}
}
// редактор бонусов/особых точек
void CEditor::item_editor(void)
{
	int cur_group=0;//тип кисти
	int cur_line=0;//подгруппа кисти
	int cur_brush=0;//кисть
	dx=0,dy=0;
	int x,y,i,j;
	lb=20,tb=90;//границы

	int first_brush=0;

	GCL_Keyboard my_kb;
	GCL_Mouse my_mouse;
	fnt_misc->set_color(CL_Color::aquamarine);

	//абсолютные значения
	int gl_brush=0,gl_angle=0;

	for(i=0;i<12;i++)
	{
		brushes[i]=i;
	}

	//brushes[11]=-1;

	while (!my_kb.KeyPress(CL_KEY_ESCAPE))
	{
		Draw();
		DrawItems();

		//рисуем кисти
		for (i=0;i<12;i++)
		{
			if (brushes[i]<0)
				continue;
			spr_items->set_frame(brushes[i]+first_brush);
			spr_items->draw(lb+i*64+32,32);
		}
		CL_Display::draw_rect(CL_Rect(lb+cur_brush*64,0,lb+cur_brush*64+64,64),CL_Color::red);


		if (my_kb.KeyPress(CL_KEY_RIGHT)&&(dx+12<mwidth))
			dx++;
		if (my_kb.KeyPress(CL_KEY_LEFT)&&(dx>0))
			dx--;
		if (my_kb.KeyPress(CL_KEY_DOWN)&&(dy+8<mheight))
			dy++;
		if (my_kb.KeyPress(CL_KEY_UP)&&(dy>0))
			dy--;

		if (my_kb.KeyPress(CL_KEY_1))
		{
			first_brush=0;
		}
		if (my_kb.KeyPress(CL_KEY_2))
		{
			first_brush=12;
		}

		if (my_mouse.KeyDown(CL_MOUSE_LEFT))
			EditItem(CL_Mouse::get_x(),CL_Mouse::get_y(),gl_brush);

		//пипетка
		if (my_mouse.KeyDown(CL_MOUSE_RIGHT))
			GetItem(CL_Mouse::get_x(),CL_Mouse::get_y(),gl_brush);


		for (int key=CL_KEY_F1;key<=CL_KEY_F12;key++)
			if (my_kb.KeyPress(key))
			{
				cur_brush=key-CL_KEY_F1;
				gl_brush=brushes[cur_brush]+first_brush;
			}

		CL_Display::flip();
		CL_System::keep_alive();
		my_kb.Refresh();
		my_mouse.Refresh();
	}
	SaveMap();	
	SaveLevel(GAME_DM);
}

// рисует объекты
void CEditor::DrawItems(void)
{
	spr_items->set_alpha(0.5);
	int x,y;
	//рисуем объекты
	for (x=0;x<12;x++)
		for (y=0;y<12;y++)
		{		
			int item=map[(dx+x)*mwidth+(dy+y)].item;

			if (item<0)
				continue;

			spr_items->set_frame(item);
			spr_items->draw(lb+x*64+32,tb+y*64+32);
		}
	spr_items->set_alpha(1.0);
}

void CEditor::EditItem(int x, int y, int brush)
{
	int mx,my;
	mx=(x-lb)/64+dx;
	my=(y-tb)/64+dy;

	map[mx*mwidth+my].item=brush;
}

void CEditor::GetItem(int x, int y, int &brush)
{
	int mx,my;
	mx=(x-lb)/64+dx;
	my=(y-tb)/64+dy;

	brush=map[mx*mwidth+my].item;
}