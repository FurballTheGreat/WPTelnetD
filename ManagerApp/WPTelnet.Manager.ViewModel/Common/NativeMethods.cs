using System;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class NativeMethods
    {
        static NativeMethods()
        {
            Current = new NativeMethods();
        }

        public static NativeMethods Current { get; private set; }

        public string GetCommandLine()
        {
            try
            {
                return DevProgram.DevProgramReg.GetCommandLine();
            }
            catch (Exception)
            {
                
            }

            try
            {
                return RegistryUtils.RegistryUtils.GetCommandLine();
            }
            catch (Exception)
            {
                
            }

            try
            {
                return RegistryWPRT.CRegistryWPRT.GetCommandLine();
            }
            catch (Exception)
            {

            }

            try
            {
                return DiagDebugLogWinPRT.CDiagDebugLogWinPRT.GetCommandLine();
            }
            catch (Exception)
            {

            }

            try
            {
                return CSDDiagRT.CCSDDiagRT.GetCommandLine();
            }
            catch (Exception)
            {

            }

            try
            {
                return HTCDebugLogWinPRT.HTCLog.GetCommandLine();
            }
            catch (Exception)
            {

            }

            try
            {
                return RPCComponent.CRPCComponent.GetCommandLine();
            }
            catch (Exception)
            {

            }

            return "Failed to call GetCommnadLine";
        }

        public int CreateProcess(string pCommandLine)
        {
            try
            {
                return DevProgram.DevProgramReg.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }

            try
            {
                return 4;
            }
            catch (Exception)
            {

            }

            try
            {
                return RegistryWPRT.CRegistryWPRT.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }
            
            try
            {
                return DiagDebugLogWinPRT.CDiagDebugLogWinPRT.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }

            try
            {
                return CSDDiagRT.CCSDDiagRT.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }

            try
            {
                return HTCDebugLogWinPRT.HTCLog.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }

            try
            {
                return RPCComponent.CRPCComponent.CreateProcess(pCommandLine);
            }
            catch (Exception)
            {

            }


            throw new InvalidProgramException("Failed to call create process (WinRT)");
        }


        public int GetLastError()
        {
            try
            {
                return DevProgram.DevProgramReg.GetLastError();
            }
            catch (Exception)
            {

            }

            try
            {
                return RegistryUtils.RegistryUtils.GetLastError();
            }
            catch (Exception)
            {

            }

            try
            {
                return RegistryWPRT.CRegistryWPRT.GetLastError();
            }
            catch (Exception)
            {

            }

            try
            {
                return DiagDebugLogWinPRT.CDiagDebugLogWinPRT.GetLastError();
            }
            catch (Exception)
            {

            }

            try
            {
                return CSDDiagRT.CCSDDiagRT.GetLastError();
            }
            catch (Exception)
            {

            }


            try
            {
                return HTCDebugLogWinPRT.HTCLog.GetLastError();
            }
            catch (Exception)
            {

            }


            try
            {
                return RPCComponent.CRPCComponent.GetLastError();
            }
            catch (Exception)
            {

            }
            throw new InvalidProgramException("Failed to call GetLastError (WinRT)");
        }

      
    }
}
