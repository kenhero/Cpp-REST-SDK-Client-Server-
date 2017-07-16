#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#pragma once

//#include "targetver.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <string>

// TODO: reference additional headers your program requires here


#include <psapi.h>
#include <iostream>
#include <tlhelp32.h>
#include <pdh.h>
#include <fstream>

/**
*  TaskManager Class
*
*  Defines the required function to parse and get data from XML File
**/
class TaskManager {
private:


public:


	TaskManager();
	~TaskManager();
	/**
	*  Get Task Manager processes List
	*
	*  
	*/
	static void PrintTasksInfo();
	static int CLientInfo();
	

	

};
#endif
