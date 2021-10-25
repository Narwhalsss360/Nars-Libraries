namespace Nars_Cs_Libraries
{

    /// <summary>
    /// Class for communication with an arduino through a serial port. Must use with Nars Arduino library.
    /// </summary>
    public class NarsSerialCom
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

        public System.IO.Ports.SerialPort serialPort = new System.IO.Ports.SerialPort();
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

        /// <summary>
        /// Constructor
        /// </summary>
        public NarsSerialCom()
        {
            serialPort.BaudRate = 1000000;
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.StopBits = System.IO.Ports.StopBits.One;
            serialPort.DataBits = 8;
            serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(DataRecievedHandler);
        }

        /// <summary>
        /// Add handler that invokes after recieving data.
        /// </summary>
        /// <param name="onReceivePointer">System.Action pointer</param>
        public void addOnReceiveHandler(System.Action<message> onReceivePointer)
        {
            onReceiveHandler = onReceivePointer;
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
                        string registerString = NarsMethods.fixedLengthHex(register, 4);
                        string dataString = NarsMethods.fixedLengthHex(data, 8);
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
                    string registerString = NarsMethods.fixedLengthHex(register, 4);
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
        /// Connect to arduino. Sends connect message.
        /// </summary>
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

        /// <summary>
        /// On Receive
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DataRecievedHandler(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            System.IO.Ports.SerialPort temp = (System.IO.Ports.SerialPort)sender;
            string line = temp.ReadLine();
            message newMessage;
            if (line.Substring(0, 2) == "*D")
            {
                if (line.Length == 16)
                {
                    newMessage.raw = line;
                    newMessage.special = false;
                    newMessage.register = int.Parse(line.Substring(2, 4), System.Globalization.NumberStyles.HexNumber);
                    newMessage.data = line.Substring(6, 8);
                    receivedData[newMessage.register] = newMessage.data;
                    onReceiveHandler(newMessage);
                }
            }
            else if(line.Substring(0, 2) == "*S")
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
