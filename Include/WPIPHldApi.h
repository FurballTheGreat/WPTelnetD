#pragma once

#include <winapifamily.h>

#include <apiset.h>
#include <apisetcconv.h>
#include <minwindef.h>
#include <minwinbase.h>

//
// TCP states, as defined in the MIB.
//
typedef enum {
    MIB_TCP_STATE_CLOSED     =  1,
    MIB_TCP_STATE_LISTEN     =  2,
    MIB_TCP_STATE_SYN_SENT   =  3,
    MIB_TCP_STATE_SYN_RCVD   =  4,
    MIB_TCP_STATE_ESTAB      =  5,
    MIB_TCP_STATE_FIN_WAIT1  =  6,
    MIB_TCP_STATE_FIN_WAIT2  =  7,
    MIB_TCP_STATE_CLOSE_WAIT =  8,
    MIB_TCP_STATE_CLOSING    =  9,
    MIB_TCP_STATE_LAST_ACK   = 10,
    MIB_TCP_STATE_TIME_WAIT  = 11,
    MIB_TCP_STATE_DELETE_TCB = 12,
} MIB_TCP_STATE;

//
// Various Offload states a TCP connection can be in.
//
typedef enum {
    TcpConnectionOffloadStateInHost,
    TcpConnectionOffloadStateOffloading,
    TcpConnectionOffloadStateOffloaded,
    TcpConnectionOffloadStateUploading,
    TcpConnectionOffloadStateMax
} TCP_CONNECTION_OFFLOAD_STATE, *PTCP_CONNECTION_OFFLOAD_STATE;

typedef struct _MIB_TCPROW_LH {
    union {
        DWORD dwState;       // Old field used DWORD type.
        MIB_TCP_STATE State; // New field uses enum type.
    };
    DWORD dwLocalAddr;
    DWORD dwLocalPort;
    DWORD dwRemoteAddr;
    DWORD dwRemotePort;
} MIB_TCPROW_LH, *PMIB_TCPROW_LH;

typedef struct _MIB_TCPROW_W2K {
    DWORD       dwState;
    DWORD       dwLocalAddr;
    DWORD       dwLocalPort;
    DWORD       dwRemoteAddr;
    DWORD       dwRemotePort;
} MIB_TCPROW_W2K, *PMIB_TCPROW_W2K;


typedef MIB_TCPROW_LH MIB_TCPROW;
typedef MIB_TCPROW_LH *PMIB_TCPROW;




#define ANY_SIZE 1

typedef struct _MIB_TCPROW2 {
    DWORD dwState;
    DWORD dwLocalAddr;
    DWORD dwLocalPort;
    DWORD dwRemoteAddr;
    DWORD dwRemotePort;
    DWORD dwOwningPid;
    TCP_CONNECTION_OFFLOAD_STATE dwOffloadState;
} MIB_TCPROW2, *PMIB_TCPROW2;

typedef struct _MIB_TCPTABLE2 {
    DWORD dwNumEntries;
    MIB_TCPROW2 table[ANY_SIZE];
} MIB_TCPTABLE2, *PMIB_TCPTABLE2;

typedef struct _MIB_TCPTABLE {
    DWORD dwNumEntries;
    MIB_TCPROW table[ANY_SIZE];
} MIB_TCPTABLE, *PMIB_TCPTABLE;
#define TCPIP_OWNING_MODULE_SIZE 16
typedef struct _MIB_UDPROW_OWNER_MODULE {
    DWORD           dwLocalAddr;
    DWORD           dwLocalPort;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    union {
        struct {
            int     SpecificPortBind : 1;
        };
        int         dwFlags;
    };
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_UDPROW_OWNER_MODULE, *PMIB_UDPROW_OWNER_MODULE;


typedef struct _MIB_TCPROW_OWNER_MODULE
{
    DWORD           dwState;
    DWORD           dwLocalAddr;
    DWORD           dwLocalPort;
    DWORD           dwRemoteAddr;
    DWORD           dwRemotePort;
    DWORD           dwOwningPid;
    LARGE_INTEGER   liCreateTimestamp;
    ULONGLONG       OwningModuleInfo[TCPIP_OWNING_MODULE_SIZE];
} MIB_TCPROW_OWNER_MODULE, *PMIB_TCPROW_OWNER_MODULE;


typedef struct _MIB_TCPTABLE_OWNER_MODULE
{
    DWORD                   dwNumEntries;
    MIB_TCPROW_OWNER_MODULE table[ANY_SIZE];
} MIB_TCPTABLE_OWNER_MODULE, *PMIB_TCPTABLE_OWNER_MODULE;

typedef struct _MIB_UDPTABLE_OWNER_MODULE
{
    DWORD                   dwNumEntries;
    MIB_UDPROW_OWNER_MODULE table[ANY_SIZE];
} MIB_UDPTABLE_OWNER_MODULE, *PMIB_UDPTABLE_OWNER_MODULE;



typedef struct _MIB_UDPROW_OWNER_PID {
    DWORD dwLocalAddr;
    DWORD dwLocalPort;
    DWORD dwOwningPid;
} MIB_UDPROW_OWNER_PID, *PMIB_UDPROW_OWNER_PID;

typedef struct _MIB_UDPTABLE_OWNER_PID
{
    DWORD                   dwNumEntries;
    MIB_UDPROW_OWNER_PID    table[ANY_SIZE];
} MIB_UDPTABLE_OWNER_PID, *PMIB_UDPTABLE_OWNER_PID;

typedef enum _TCP_TABLE_CLASS {
    TCP_TABLE_BASIC_LISTENER,
    TCP_TABLE_BASIC_CONNECTIONS,
    TCP_TABLE_BASIC_ALL,
    TCP_TABLE_OWNER_PID_LISTENER,
    TCP_TABLE_OWNER_PID_CONNECTIONS,
    TCP_TABLE_OWNER_PID_ALL,
    TCP_TABLE_OWNER_MODULE_LISTENER,
    TCP_TABLE_OWNER_MODULE_CONNECTIONS,
    TCP_TABLE_OWNER_MODULE_ALL
} TCP_TABLE_CLASS, *PTCP_TABLE_CLASS;

typedef enum _UDP_TABLE_CLASS {
    UDP_TABLE_BASIC,
    UDP_TABLE_OWNER_PID,
    UDP_TABLE_OWNER_MODULE
} UDP_TABLE_CLASS, *PUDP_TABLE_CLASS;

typedef enum _TCPIP_OWNER_MODULE_INFO_CLASS {
    TCPIP_OWNER_MODULE_INFO_BASIC
} TCPIP_OWNER_MODULE_INFO_CLASS, *PTCPIP_OWNER_MODULE_INFO_CLASS;

typedef struct _TCPIP_OWNER_MODULE_BASIC_INFO {
    PWCHAR pModuleName;
    PWCHAR pModulePath;
} TCPIP_OWNER_MODULE_BASIC_INFO, *PTCPIP_OWNER_MODULE_BASIC_INFO;

extern "C" ULONG
WINAPI
GetTcpTable(
    _Out_writes_bytes_opt_(*SizePointer)   PMIB_TCPTABLE TcpTable,
    _Inout_                          PULONG        SizePointer,
    _In_                             BOOL          Order
    );

extern "C" ULONG
WINAPI
GetTcpTable2(
    _Out_writes_bytes_opt_(*SizePointer)   PMIB_TCPTABLE2 TcpTable,
    _Inout_                          PULONG         SizePointer,
    _In_                             BOOL           Order
    );


extern "C" DWORD
WINAPI
GetExtendedUdpTable(
    _Out_writes_bytes_opt_(*pdwSize)    PVOID           pUdpTable,
    _Inout_                       PDWORD          pdwSize,
    _In_                          BOOL            bOrder,
    _In_                          ULONG           ulAf,
    _In_                          UDP_TABLE_CLASS TableClass,
    _In_                          ULONG           Reserved
    );

extern "C"  DWORD
WINAPI
GetOwnerModuleFromUdpEntry(
    _In_                      PMIB_UDPROW_OWNER_MODULE      pUdpEntry,
    _In_                      TCPIP_OWNER_MODULE_INFO_CLASS Class,
    _Out_writes_bytes_(*pdwSize)    PVOID                         pBuffer,
    _Inout_                   PDWORD                        pdwSize
    );

extern "C" DWORD
WINAPI
GetExtendedTcpTable(
    _Out_writes_bytes_opt_(*pdwSize)        PVOID           pTcpTable,
    _Inout_
    _When_(ulAf == AF_INET, _Deref_in_range_(>=, sizeof(MIB_TCPTABLE)))
    _When_(
        (TableClass == TCP_TABLE_OWNER_PID_LISTENER ||
         TableClass == TCP_TABLE_OWNER_PID_CONNECTIONS ||
         TableClass == TCP_TABLE_OWNER_PID_ALL) && ulAf == AF_INET6,
        _Deref_in_range_(>=, sizeof(MIB_TCP6TABLE_OWNER_PID)))
    _When_(
        (TableClass == TCP_TABLE_OWNER_MODULE_LISTENER ||
         TableClass == TCP_TABLE_OWNER_MODULE_CONNECTIONS ||
         TableClass == TCP_TABLE_OWNER_MODULE_ALL) && ulAf == AF_INET6,
        _Deref_in_range_(>=, sizeof(MIB_TCP6TABLE_OWNER_MODULE)))
    PDWORD          pdwSize,
    _In_                          BOOL            bOrder,
    _In_                          ULONG           ulAf,
    _In_                          TCP_TABLE_CLASS TableClass,
    _In_                          ULONG           Reserved
    );

extern "C" DWORD
WINAPI
GetOwnerModuleFromTcpEntry(
    _In_                   PMIB_TCPROW_OWNER_MODULE      pTcpEntry,
    _In_                   TCPIP_OWNER_MODULE_INFO_CLASS Class,
    _Out_writes_bytes_(*pdwSize) PVOID                         pBuffer,
    _Inout_                PDWORD                        pdwSize
    );
