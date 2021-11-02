using System;
using UnityEngine;
using Nars_Libraries_Framework45.Serial;
using Nars_Libraries_Framework45;

namespace KSP_NarsSerialComOpenNET_Test
{

    [KSPAddon(KSPAddon.Startup.Flight, false)]
    public class KSP_TestFlight : MonoBehaviour
    {
        uint[] data = new uint[3];
        System.Timers.Timer timer = new System.Timers.Timer();
        System.Diagnostics.Stopwatch timeout = new System.Diagnostics.Stopwatch();
        NarsSerialComOpenNET port = new NarsSerialComOpenNET();
        bool first = true;

        public void Start()
        {
            timer.Enabled = false;
            timer.AutoReset = true;
            timer.Interval = 500;
            timer.Elapsed += send;
            port.queueSize = 35;
            port.addOnReceiveHandler(onRecv);
        }

        public void FixedUpdate()
        {
            getVars();
            if (port.state == State.Disconnected)
            {
                while (first)
                {
                    if (!timeout.IsRunning)
                    {
                        port.connect("\\\\.\\COM28");
                        timeout.Start();
                    }
                    if (timeout.ElapsedMilliseconds > 2000)
                    {
                        timer.Enabled = true;
                        timeout.Stop();
                        first = false;
                        break;
                    }
                }
            }
        }

        void getVars()
        {
            data[1] = (uint)FlightGlobals.ActiveVessel.speed;
            data[2] = (uint)FlightGlobals.ActiveVessel.altitude;
        }

        void send(object sender, System.Timers.ElapsedEventArgs eventArgs)
        {
            Debug.Log(port.ready.ToString())
            Debug.Log(port.send(1, data[1]).message);
            port.checkQueue();
        }

        void onRecv(Receive receive)
        {

        }
    }
}