namespace Nars_Libraries_Framework45
{
    public class Result
    {
        readonly public bool success;

        readonly public string message;
        public Result(bool _success, string _message)
        {
            this.success = _success;
            this.message = _message;
        }
    }

    namespace Serial
    {
        public enum State
        {
            Disconnected,
            Connected,
            Lost
        }

        public class Receive : Result
        {
            readonly public bool special;
            readonly public ushort register;
            readonly public object data;

            public Receive(bool _success, string _message, bool _special, ushort _register, object _data) : base(_success, _message)
            {
                this.special = _special;
                this.register = _register;
                this.data = _data;
            }
        }

        /// <summary>
        /// Class for communication with an arduino through a serial port. Must use with Nars Arduino library.
        /// </summary>
        public class NarsSerialCom
        {
            public object[] receivedData = new object[65535];

            public State state = State.Disconnected;

            public System.Collections.Generic.Queue<string> sendQueue = new System.Collections.Generic.Queue<string>();

            private System.Action<Receive> onReceiveHandler = null;

            public bool ready = false;

            private System.Timers.Timer timer = new System.Timers.Timer();

            public System.IO.Ports.SerialPort serialPort = new System.IO.Ports.SerialPort();

            private System.IO.Ports.SerialPort tempPort = new System.IO.Ports.SerialPort();

            public Receive lastRecieve;

            public Result lastResult;

            public int timeout = 700;

            private bool timedOut = false;

            public byte queueSize = 10;

            public NarsSerialCom()
            {
                this.serialPort.BaudRate = 1000000;
                this.serialPort.Parity = System.IO.Ports.Parity.None;
                this.serialPort.StopBits = System.IO.Ports.StopBits.One;
                this.serialPort.DataBits = 8;
                this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(DataRecievedHandler);
                this.lastResult = new Result(true, "Instantiated");
                this.timer = new System.Timers.Timer(timeout);
                this.timer.AutoReset = false;
                this.timer.Enabled = true;
                this.timer.Elapsed += this.onTimeOut;
                sendQueue.Clear();
            }

            public void addOnReceiveHandler(System.Action<Receive> _onReceiveHandler)
            {
                this.onReceiveHandler = _onReceiveHandler;
            }

            public Result connect(string port)
            {
                if (this.state == State.Disconnected)
                {
                    this.serialPort.PortName = port;
                    this.serialPort.Open();
                    this.timer.Start();
                    /*while (!this.ready)
                    {
                        if (this.timedOut)
                        {
                            this.timedOut = false;
                            this.lastResult = new Result(false, "Timed out");
                            this.timer.Stop();
                            return this.lastResult;
                        }
                    }*/
                    if (ready)
                    {
                        this.serialPort.WriteLine("*B-");
                        this.lastResult = new Result(true, "Connected");
                        this.timer.Stop();
                        this.timedOut = false;
                        this.state = State.Connected;
                    }
                    this.state = State.Connected;
                }
                else
                {
                    lastResult = new Result(false, "Already Connected");
                }

                return this.lastResult;
            }

            public Result checkQueue()
            {
                if (this.sendQueue.Count > 0)
                {
                    if (this.ready)
                    {
                        string sent = sendQueue.Dequeue();
                        this.serialPort.WriteLine(sent);
                        this.lastResult = new Result(true, "Sent: " + sent);
                    }
                    else
                    {
                        this.lastResult = new Result(false, "Not ready, Next: " + this.sendQueue.Peek());
                    }
                }
                return this.lastResult;
            }

            public Result send(ushort register, object data)
            {
                if (register <= 65535)
                {
                    if (data is int)
                    {
                        if ((int)data >= 0)
                        {
                            string sent = "*D" + register.ToString() + data.ToString() + "-";
                            if (this.ready)
                            {
                                this.serialPort.WriteLine(sent);
                                this.lastResult = new Result(true, "Sent: " + sent);
                            }
                            else
                            {
                                if (this.sendQueue.Count <= queueSize)
                                {
                                    this.sendQueue.Enqueue(sent);
                                    this.lastResult = new Result(true, "Queued: " + sent);
                                }
                                else
                                {
                                    this.lastResult = new Result(false, "Max queue size limit reached");
                                }
                            }
                            this.ready = false;
                            return this.lastResult;
                        }
                        else
                        {
                            this.lastResult = new Result(false, "Out of range");
                        }
                    }
                    else if (data is uint)
                    {
                        if ((uint)data <= 4294967295)
                        {
                            string sent = "*D" + Usefuls.NarsMethods.fixedLengthHex(register, 4) + Usefuls.NarsMethods.fixedLengthHex((uint)data, 8) + "-";
                            if (this.ready)
                            {
                                this.serialPort.WriteLine(sent);
                                this.lastResult = new Result(true, "Sent: " + sent);
                            }
                            else
                            {
                                if (this.sendQueue.Count <= queueSize)
                                {
                                    this.sendQueue.Enqueue(sent);
                                    this.lastResult = new Result(true, "Queued: " + sent);
                                }
                                else
                                {
                                    this.lastResult = new Result(false, "Max queue size limit reached");
                                }
                            }
                            this.ready = false;
                            return this.lastResult;
                        }
                        else
                        {
                            this.lastResult = new Result(false, "Out of range");
                            return this.lastResult;
                        }
                    }
                    else if (data is string)
                    {
                        string sent = "*D" + register.ToString() + data + "-";
                        if (this.ready)
                        {
                            this.serialPort.WriteLine(sent);
                            this.lastResult = new Result(true, "Sent: " + sent);
                        }
                        else
                        {
                            if (this.sendQueue.Count <= queueSize)
                            {
                                this.sendQueue.Enqueue(sent);
                                this.lastResult = new Result(true, "Queued: " + sent);
                            }
                            else
                            {
                                this.lastResult = new Result(false, "Max queue size limit reached");
                            }
                        }
                        this.ready = false;
                        return this.lastResult;
                    }
                    this.lastResult = new Result(false, "False type: " + data.GetType().ToString());
                    return this.lastResult;
                }
                else
                {
                    this.lastResult = new Result(false, "Out of range");
                    return this.lastResult;
                }
            }

            public Result disconnect()
            {
                if (this.state == State.Connected)
                {
                    this.state = State.Disconnected;
                    this.serialPort.WriteLine("*E-");
                    this.serialPort.Close();
                    return lastResult = new Result(true, "Disconnected");
                }
                else
                {
                    return this.lastResult = new Result(false, "Already Disconnected");
                }
            }

            public bool checkIfSpecial(ushort register)
            {
                if (this.receivedData[register].Equals(typeof(ushort)))
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }

            private void DataRecievedHandler(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
            {
                this.tempPort = (System.IO.Ports.SerialPort)sender;
                string line = this.tempPort.ReadLine();

                if (line.Length >= 6)
                {
                    string command = line.Substring(0, 2);
                    if (line.Substring(0, 2) == "*D")
                    {
                        if (line.Length == 16)
                        {
                            ushort tempRegister = ushort.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                            uint tempData = uint.Parse(line.Substring(6, 8), System.Globalization.NumberStyles.HexNumber);
                            this.receivedData[tempRegister] = tempData;
                            if (tempRegister == 0)
                            {
                                this.ready = System.Convert.ToBoolean(tempData);
                                lastRecieve = new Receive(true, "Received: " + line, false, tempRegister, tempData);
                            }
                            else
                            {
                                lastRecieve = new Receive(true, "Received: " + line, false, tempRegister, tempData);
                            }
                        }
                        else
                        {
                            this.lastRecieve = new Receive(false, "Data Loss, DCommand.Length", false, 0, 0);
                        }
                    }
                    else
                    {
                        this.lastRecieve = new Receive(false, "Data Loss, DCommand", false, 0, 0);
                    }

                    if (line.Substring(0, 2) == "*S")
                    {
                        ushort tempRegister = ushort.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                        string tempData = line.Substring(6, line.Length - 8);
                        this.receivedData[tempRegister] = tempData;
                        lastRecieve = new Receive(true, "Received: " + line, true, tempRegister, tempData);
                    }
                    else if (!lastRecieve.success)
                    {
                        this.lastRecieve = new Receive(false, "Data Loss, SCommand ", true, 0, 0);
                    }
                }
                else
                {
                    this.lastRecieve = new Receive(false, "Possible Data Loss Line: " + line, false, 0, 0);
                }

                if (onReceiveHandler != null)
                {
                    onReceiveHandler(this.lastRecieve);
                }
            }

            private void onTimeOut(object source, System.Timers.ElapsedEventArgs e)
            {
                timedOut = true;
            }
        }

        /// <summary>
        /// Class for communication with an arduino through a serial port. Must use with Nars Arduino library.
        /// </summary>
        public class NarsSerialComOpenNET
        {

            public object[] receivedData = new object[65535];

            public State state = State.Disconnected;

            public System.Collections.Generic.Queue<string> sendQueue = new System.Collections.Generic.Queue<string>();

            private System.Action<Receive> onReceiveHandler = null;

            public bool ready = false;

            private System.Timers.Timer timer = new System.Timers.Timer();

            public OpenNETCF.IO.Ports.SerialPort serialPort = new OpenNETCF.IO.Ports.SerialPort();

            public OpenNETCF.IO.Ports.SerialPort tempPort = new OpenNETCF.IO.Ports.SerialPort();

            public Receive lastRecieve;

            public Result lastResult;

            public int timeout = 700;

            private bool timedOut = false;

            public byte queueSize = 10;

            public NarsSerialComOpenNET()
            {
                this.serialPort.BaudRate = 1000000;
                this.serialPort.DataBits = 8;
                this.serialPort.Parity = OpenNETCF.IO.Ports.Parity.None;
                this.serialPort.StopBits = OpenNETCF.IO.Ports.StopBits.One;
                this.serialPort.ReceivedEvent += DataRecievedHandler;
                this.timer = new System.Timers.Timer();
                this.timer.AutoReset = false;
                this.timer.Enabled = true;
                this.timer.Elapsed += onTimeOut;
                this.lastResult = new Result(true, "Instantiated");
            }

            public void addOnReceiveHandler(System.Action<Receive> _onReceiveHandler)
            {
                this.onReceiveHandler = _onReceiveHandler;
            }

            public Result connect(string port)
            {
                if (this.state == State.Disconnected)
                {
                    this.serialPort.PortName = port;
                    this.serialPort.Open();
                    this.state = State.Connected;
                    return lastResult = new Result(true, "Connected");
                }
                else
                {
                    return lastResult = new Result(false, "Already connected");
                }
            }

            public Result checkQueue()
            {
                if (this.sendQueue.Count > 0)
                {
                    if (this.ready)
                    {
                        string sent = sendQueue.Dequeue();
                        this.serialPort.WriteLine(sent);
                        return this.lastResult = new Result(true, $"Sent: {sent}");
                    }
                    else
                    {
                        return this.lastResult = new Result(false, "Not Ready");
                    }
                }
                else
                {
                    return this.lastResult = new Result(false, "Queue Empty");
                }
            }

            public Result send(ushort register, object data)
            {
                if (register <= 65535)
                {
                    if (data is int)
                    {
                        if ((int)data >= 0)
                        {
                            string sent = "*D" + Usefuls.NarsMethods.fixedLengthHex(register, 4) + Usefuls.NarsMethods.fixedLengthHex((long)data, 8) + "-";
                            if (this.ready)
                            {
                                this.serialPort.WriteLine(sent);
                                return this.lastResult = new Result(true, $"Sent: {sent}");
                            }
                            else
                            {
                                if (sendQueue.Count <= queueSize)
                                {
                                    this.sendQueue.Enqueue(sent);
                                    return this.lastResult = new Result(true, $"Queued: {sent}");
                                }
                                else
                                {
                                    return this.lastResult = new Result(false, "Max queue size limit reached");
                                }
                            }
                        }
                        else
                        {
                            return this.lastResult = new Result(false, "Out of range");
                        }
                    }
                    else if (data is uint)
                    {
                        string sent = "*D" + Usefuls.NarsMethods.fixedLengthHex(register, 4) + Usefuls.NarsMethods.fixedLengthHex((uint)data, 8) + "-";
                        if (this.ready)
                        {
                            this.serialPort.WriteLine(sent);
                            return this.lastResult = new Result(true, $"Sent: {sent}");
                        }
                        else
                        {
                            if (sendQueue.Count <= queueSize)
                            {
                                this.sendQueue.Enqueue(sent);
                                return this.lastResult = new Result(true, $"Queued: {sent}");
                            }
                            else
                            {
                                return this.lastResult = new Result(false, "Max queue size limit reached");
                            }
                        }
                    }
                    else if (data is string)
                    {
                        string sent = "*D" + Usefuls.NarsMethods.fixedLengthHex(register, 4) + (string)data + "-";
                        if (this.ready)
                        {
                            this.serialPort.WriteLine(sent);
                            return this.lastResult = new Result(true, $"Sent: {sent}");
                        }
                        else
                        {
                            if (sendQueue.Count <= queueSize)
                            {
                                this.sendQueue.Enqueue(sent);
                                return this.lastResult = new Result(true, $"Queued: {sent}");
                            }
                            else
                            {
                                return this.lastResult = new Result(false, "Max queue size limit reached");
                            }
                        }
                    }
                    else
                    {
                        return this.lastResult = new Result(false, $"Invalid type: {data.GetType().ToString()}");
                    }
                }
                else
                {
                    return this.lastResult = new Result(false, "Out of range");
                }
            }

            public Result disconnect()
            {
                if (this.state == State.Connected)
                {
                    serialPort.WriteLine("*E-");
                    serialPort.Close();
                    return this.lastResult = new Result(true, "Disconneced");
                }
                else
                {
                    return this.lastResult = new Result(false, "Already disconnected");
                }
            }

            private void onTimeOut(object source, System.Timers.ElapsedEventArgs e)
            {
                this.timedOut = true;
            }

            private void DataRecievedHandler(object sender, OpenNETCF.IO.Ports.SerialReceivedEventArgs e)
            {
                tempPort = (OpenNETCF.IO.Ports.SerialPort)sender;
                string temp = tempPort.ReadExisting();

                foreach (string line in temp.Split('\n'))
                {
                    if (line.Length >= 6)
                    {
                        if (line.Substring(0, 2) == "*D")
                        {
                            if (line.Length == 16)
                            {

                                ushort tempRegister = ushort.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                                uint tempData = uint.Parse(line.Substring(6, 8), System.Globalization.NumberStyles.HexNumber);
                                this.receivedData[tempRegister] = tempData;
                                if (tempRegister == 0)
                                {
                                    this.ready = System.Convert.ToBoolean(tempData);
                                    this.lastRecieve = new Receive(true, "Ready set: " + line, false, 0, tempData);
                                    if (this.onReceiveHandler != null) onReceiveHandler(lastRecieve);
                                }
                                else
                                {
                                    this.lastRecieve = new Receive(true, "Received: " + line, false, tempRegister, tempData);
                                    if (this.onReceiveHandler != null) onReceiveHandler(lastRecieve);
                                }
                            }
                        }


                        if (line.Substring(0, 2) == "*S")
                        {
                            ushort tempRegister = ushort.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                            string tempData = line.Substring(6, line.Length - 8);
                            this.receivedData[tempRegister] = tempData;
                            this.lastRecieve = new Receive(true, "Received: " + line, true, tempRegister, tempData);
                            if (this.onReceiveHandler != null) onReceiveHandler(lastRecieve);
                        }
                    }
                    else
                    {
                        this.lastRecieve = new Receive(false, "Possible Data Loss Line (less than 6): " + line, false, 0, 0);
                        if (this.onReceiveHandler != null) onReceiveHandler(lastRecieve);
                    }
                }
            }
        }
    }

    namespace Usefuls
    {
        /// <summary>
        /// Useful methods
        /// </summary>
        public static class NarsMethods
        {
            /// <summary>
            /// Returns fixed length hex string from number.
            /// </summary>
            /// <param name="input">Number</param>
            /// <param name="length">Length</param>
            public static string fixedLengthHex(long input, byte length)
            {
                string output = input.ToString("X");
                if (output.Length != length)
                {
                    string original = output;
                    int originalLength = output.Length;
                    output = "";
                    for (int i = 0; i < length - originalLength; i++)
                    {
                        output += "0";
                    }
                    output += original;
                }
                return output;
            }

            /// <summary>
            /// Returns fixed length hex string from number.
            /// </summary>
            /// <param name="input">Number</param>
            /// <param name="length">Length</param>
            public static string fixedLengthHex(uint input, byte length)
            {
                string output = input.ToString("X");
                if (output.Length != length)
                {
                    string original = output;
                    int originalLength = output.Length;
                    output = "";
                    for (int i = 0; i < length - originalLength; i++)
                    {
                        output += "0";
                    }
                    output += original;
                }
                return output;
            }
        }

        /// <summary>
        /// Byte bit operations extension
        /// </summary>
        public static class ByteExtenstions
        {
            /// <summary>
            /// Toggle bit.
            /// </summary>
            /// <param name="b"></param>
            /// <param name="pos">bit</param>
            /// <returns></returns>
            /// <exception cref="System.ArgumentOutOfRangeException"></exception>
            public static byte ToggleBit(this byte b, int pos)
            {
                if (pos < 0 || pos > 7)
                    throw new System.ArgumentOutOfRangeException("pos", "Index must be in the range of 0-7.");

                return (byte)(b ^ (1 << pos));
            }

            /// <summary>
            /// Check if bit is set.
            /// </summary>
            /// <param name="b"></param>
            /// <param name="pos">bit</param>
            /// <returns></returns>
            /// <exception cref="System.ArgumentOutOfRangeException"></exception>
            public static bool IsBitSet(this byte b, int pos)
            {
                if (pos < 0 || pos > 7)
                    throw new System.ArgumentOutOfRangeException("pos", "Index must be in the range of 0-7.");

                return (b & (1 << pos)) != 0;
            }

            /// <summary>
            /// Set bit.
            /// </summary>
            /// <param name="b"></param>
            /// <param name="pos">bit</param>
            /// <returns></returns>
            /// <exception cref="System.ArgumentOutOfRangeException"></exception>
            public static byte SetBit(this byte b, int pos)
            {
                if (pos < 0 || pos > 7)
                    throw new System.ArgumentOutOfRangeException("pos", "Index must be in the range of 0-7.");

                return (byte)(b | (1 << pos));
            }

            /// <summary>
            /// Unset bit.
            /// </summary>
            /// <param name="b"></param>
            /// <param name="pos">bit</param>
            /// <returns></returns>
            /// <exception cref="System.ArgumentOutOfRangeException"></exception>
            public static byte UnsetBit(this byte b, int pos)
            {
                if (pos < 0 || pos > 7)
                    throw new System.ArgumentOutOfRangeException("pos", "Index must be in the range of 0-7.");

                return (byte)(b & ~(1 << pos));
            }

            /// <summary>
            /// To string in binary
            /// </summary>
            /// <param name="b"></param>
            /// <returns></returns>
            public static string ToBinaryString(this byte b)
            {
                return System.Convert.ToString(b, 2).PadLeft(8, '0');
            }
        }
    }
}