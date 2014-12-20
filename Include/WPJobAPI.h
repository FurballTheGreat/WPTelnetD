#pragma once
#include <winapifamily.h>

#include <apiset.h>
#include <apisetcconv.h>
#include <minwindef.h>
#include <minwinbase.h>
extern "C" WINBASEAPI
BOOL
WINAPI
QueryInformationJobObject(
    _In_opt_  HANDLE hJob,
    _In_      JOBOBJECTINFOCLASS JobObjectInformationClass,
    _Out_writes_bytes_to_(cbJobObjectInformationLength, *lpReturnLength) LPVOID lpJobObjectInformation,
    _In_      DWORD cbJobObjectInformationLength,
    _Out_opt_ LPDWORD lpReturnLength
    );

extern "C" WINBASEAPI
BOOL
WINAPI
IsProcessInJob(
    _In_ HANDLE ProcessHandle,
    _In_opt_ HANDLE JobHandle,
    _Out_ PBOOL Result
    );
