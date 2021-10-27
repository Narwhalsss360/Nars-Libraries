using System;
using UnityEngine;
using KSP.IO;
using Nars_Libraries_Framework45;

namespace KSP_NarsSerialComOpenNET_Test
{
    [KSPAddon(KSPAddon.Startup.MainMenu, false)]
    public class KSP_NarsSerialComOpenNET_Test_Main
    {
        Nars_Libraries_Framework45.Serial.NarsSerialComOpenNET port = new Nars_Libraries_Framework45.Serial.NarsSerialComOpenNET();
        void Awake()
        {
            Debug.Log("Compiled and ran!");
            port.connect("\\\\.\\COM28");
        }
    }
}
