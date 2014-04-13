/*
	Heavysoft Game Creation Library 2005 (GCL)

	name: 'GCL_MovingButton'
	description: class for using edit boxes
	author: 'Tosha'
	date: '07.10.2005'
	version: '0.0.50'
*/

#include ".\gcl_editbox.h"
#include ".\gcl_menu.h"

GCL_EditBox::GCL_EditBox(int bx, int by, int bid, string res_id, CL_ResourceManager *RM)
									:GCL_Button(bx,by,false,bid,res_id,NULL)
{
	value="";
	name=res_id;
	cur_pos=0;
	tick=0;
	dt=0;

	max_len=20;
	only_num=false;

	w=GCL_Menu::menu_font->get_width("0")*(name.length()+max_len);
	h=GCL_Menu::menu_font->get_height("0");
}

GCL_EditBox::~GCL_EditBox(void)
{
}

void GCL_EditBox::Draw()
{
	//sprite->set_frame(frame);
	//sprite->draw(x,y);
	if (frame==GCL_BUTTON_FRAME_OFF)
		GCL_Menu::menu_font->set_color(0.7,.7,0.7);
	else if(frame==GCL_BUTTON_FRAME_ON)
		GCL_Menu::menu_font->set_color(0.7,0.7,1);
	else 
		GCL_Menu::menu_font->set_color(1,0.7,0.7);
	GCL_Menu::menu_font->draw(x,y,name);

	string left=value.substr(0,cur_pos);
	string right=value.substr(cur_pos,value.length()-cur_pos);
	int nx=x+GCL_Menu::menu_font->get_width(name)+10;

	if (tick==0)
	{
		tick++;dt=1;
	}	
	if (tick==20)
	{
		tick--;dt=-1;
	}

	tick+=dt;

	GCL_Menu::menu_font->set_color(1,1,1);
	/*GCL_Menu::menu_font->draw(nx,y,left);
	nx+=GCL_Menu::menu_font->get_width(left);
	GCL_Menu::menu_font->set_color(1,1,1);
	GCL_Menu::menu_font->draw(nx,y,right);*/
	int nx2=nx;
	int l=value.length();
	for (int i=0;i<l;i++)
	{
		string ch=value.substr(i,1);

		GCL_Menu::menu_font->draw(nx,y,ch);

		if (ch!=" ")
			nx+=GCL_Menu::menu_font->get_width(ch);
		else
			nx+=GCL_Menu::menu_font->get_width("W");

		if (i==cur_pos-1)
			nx2=nx;

	}

	if (check)//активна
	{
		if (tick<10)
			GCL_Menu::menu_font->set_color(0,0,0,0);
		else
			GCL_Menu::menu_font->set_color(1,0.5,0.5);

		GCL_Menu::menu_font->draw(nx2-2,y,"|");
		//nx+=GCL_Menu::menu_font->get_width("|");
	}
	//GCL_Menu::menu_font->draw(x+sprite->get_width()+10,y+sprite->get_height()+10,value);

}

void GCL_EditBox::OnKeyPress(int key)
{
	if (key==CL_KEY_LEFT && cur_pos)
		cur_pos--;
	else if (key==CL_KEY_RIGHT && cur_pos<value.length())
		cur_pos++;
	else if (key==CL_KEY_DELETE && cur_pos<value.length())
	{
		value=value.substr(0,cur_pos)+value.substr(cur_pos+1,value.length()-1-cur_pos);
	}
	else if (key==CL_KEY_BACKSPACE && cur_pos>0)
	{
		value=value.substr(0,cur_pos-1)+value.substr(cur_pos,value.length()-cur_pos);
		if (cur_pos)
			cur_pos--;
	}
	else if (key==CL_KEY_RETURN)
	{
		check=false;
	}
	/*else if (key==CL_KEY_SPACE)
	{
		value=value.substr(0,cur_pos)+" "+value.substr(cur_pos,value.length()-cur_pos);
		cur_pos++;
	}*/
	else if (key>31 && value.length()<max_len)
	{
		unsigned char keys_down[256];
		// Keep get_keycode() up-to-date.
		GetKeyboardState(keys_down);
		// Figure out what character sequence this maps to:
		char buf[4];
		int result = ToAscii(
			(UINT) key,
			MapVirtualKey((UINT) key, 0),
			keys_down,
			(LPWORD) buf,
			0);
		string str;
		if (result == 1 || result == 2) str = std::string(buf, result);

		if (!only_num || (only_num && *buf>='0' && *buf<='9'))
		{
			value=value.substr(0,cur_pos)+str+value.substr(cur_pos,value.length()-cur_pos);
			cur_pos+=str.length();
		}

	}
}
// устанавливает параметры поля ввода
void GCL_EditBox::SetParam(int id, string value)
{
	switch (id)
	{
	case GCL_EDITBOX_NAME:
		name=value;
		break;
	case GCL_EDITBOX_MAXLEN:
		max_len=CL_String::to_int(value);
		w=GCL_Menu::menu_font->get_width("0")*(name.length()+max_len);
		h=GCL_Menu::menu_font->get_height("0");
		break;
	case GCL_EDITBOX_ONLYNUM:
		only_num=CL_String::to_bool(value);
		break;
	case GCL_EDITBOX_VALUE:
		this->value=value;
		cur_pos=value.length();
		break;
	}
}

// возвращает параметры поля ввода
string GCL_EditBox::GetParam(int id)
{
	switch (id)
	{
	case GCL_EDITBOX_NAME:
		return name;
		break;
	case GCL_EDITBOX_MAXLEN:
		return CL_String::from_int(max_len);
		break;
	case GCL_EDITBOX_ONLYNUM:
		return CL_String::from_bool(only_num);
		break;
	case GCL_EDITBOX_VALUE:
		return value;
		break;
	}
	return "";
}
