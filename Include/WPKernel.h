#pragma once

#include <winapifamily.h>

#include <apiset.h>
#include <apisetcconv.h>
#include <minwindef.h>
#include <minwinbase.h>


extern "C" HANDLE
WINAPI
CreateToolhelp32Snapshot(
    DWORD dwFlags,
    DWORD th32ProcessID
    );

extern "C" WINBASEAPI
DWORD
WINAPI
GetFileAttributesA(
_In_ LPCSTR lpFileName
);

extern "C" DWORD
WINAPI
GetFullPathNameA(
_In_ LPCSTR lpFileName,
_In_ DWORD nBufferLength,
_Out_writes_to_opt_(nBufferLength, return +1) LPSTR lpBuffer,
_Outptr_opt_ LPSTR * lpFilePart
);



/***** Process walking *************************************************/

typedef struct tagPROCESSENTRY32W
{
    DWORD   dwSize;
    DWORD   cntUsage;
    DWORD   th32ProcessID;          // this process
    ULONG_PTR th32DefaultHeapID;
    DWORD   th32ModuleID;           // associated exe
    DWORD   cntThreads;
    DWORD   th32ParentProcessID;    // this process's parent process
    LONG    pcPriClassBase;         // Base priority of process's threads
    DWORD   dwFlags;
    WCHAR   szExeFile[MAX_PATH];    // Path
} PROCESSENTRY32W;
typedef PROCESSENTRY32W *  PPROCESSENTRY32W;
typedef PROCESSENTRY32W *  LPPROCESSENTRY32W;

extern "C" BOOL
WINAPI
Process32FirstW(
    HANDLE hSnapshot,
    LPPROCESSENTRY32W lppe
    );

extern "C" 
WINBASEAPI
LPWSTR
WINAPI
GetCommandLineW(
    VOID
    );


extern "C" BOOL
WINAPI
Process32NextW(
    HANDLE hSnapshot,
    LPPROCESSENTRY32W lppe
    );

extern "C" WINADVAPI
BOOL
APIENTRY
InitiateSystemShutdownExW(
    _In_opt_ LPWSTR lpMachineName,
    _In_opt_ LPWSTR lpMessage,
    _In_ DWORD dwTimeout,
    _In_ BOOL bForceAppsClosed,
    _In_ BOOL bRebootAfterShutdown,
    _In_ DWORD dwReason
    );


extern "C" WINADVAPI
_Success_(return != FALSE) BOOL
WINAPI
LookupAccountSidW(
_In_opt_ LPCWSTR lpSystemName,
_In_ PSID Sid,
_Out_writes_to_opt_(*cchName, *cchName + 1) LPWSTR Name,
_Inout_  LPDWORD cchName,
_Out_writes_to_opt_(*cchReferencedDomainName, *cchReferencedDomainName + 1) LPWSTR ReferencedDomainName,
_Inout_ LPDWORD cchReferencedDomainName,
_Out_ PSID_NAME_USE peUse
);

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;



typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;
extern "C"  WINBASEAPI
BOOL
WINAPI
CreateProcessW(
    _In_opt_ LPCWSTR lpApplicationName,
    _Inout_opt_ LPWSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCWSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOW lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
    );
extern "C"  BOOL
WINAPI
EnumProcesses (
    _Out_writes_bytes_(cb) DWORD * lpidProcess,
    _In_ DWORD cb,
    _Out_ LPDWORD lpcbNeeded
    );

extern "C" WINBASEAPI
HANDLE
WINAPI
CreateNamedPipeA(
    _In_     LPCSTR lpName,
    _In_     DWORD dwOpenMode,
    _In_     DWORD dwPipeMode,
    _In_     DWORD nMaxInstances,
    _In_     DWORD nOutBufferSize,
    _In_     DWORD nInBufferSize,
    _In_     DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

extern "C" WINBASEAPI
HANDLE
WINAPI
OpenProcess(
_In_ DWORD dwDesiredAccess,
_In_ BOOL bInheritHandle,
_In_ DWORD dwProcessId
);

extern "C" WINBASEAPI
BOOL
WINAPI
TerminateProcess(
_In_ HANDLE hProcess,
_In_ UINT uExitCode
);


extern "C" WINBASEAPI
BOOL
WINAPI
CopyFileA(
_In_ LPCSTR lpExistingFileName,
_In_ LPCSTR lpNewFileName,
_In_ BOOL bFailIfExists
);


extern "C" WINBASEAPI
BOOL
WINAPI
MoveFileA(
_In_ LPCSTR lpExistingFileName,
_In_ LPCSTR lpNewFileName
);


extern "C" WINBASEAPI
BOOL
WINAPI
DeleteFileA(
_In_ LPCSTR lpFileName
);


extern "C" WINBASEAPI
BOOL
WINAPI
ConnectNamedPipe(
    _In_ HANDLE hNamedPipe,
    _Inout_opt_ LPOVERLAPPED lpOverlapped
    );

extern "C" WINBASEAPI
BOOL
WINAPI
DisconnectNamedPipe(
    _In_ HANDLE hNamedPipe
    );

extern "C" WINBASEAPI
BOOL
WINAPI
WaitNamedPipeA(
    _In_ LPCSTR lpNamedPipeName,
    _In_ DWORD nTimeOut
    );


extern "C" WINBASEAPI
HANDLE
WINAPI
CreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );


extern "C" WINBASEAPI
VOID
WINAPI
Sleep(
    _In_ DWORD dwMilliseconds
    );

extern "C" WINBASEAPI
HANDLE
WINAPI
CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );

extern "C" WINBASEAPI
BOOL
WINAPI
WaitNamedPipeW(
    _In_ LPCWSTR lpNamedPipeName,
    _In_ DWORD nTimeOut
    );

extern "C" WINBASEAPI
HANDLE
WINAPI
CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
    );

extern "C" WINBASEAPI
HANDLE
WINAPI
CreateNamedPipeW(
    _In_ LPCWSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );


extern "C" HANDLE
WINAPI
CreateToolhelp32Snapshot(
    DWORD dwFlags,
    DWORD th32ProcessID
    );

extern "C" WINBASEAPI
	DWORD
WINAPI
GetCurrentDirectoryA(
    _In_ DWORD nBufferLength,
    _Out_writes_to_opt_(nBufferLength, return + 1) LPSTR lpBuffer
    );

extern "C" WINUSERAPI
DWORD
WINAPI
GetWindowThreadProcessId(
_In_ HWND hWnd,
_Out_opt_ LPDWORD lpdwProcessId);

extern "C"  BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
extern "C"  typedef BOOL(CALLBACK* WNDENUMPROC)(HWND, LPARAM);


extern "C"  WINUSERAPI
BOOL
WINAPI
EnumWindows(
_In_ WNDENUMPROC lpEnumFunc,
_In_ LPARAM lParam);

extern "C" WINADVAPI
BOOL
WINAPI
LookupPrivilegeValueW(
_In_opt_ LPCWSTR lpSystemName,
_In_     LPCWSTR lpName,
_Out_    PLUID   lpLuid
);

//typedef struct _TOKEN_PRIVILEGES {
//	DWORD               PrivilegeCount;
//	LUID_AND_ATTRIBUTES Privileges[ANYSIZE_ARRAY];
//} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;


extern "C" WINADVAPI
BOOL
WINAPI
AdjustTokenPrivileges(
_In_ HANDLE TokenHandle,
_In_ BOOL DisableAllPrivileges,
_In_opt_ PTOKEN_PRIVILEGES NewState,
_In_ DWORD BufferLength,
_Out_writes_bytes_to_opt_(BufferLength, *ReturnLength) PTOKEN_PRIVILEGES PreviousState,
_Out_opt_ PDWORD ReturnLength
);

extern "C"
WINADVAPI
BOOL
WINAPI
OpenProcessToken(
_In_ HANDLE ProcessHandle,
_In_ DWORD DesiredAccess,
_Outptr_ PHANDLE TokenHandle
);

extern "C" WINADVAPI
BOOL
APIENTRY
InitiateSystemShutdownExW(
_In_opt_ LPWSTR lpMachineName,
_In_opt_ LPWSTR lpMessage,
_In_ DWORD dwTimeout,
_In_ BOOL bForceAppsClosed,
_In_ BOOL bRebootAfterShutdown,
_In_ DWORD dwReason
);


extern "C"  WINUSERAPI
int
WINAPI
GetClassNameW(
_In_ HWND hWnd,
_Out_writes_to_(nMaxCount, return) LPWSTR lpClassName,
_In_ int nMaxCount
);

extern "C"  WINUSERAPI
int
WINAPI
GetWindowTextW(
_In_ HWND hWnd,
_Out_writes_(nMaxCount) LPWSTR lpString,
_In_ int nMaxCount);

extern "C"  WINADVAPI
BOOL
WINAPI
GetTokenInformation(
_In_ HANDLE TokenHandle,
_In_ TOKEN_INFORMATION_CLASS TokenInformationClass,
_Out_writes_bytes_to_opt_(TokenInformationLength, *ReturnLength) LPVOID TokenInformation,
_In_ DWORD TokenInformationLength,
_Out_ PDWORD ReturnLength
);

extern "C" WINADVAPI
_Success_(return != FALSE) BOOL
WINAPI
LookupPrivilegeNameW(
_In_opt_ LPCWSTR lpSystemName,
_In_     PLUID   lpLuid,
_Out_writes_to_opt_(*cchName, *cchName + 1) LPWSTR lpName,
_Inout_  LPDWORD cchName
);



extern "C" WINUSERAPI
BOOL
WINAPI
EnumChildWindows(
_In_opt_ HWND hWndParent,
_In_ WNDENUMPROC lpEnumFunc,
_In_ LPARAM lParam);
#define HKEY_CLASSES_ROOT                   (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define HKEY_CURRENT_USER                   (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define HKEY_LOCAL_MACHINE                  (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define HKEY_USERS                          (( HKEY ) (ULONG_PTR)((LONG)0x80000003) )
#define HKEY_PERFORMANCE_DATA               (( HKEY ) (ULONG_PTR)((LONG)0x80000004) )
#define HKEY_PERFORMANCE_TEXT               (( HKEY ) (ULONG_PTR)((LONG)0x80000050) )
#define HKEY_PERFORMANCE_NLSTEXT            (( HKEY ) (ULONG_PTR)((LONG)0x80000060) )

typedef ACCESS_MASK REGSAM;
#define HKEY_CURRENT_CONFIG                 (( HKEY ) (ULONG_PTR)((LONG)0x80000005) )
#define HKEY_DYN_DATA                       (( HKEY ) (ULONG_PTR)((LONG)0x80000006) )
#define HKEY_CURRENT_USER_LOCAL_SETTINGS    (( HKEY ) (ULONG_PTR)((LONG)0x80000007) )
typedef _Return_type_success_(return == ERROR_SUCCESS) LONG LSTATUS;
extern "C" WINADVAPI
LSTATUS
APIENTRY
RegOpenKeyExA(
_In_ HKEY hKey,
_In_opt_ LPCSTR lpSubKey,
_In_opt_ DWORD ulOptions,
_In_ REGSAM samDesired,
_Out_ PHKEY phkResult
);


extern "C" WINADVAPI
WINADVAPI
LSTATUS
APIENTRY
RegDeleteKeyExW(
_In_ HKEY hKey,
_In_ LPCWSTR lpSubKey,
_In_ REGSAM samDesired,
_Reserved_ DWORD Reserved
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegSaveKeyW(
_In_ HKEY hKey,
_In_ LPCWSTR lpFile,
_In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
);

extern "C" WINADVAPI
WINADVAPI
LSTATUS
APIENTRY
RegDeleteTreeW(
_In_ HKEY hKey,
_In_opt_ LPCWSTR lpSubKey
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegCreateKeyExA(
_In_ HKEY hKey,
_In_opt_ LPCSTR lpSubKey,
_In_opt_ DWORD reserved,
_In_opt_ LPCSTR lpClass,
_In_ DWORD options,
_In_ REGSAM samDesired,
_In_opt_ LPVOID security,
_Out_ PHKEY phkResult,
_Out_opt_   LPDWORD lpdwDisposition
);


extern "C" WINADVAPI
LSTATUS
APIENTRY
RegEnumValueA(
_In_ HKEY hKey,
_In_ DWORD dwIndex,
_Out_writes_to_opt_(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
_Inout_ LPDWORD lpcchValueName,
_Reserved_ LPDWORD lpReserved,
_Out_opt_ LPDWORD lpType,
_Out_writes_bytes_to_opt_(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
_Inout_opt_ LPDWORD lpcbData
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegSetValueExA(
_In_ HKEY hKey,
_In_opt_ LPCSTR lpValueName,
_Reserved_ DWORD Reserved,
_In_ DWORD dwType,
_In_reads_bytes_opt_(cbData) CONST BYTE * lpData,
_In_ DWORD cbData
);
extern "C" WINBASEAPI
_Ret_maybenull_
HANDLE
WINAPI
CreateMutexExA(
_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
_In_opt_ LPCSTR lpName,
_In_ DWORD dwFlags,
_In_ DWORD dwDesiredAccess
);
extern "C" WINADVAPI
LSTATUS
APIENTRY
RegGetValueA(
_In_ HKEY hkey,
_In_opt_ LPCSTR lpSubKey,
_In_opt_ LPCSTR lpValue,
_In_ DWORD dwFlags,
_Out_opt_ LPDWORD pdwType,
_When_((dwFlags & 0x7F) == RRF_RT_REG_SZ ||
(dwFlags & 0x7F) == RRF_RT_REG_EXPAND_SZ ||
(dwFlags & 0x7F) == (RRF_RT_REG_SZ | RRF_RT_REG_EXPAND_SZ) ||
*pdwType == REG_SZ ||
*pdwType == REG_EXPAND_SZ, _Post_z_)
_When_((dwFlags & 0x7F) == RRF_RT_REG_MULTI_SZ ||
*pdwType == REG_MULTI_SZ, _Post_ _NullNull_terminated_)
_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PVOID pvData,
_Inout_opt_ LPDWORD pcbData
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegEnumKeyExA(
_In_ HKEY hKey,
_In_ DWORD dwIndex,
_Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName,
_Inout_ LPDWORD lpcchName,
_Reserved_ LPDWORD lpReserved,
_Out_writes_to_opt_(*lpcchClass, *lpcchClass + 1) LPSTR lpClass,
_Inout_opt_ LPDWORD lpcchClass,
_Out_opt_ PFILETIME lpftLastWriteTime
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegCloseKey(
_In_ HKEY hKey
);


extern "C" WINUSERAPI
BOOL
WINAPI
PostMessageW(
_In_opt_ HWND hWnd,
_In_ UINT Msg,
_In_ WPARAM wParam,
_In_ LPARAM lParam);

extern "C" WINBASEAPI
BOOL
WINAPI
SetCurrentDirectoryA(
    _In_ LPCSTR lpPathName
    );
#ifdef PHONE

extern "C" WINBASEAPI
DWORD
WINAPI
GetLogicalDriveStringsW(
    _In_ DWORD nBufferLength,
    _Out_writes_to_opt_(nBufferLength, return + 1) LPWSTR lpBuffer
    );


#ifdef UNICODE
#define GetLogicalDriveStrings  GetLogicalDriveStringsW
#endif


#define NET_API_STATUS          DWORD
#define API_RET_TYPE            NET_API_STATUS      // Old value: do not use
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NET_API_FUNCTION    __stdcall
#else
#define NET_API_FUNCTION
#endif
#define LMSTR   LPWSTR
_Check_return_
_Success_( return == 0 || return == ERROR_MORE_DATA )
extern "C" NET_API_STATUS NET_API_FUNCTION
NetShareEnum(
    _In_opt_ LMSTR servername,
    _In_ DWORD level,
    _When_( level == 0,_Outptr_result_bytebuffer_(*entriesread * sizeof(SHARE_INFO_0)) )
    _When_( level == 1, _Outptr_result_bytebuffer_(*entriesread * sizeof(SHARE_INFO_1)) )
    _When_( level == 2, _Outptr_result_bytebuffer_(*entriesread * sizeof(SHARE_INFO_2)) )
    _When_( level == 502, _Outptr_result_bytebuffer_(*entriesread * sizeof(SHARE_INFO_502)) )
    _When_( level == 503, _Outptr_result_bytebuffer_(*entriesread * sizeof(SHARE_INFO_503)) )
    LPBYTE *bufptr,
    _In_ DWORD prefmaxlen,
    _Out_ LPDWORD entriesread,
    _Out_ LPDWORD totalentries,
    _Inout_opt_ LPDWORD resume_handle
    );



extern "C" NET_API_STATUS NET_API_FUNCTION
NetApiBufferFree (
    IN LPVOID Buffer
    );

extern "C" WINBASEAPI
_Ret_maybenull_
HANDLE
WINAPI
CreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ __drv_aliasesMem LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId
    );

extern "C" WINBASEAPI
BOOL
WINAPI
SetNamedPipeHandleState(
    _In_ HANDLE hNamedPipe,
    _In_opt_ LPDWORD lpMode,
    _In_opt_ LPDWORD lpMaxCollectionCount,
    _In_opt_ LPDWORD lpCollectDataTimeout
    );


extern "C"  WINBASEAPI
_Success_(return != 0 && return < nBufferLength)
DWORD
WINAPI
GetCurrentDirectoryW(
    _In_ DWORD nBufferLength,
    _Out_writes_to_opt_(nBufferLength, return + 1) LPWSTR lpBuffer
    );
typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

extern "C" WINBASEAPI
DWORD
WINAPI
WaitForSingleObject(
    _In_ HANDLE hHandle,
    _In_ DWORD dwMilliseconds
    );

extern "C" WINBASEAPI
BOOL
WINAPI
SetHandleInformation(
    _In_ HANDLE hObject,
    _In_ DWORD dwMask,
    _In_ DWORD dwFlags
    );



extern "C" WINBASEAPI
BOOL
WINAPI
CreatePipe(
    _Out_ PHANDLE hReadPipe,
    _Out_ PHANDLE hWritePipe,
    _In_opt_ LPSECURITY_ATTRIBUTES lpPipeAttributes,
    _In_ DWORD nSize
    );

extern "C" WINBASEAPI
BOOL
WINAPI
CreateProcessA(
    _In_opt_ LPCSTR lpApplicationName,
    _Inout_opt_ LPSTR lpCommandLine,
    _In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ BOOL bInheritHandles,
    _In_ DWORD dwCreationFlags,
    _In_opt_ LPVOID lpEnvironment,
    _In_opt_ LPCSTR lpCurrentDirectory,
    _In_ LPSTARTUPINFOA lpStartupInfo,
    _Out_ LPPROCESS_INFORMATION lpProcessInformation
    );

extern "C"  WINBASEAPI
_NullNull_terminated_
LPCH
WINAPI
GetEnvironmentStrings(
    VOID
    );

extern "C" WINBASEAPI
BOOL
WINAPI
TerminateThread(
    _In_ HANDLE hThread,
    _In_ DWORD dwExitCode
    );


extern "C" WINBASEAPI
BOOL
WINAPI
ReadFile(
_In_ HANDLE hFile,
_Out_writes_bytes_to_opt_(nNumberOfBytesToRead, *lpNumberOfBytesRead) __out_data_source(FILE) LPVOID lpBuffer,
_In_ DWORD nNumberOfBytesToRead,
_Out_opt_ LPDWORD lpNumberOfBytesRead,
_Inout_opt_ LPOVERLAPPED lpOverlapped
);

#define HANDLE_FLAG_INHERIT             0x00000001
#define STARTF_USESTDHANDLES       0x00000100
#endif


typedef void * I_RPC_HANDLE;
typedef I_RPC_HANDLE RPC_BINDING_HANDLE;
typedef RPC_BINDING_HANDLE handle_t;
#define rpc_binding_handle_t RPC_BINDING_HANDLE

typedef I_RPC_HANDLE * RPC_EP_INQ_HANDLE;
typedef _Return_type_success_(return == 0) long RPC_STATUS;
typedef _Null_terminated_ unsigned char* RPC_CSTR;


#define RPCRTAPI DECLSPEC_IMPORT
#	define  RPC_ENTRY __stdcall
extern "C"  RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcStringBindingComposeA(
_In_opt_ RPC_CSTR ObjUuid,
_In_opt_ RPC_CSTR ProtSeq,
_In_opt_ RPC_CSTR NetworkAddr,
_In_opt_ RPC_CSTR Endpoint,
_In_opt_ RPC_CSTR Options,
_Outptr_opt_ RPC_CSTR * StringBinding
);
typedef struct _RPC_IF_ID
{
	GUID Uuid;
	unsigned short VersMajor;
	unsigned short VersMinor;
} RPC_IF_ID;
extern "C"  RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcBindingFromStringBindingA(
_In_ RPC_CSTR StringBinding,
_Out_ RPC_BINDING_HANDLE* Binding
);

extern "C" RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcStringFreeA(
_Inout_ RPC_CSTR* String
);
typedef GUID UUID;
extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcMgmtEpEltInqBegin(
_In_opt_ OPTIONAL RPC_BINDING_HANDLE EpBinding,
_In_ unsigned long InquiryType,
_In_opt_ RPC_IF_ID* IfId,
_In_opt_ unsigned long VersOption,
_In_opt_ UUID* ObjectUuid,
_Out_ RPC_EP_INQ_HANDLE* InquiryContext
);

#define  RPC_C_EP_ALL_ELTS        0
#define  RPC_C_EP_MATCH_BY_IF     1
#define  RPC_C_EP_MATCH_BY_OBJ    2
#define  RPC_C_EP_MATCH_BY_BOTH   3
#define __RPC_FAR

#define RPC_C_AUTHN_NONE          0
#define RPC_C_AUTHN_DCE_PRIVATE   1
#define RPC_C_AUTHN_DCE_PUBLIC    2
#define RPC_C_AUTHN_DEC_PUBLIC    4
#define RPC_C_AUTHN_GSS_NEGOTIATE 9
#define RPC_C_AUTHN_WINNT        10
#define RPC_C_AUTHN_GSS_SCHANNEL 14
#define RPC_C_AUTHN_GSS_KERBEROS 16
#define RPC_C_AUTHN_DPA          17
#define RPC_C_AUTHN_MSN          18

typedef struct
{
	unsigned long Count;
	RPC_IF_ID* IfId[1];
} RPC_IF_ID_VECTOR;

typedef struct
{
	unsigned int Count;
	unsigned long Stats[1];
} RPC_STATS_VECTOR;
extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcMgmtEpEltInqNextA(
_In_ RPC_EP_INQ_HANDLE InquiryContext,
_Out_ RPC_IF_ID __RPC_FAR * IfId,
_Out_opt_ RPC_BINDING_HANDLE __RPC_FAR * Binding,
_Out_opt_ UUID __RPC_FAR * ObjectUuid,
_Outptr_opt_ RPC_CSTR __RPC_FAR * Annotation
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
UuidFromStringA(
_In_opt_ RPC_CSTR StringUuid,
_Out_ UUID __RPC_FAR * Uuid
);
extern "C" RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcBindingFree(
_Inout_ RPC_BINDING_HANDLE* Binding
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcMgmtInqIfIds(
_In_opt_ RPC_BINDING_HANDLE Binding,
_Outptr_ RPC_IF_ID_VECTOR __RPC_FAR * __RPC_FAR * IfIdVector
);

extern "C" RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcMgmtStatsVectorFree(
_Inout_ RPC_STATS_VECTOR ** StatsVector
);

#undef RpcMgmtInqStats
/* server */
extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcMgmtInqStats(
_In_opt_ RPC_BINDING_HANDLE Binding,
_Outptr_ RPC_STATS_VECTOR ** Statistics
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcMgmtInqServerPrincNameA (
    _In_opt_ RPC_BINDING_HANDLE Binding,
    _In_ unsigned long AuthnSvc,
    _Outptr_ RPC_CSTR __RPC_FAR * ServerPrincName
    );

extern "C" RPCRTAPI
RPC_STATUS
RPC_ENTRY
RpcIfIdVectorFree(
_Inout_ RPC_IF_ID_VECTOR __RPC_FAR * __RPC_FAR * IfIdVector
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcStringBindingParseA(
_In_ RPC_CSTR StringBinding,
_Outptr_opt_ RPC_CSTR __RPC_FAR * ObjUuid,
_Outptr_opt_ RPC_CSTR __RPC_FAR * Protseq,
_Outptr_opt_ RPC_CSTR __RPC_FAR * NetworkAddr,
_Outptr_opt_ RPC_CSTR __RPC_FAR * Endpoint,
_Outptr_opt_ RPC_CSTR __RPC_FAR * NetworkOptions
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
RpcBindingToStringBindingA(
_In_ RPC_BINDING_HANDLE Binding,
_Outptr_ RPC_CSTR* StringBinding
);

extern "C" RPCRTAPI
_Must_inspect_result_
RPC_STATUS
RPC_ENTRY
UuidToStringA(
_In_ const UUID __RPC_FAR * Uuid,
_Outptr_ RPC_CSTR __RPC_FAR * StringUuid
);


extern "C" WINBASEAPI
UINT
WINAPI
GetSystemFirmwareTable(
_In_ DWORD FirmwareTableProviderSignature,
_In_ DWORD FirmwareTableID,
_Out_writes_bytes_to_opt_(BufferSize, return) PVOID pFirmwareTableBuffer,
_In_ DWORD BufferSize
);

typedef struct _CRYPTOAPI_BLOB {
	DWORD   cbData;
	_Field_size_bytes_(cbData)  BYTE    *pbData;
} CRYPT_INTEGER_BLOB, *PCRYPT_INTEGER_BLOB,
CRYPT_UINT_BLOB, *PCRYPT_UINT_BLOB,
CRYPT_OBJID_BLOB, *PCRYPT_OBJID_BLOB,
CERT_NAME_BLOB, *PCERT_NAME_BLOB,
CERT_RDN_VALUE_BLOB, *PCERT_RDN_VALUE_BLOB,
CERT_BLOB, *PCERT_BLOB,
CRL_BLOB, *PCRL_BLOB,
DATA_BLOB, *PDATA_BLOB,
CRYPT_DATA_BLOB, *PCRYPT_DATA_BLOB,
CRYPT_HASH_BLOB, *PCRYPT_HASH_BLOB,
CRYPT_DIGEST_BLOB, *PCRYPT_DIGEST_BLOB,
CRYPT_DER_BLOB, *PCRYPT_DER_BLOB,
CRYPT_ATTR_BLOB, *PCRYPT_ATTR_BLOB;

typedef struct _CRYPT_ALGORITHM_IDENTIFIER {
	LPSTR               pszObjId;
	CRYPT_OBJID_BLOB    Parameters;
} CRYPT_ALGORITHM_IDENTIFIER, *PCRYPT_ALGORITHM_IDENTIFIER;


typedef struct _CRYPT_BIT_BLOB {
	DWORD   cbData;
	BYTE    *pbData;
	DWORD   cUnusedBits;
} CRYPT_BIT_BLOB, *PCRYPT_BIT_BLOB;

typedef struct _CERT_PUBLIC_KEY_INFO {
	CRYPT_ALGORITHM_IDENTIFIER    Algorithm;
	CRYPT_BIT_BLOB                PublicKey;
} CERT_PUBLIC_KEY_INFO, *PCERT_PUBLIC_KEY_INFO;


typedef struct _CERT_EXTENSION {
	LPSTR               pszObjId;
	BOOL                fCritical;
	CRYPT_OBJID_BLOB    Value;
} CERT_EXTENSION, *PCERT_EXTENSION;
typedef const CERT_EXTENSION* PCCERT_EXTENSION;

typedef struct _CERT_INFO {
	DWORD                       dwVersion;
	CRYPT_INTEGER_BLOB          SerialNumber;
	CRYPT_ALGORITHM_IDENTIFIER  SignatureAlgorithm;
	CERT_NAME_BLOB              Issuer;
	FILETIME                    NotBefore;
	FILETIME                    NotAfter;
	CERT_NAME_BLOB              Subject;
	CERT_PUBLIC_KEY_INFO        SubjectPublicKeyInfo;
	CRYPT_BIT_BLOB              IssuerUniqueId;
	CRYPT_BIT_BLOB              SubjectUniqueId;
	DWORD                       cExtension;
	PCERT_EXTENSION             rgExtension;
} CERT_INFO, *PCERT_INFO;



typedef void *HCERTSTORE;

typedef struct _CERT_CONTEXT {
	DWORD                   dwCertEncodingType;
	BYTE                    *pbCertEncoded;
	DWORD                   cbCertEncoded;
	PCERT_INFO              pCertInfo;
	HCERTSTORE              hCertStore;
} CERT_CONTEXT, *PCERT_CONTEXT;
typedef const CERT_CONTEXT *PCCERT_CONTEXT;
#define WINCRYPT32API DECLSPEC_IMPORT
extern "C" WINCRYPT32API
PCCERT_CONTEXT
WINAPI
CertEnumCertificatesInStore(
_In_ HCERTSTORE hCertStore,
_In_opt_ PCCERT_CONTEXT pPrevCertContext
);

WINBASEAPI
FARPROC
WINAPI
GetProcAddress(
_In_ HMODULE hModule,
_In_ LPCSTR lpProcName
);

typedef PCCERT_CONTEXT(WINAPI *PCertEnumCertificatesInStore)(
	_In_ HCERTSTORE,
	_In_opt_ PCCERT_CONTEXT);

//
//extern "C" WINCRYPT32API
//HCERTSTORE
//WINAPI
//CertOpenSystemStoreA(
//_In_opt_ DWORD*      hProv,
//_In_ LPCSTR            szSubsystemProtocol
//);
// By default, when the CurrentUser "Root" store is opened, any SystemRegistry
// roots not also on the protected root list are deleted from the cache before
// CertOpenStore() returns. Set the following flag to return all the roots
// in the SystemRegistry without checking the protected root list.
#define CERT_SYSTEM_STORE_UNPROTECTED_FLAG      0x40000000

#define CERT_SYSTEM_STORE_DEFER_READ_FLAG       0x20000000

// Location of the system store:
#define CERT_SYSTEM_STORE_LOCATION_MASK         0x00FF0000
#define CERT_SYSTEM_STORE_LOCATION_SHIFT        16


//  Registry: HKEY_CURRENT_USER or HKEY_LOCAL_MACHINE
#define CERT_SYSTEM_STORE_CURRENT_USER_ID       1
#define CERT_SYSTEM_STORE_LOCAL_MACHINE_ID      2
//  Registry: HKEY_LOCAL_MACHINE\Software\Microsoft\Cryptography\Services
#define CERT_SYSTEM_STORE_CURRENT_SERVICE_ID    4
#define CERT_SYSTEM_STORE_SERVICES_ID           5
//  Registry: HKEY_USERS
#define CERT_SYSTEM_STORE_USERS_ID              6

//  Registry: HKEY_CURRENT_USER\Software\Policies\Microsoft\SystemCertificates
#define CERT_SYSTEM_STORE_CURRENT_USER_GROUP_POLICY_ID    7
//  Registry: HKEY_LOCAL_MACHINE\Software\Policies\Microsoft\SystemCertificates
#define CERT_SYSTEM_STORE_LOCAL_MACHINE_GROUP_POLICY_ID   8

//  Registry: HKEY_LOCAL_MACHINE\Software\Microsoft\EnterpriseCertificates
#define CERT_SYSTEM_STORE_LOCAL_MACHINE_ENTERPRISE_ID     9

#define CERT_SYSTEM_STORE_CURRENT_USER          \
    (CERT_SYSTEM_STORE_CURRENT_USER_ID << CERT_SYSTEM_STORE_LOCATION_SHIFT)
#define CERT_SYSTEM_STORE_LOCAL_MACHINE         \
    (CERT_SYSTEM_STORE_LOCAL_MACHINE_ID << CERT_SYSTEM_STORE_LOCATION_SHIFT)
#define CERT_SYSTEM_STORE_CURRENT_SERVICE       \
    (CERT_SYSTEM_STORE_CURRENT_SERVICE_ID << CERT_SYSTEM_STORE_LOCATION_SHIFT)
#define CERT_SYSTEM_STORE_SERVICES              \
    (CERT_SYSTEM_STORE_SERVICES_ID << CERT_SYSTEM_STORE_LOCATION_SHIFT)
#define CERT_SYSTEM_STORE_USERS                 \
    (CERT_SYSTEM_STORE_USERS_ID << CERT_SYSTEM_STORE_LOCATION_SHIFT)

#define CERT_SYSTEM_STORE_CURRENT_USER_GROUP_POLICY   \
    (CERT_SYSTEM_STORE_CURRENT_USER_GROUP_POLICY_ID << \
        CERT_SYSTEM_STORE_LOCATION_SHIFT)
#define CERT_SYSTEM_STORE_LOCAL_MACHINE_GROUP_POLICY  \
    (CERT_SYSTEM_STORE_LOCAL_MACHINE_GROUP_POLICY_ID << \
        CERT_SYSTEM_STORE_LOCATION_SHIFT)

#define CERT_SYSTEM_STORE_LOCAL_MACHINE_ENTERPRISE  \
    (CERT_SYSTEM_STORE_LOCAL_MACHINE_ENTERPRISE_ID << \
        CERT_SYSTEM_STORE_LOCATION_SHIFT)

#define CERT_STORE_PROV_SYSTEM_W            ((LPCSTR) 10)

//WINCRYPT32API
//_Must_inspect_result_
//HCERTSTORE
//WINAPI
//CertOpenStore(
//_In_ LPCSTR lpszStoreProvider,
//_In_ DWORD dwEncodingType,
//_In_opt_ HCRYPTPROV_LEGACY hCryptProv,
//_In_ DWORD dwFlags,
//_In_opt_ const void *pvPara
//);
#define CERT_STORE_PROV_SYSTEM_A            ((LPCSTR) 9)
typedef HCERTSTORE(WINAPI *PCertOpenStore)(
	_In_ LPCSTR,
	_In_ DWORD,
	_In_opt_ DWORD* ,
	_In_ DWORD ,
	_In_opt_ const void *);


typedef HCERTSTORE (WINAPI *PCertOpenSystemStoreA)(
	_In_opt_ DWORD*,
	_In_ LPCSTR);

extern "C" WINCRYPT32API
BOOL
WINAPI
CertCloseStore(
_In_opt_ HCERTSTORE hCertStore,
_In_ DWORD dwFlags
);


typedef BOOL (WINAPI *PCertCloseStore)(
	HCERTSTORE,
	_In_ DWORD);


extern "C" WINBASEAPI
_Ret_maybenull_
HMODULE
WINAPI
LoadLibraryA(
_In_ LPCSTR lpLibFileName
);


typedef DWORD (WINAPI *PCertGetNameStringA)(
	_In_ PCCERT_CONTEXT,
	_In_ DWORD,
	_In_ DWORD,
	_In_opt_ void *,
	_Out_writes_to_opt_(cchNameString, return) LPSTR,
	_In_ DWORD cchNameString);

extern "C" WINCRYPT32API
DWORD
WINAPI
CertGetNameStringA(
_In_ PCCERT_CONTEXT pCertContext,
_In_ DWORD dwType,
_In_ DWORD dwFlags,
_In_opt_ void *pvTypePara,
_Out_writes_to_opt_(cchNameString, return) LPSTR pszNameString,
_In_ DWORD cchNameString
);

#define CERT_NAME_EMAIL_TYPE            1
#define CERT_NAME_RDN_TYPE              2
#define CERT_NAME_ATTR_TYPE             3
#define CERT_NAME_SIMPLE_DISPLAY_TYPE   4
#define CERT_NAME_FRIENDLY_DISPLAY_TYPE 5
#define CERT_NAME_DNS_TYPE              6
#define CERT_NAME_URL_TYPE              7
#define CERT_NAME_UPN_TYPE              8
#define WINOLEAPI        STDAPI

_Check_return_ WINOLEAPI
IIDFromString(
_In_  LPCWSTR lpsz,
_Out_ LPIID lpiid
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegDeleteValueA(
_In_ HKEY hKey,
_In_opt_ LPCSTR lpValueName
);

extern "C" WINADVAPI
LSTATUS
APIENTRY
RegDeleteValueW(
_In_ HKEY hKey,
_In_opt_ LPCWSTR lpValueName
);

typedef _Null_terminated_ WCHAR *BSTR;
typedef HRESULT(WINAPI *PDMProcessConfigXMLFiltered)(
	LPCWSTR pszXmlIn,
	const WCHAR **rgszAllowedCspNode,
	const DWORD dwNumAllowedCspNodes,
	BSTR* pbstrXmlOut);



extern "C" WINBASEAPI
BOOL
WINAPI
DefineDosDeviceW(
_In_ DWORD dwFlags,
_In_ LPCWSTR lpDeviceName,
_In_opt_ LPCWSTR lpTargetPath
);


typedef HRESULT(WINAPI *PActivateExternalVoicemail)(
	);


typedef enum _SE_OBJECT_TYPE
{
	SE_UNKNOWN_OBJECT_TYPE = 0,
	SE_FILE_OBJECT,
	SE_SERVICE,
	SE_PRINTER,
	SE_REGISTRY_KEY,
	SE_LMSHARE,
	SE_KERNEL_OBJECT,
	SE_WINDOW_OBJECT,
	SE_DS_OBJECT,
	SE_DS_OBJECT_ALL,
	SE_PROVIDER_DEFINED_OBJECT,
	SE_WMIGUID_OBJECT,
	SE_REGISTRY_WOW64_32KEY,
} SE_OBJECT_TYPE;

typedef HRESULT(WINAPI *PGetNamedSecurityInfoW)(
	_In_  LPCWSTR               pObjectName,
	_In_  SE_OBJECT_TYPE         ObjectType,
	_In_  SECURITY_INFORMATION   SecurityInfo,
	_Out_opt_       PSID         * ppsidOwner,
	_Out_opt_       PSID         * ppsidGroup,
	_Out_opt_       PACL         * ppDacl,
	_Out_opt_       PACL         * ppSacl,
	_Out_ PSECURITY_DESCRIPTOR   * ppSecurityDescriptor
	);

typedef HRESULT(WINAPI *PGetSecurityInfo)(
	_In_  HANDLE                 handle,
	_In_  SE_OBJECT_TYPE         ObjectType,
	_In_  SECURITY_INFORMATION   SecurityInfo,
	_Out_opt_ PSID                 * ppsidOwner,
	_Out_opt_ PSID                 * ppsidGroup,
	_Out_opt_ PACL                 * ppDacl,
	_Out_opt_ PACL                 * ppSacl,
	_Out_opt_ PSECURITY_DESCRIPTOR * ppSecurityDescriptor
	);


extern "C" WINADVAPI
BOOL
WINAPI
GetSecurityDescriptorDacl(
_In_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
_Out_ LPBOOL lpbDaclPresent,
_Outptr_ PACL * pDacl,
_Out_ LPBOOL lpbDaclDefaulted
);

extern "C" WINADVAPI
BOOL
WINAPI
GetSecurityDescriptorGroup(
_In_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
_Outptr_ PSID * pGroup,
_Out_ LPBOOL lpbGroupDefaulted
);


extern "C" WINADVAPI
BOOL
WINAPI
GetSecurityDescriptorOwner(
_In_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
_Outptr_ PSID * pOwner,
_Out_ LPBOOL lpbOwnerDefaulted
);


extern "C" WINADVAPI
LSTATUS
APIENTRY
RegGetKeySecurity(
_In_ HKEY hKey,
_In_ SECURITY_INFORMATION SecurityInformation,
_Out_writes_bytes_opt_(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
_Inout_ LPDWORD lpcbSecurityDescriptor
);

extern "C" WINADVAPI
BOOL
WINAPI
GetAclInformation(
_In_ PACL pAcl,
_Out_writes_bytes_(nAclInformationLength) LPVOID pAclInformation,
_In_ DWORD nAclInformationLength,
_In_ ACL_INFORMATION_CLASS dwAclInformationClass
);

extern "C" WINADVAPI
BOOL
WINAPI
GetAce(
_In_ PACL pAcl,
_In_ DWORD dwAceIndex,
_Outptr_ LPVOID * pAce
);


extern "C" WINADVAPI
_Success_(return != FALSE) BOOL
WINAPI
LookupAccountSidW(
_In_opt_ LPCWSTR lpSystemName,
_In_ PSID Sid,
_Out_writes_to_opt_(*cchName, *cchName + 1) LPWSTR Name,
_Inout_  LPDWORD cchName,
_Out_writes_to_opt_(*cchReferencedDomainName, *cchReferencedDomainName + 1) LPWSTR ReferencedDomainName,
_Inout_ LPDWORD cchReferencedDomainName,
_Out_ PSID_NAME_USE peUse
);

//extern "C" WINBASEAPI
//HRESULT
//WINAPI
//ActivateExternalVoicemail();

typedef enum
{
	NameUnknown = 0,
	NameFullyQualifiedDN = 1,
	NameSamCompatible = 2,
	NameDisplay = 3,
	NameUniqueId = 6,
	NameCanonical = 7,
	NameUserPrincipal = 8,
	NameCanonicalEx = 9,
	NameServicePrincipal = 10,
	NameDnsDomain = 12,
	NameGivenName = 13,
	NameSurname = 14
} EXTENDED_NAME_FORMAT, *PEXTENDED_NAME_FORMAT;

typedef BOOLEAN(WINAPI *PGetUserNameExA)(
	_In_ EXTENDED_NAME_FORMAT  NameFormat,
	_Out_writes_to_opt_(*nSize, *nSize) LPSTR lpNameBuffer,
	_Inout_ PULONG nSize
	);


extern "C" BOOL
WINAPI
ConvertSidToStringSidA(
_In_ PSID Sid,
_Outptr_ LPSTR * StringSid
);

extern "C" WINBASEAPI
HLOCAL
WINAPI
LocalFree(
    _Pre_opt_valid_ HLOCAL hMem
    );



typedef struct _MEMORYSTATUSEX {
	DWORD dwLength;
	DWORD dwMemoryLoad;
	DWORDLONG ullTotalPhys;
	DWORDLONG ullAvailPhys;
	DWORDLONG ullTotalPageFile;
	DWORDLONG ullAvailPageFile;
	DWORDLONG ullTotalVirtual;
	DWORDLONG ullAvailVirtual;
	DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

extern "C" WINBASEAPI
BOOL
WINAPI
GlobalMemoryStatusEx(
_Out_ LPMEMORYSTATUSEX lpBuffer
);

#define NET_API_FUNCTION    __stdcall
#define NET_API_STATUS          DWORD

#define MAX_PREFERRED_LENGTH    ((DWORD) -1)
#define FILTER_NORMAL_ACCOUNT               (0x0002)


typedef HRESULT(WINAPI *PNetUserEnum)(
	_In_opt_    LPCWSTR     servername OPTIONAL,
	_In_        DWORD      level,
	_In_        DWORD      filter,
	_Outptr_result_buffer_(_Inexpressible_("varies")) LPBYTE     *bufptr,
	_In_        DWORD      prefmaxlen,
	_Out_       LPDWORD    entriesread,
	_Out_       LPDWORD    totalentries,
	_Inout_opt_ PDWORD resume_handle OPTIONAL
	);

typedef struct _USER_INFO_0 {
	LPWSTR   usri0_name;
}USER_INFO_0, *PUSER_INFO_0, *LPUSER_INFO_0;


extern "C" WINBASEAPI
HANDLE
WINAPI
FindFirstFileA(
_In_ LPCSTR lpFileName,
_Out_ LPWIN32_FIND_DATAA lpFindFileData
);


extern "C" WINBASEAPI
HRESULT
WINAPI CreateProcessInChamber(
	LPCWSTR lpChamberSid, 
	LPCWSTR lpChamberName, 
	LPCWSTR lpApplicationName, 
	LPWSTR lpCommandLine, 
	BOOL bInheritHandles, 
	DWORD dwCreationFlags, 
	LPCWSTR lpCurrentDirectory, 
	LPSTARTUPINFOW lpStartupInfo, 
	LPPROCESS_INFORMATION lpProcessInformation);


extern "C" WINBASEAPI HRESULT WINAPI GetChamberSidFromId(PWCHAR pId, PWCHAR *pSid);