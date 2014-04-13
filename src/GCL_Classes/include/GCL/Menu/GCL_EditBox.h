#pragma once
#include "gcl_button.h"

#define GCL_EDITBOX_NAME    1
#define GCL_EDITBOX_VALUE   2
#define GCL_EDITBOX_MAXLEN  3
#define GCL_EDITBOX_ONLYNUM 4

class GCL_EditBox :
	public GCL_Button
{
	string value;
	
	string name;
	int max_len;
	bool only_num;
	
	int cur_pos;
	int tick;
	int dt;

public:
	GCL_EditBox(int bx, int by, int bid, string res_id, CL_ResourceManager *RM);
	~GCL_EditBox(void);

	void Draw();
	void OnKeyPress(int key);
	// устанавливает параметры поля ввода
	void SetParam(int id, string value);
	// возвращает параметры поля ввода
	string GetParam(int id);
};
