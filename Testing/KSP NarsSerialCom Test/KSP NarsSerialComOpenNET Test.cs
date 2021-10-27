using System;
using UnityEngine;
using KSP.IO;

namespace KSP_NarsSerialComOpenNET_Test
{
    [KSPAddon(KSPAddon.Startup.MainMenu, false)]
    public class KSP_NarsSerialComOpenNET_Test_Main
    {
        void Awake()
        {
            Debug.Log("Compiled and ran!");
        }
    }
}
