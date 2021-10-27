using System;
using UnityEngine;
using Nars_Libraries_Framework45.Serial;
using Nars_Libraries_Framework45.Usefuls;

namespace KSP_NarsSerialComOpenNET_Test
{
    [KSPAddon(KSPAddon.Startup.Flight, false)]
    public class KSP_Test : MonoBehaviour
    {
        NarsSerialComOpenNET port = new NarsSerialComOpenNET();
        byte data1;

        public void Start()
        {
            port.connect("\\\\.\\COM28");
            port.addOnReceiveHandler(onRecv);
        }

        public void Update()
        {
            /*NarsSerialComOpenNET.result result = port.sendData(12, (long)FlightGlobals.ActiveVessel.orbit.ApA);
            if (result.complete)
            {
                Debug.Log("Sent " + result.message);
            }
            else
            {
                Debug.Log("Error: " + result.message);
            }*/

        }

        void getGlobals()
        {
            if (FlightGlobals.ActiveVessel.ActionGroups.groups[3])
            {
                Debug.Log("True");
            }
            else
            {
                Debug.Log("False");
            }
        }

        void onRecv(NarsSerialComOpenNET.message message)
        {
            Debug.Log(long.Parse(message.data, System.Globalization.NumberStyles.HexNumber));
        }
    }
}