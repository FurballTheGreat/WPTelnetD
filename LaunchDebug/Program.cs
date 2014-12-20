using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Management.Instrumentation;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;
using Microsoft.VisualStudio.DeviceConnectivity.Interop;

namespace LaunchDebug
{
    internal class Program
    {
        [Flags]
        public enum DeploymentOptions
        {
            None = 0,
            PA = 1,
            Debug = 2,
            Infused = 4,
            Lightup = 8,
            Enterprise = 16,
            Sideload = 32,
            TypeMask = 255,
            UninstallDisabled = 256,
            SkipUpdateAppInForeground = 512,
            DeleteXap = 1024,
            InstallOnSD = 65536,
            OptOutSD = 131072
        }

        [DllImport("PhoneRegDll.dll")]
        internal static extern uint GetWinPhone8Port(int port, out int portMapped);
//        C4C3244F-DA9E-4CEC-BB3A-9BCEA1CCE92F
//8995B972-A375-4B6D-90A5-A778CD43FCB1
//4083BA63-3FEE-4E68-9D3F-1D0C02999356
//B2FC26AB-D6EC-4426-91FA-9E039F92A639  // SLProfiler
        public class TestServiceConnection : CcConnectionDeviceService
        {
            public TestServiceConnection(ConnectableDevice pDeviceToConnect, Guid pProductId) :
                base(
                pDeviceToConnect,                 
                new Guid("88B0B986-543F-4F04-8E5F-A80A7E189F11"),
                new Guid("B2FC26AB-D6EC-4426-91FA-9E039F92A639"))
            {
                Connect("");
                SendPacket(pPacket => pPacket.WriteInt32(10));

                SendPacket(pPacket => pPacket.WriteString(@"\Data\Programs\{EA163CC8-8B9B-49A5-A3AB-4BFDAFD818DF}\Install\AppManifest.xaml"));


                var packet = ReadPacket<int>(pPacket =>
                {
                    Console.WriteLine("{0} {1} {2}", pPacket.ReadInt32(), pPacket.ReadInt32(),pPacket.IsEndOfPacket());
                    
                    return 0;
                });
            }
        }

        static void PrintPath(string pIndent, string pPath, ICcConnection4 pConnection4)
        {
            Console.WriteLine(pIndent+Path.GetFileName(pPath));
            try
            {
                foreach (string item in pConnection4.GetDirectoryListing(pPath))
                {
                    PrintPath(pIndent + " ", pPath + "\\" + item, pConnection4);

                }
            }
            catch (Exception e)
            {
                
            }
        }

        private static string[] guids = new[]

        {
            "{02ADAAFF-AD55-42B4-9CA1-FD4D1DADD32A}",
            "{04C9C389-F305-4B19-8851-53020E506770}",
            "{0788143F-3362-4DA9-AF4D-00501A189B7B}",
            "{07D87655-E4F0-474B-895A-773790AD4A32}",
            "{0CB79943-5379-4A79-8788-DD09E04AE9C2}",
            "{106E0A97-8B19-42CF-8879-A8ED2598FCBB}",
            "{10F2995D-1F82-4203-B7FA-46DDBD07A6E6}",
            "{134E363E-8811-44BE-B1E3-D8A0C60D4692}",
            "{1672C093-8E9F-4D13-980B-77A3CE2CFFA5}",
            "{178AC8A1-6519-4A0B-960C-038393741E96}",
            "{1CEA0568-1794-45A0-9ADD-4DA0BA89B670}",
            "{1FD49676-9500-E011-9264-00237DE2DB9E}",
            "{201FFBE8-3026-4220-843C-060F11770F06}",
            "{220BFBF2-EE02-496C-A656-651A6C0C6518}",
            "{222A57A4-11E1-486B-A762-2D8E340F9792}",
            "{2377FE1B-C10F-47DA-92F3-FC517345A3C0}",
            "{24B9A234-D074-4487-B5EA-B805B6825934}",
            "{25F7C2FA-CA89-49A7-9937-C7347BE73FEC}",
            "{2676904A-3C87-4647-A7C0-5D0C0886C7A1}",
            "{268C094C-127A-4544-92B0-1639C0BD8399}",
            "{2BA43182-BA12-42C0-B503-158284F48BF8}",
            "{2E0F86F9-D41F-4F92-8933-5FD620A49858}",
            "{31BBC68C-503E-4561-8D85-A294D54DF06F}",
            "{3219D30D-4A23-4F58-A91C-C44B04E6A0C7}",
            "{4C4AD968-7100-49DE-8CD1-402E198D869E}",
            "{4FCD4DE1-050B-44DC-B123-A786808EB49B}",
            "{51F96ABA-9924-43D7-8D6C-76A24018D3E0}",
            "{594477C0-E991-4ED4-8BE4-466055670E69}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5601}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5602}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5603}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5605}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5607}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA560A}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA560B}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA560C}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA560D}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA560E}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5610}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5611}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5614}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5615}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5616}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5617}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5618}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA561B}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA561D}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA561E}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA561F}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5620}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5623}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5624}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA562A}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA562C}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5630}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5631}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5632}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5633}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5640}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5646}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5648}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA564B}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA564D}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA564E}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5660}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5663}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5664}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5666}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5670}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5683}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5686}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA568A}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA568C}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5702}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5801}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5804}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5806}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5808}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5901}",
            "{5B04B775-356B-4AA0-AAF8-6491FFEA5908}",
            "{62F172D1-F552-4749-871C-2AFD1C95C245}",
            "{6BAE10BF-E831-49B7-A51F-CEE8E91C7B1D}",
            "{71403BB5-A1A4-4F20-BB69-9E478CC70B95}",
            "{79FFBD59-D090-4365-AABF-384EE84EE3E5}",
            "{7BBC8703-FB43-4E9E-88C9-62957EA0D59B}",
            "{7CD95034-77FC-46FE-9E99-C458A8926641}",
            "{7F8072C8-57A1-4DC7-AF48-5776E16C28E0}",
            "{818EA0B2-37DC-46B1-8B43-B9EEE2153F02}",
            "{82FA6341-28DC-4203-BD08-9749B167BC4B}",
            "{85A2F8EA-50EB-442D-A759-CBABED7A0A1F}",
            "{96CFA04C-1675-4395-9708-0AC4A559BF90}",
            "{971C41E5-3596-4A7A-BA2C-BCD7780D7DB5}",
            "{99661466-8C5C-489B-A567-569C1F480D29}",
            "{A4F6CC7D-7AB0-41E8-90A9-D74751FCA1C2}",
            "{A8DDC8F6-C12C-44E6-B22E-52E2F0905F3E}",
            "{AEC3BFAD-E38C-4994-9C32-50BD030730EC}",
            "{AFE91DD5-26FB-4BA6-A5A4-4BCEDE8FB3E6}",
            "{B0894DFD-4671-4BB9-BC17-A8B39947FFB6}",
            "{B08997CA-60AB-4DCE-B088-F92E9C7994F3}",
            "{B46B814D-6469-409B-B8FE-133574CDD2B0}",
            "{BA3A28E1-9B7E-46B2-9972-3E6FA414D1C0}",
            "{BBC57C87-46AF-4C2C-824E-AC8104CCEB38}",
            "{BFAFC44D-55C7-42BF-9A90-D2D23B2EF019}",
            "{C14E93AA-27D7-DF11-A844-00237DE2DB9E}",
            "{C459945B-D93F-4AAE-9207-C6AB1D971357}",
            "{CBB8C3BD-99E8-4176-AD8C-95EC6A3641C2}",
            "{D061135A-5AAF-4033-B8B7-EEC68ABBA397}",
            "{DA52FA01-AC0F-479D-957F-BFE4595941CB}",
            "{DC08943B-7B3D-4EE5-AA3C-30F1A826AF02}",
            "{DFC27B84-BAF4-4706-A813-91E7AC75B8B7}",
            "{E0B331A9-85E8-DF11-9264-00237DE2DB9E}",
            "{E75EA363-F86C-45F7-B402-1AF0E77B6FCB}",
            "{E8227429-53C2-4463-8A2B-97832BAFDF3E}",
            "{E8863E77-206D-E011-81D2-78E7D1FA76F8}",
            "{E8AA1299-E011-486C-9F61-3232C892B17B}",
            "{E99421BF-09E6-483E-89BB-CBCEDAC6B4E2}",
            "{EA163CC8-8B9B-49A5-A3AB-4BFDAFD818DF}",
            "{EC418E52-B788-E011-986B-78E7D1FA76F8}",
            "{EFA4B4A7-7499-46CE-AA95-3E4AB3B39313}",
            "{F07F83EB-A8A4-49FD-8946-C67A9349E062}",
            "{F10991B2-3E1A-4FB0-99BC-833338A33502}",
            "{F5874252-1F04-4C3F-A335-4FA3B7B85329}",
            "{F725010E-455D-4C09-AC48-BCDEF0D4B626}",
            "{FC09E558-44E5-489C-A1B2-DC96C1B761FA}",
            "{FD27A6CE-139B-4ED2-ABC2-98ACDC6EBA97}"
        };

        private static void Main(string[] args)
        {
            //var ss = new WpPowerToolsService(WpPowerToolsService.Devices.First());
            //ss.StartWAppVerifier("blah");
            //Console.ReadLine();
         //   var test = new DebugConnectionService(WpPowerToolsService.Devices.First()); ;
        //   uint out_pProcessID;
       //    uint handle;
            string outxml;
     //       test.CcConnection4.GetApplicationType()
     ////       test.CcConnection2.ProvisionDeviceXML(@"dfgdfgd",0,out outxml,"ddd.dll");
     ////       test.CcConnection.RemoveFile(@"\DATA\SharedData\PhoneTools\11.0\Debugger\bin\RemoteDebugger\msvsmon.exe");
   //         test.CcConnection.SendFile(@"C:\personalwork\WPTelnet\WPTelnetManagerApp\WPTelnetManagerApp\ARM\Debug\QueryJob\QueryJob.dll",
    //            @"\DATA\SharedData\PhoneTools\11.0\Debugger\bin\RemoteDebugger\msvsmon.exe",(int)FileMode.Create,tagFileCustomAction.FileCustomAction_COM);
      //     test.CcConnection.LaunchProcess(@"\DATA\SharedData\PhoneTools\11.0\Debugger\bin\RemoteDebugger\msvsmon.exe", "", 10, out out_pProcessID, out handle);
         
      //      string resyk;
       //    deb.CcConnection2.ProvisionDeviceXML("<blah></blah>",0, out resyk, "blah.dll");
          //    PrintPath("", @"\DATA\SharedData\PhoneTools", deb.CcConnection4);{EA163CC8-8B9B-49A5-A3AB-4BFDAFD818DF}
        
           var deb = new DebugConnectionService(WpPowerToolsService.Devices.First());
          deb.DebugLaunch("wptd.exe", new Guid("{F9D14EFD-32F4-4015-B4C7-6D807FD12B38}"));
          Console.ReadLine();
        ////    PrintPath("", @"\DATA\SharedData\PhoneTools", deb.CcConnection4);
        //  //  deb.DebugLaunch("wptd.exe", new Guid("{EA163CC8-8B9B-49A5-A3AB-4BFDAFD818DF}"));
        //    uint ph;
        //    uint pid;
        //    Array out_pProductIds;
        //    Array instances;
        //   deb.CcConnection3.LaunchApplicationWithService("{9A0F7585-9F16-47D5-8041-28018FCEA606}",@"c:\windows\system32\TaskHost.exe");//.GetApplicationType("{9A0F7585-9F16-47D5-8041-28018FCEA606}");
        //    deb.CcConnection.LaunchProcess(@"C:\Data\Programs\{EA163CC8-8B9B-49A5-A3AB-4BFDAFD818DF}\Install\wptda.exe","",0, out pid,out ph);
        //    //    wp.CcConnection3.LaunchApplication("{5B04B775-356B-4AA0-AAF8-6491FFEA5660}");
//            00 01 00 01 02 02 37 6e  65 74 2e 74 63 70 3a 2f ......7n et.tcp:/
//00000010  2f 31 32 37 2e 30 2e 30  2e 31 3a 38 31 31 37 2f /127.0.0 .1:8117/
//00000020  4d 69 63 72 6f 73 6f 66  74 2f 56 69 73 75 61 6c Microsof t/Visual
//00000030  53 74 75 64 69 6f 2f 6d  73 76 73 6d 6f 6e 03 08 Studio/m svsmon..
//00000040  0c 
            var codes = new Dictionary<int,List<string>>();
            //var dev = CcConnectionDeviceService.Devices.First();
            //foreach (var guid in guids)
            //{
            //    using (var deb = new DebugConnectionService(dev))
            //    {
            //        var status = deb.DebugLaunch(@"C:\personalwork\WPTelnet\wp\signed\wptd.exe", new Guid(guid));
            //        if (status == 0)
            //        {
            //            Console.WriteLine("Success " + guid);
            //            Console.ReadLine();
            //        }
            //        else
            //        {
            //            if (!codes.ContainsKey(status))
            //            {
            //                codes.Add(status, new List<string> {guid});
            //            }
            //            else
            //            {
            //                codes[status].Add(guid);
            //            }
            //        }
            //    }
            //}
          
            int port;
            GetWinPhone8Port(8117, out port);
            //     var appId = new Guid("{493bea34-d9bf-495a-a489-344058cdc795}");
            //     //      deb.UninstallApp(new Guid("{ea163cc8-8b9b-49a5-a3ab-4bfdafd818d6}")); 
            //     deb.UpdateApp(appId, @"C:\personalwork\WPTelnet\wptd\wptd\SilverHostApp\Bin\ARM\Debug\SilverHostApp_Debug_ARM.xap");
            //     deb.UninstallApp(appId); 
            //     // Change guid 

            //     deb.RetrieveFile(@"\DATA\SharedData\PhoneTools\PwTools\log\wpr_output.txt", @"wproutput3.txt"););
            //var wp = new WpPowerToolsService(WpPowerToolsService.Devices.First());

            //wp.AddMetaData();
        }

      
    }
}
