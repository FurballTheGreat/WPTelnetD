#pragma once

#include <winapifamily.h>

#include <apiset.h>
#include <apisetcconv.h>
#include <minwindef.h>
#include <minwinbase.h>


typedef struct _TOAST_MESSAGE
{
    CLSID guid;
	LPCWSTR lpTitle;
	LPCWSTR lpContent;
	LPCWSTR lpUri;
	LPCWSTR lpType;
} TOAST_MESSAGE;

extern "C"
WINADVAPI
HRESULT
APIENTRY
Shell_PostMessageToast(
    _In_ TOAST_MESSAGE* toastMessage
    );