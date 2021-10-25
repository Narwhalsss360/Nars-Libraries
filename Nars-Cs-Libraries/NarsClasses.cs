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

        public struct message
        {
            public bool complete;
            public string sentMessage;
            public string errorMessage;
        }

        public System.IO.Ports.SerialPort serialPort;
        /// <summary>
        /// Data storage array for all recieved data. Array index is register.
        /// </summary>
        public long[] receivedData = new long[65535];
        /// <summary>
        /// State of connection.
        /// </summary>
        States state = States.DISCONNECTED;

        NarsSerialCom()
        {
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.StopBits = System.IO.Ports.StopBits.One;
            serialPort.DataBits = 8;
            serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(DataRecievedHandler);
        }

        /// <summary>
        /// Sends data to arduino using Nars Protocol. Returns message Data-Type
        /// </summary>
        /// <param name="register">Data register</param>
        /// <param name="data">Data</param>
        /// <returns>Message Data-Type</returns>
        public message sendData(int register, long data)
        {
            message newMessage;

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
                        newMessage.sentMessage = completeString;
                        newMessage.errorMessage = "";
                        return newMessage;
                    }
                    else
                    {
                        newMessage.complete = false;
                        newMessage.sentMessage = "";
                        newMessage.errorMessage = "Data Out-Of-Range";
                        return newMessage;
                    }
                }
                else
                {
                    newMessage.complete = false;
                    newMessage.sentMessage = "";
                    newMessage.errorMessage = "Register Out-Of-Range";
                    return newMessage;
                }
            }
            else
            {
                newMessage.complete = false;
                newMessage.sentMessage = "";
                newMessage.errorMessage = "Not Connected";
                return newMessage;
            }
        }

        /// <summary>
        /// Sends data as string to arduino using Nars Protocol. Returns message Data-Type
        /// </summary>
        /// <param name="register">Data register</param>
        /// <param name="data">Data string</param>
        /// <returns></returns>
        public message sendSpecialData(int register, string data)
        {
            message newMessage;

            if (state == States.CONNECTED)
            {
                string completeString = "*D";
                if (register <= 65535)
                {
                    string registerString = NarsMethods.fixedLengthHex(register, 4);
                    completeString += registerString + data + "-";
                    serialPort.WriteLine(completeString);
                    newMessage.complete = true;
                    newMessage.sentMessage = completeString;
                    newMessage.errorMessage = "";
                    return newMessage;
                }
                else
                {
                    newMessage.complete = false;
                    newMessage.sentMessage = "";
                    newMessage.errorMessage = "Register Out-Of-Range";
                    return newMessage;
                }
            }
            else
            {
                newMessage.complete = false;
                newMessage.sentMessage = "";
                newMessage.errorMessage = "Not Connected";
                return newMessage;
            }
        }

        /// <summary>
        /// Connect to arduino. Sends connect message.
        /// </summary>
        public void connect()
        {
            if (state == States.DISCONNECTED)
            {
                serialPort.Open();
                state = States.CONNECTED;
                serialPort.WriteLine("*B-");
            }
        }

        /// <summary>
        /// Disonnect from arduino. Sends disconnect message.
        /// </summary>
        public void disconnect()
        {
            if (state == States.CONNECTED)
            {
                serialPort.WriteLine("*E-");
                serialPort.Close();
                state = States.DISCONNECTED;
            }
        }

        private void DataRecievedHandler(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {

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
