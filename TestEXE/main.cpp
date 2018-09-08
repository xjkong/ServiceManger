#include<iostream>
#include<Windows.h>
using namespace std;
SERVICE_STATUS_HANDLE scshandle;
SERVICE_STATUS oServiceState; 
bool bRunning = true;

void WINAPI BDHandler(DWORD dwControl)
{
	SC_HANDLE hCurrent_Module =(SC_HANDLE)GetModuleHandle(NULL);
	switch(dwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		bRunning = false;
	oServiceState.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(scshandle, &oServiceState);
		CloseHandle(scshandle);
		break;
	default:
		break;
	}
}

void SvcMain()
{
	scshandle = RegisterServiceCtrlHandler("Test", BDHandler);
	if (NULL == scshandle)
	{
		return;
	}
	oServiceState.dwServiceType = SERVICE_WIN32;
	oServiceState.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
	oServiceState.dwWin32ExitCode = 0;
	oServiceState.dwCheckPoint = 0;
	oServiceState.dwWaitHint = 0;
	oServiceState.dwServiceSpecificExitCode = 0;
	oServiceState.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(scshandle, &oServiceState);
	while (bRunning)
	{
		Sleep(1000);
	}
}

void main()
{
	SERVICE_TABLE_ENTRY DispatchTable[] = 
	{ 
		{ "Test", (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
		{ NULL, NULL } 
	}; 
	StartServiceCtrlDispatcher(DispatchTable);
}