#pragma once

#include <winapifamily.h>

#include <apiset.h>
#include <apisetcconv.h>
#include <minwindef.h>
#include <minwinbase.h>

#define INTERNETAPI        EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE
#define INTERNETAPI_(type) EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
#define INTERNETAPIX(type, salannotation) EXTERN_C DECLSPEC_IMPORT salannotation type STDAPICALLTYPE
#define URLCACHEAPI        EXTERN_C DECLSPEC_IMPORT DWORD STDAPICALLTYPE
#define URLCACHEAPI_(type) EXTERN_C DECLSPEC_IMPORT type STDAPICALLTYPE
typedef LPVOID HINTERNET;
typedef HINTERNET * LPHINTERNET;


#define INTERNET_OPEN_TYPE_PRECONFIG                    0   // use registry configuration
#define INTERNET_OPEN_TYPE_DIRECT                       1   // direct to net
#define INTERNET_OPEN_TYPE_PROXY                        3   // via named proxy
#define INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY  4   // prevent using java/script/INS

//
// old names for access types
//

#define PRE_CONFIG_INTERNET_ACCESS  INTERNET_OPEN_TYPE_PRECONFIG
#define LOCAL_INTERNET_ACCESS       INTERNET_OPEN_TYPE_DIRECT
#define CERN_PROXY_INTERNET_ACCESS  INTERNET_OPEN_TYPE_PROXY
#define BOOLAPI INTERNETAPIX(BOOL, _Success_(return != FALSE))
#define INTERNET_INVALID_PORT_NUMBER    0           // use the protocol-specific default

#define INTERNET_FLAG_RESYNCHRONIZE     0x00000800  // asking wininet to update an item if it is newer
#define INTERNET_FLAG_HYPERLINK         0x00000400  // asking wininet to do hyperlinking semantic which works right for scripts
#define INTERNET_FLAG_NO_UI             0x00000200  // no cookie popup
#define INTERNET_FLAG_PRAGMA_NOCACHE    0x00000100  // asking wininet to add "pragma: no-cache"
#define INTERNET_FLAG_CACHE_ASYNC       0x00000080  // ok to perform lazy cache-write
#define INTERNET_FLAG_FORMS_SUBMIT      0x00000040  // this is a forms submit
#define INTERNET_FLAG_FWD_BACK          0x00000020  // fwd-back button op
#define INTERNET_FLAG_NEED_FILE         0x00000010  // need a file for this request
#define INTERNET_FLAG_NO_CACHE_WRITE    0x04000000  // don't write this item to the cache
#define INTERNET_FLAG_DONT_CACHE        INTERNET_FLAG_NO_CACHE_WRITE
#define INTERNET_FLAG_RELOAD            0x80000000  // retrieve the original item
#define INTERNET_FLAG_MAKE_PERSISTENT   0x02000000  // make this item persistent in cache
#define INTERNET_FLAG_FROM_CACHE        0x01000000  // use offline semantics
#define INTERNET_FLAG_OFFLINE           INTERNET_FLAG_FROM_CACHE


#define HTTP_QUERY_CONTENT_LENGTH               5
#define INTERNET_FLAG_MUST_CACHE_REQUEST INTERNET_FLAG_NEED_FILE


#define HTTP_QUERY_FLAG_NUMBER                  0x20000000

extern "C" INTERNETAPI_(HINTERNET) InternetOpenA(
    _In_opt_ LPCSTR lpszAgent,
    _In_ DWORD dwAccessType,
    _In_opt_ LPCSTR lpszProxy,
    _In_opt_ LPCSTR lpszProxyBypass,
    _In_ DWORD dwFlags
    );


extern "C" BOOLAPI InternetReadFile(
    _In_ HINTERNET hFile,
    _Out_writes_bytes_(dwNumberOfBytesToRead) __out_data_source(NETWORK) LPVOID lpBuffer,
    _In_ DWORD dwNumberOfBytesToRead,
    _Out_ LPDWORD lpdwNumberOfBytesRead
    );

INTERNETAPI_(HINTERNET) InternetOpenUrlA(
    _In_ HINTERNET hInternet,
    _In_ LPCSTR lpszUrl,
    _In_reads_opt_(dwHeadersLength) LPCSTR lpszHeaders,
    _In_ DWORD dwHeadersLength,
    _In_ DWORD dwFlags,
    _In_opt_ DWORD_PTR dwContext
    );

BOOLAPI
HttpQueryInfoA(
    _In_ HINTERNET hRequest,
    _In_ DWORD dwInfoLevel,
    _Inout_updates_bytes_to_opt_(*lpdwBufferLength, *lpdwBufferLength) __out_data_source(NETWORK) LPVOID lpBuffer,
    _Inout_ LPDWORD lpdwBufferLength,
    _Inout_opt_ LPDWORD lpdwIndex
    );
