//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

// Windows Header Files:


#ifdef PHONE
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
#endif 


#ifndef PHONE
#include <windows.h>
#include <Wininet.h>
#include <Iphlpapi.h>
#include <TlHelp32.h>
#endif


#include "stdio.h"
#include "stdlib.h"
#include<string>
#include <list>


#ifdef PHONE
#include<WPKernel.h>
#include<WPInetAPI.h>
#include<WPShellChromeAPI.h>
#endif
#include<vector>

