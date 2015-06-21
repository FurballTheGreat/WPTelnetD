#include "pch.h"
#include "ServiceControl.h"


ServiceControl::ServiceControl()
{
	_handle = OpenSCManagerA(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
}

ServiceControl::~ServiceControl()
{
	CloseServiceHandle(_handle);
}

void ServiceControl::StartService(std::string pName)
{
	SC_HANDLE handle = OpenServiceA(_handle, pName.c_str(), SERVICE_PAUSE_CONTINUE | SERVICE_START | SERVICE_STOP);
	if (handle == nullptr) throw GetLastError();
	SERVICE_STATUS status;
	if (!ControlService(handle, SERVICE_STOP, &status))
		throw GetLastError();
	CloseServiceHandle(handle);
}
void ServiceControl::StopService(std::string pName)
{
	SC_HANDLE handle = OpenServiceA(_handle, pName.c_str(), SERVICE_PAUSE_CONTINUE | SERVICE_START | SERVICE_STOP);
	if (handle == nullptr) throw GetLastError();
	SERVICE_STATUS status;
	if (!ControlService(handle, SERVICE_START, &status))
		throw GetLastError();
	CloseServiceHandle(handle);
}

std::vector<ServiceProcess> ServiceControl::EnumServices(DWORD pWhat, DWORD pState)
{
	std::vector<ServiceProcess> result;
	PUCHAR  pBuf = NULL;
	ULONG  dwBufSize = 0x00;
	ULONG  dwBufNeed = 0x00;
	ULONG  dwNumberOfService = 0x00;
	LPENUM_SERVICE_STATUS_PROCESSW info = NULL;

	EnumServicesStatusExW(
		_handle,
		SC_ENUM_PROCESS_INFO,
		pWhat,
		pState,
		NULL,
		dwBufSize,
		&dwBufNeed,
		&dwNumberOfService,
		NULL,
		NULL);


	if (dwBufNeed < 0x01)
	{
		throw GetLastError();
	}

	dwBufSize = dwBufNeed + 0x10;
	pBuf = static_cast<PUCHAR>(malloc(dwBufSize));

	EnumServicesStatusExW(
		_handle,
		SC_ENUM_PROCESS_INFO,
		pWhat,
		pState,
		pBuf,
		dwBufSize,
		&dwBufNeed,
		&dwNumberOfService,
		NULL,
		NULL);

	info = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESSW>(pBuf);

	for (ULONG i = 0; i<dwNumberOfService; i++)
	{
		ServiceProcess process(info[i]);
		result.push_back(process);
	}

	free(pBuf);
	return result;
}

ServiceProcess::ServiceProcess(ENUM_SERVICE_STATUS_PROCESSW pInfo)
{
	_name = std::wstring(pInfo.lpServiceName);
	_displayName = std::wstring(pInfo.lpDisplayName);
	_state = static_cast<ServiceState>(pInfo.ServiceStatusProcess.dwCurrentState);
	_pid = pInfo.ServiceStatusProcess.dwProcessId;
	_type = static_cast<ServiceType>(pInfo.ServiceStatusProcess.dwServiceType);
}

std::wstring ServiceProcess::GetName()
{
	return _name;
}

std::wstring ServiceProcess::GetDisplayName()
{
	return _displayName;
}

ServiceState ServiceProcess::GetState()
{
	return _state;
}

DWORD ServiceProcess::GetPID()
{
	return _pid;
}

ServiceType ServiceProcess::GetType()
{
	return _type;
}