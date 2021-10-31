namespace Nars_Libraries_Framework48
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
            DISCONNECTED,
            CONNECTED,
            LOST
        }

        public class Receive : Result
        {
            readonly bool special;
            readonly ushort register;
            readonly object data;

            public Receive(bool _success, string _message, bool _special, ushort _register, object _data):base(_success, _message)
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

            public State state = State.DISCONNECTED;

            public System.Collections.Generic.Queue<string> sendQueue = new System.Collections.Generic.Queue<string>();

            private System.Action<Receive> onReceiveHandler = null;

            private bool ready = false;

            private System.Timers.Timer timer = new System.Timers.Timer();

            public System.IO.Ports.SerialPort serialPort = new System.IO.Ports.SerialPort();

            private System.IO.Ports.SerialPort tempPort = new System.IO.Ports.SerialPort();

            public Receive lastRecieve;

            public Result lastResult;

            public int timeout = 700;

            private bool timedOut = false;

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
                if (this.state == State.DISCONNECTED)
                {
                    this.serialPort.PortName = port;
                    this.serialPort.Open();
                    this.timer.Start();
                    while (!this.ready)
                    {
                        if (this.timedOut)
                        {
                            this.timedOut = false;
                            this.lastResult = new Result(false, "Timed out");
                            this.timer.Stop();
                            return this.lastResult;
                        }
                    }
                    if (ready)
                    {
                        this.serialPort.WriteLine("*B-");
                        this.lastResult = new Result(true, "Connected");
                        this.timer.Stop();
                        this.timedOut = false;
                    }
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
                        this.lastResult = new Result(false, "Not ready");
                    }
                }
                return this.lastResult;
            }

            public Result send(ushort register, object data)
            {
                if (register <= 65535)
                {
                    if (data.Equals(typeof(int)))
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
                                this.sendQueue.Enqueue(sent);
                                this.lastResult = new Result(true, "Queued: " + sent);
                            }
                        }
                        else
                        {
                            this.lastResult = new Result(false, "Out of range");
                        }
                    } 
                    else if (data.Equals(typeof(uint)))
                    {
                        if ((uint)data <= 4294967295)
                        {
                            string sent = "*D" + register.ToString() + data.ToString() + "-";
                            if (this.ready)
                            {
                                this.serialPort.WriteLine(sent);
                                this.lastResult = new Result(true, "Sent: " + sent);
                            }
                            else
                            {
                                this.sendQueue.Enqueue(sent);
                                this.lastResult = new Result(true, "Queued: " + sent);
                            }
                        }
                        else
                        {
                            this.lastResult = new Result(false, "Out of range");
                        }
                    }
                    else if (data.Equals(typeof(string)))
                    {
                        string sent = "*D" + register.ToString() + data + "-";
                        if (this.ready)
                        {
                            this.serialPort.WriteLine(sent);
                            this.lastResult = new Result(true, "Sent: " + sent);
                        }
                        else
                        {
                            this.sendQueue.Enqueue(sent);
                            this.lastResult = new Result(true, "Queued: " + sent);
                        }
                    }
                    else
                    {
                        this.lastResult = new Result(false, "Invalid type");
                    }
                }
                else
                {
                    this.lastResult = new Result(false, "Out of range");
                }
                return this.lastResult;
            }

            public Result disconnect()
            {
                if (this.state == State.CONNECTED)
                {
                    this.timer.Start();
                    while(!this.ready)
                    {
                        if (this.timedOut)
                        {
                            this.timedOut = false;
                            this.serialPort.WriteLine("*E-");
                            this.lastResult = new Result(false, "Timed out, Disconnected");
                            this.timer.Stop();
                            return this.lastResult;
                        }
                    }
                    if (this.ready)
                    {
                        this.serialPort.WriteLine("*E-");
                        this.lastResult = new Result(true, "Disconnected");
                        this.timer.Stop();
                        this.timedOut = false;
                    }
                }
                else
                {
                    lastResult = new Result(false, "Already Disconnected");
                }
                return lastResult;
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
                            lastRecieve = new Receive(true, "READT MESSAGE: Received: " + line, false, tempRegister, tempData);
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
                    if (line.Length <= 6)
                    {
                        ushort tempRegister = ushort.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                        string tempData = line.Substring(6, line.Length - 8);
                        this.receivedData[tempRegister] = tempData;
                        lastRecieve = new Receive(true, "Received: " + line, true, tempRegister, tempData);
                    }
                    else
                    {
                        this.lastRecieve = new Receive(false, "Data Loss, SCommand.Length", true, 0, 0);
                    }
                }
                else
                {
                    this.lastRecieve = new Receive(false, "Data Loss, SCommand", true, 0, 0);
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
            /// <summary>
            /// States for connected enumerator.
            /// </summary>
            public enum States
            {
                DISCONNECTED,
                CONNECTED,
            }

            /// <summary>
            /// Error type
            /// </summary>
            public enum Errors
            {
                NONE,
                OUT_OF_RANGE,
                NOT_CONNECTED,
                ALREADY_CONNECTED
            }

            /// <summary>
            /// message type for receiving
            /// </summary>
            public struct message
            {
                public bool special;
                public int register;
                public string data;
                public string raw;
            }

            /// <summary>
            /// result type for method returns
            /// </summary>
            public struct result
            {
                public bool complete;
                public string message;
                public Errors error;
            }

            /// <summary>
            /// dataResult type for getter method
            /// </summary>
            public struct dataResult
            {
                public bool isString;
                public string dataString;
                public long dataLong;
            }

            /// <summary>
            /// Data storage array for all recieved data. Array index is register.
            /// </summary>
            public string[] receivedData = new string[65535];

            /// <summary>
            /// State of connection.
            /// </summary>
            public States state = States.DISCONNECTED;

            /// <summary>
            /// Pointer for data recieve handler
            /// </summary>
            private System.Action<message> onReceiveHandler = null;

            public OpenNETCF.IO.Ports.SerialPort serialPort = new OpenNETCF.IO.Ports.SerialPort();

            /// <summary>
            /// Constructor
            /// </summary>
            public NarsSerialComOpenNET()
            {
                serialPort.BaudRate = 1000000;
                serialPort.DataBits = 8;
                serialPort.Parity = OpenNETCF.IO.Ports.Parity.None;
                serialPort.StopBits = OpenNETCF.IO.Ports.StopBits.One;
                serialPort.ReceivedEvent += DataRecievedHandler;
            }

            /// <summary>
            /// Disonnect from arduino. Sends disconnect message.
            /// </summary>
            public result disconnect()
            {
                result result;
                if (state == States.CONNECTED)
                {
                    result.complete = true;
                    result.message = "Disconnected";
                    result.error = Errors.NONE;
                    serialPort.WriteLine("*E-");
                    serialPort.Close();
                    state = States.DISCONNECTED;
                    return result;
                }
                else
                {
                    result.complete = false;
                    result.message = "Error";
                    result.error = Errors.NOT_CONNECTED;
                    return result;
                }
            }

            public result connect(string port)
            {
                result result;
                if (state == States.DISCONNECTED)
                {
                    result.complete = true;
                    result.message = "Connected";
                    result.error = Errors.NONE;
                    serialPort.PortName = port;
                    serialPort.Open();
                    state = States.CONNECTED;
                    serialPort.WriteLine("*B-");
                    return result;
                }
                else
                {
                    result.complete = false;
                    result.message = "Error";
                    result.error = Errors.ALREADY_CONNECTED;
                    return result;
                }
            }

            /// <summary>
            /// Get data from a register.
            /// </summary>
            /// <param name="register"></param>
            /// <returns>dataResult type</returns>
            public dataResult getData(int register)
            {
                dataResult data;
                long val;
                if (long.TryParse(receivedData[register], System.Globalization.NumberStyles.HexNumber, null, out val))
                {
                    data.dataLong = val;
                    data.isString = false;
                    data.dataString = "";
                    return data;
                }
                else
                {
                    data.dataLong = 0;
                    data.isString = true;
                    data.dataString = receivedData[register];
                    return data;
                }
            }

            /// <summary>
            /// Sends data to arduino using Nars Protocol. Returns message Data-Type
            /// </summary>
            /// <param name="register">Data register</param>
            /// <param name="data">Data</param>
            /// <returns>Message Data-Type</returns>
            public result sendData(int register, long data)
            {
                result newMessage;

                if (state == States.CONNECTED)
                {
                    string completeString = "*D";
                    if (register <= 65535)
                    {
                        if (data <= 4294967295)
                        {
                            string registerString = Usefuls.NarsMethods.fixedLengthHex(register, 4);
                            string dataString = Usefuls.NarsMethods.fixedLengthHex(data, 8);
                            completeString += registerString + dataString + "-";
                            serialPort.WriteLine(completeString);
                            newMessage.complete = true;
                            newMessage.message = completeString;
                            newMessage.error = Errors.NONE;
                            return newMessage;
                        }
                        else
                        {
                            newMessage.complete = false;
                            newMessage.message = "Error";
                            newMessage.error = Errors.OUT_OF_RANGE;
                            return newMessage;
                        }
                    }
                    else
                    {
                        newMessage.complete = false;
                        newMessage.message = "";
                        newMessage.error = Errors.OUT_OF_RANGE;
                        return newMessage;
                    }
                }
                else
                {
                    newMessage.complete = false;
                    newMessage.message = "Error";
                    newMessage.error = Errors.NOT_CONNECTED;
                    return newMessage;
                }
            }

            /// <summary>
            /// Sends data as string to arduino using Nars Protocol. Returns message Data-Type
            /// </summary>
            /// <param name="register">Data register</param>
            /// <param name="data">Data string</param>
            /// <returns></returns>
            public result sendSpecialData(int register, string data)
            {
                result newMessage;

                if (state == States.CONNECTED)
                {
                    string completeString = "*S";
                    if (register <= 65535)
                    {
                        string registerString = Usefuls.NarsMethods.fixedLengthHex(register, 4);
                        completeString += registerString + data + "-";
                        serialPort.WriteLine(completeString);
                        newMessage.complete = true;
                        newMessage.message = completeString;
                        newMessage.error = Errors.NONE;
                        return newMessage;
                    }
                    else
                    {
                        newMessage.complete = false;
                        newMessage.message = "Error";
                        newMessage.error = Errors.OUT_OF_RANGE;
                        return newMessage;
                    }
                }
                else
                {
                    newMessage.complete = false;
                    newMessage.message = "Error";
                    newMessage.error = Errors.NOT_CONNECTED;
                    return newMessage;
                }
            }

            /// <summary>
            /// Add handler that invokes after recieving data.
            /// </summary>
            /// <param name="onReceivePointer">System.Action pointer</param>
            public void addOnReceiveHandler(System.Action<message> onReceivePointer)
            {
                onReceiveHandler = onReceivePointer;
            }

            private void DataRecievedHandler(object sender, OpenNETCF.IO.Ports.SerialReceivedEventArgs e)
            {
                OpenNETCF.IO.Ports.SerialPort temp = (OpenNETCF.IO.Ports.SerialPort)sender;
                string line = temp.ReadExisting();
                message newMessage;
                if (line.Length >= 6)
                {
                    if (line.Substring(0, 2) == "*D")
                    {
                        if (line.Length == 17)
                        {
                            newMessage.raw = line;
                            newMessage.special = false;
                            newMessage.register = int.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                            newMessage.data = line.Substring(6, 8);
                            receivedData[newMessage.register] = newMessage.data;
                            onReceiveHandler(newMessage);
                        }
                    }
                    else if (line.Substring(0, 2) == "*S")
                    {
                        newMessage.raw = line;
                        newMessage.special = true;
                        newMessage.register = int.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                        newMessage.data = line.Substring(6, line.Length - 8);
                        receivedData[newMessage.register] = newMessage.data;
                        onReceiveHandler(newMessage);
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