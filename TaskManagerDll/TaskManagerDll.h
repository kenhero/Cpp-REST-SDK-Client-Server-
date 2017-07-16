#ifdef TASKMANAGERDLL_EXPORTS
#define TASKMANAGERDLL_API __declspec(dllexport) 
#else
#define TASKMANAGERDLL_API __declspec(dllimport) 
#endif



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
#include "XMLLiteReaderDll.h"

/**
*  TaskManager Class
*
*  Defines the required function to parse and get data from XML File
**/

namespace TManager
{

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
		static void TASKMANAGERDLL_API PrintTasksInfo();
		static int TASKMANAGERDLL_API CLientInfo();




	};
}

