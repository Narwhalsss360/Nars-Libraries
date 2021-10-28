using System;
using UnityEngine;
using Nars_Libraries_Framework45.Serial;

namespace KSP_NarsSerialComOpenNET_Test
{
    [KSPAddon(KSPAddon.Startup.Flight, false)]
    public class KSP_Test : MonoBehaviour
    {
        const int regize = 2;
        NarsSerialComOpenNET port = new NarsSerialComOpenNET();
        long[] data = new long[regize];
        int counter = 0;

        public void Start()
        {
            if (port.state == NarsSerialComOpenNET.States.DISCONNECTED)
            {
                port.connect("\\\\.\\COM28");
            }
            port.addOnReceiveHandler(onRecv);
        }

        public void FixedUpdate()
        {
            getVars();
            send();
        }

        void getVars()
        {
            data[1] = (long)FlightGlobals.ActiveVessel.speed;
            data[2] = (long)FlightGlobals.ActiveVessel.altitude;
        }

        //Only sending one register each update, microcontroller to slow
        void send()
        {
            switch (counter)
            {
                case 0:
                    port.sendSpecialData(0, FlightGlobals.ActiveVessel.GetDisplayName());
                    break;
                default:        
                    port.sendData(counter, (uint)data[counter]);
                    break;
            }

            if (counter >= regize)
            {
                counter = 0;
            }
            else
            {
                counter++;
            }
        }

        void onRecv(NarsSerialComOpenNET.message message)
        {
            Debug.Log($"Recieved: {message.raw}");
        }
    }
}