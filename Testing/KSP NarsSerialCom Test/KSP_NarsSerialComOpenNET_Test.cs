using System;
using UnityEngine;
using Nars_Libraries_Framework45.Serial;
using Nars_Libraries_Framework45;

namespace KSP_NarsSerialComOpenNET_Test
{
    [KSPAddon(KSPAddon.Startup.Flight, false)]
    public class KSP_Test : MonoBehaviour
    {
        const int regize = 2;
        NarsSerialComOpenNET port = new NarsSerialComOpenNET();
        long[] data = new long[regize];
        int counter = 0;
        System.Timers.Timer timer = new System.Timers.Timer();

        public void Start()
        {
            if (port.state == State.Connected)
            {
                port.connect("\\\\.\\COM28");
            }
            port.addOnReceiveHandler(onRecv);
            timer.Enabled = true;
            timer.AutoReset = true;
            timer.Interval = 30;
            timer.Elapsed += send;
        }

        public void FixedUpdate()
        {
            getVars();
        }

        void getVars()
        {
            data[1] = (long)FlightGlobals.ActiveVessel.speed;
            data[2] = (long)FlightGlobals.ActiveVessel.altitude;
        }

        //Only sending one register each update, microcontroller to slow
        void send(object sender, System.Timers.ElapsedEventArgs e)
        {
            Result result;
            try
            {
                result = port.send(1, 5);
            }
            catch (Exception er)
            {
                Debug.Log(er.StackTrace);
                throw;
            }

            Debug.Log(result.message);
        }

        void onRecv(Receive message)
        {
            //Debug.Log($"Message: {message.message} Success: {message.success}");
        }
    }
}