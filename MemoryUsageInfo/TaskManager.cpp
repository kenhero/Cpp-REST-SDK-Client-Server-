// TaskManager.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TaskManager.h"


//function to obtain right privilege to access all process

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

using namespace std;
//#pragma comment(lib, "cmcfg32.lib")

BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
	)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		cout << "LookupPrivilegeValue error: %u\n" << GetLastError();
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		cout << "AdjustTokenPrivileges error: %u\n" << GetLastError();
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		cout <<"The token does not have the specified privilege. \n";
		return FALSE;
	}

	return TRUE;
}






void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}






// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1



void TaskManager::PrintTasksInfo()
{

	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	PROCESS_MEMORY_COUNTERS pmc;
	using namespace std;

	//out.txt with Client info
	wofstream f("out.txt", ios::app);

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of processes)"));

	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object

	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		//wcout << Process name 
		wcout << pe32.szExeFile << "\t";

		f << pe32.szExeFile << "\t";


		hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(TEXT("OpenProcess"));
		else
		{
			//qui dovrei prendermi tutte le info sul processo che mi servono

			GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));
			//cout << "Physical Memory currently used by current process in KB" 
			wcout << (pmc.WorkingSetSize / 1024) << endl;
			f <<  (pmc.WorkingSetSize / 1024) << endl;

			CloseHandle(hProcess);
		}


	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	f << endl;
	f.close();

}




void init(){
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}

double getCurrentValue(){
	PDH_FMT_COUNTERVALUE counterVal;
	//DWORD last_error;
	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	//last_error = GetLastError();
	return counterVal.doubleValue;
}





int TaskManager::CLientInfo()
{

	MEMORYSTATUSEX memInfo;

	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	
	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
//	unsigned int i;
	double CPU_Usage;

	//Total Physical Memory (RAM)
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;


	//Physical Memory currently used :

	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

	
	// Calculate how many process identifiers were returned. 

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	
	init();
	ofstream f("out.txt", ios::out);
	
	//cout << "Phisical memory currently used(RAM) : GB\n" << (double)physMemUsed / (1024 * 1024 * 1024) << endl;
	f << (double)physMemUsed / (1024 * 1024 * 1024) << endl;
	//cout << "Free System Memory (RAM): GB\n" << (double) (totalPhysMem - physMemUsed) / (1024*1024*1024) << endl;
	f << (double)(totalPhysMem - physMemUsed) / (1024 * 1024 * 1024) *100 << endl;
	//Number of processes
	f << cProcesses << endl;
	init();
	//Need Sleep for calculate right CPU Usage ,else it's always 0!!
	Sleep(200);
	CPU_Usage = getCurrentValue();
	//cout << "Total CPU usage:\n" << CPU_Usage << endl;
	f << CPU_Usage << endl;
	//id client that u need from server side to save info in the right file (lo faccio da POSTClient)
	/*ifstream client("..\POSTClient\config.conf");
	std:string signiture= "Bera";
	f  << signiture << endl;
	*/
	
	f.close();
	// Write Task Manager Info on file
	TaskManager::PrintTasksInfo();

	return 0;


}




