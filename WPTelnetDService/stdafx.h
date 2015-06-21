// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>


#ifndef WINAPI_PARTITION_DESKTOP 
#define WINAPI_PARTITION_DESKTOP 1
#endif
#include "winsock2.h"
#include <winuser.h>
#include <processenv.h>
#include <handleapi.h>
#include <fileapi.h>
#include <processthreadsapi.h>




#include "stdio.h"
#include "stdlib.h"
#include<string>
#include <list>


#ifdef PHONE
#include<WPKernel.h>
#include<WPInetAPI.h>
#include<WPShellChromeAPI.h>
#include<WPIPHldAPI.h>
#endif

#include<vector>

#ifdef PHONE
typedef VOID(WINAPI *LPSERVICE_MAIN_FUNCTIONW)(
	DWORD   dwNumServicesArgs,
	LPWSTR  *lpServiceArgVectors
	);



typedef struct _SERVICE_TABLE_ENTRYW {
	LPWSTR                      lpServiceName;
	LPSERVICE_MAIN_FUNCTIONW    lpServiceProc;
}SERVICE_TABLE_ENTRYW, *LPSERVICE_TABLE_ENTRYW;


DECLARE_HANDLE(SERVICE_STATUS_HANDLE);

typedef VOID WINAPI HANDLER_FUNCTION(
	DWORD    dwControl
	);

typedef VOID(WINAPI *LPHANDLER_FUNCTION)(
	DWORD    dwControl
	);
extern "C" {
_Must_inspect_result_
WINADVAPI
SERVICE_STATUS_HANDLE
WINAPI
RegisterServiceCtrlHandlerW(
_In_    LPCWSTR                    lpServiceName,
_In_    __callback
LPHANDLER_FUNCTION          lpHandlerProc
);

WINADVAPI
BOOL
WINAPI
SetServiceStatus(
_In_        SERVICE_STATUS_HANDLE   hServiceStatus,
_In_        LPSERVICE_STATUS        lpServiceStatus
);

WINADVAPI
BOOL
WINAPI
StartServiceCtrlDispatcherW(
_In_  CONST  SERVICE_TABLE_ENTRYW   *lpServiceStartTable
);

WINADVAPI
BOOL
WINAPI
StartServiceCtrlDispatcherW(
_In_ CONST  SERVICE_TABLE_ENTRYW    *lpServiceStartTable
);

}
//
// Controls
//
#define SERVICE_CONTROL_STOP                   0x00000001
#define SERVICE_CONTROL_PAUSE                  0x00000002
#define SERVICE_CONTROL_CONTINUE               0x00000003
#define SERVICE_CONTROL_INTERROGATE            0x00000004
#define SERVICE_CONTROL_SHUTDOWN               0x00000005
#define SERVICE_CONTROL_PARAMCHANGE            0x00000006
#define SERVICE_CONTROL_NETBINDADD             0x00000007
#define SERVICE_CONTROL_NETBINDREMOVE          0x00000008
#define SERVICE_CONTROL_NETBINDENABLE          0x00000009
#define SERVICE_CONTROL_NETBINDDISABLE         0x0000000A
#define SERVICE_CONTROL_DEVICEEVENT            0x0000000B
#define SERVICE_CONTROL_HARDWAREPROFILECHANGE  0x0000000C
#define SERVICE_CONTROL_POWEREVENT             0x0000000D
#define SERVICE_CONTROL_SESSIONCHANGE          0x0000000E
#define SERVICE_CONTROL_PRESHUTDOWN            0x0000000F
#define SERVICE_CONTROL_TIMECHANGE             0x00000010
#define SERVICE_CONTROL_TRIGGEREVENT           0x00000020


//
// Controls Accepted  (Bit Mask)
//
#define SERVICE_ACCEPT_STOP                    0x00000001
#define SERVICE_ACCEPT_PAUSE_CONTINUE          0x00000002
#define SERVICE_ACCEPT_SHUTDOWN                0x00000004
#define SERVICE_ACCEPT_PARAMCHANGE             0x00000008
#define SERVICE_ACCEPT_NETBINDCHANGE           0x00000010
#define SERVICE_ACCEPT_HARDWAREPROFILECHANGE   0x00000020
#define SERVICE_ACCEPT_POWEREVENT              0x00000040
#define SERVICE_ACCEPT_SESSIONCHANGE           0x00000080
#define SERVICE_ACCEPT_PRESHUTDOWN             0x00000100
#define SERVICE_ACCEPT_TIMECHANGE              0x00000200
#define SERVICE_ACCEPT_TRIGGEREVENT            0x00000400

#endif