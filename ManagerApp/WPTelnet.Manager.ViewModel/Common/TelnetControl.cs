using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class TelnetControlMethods
    {
          public static void InitNetWorking()
          {
              var wsaError = 0;
              var result = false;
            try
            {
                result = DevProgram.DevProgramReg.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = RegistryUtils.RegistryUtils.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = RegistryWPRT.CRegistryWPRT.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = DiagDebugLogWinPRT.CDiagDebugLogWinPRT.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

               try
            {
                result = CSDDiagRT.CCSDDiagRT.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = HTCDebugLogWinPRT.HTCLog.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = RPCComponent.CRPCComponent.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            try
            {
                result = FactoryKey.CFactoryKey.TelnetInitNetworking(out wsaError);
                goto done;
            }
            catch (Exception)
            {

            }

            done:
            if(!result)
                throw new SocketException(wsaError);

        }

          public static void Shutdown()
          {
              var wsaError = 0;
              var result = false;
              try
              {
                  result = DevProgram.DevProgramReg.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryUtils.RegistryUtils.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryWPRT.CRegistryWPRT.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = DiagDebugLogWinPRT.CDiagDebugLogWinPRT.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }
              try
              {
                  result = CSDDiagRT.CCSDDiagRT.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = HTCDebugLogWinPRT.HTCLog.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RPCComponent.CRPCComponent.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }
      
              try
              {
                  result = FactoryKey.CFactoryKey.TelnetShutDownNetworking();
                  goto done;
              }
              catch (Exception)
              {

              }

            done:
              if (!result)
                  throw new SocketException(wsaError);

          }

          public static int ListenForOneConnection(int pPort)
          {
              var wsaError = 0;
              var result = 0;
              var socket = 0;
              try
              {
                  result = DevProgram.DevProgramReg.TelnetListenForOneConnection(pPort, out socket, out wsaError );
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryUtils.RegistryUtils.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryWPRT.CRegistryWPRT.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = DiagDebugLogWinPRT.CDiagDebugLogWinPRT.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }
              
              try
              {
                  result = CSDDiagRT.CCSDDiagRT.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = HTCDebugLogWinPRT.HTCLog.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RPCComponent.CRPCComponent.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = FactoryKey.CFactoryKey.TelnetListenForOneConnection(pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

            done:
              if (result == 0)
                  return socket;

              throw new InvalidOperationException(string.Format("Error listening on socket  (TelD: {0}, WSAError: {1}, Port: {2})", result, wsaError,pPort));

          }


          public static int ConnectTo(string pIPAddress, int pPort)
          {
              var wsaError = 0;
              var result = 0;
              var socket = 0;
              try
              {
                  result = DevProgram.DevProgramReg.TelnetConnectTo(pIPAddress,pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryUtils.RegistryUtils.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryWPRT.CRegistryWPRT.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = DiagDebugLogWinPRT.CDiagDebugLogWinPRT.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

               try
              {
                  result = CSDDiagRT.CCSDDiagRT.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = HTCDebugLogWinPRT.HTCLog.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RPCComponent.CRPCComponent.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = FactoryKey.CFactoryKey.TelnetConnectTo(pIPAddress, pPort, out socket, out wsaError);
                  goto done;
              }
              catch (Exception)
              {

              }

            done:
              if (result == 0)
                  return socket;

              throw new InvalidOperationException(string.Format("Error connecting to {2}:{3} (TelD: {0}, WSAError: {1}", result, wsaError, pIPAddress, pPort));

          }


          public static void ProcessConnection(int pSocket, string pWelcomeMessage)
          {
              var result = false;
              try
              {
                  result = DevProgram.DevProgramReg.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryUtils.RegistryUtils.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RegistryWPRT.CRegistryWPRT.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

            try
              {
                  result = DiagDebugLogWinPRT.CDiagDebugLogWinPRT.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = CSDDiagRT.CCSDDiagRT.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = HTCDebugLogWinPRT.HTCLog.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = RPCComponent.CRPCComponent.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

              try
              {
                  result = FactoryKey.CFactoryKey.TelnetProcessConnection(pSocket, pWelcomeMessage);
                  goto done;
              }
              catch (Exception)
              {

              }

            done:
              if (result)
                  return;

              throw new InvalidOperationException("ProcessConnection Failed");

          }
    }
}
