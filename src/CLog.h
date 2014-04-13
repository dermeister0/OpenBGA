#ifndef _CLOG_H_
#define _CLOG_H_

#include <fstream>
using std::ofstream;

// output file
#define LOG_FILE "BGA_log.txt"

// class Log declaration
class CLog : public ofstream {
	static CLog* instance;					// Log will be a soliteire object
public:
	// constructor
	CLog() : ofstream( LOG_FILE ) {}			
	// returns the only instance do Log
	static CLog* getInstance() {
		// if no Log object has been created
		if(!instance){						
			// create
			instance = new CLog;
		}
		// returns
		return instance;
	}

	// возвращает строку с текущим временем
	static string Time(void)
	{	
		SYSTEMTIME cur_time;
		GetLocalTime(&cur_time);
		char time[20];
		sprintf(time,"%02d:%02d:%02d",cur_time.wHour,cur_time.wMinute,cur_time.wSecond);
		string str_time="["+CL_String::format("%1",time)+"] ";

		return str_time;
	}
};

// macro to be used
#define LOG ( *( CLog::getInstance() ) )

#endif