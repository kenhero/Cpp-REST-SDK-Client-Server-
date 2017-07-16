/********************************************************************
This sample enumerates through all running tasks on the local computer and
displays their name and state.
********************************************************************/

#include "stdafx.h"


using namespace std;

int __cdecl wmain()
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
		return 1;
	}

	//  Set general COM security levels.
	hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		0,
		NULL);

	if (FAILED(hr))
	{
		printf("\nCoInitializeSecurity failed: %x", hr);
		CoUninitialize();
		return 1;
	}

	//  ------------------------------------------------------
	//  Create an instance of the Task Service. 
	ITaskService *pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&pService);
	if (FAILED(hr))
	{
		printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
		CoUninitialize();
		return 1;
	}

	//  Connect to the task service.
	hr = pService->Connect(_variant_t(), _variant_t(),
		_variant_t(), _variant_t());
	if (FAILED(hr))
	{
		printf("ITaskService::Connect failed: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}

	// Get the running tasks.
	IRunningTaskCollection* pRunningTasks = NULL;
	hr = pService->GetRunningTasks(TASK_ENUM_HIDDEN, &pRunningTasks);

	pService->Release();
	if (FAILED(hr))
	{
		printf("Cannot get Root Folder pointer: %x", hr);
		CoUninitialize();
		return 1;
	}

	LONG numTasks = 0;
	hr = pRunningTasks->get_Count(&numTasks);

	if (numTasks == 0)
	{
		printf("\nNo Tasks are currently running");
		pRunningTasks->Release();
		CoUninitialize();
		return 1;
	}

	printf("\nNumber of running tasks : %d", numTasks);

	TASK_STATE taskState;

	for (LONG i = 0; i < numTasks; i++)
	{
		IRunningTask* pRunningTask = NULL;
		hr = pRunningTasks->get_Item(_variant_t(i + 1), &pRunningTask);

		if (SUCCEEDED(hr))
		{
			BSTR taskName = NULL;
			hr = pRunningTask->get_Name(&taskName);
			if (SUCCEEDED(hr))
			{
				printf("\nTask Name: %S", taskName);
				SysFreeString(taskName);

				hr = pRunningTask->get_State(&taskState);
				if (SUCCEEDED(hr))
					printf("\n\tState: %d", taskState);
				else
					printf("\n\tCannot get the registered task state: %x", hr);
			}
			else
			{
				printf("\nCannot get the registered task name: %x", hr);
			}
			pRunningTask->Release();
		}
		else
		{
			printf("\nCannot get the registered task item at index=%d: %x", i + 1, hr);
		}
	}

	pRunningTasks->Release();
	CoUninitialize();
	return 0;
}
