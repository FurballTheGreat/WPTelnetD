#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

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

#ifndef PHONE
#define SECURITY_WIN32 
#include "Wininet.h"
#include "Iphlpapi.h"
#include "WTypes.h"
#include "TlHelp32.h"
#include "Wincrypt.h"
#include "Aclapi.h"
#include "Security.h"
#endif

#include<vector>
