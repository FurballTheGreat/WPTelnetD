#pragma once
#include "pch.h"

enum ServiceState
{
	Stopped =  0x00000001,
	StartPending = 0x00000002,
	StopPending = 0x00000003,
	Running = 0x00000004,
	ContinuePending = 0x00000005,
	PausePending = 0x00000006,
	Paused = 0x00000007
};

enum ServiceType
{
	FileSystemDriver = 0x2,
	KernelDriver = 0x1,
	Win32OwnProcess = 0x10,
	Win32ShareProcess = 0x20
};



class ServiceProcess
{
	DWORD _pid;
	std::wstring _name;
	std::wstring _displayName;
	ServiceState _state;
	ServiceType _type;
public: 
	ServiceProcess(ENUM_SERVICE_STATUS_PROCESSW pInfo);
	std::wstring GetName();
	std::wstring GetDisplayName();
	ServiceState GetState();
	DWORD GetPID();
	ServiceType GetType();
};

class ServiceControl 
{
	SC_HANDLE _handle;
public:
	ServiceControl();
	~ServiceControl();

	void StartService(std::string pName);
	void StopService(std::string pName);

	std::vector<ServiceProcess> EnumServices(DWORD pWhat, DWORD pState);
};

