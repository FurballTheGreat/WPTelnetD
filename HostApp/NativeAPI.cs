using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace HostApp
{
    public static class NativeAPI
    {

        [StructLayout(LayoutKind.Sequential)]
        public struct PROCESS_INFORMATION
        {
            public IntPtr hProcess;
            public IntPtr hThread;
            public int dwProcessId;
            public int dwThreadId;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct STARTUPINFO
        {
            public Int32 cb;
            public string lpReserved;
            public string lpDesktop;
            public string lpTitle;
            public Int32 dwX;
            public Int32 dwY;
            public Int32 dwXSize;
            public Int32 dwYSize;
            public Int32 dwXCountChars;
            public Int32 dwYCountChars;
            public Int32 dwFillAttribute;
            public Int32 dwFlags;
            public Int16 wShowWindow;
            public Int16 cbReserved2;
            public IntPtr lpReserved2;
            public IntPtr hStdInput;
            public IntPtr hStdOutput;
            public IntPtr hStdError;
        }
        [DllImport("KERNELBASE.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool CreateProcessW(string lpApplicationName,
           string lpCommandLine, IntPtr psecuratrribs,
           IntPtr lpThreadAttributes, bool bInheritHandles,
           uint dwCreationFlags, IntPtr lpEnvironment, string lpCurrentDirectory,
           [In] ref STARTUPINFO lpStartupInfo,
           out PROCESS_INFORMATION lpProcessInformation);

        public const uint MAX_DEEP_PATH = 32767;

        [DllImport("KERNELBASE.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern uint GetCurrentDirectoryW(uint nBufferLengt, StringBuilder lpBuffer);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr OpenMutexW(uint dwDesiredAccess, bool bInheritHandle,
            string lpName);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern uint GetFileAttributesW(string lpFileName);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool SetFileAttributesW(string lpFileName, uint dwFileAttributes);

         [DllImport("kernel32legacy.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool MoveFileW(string lpExistingFileName, string lpNewFileName);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public  static extern IntPtr FindFirstFileW(string lpFileName, out WIN32_FIND_DATA lpFindFileData);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool FindNextFileW(IntPtr hFindFile, out WIN32_FIND_DATA lpFindFileData);

        [DllImport("kernelbase.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool RemoveDirectoryW(string lpPathName);

        [DllImport("kernelbase.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteFileW([MarshalAs(UnmanagedType.LPWStr)]string lpFileName);

        [DllImport("kernelbase.dll")]
        public static extern bool FindClose(IntPtr hFindFile);
        public const int MAX_PATH = 260;
         public const int MAX_ALTERNATE = 14;

         [StructLayout(LayoutKind.Sequential)]
         public struct FILETIME
         {
             public uint dwLowDateTime;
             public uint dwHighDateTime;
         };

         [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
         public struct WIN32_FIND_DATA
         {
             public uint dwFileAttributes;
             public FILETIME ftCreationTime;
             public FILETIME ftLastAccessTime;
             public FILETIME ftLastWriteTime;
             public uint nFileSizeHigh; //changed all to uint, otherwise you run into unexpected overflow
             public uint nFileSizeLow;  //|
             public uint dwReserved0;   //|
             public uint dwReserved1;   //v
             [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_PATH)]
             public string cFileName;
             [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_ALTERNATE)]
             public string cAlternate;
         }


        [DllImport("kernelbase.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool CloseHandle(IntPtr hObject);
    }
}
