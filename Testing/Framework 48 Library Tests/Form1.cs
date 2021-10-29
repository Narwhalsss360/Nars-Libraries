using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Nars_Libraries_Framework48.Serial;
using Nars_Libraries_Framework48.Usefuls;

namespace Framework48_NarsSerialCom_Test
{
    public partial class Form1 : Form
    {

        NarsSerialComOpenNET comOpenNET = new NarsSerialComOpenNET(); //global serialport for openNET
        NarsSerialCom comSystem = new NarsSerialCom(); //global serialport for System
        bool timeStamps; //global if timestamps is on
        DateTime dateTime; //datetime struct instance for timestamps
        object arg1 = null;
        object arg2 = null;
        bool randomSends = false;
        Timer timer = new Timer();

        enum Sources
        {
            NONE,
            SYSTEM,
            OPENNET,
            USEFULS
        } //sources for testing

        Sources source; //source instance
        string selectedPort; //selected port 

        public Form1()
        {
            InitializeComponent();
            sourceComboBox.SelectedIndex = 0;
            comSystem.addOnReceiveHandler(onSystemRecv);
            comOpenNET.addOnReceiveHandler(onOpenRecv);
            this.FormClosing += onExit;
            timer.Tick += sendRandom;
        }

        private void sourceComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comOpenNET.state == NarsSerialComOpenNET.States.CONNECTED)
            {
                comOpenNET.disconnect();
            }
            if (comSystem.state == NarsSerialCom.States.CONNECTED)
            {
                comSystem.disconnect();
            }
            switch (sourceComboBox.SelectedIndex)
            {
                case 0:
                    source = Sources.SYSTEM;
                    sendButton.Enabled = false;
                    hexCheckBox.Enabled = true;
                    getButton.Enabled = true;
                    break;
                case 1:
                    source = Sources.OPENNET;
                    sendButton.Enabled = false;
                    hexCheckBox.Enabled = true;
                    getButton.Enabled = true;
                    break;
                case 2:
                    source = Sources.USEFULS;
                    sendButton.Enabled = true;
                    hexCheckBox.Enabled = false;
                    getButton.Enabled = false;
                    break;
                default:
                    break;
            }
            setItems();
            refreshButton_Click(null, null);
        }
        
        void setItems()
        {
            switch (source)
            {
                case Sources.NONE:
                    break;
                case Sources.SYSTEM:
                    registerLabel.Text = "Register";
                    dataLabel.Text = "Data";
                    connectButton.Text = "Connect";
                    break;
                case Sources.OPENNET:
                    registerLabel.Text = "Register";
                    dataLabel.Text = "Data";
                    connectButton.Text = "Connect";
                    break;
                case Sources.USEFULS:
                    registerLabel.Text = "Argument";
                    dataLabel.Text = "Value";
                    connectButton.Text = "Calculate";
                    break;
                default:
                    break;
            }
            printOut($"Info: Switched to {source.ToString()} source.");
        }

        private void portComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (source == Sources.USEFULS)
            {
                switch (portComboBox.SelectedIndex)
                {
                    case 0:
                        printOut("Enter Argument 1 as long for number(long input, byte length)");
                        printOut("Enter Argument 2 as byte for end string length(long input, byte length)");
                        break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        printOut("Enter Argument 1 as byte value (this byte b, int pos)");
                        printOut("Enter Argument 2 as bit position (this byte b, int pos)");
                        break;
                    case 5:
                        printOut("Enter Argument 1 as byte value (this byte b)");
                        break;
                    default:
                        break;
                }
            }
        }

        private void refreshButton_Click(object sender, EventArgs e)
        {
            switch (source)
            {
                case Sources.NONE:
                    printOut("Warning: Must Select Serial Port source.");
                    break;
                case Sources.SYSTEM:
                    portComboBox.Items.Clear();
                    foreach (string portName in System.IO.Ports.SerialPort.GetPortNames())
                    {
                        portComboBox.Items.Add(portName);
                    }
                    break;
                case Sources.OPENNET:
                    portComboBox.Items.Clear();
                    foreach (string portName in OpenNETCF.IO.Ports.SerialPort.GetPortNames())
                    {
                        portComboBox.Items.Add(portName);
                    }
                    break;
                case Sources.USEFULS:
                    portComboBox.Items.Clear();
                    portComboBox.Items.Add("Fixed Length Hex");
                    portComboBox.Items.Add("Toggle Bit");
                    portComboBox.Items.Add("Is Bit Set");
                    portComboBox.Items.Add("Set Bit");
                    portComboBox.Items.Add("Unset Bit");
                    portComboBox.Items.Add("To Binary String");
                    break;
                default:
                    break;
            }
        }
        
        private void connectButton_Click(object sender, EventArgs e)
        {
            switch (source)
            {
                case Sources.NONE:
                    {
                        printOut("Warning: Must Select Serial Port source.");
                        break;
                    }
                case Sources.SYSTEM:
                    { 
                        if (comSystem.state == NarsSerialCom.States.DISCONNECTED)
                        {
                            if (portComboBox.Text != "")
                            {
                                selectedPort = portComboBox.Text;
                                NarsSerialCom.result result = comSystem.connect(selectedPort);
                                if (result.complete)
                                {
                                    connectButton.Text = "Disconnect";
                                    printOut($"Info: Connected through System source on port {selectedPort}.");
                                    sendButton.Enabled = true;
                                    sourceComboBox.Enabled = false;
                                    portComboBox.Enabled = false;
                                }
                                else
                                {
                                    printOut($" Error: {result.message} {result.error.ToString()}");
                                }
                            }
                            else
                            {
                                printOut("Error: Select a port.");
                            }
                            
                        }
                        else
                        {
                            NarsSerialCom.result result = comSystem.disconnect();
                            if (result.complete)
                            {
                                connectButton.Text = "Connect";
                                printOut($"Info: Disconnected from System source.");
                                sendButton.Enabled = false;
                                sourceComboBox.Enabled = true;
                                portComboBox.Enabled = true;
                            }
                            else
                            {
                                printOut($"Error: {result.message} {result.error.ToString()}");
                            }
                        }
                        break;
                    }
                case Sources.OPENNET:
                    {
                        if (comOpenNET.state == NarsSerialComOpenNET.States.DISCONNECTED)
                        {
                            if (portComboBox.Text != "")
                            {
                                selectedPort = "\\\\.\\" + portComboBox.Text;
                                printOut(selectedPort);
                                NarsSerialComOpenNET.result result = comOpenNET.connect(selectedPort);
                                if (result.complete)
                                {
                                    connectButton.Text = "Disconnect";
                                    printOut($"Info: Connected through openNET source on port {selectedPort}.");
                                    sendButton.Enabled = true;
                                    sourceComboBox.Enabled = false;
                                    portComboBox.Enabled = false;
                                }
                                else
                                {
                                    printOut($" Error: {result.message} {result.error.ToString()}");
                                }
                            }
                            else
                            {
                                printOut("Error: Select a port.");
                            }
                        }
                        else
                        {
                            NarsSerialComOpenNET.result result = comOpenNET.disconnect();
                            if (result.complete)
                            {
                                connectButton.Text = "Connect";
                                printOut($"Info: Disconnected from openNET source.");
                                sendButton.Enabled = false;
                                sourceComboBox.Enabled = true;
                                portComboBox.Enabled = true;
                            }
                            else
                            {
                                printOut($"Error: {result.message} {result.error.ToString()}");
                            }
                        }
                        break;
                    }
                case Sources.USEFULS:
                    {
                        switch (portComboBox.SelectedIndex)
                        {
                            case 0:
                                {
                                    if (arg1 != null && arg2 != null)
                                    {
                                        printOut("Return: " + NarsMethods.fixedLengthHex((long)arg1, (byte)arg2));
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                        printOut($"Argument 2: {arg2}");
                                    }
                                    break;
                                }
                            case 1:
                                {
                                    if (arg1 != null && arg2 != null)
                                    {
                                        byte temp = (byte)arg1;
                                        printOut($"Original: {temp.ToBinaryString()}");
                                        temp = temp.ToggleBit((int)arg2);
                                        printOut($"Return:  {temp.ToBinaryString()}");
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                        printOut($"Argument 2: {arg2}");
                                    }
                                    break;
                                }
                            case 2:
                                {
                                    if (arg1 != null && arg2 != null)
                                    {
                                        byte temp = (byte)arg1;
                                        printOut($"Original: {temp.ToBinaryString()}");
                                        printOut($"Return: {temp.IsBitSet((int)arg2)}");
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                        printOut($"Argument 2: {arg2}");
                                    }
                                    arg1 = null;
                                    arg2 = null;
                                    break;
                                }
                            case 3:
                                {
                                    if (arg1 != null && arg2 != null)
                                    {
                                        byte temp = (byte)arg1;
                                        printOut($"Original: {temp.ToBinaryString()}");
                                        printOut($"Return: {temp = temp.SetBit((int)arg2)}");
                                        printOut($"After: {temp.ToBinaryString()}");
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                        printOut($"Argument 2: {arg2}");
                                    }
                                    arg1 = null;
                                    arg2 = null;
                                    break;
                                }
                            case 4:
                                {
                                    if (arg1 != null && arg2 != null)
                                    {
                                        byte temp = (byte)arg1;
                                        printOut($"Original: {temp.ToBinaryString()}");
                                        printOut($"Return: {temp = temp.UnsetBit((int)arg2)}");
                                        printOut($"After: {temp.ToBinaryString()}");
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                        printOut($"Argument 2: {arg2}");
                                    }
                                    arg1 = null;
                                    arg2 = null;
                                    break;
                                }
                             case 5:
                                {
                                    if (arg1 != null)
                                    {
                                        byte temp = (byte)arg1;
                                        printOut("Return: " + temp.ToBinaryString());
                                        arg1 = null;
                                        arg2 = null;
                                    }
                                    else
                                    {
                                        printOut("Not all arguments set!");
                                        printOut($"Argument 1: {arg1}");
                                    }
                                    break;
                                }
                            default:
                                break;
                        }
                        break;
                    }
                default:
                    break;
            }
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            int register;
            uint data;
            bool ok = true;
            if (hexCheckBox.Checked)
            {
                if (!int.TryParse(registerTextBox.Text, System.Globalization.NumberStyles.HexNumber, null, out register))
                {
                    printOut("Error: Could not parse register.");
                    ok = false; 
                }

                if (!uint.TryParse(dataTextBox.Text, System.Globalization.NumberStyles.HexNumber, null, out data))
                {
                    printOut("Error: Could not parse data.");
                    ok = false;
                }
            }
            else
            {
                if (!int.TryParse(registerTextBox.Text, out register))
                {
                    printOut("Error: Could not parse register.");
                    ok = false;
                }
                if (!uint.TryParse(dataTextBox.Text, out data))
                {
                    printOut("Error: Could not parse data.");
                    ok = false;
                }
            }
            if (register == 0xFFFF)
            {
                if (data == 0)
                {
                    timer.Interval = 1;
                }
                else
                {
                    timer.Interval = (int)data;
                }

                if (randomSends)
                {
                    randomSends = false;
                    printOut("Info: Turned off Random Sends.");
                }
                else
                {
                    randomSends = true;
                    printOut("Info: Turned on Random Sends.");
                }
            }
            switch (source)
            {
                case Sources.NONE:
                    printOut("Warning: Must Select Serial Port source.");
                    break;
                case Sources.SYSTEM:
                    {
                        if (ok)
                        {
                            if (!randomSends)
                            {
                                NarsSerialCom.result result = comSystem.sendData(register, data);
                                if (result.complete)
                                {
                                    printOut($"Sent: {result.message}");
                                }
                                else
                                {
                                    printOut($"Error: {result.error.ToString()}");
                                }
                            }
                        }
                        break;
                    }
                case Sources.OPENNET:
                    {
                        if (ok)
                        {
                            if (!randomSends)
                            {
                                NarsSerialComOpenNET.result result = comOpenNET.sendData(register, data);
                                if (result.complete)
                                {
                                    printOut($"Sent: {result.message}");
                                }
                                else
                                {
                                    printOut($"Error: {result.error.ToString()}");
                                }
                            }

                        }
                        break;
                    }
                case Sources.USEFULS:
                    {
                        switch (portComboBox.SelectedIndex)
                        {
                            case 0:
                                {
                                    int outi;
                                    if (int.TryParse(registerTextBox.Text, out outi))
                                    {
                                        if (outi == 1)
                                        {
                                            if (arg1 == null)
                                            {
                                                long outl;
                                                if (long.TryParse(dataTextBox.Text, out outl))
                                                {
                                                    arg1 = outl;
                                                    printOut($"Info: Set Argument 1 as {outl}.");
                                                }
                                                else
                                                {
                                                    printOut($"Error: Could not parse {dataTextBox.Text}.");
                                                }
                                            }
                                            else
                                            {
                                                printOut($"Warning: Argument 1 is already set as {arg1}.");
                                            }
                                        }
                                        else
                                        {
                                            if (arg2 == null)
                                            {
                                                byte outb;
                                                if (byte.TryParse(dataTextBox.Text, out outb))
                                                {
                                                    arg2 = outb;
                                                    printOut($"Info: Set Argument 2 as {outb}.");
                                                }
                                                else
                                                {
                                                    printOut($"Error: Could not parse {dataTextBox.Text}.");
                                                }
                                            }
                                            else
                                            {
                                                printOut($"Warning: Argument 2 is already set as {arg2}.");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printOut($"Warning: Could not parse {registerTextBox.Text}");
                                    }
                                    break;
                                }
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                {
                                    int outi;
                                    if (int.TryParse(registerTextBox.Text, out outi))
                                    {
                                        if (outi == 1)
                                        {
                                            if (arg1 == null)
                                            {
                                                byte outb;
                                                if (byte.TryParse(dataTextBox.Text, out outb))
                                                {
                                                    arg1 = outb;
                                                    printOut($"Info: Set Argument 1 as {outb}.");
                                                }
                                                else
                                                {
                                                    printOut($"Error: Could not parse {dataTextBox.Text}.");
                                                }
                                            }
                                            else
                                            {
                                                printOut($"Argument 1 is already set as {arg1}.");
                                            }
                                        }
                                        else
                                        {
                                            if (arg2 == null)
                                            {
                                                int outi2;
                                                if (int.TryParse(dataTextBox.Text, out outi2))
                                                {
                                                    arg2 = outi2;
                                                    printOut($"Info: Set Argument 2 as {outi2}.");
                                                }
                                            }
                                            else
                                            {
                                                printOut($"Warning: Argument 2 is already set as {arg2}.");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printOut($"Error: Could not parse {registerTextBox.Text}");
                                    }
                                    break;
                                }
                            default:
                                break;
                        }
                        break;
                    }
                default:
                    break;
            }
            if (randomSends)
            {
                timer.Start();
                timer.Enabled = true;
            }
            else
            {
                timer.Enabled = false;
                timer.Stop();
            }
        }

        private void getButton_Click(object sender, EventArgs e)
        {
            int register;
            if(!int.TryParse(registerTextBox.Text, out register))
            {
                register = int.Parse(registerTextBox.Text, System.Globalization.NumberStyles.HexNumber);
            }
            if (register <= 65535)
            {
                switch (source)
                {
                    case Sources.NONE:
                        printOut("Warning: Must Select Serial Port source.");
                        break;
                    case Sources.SYSTEM:
                        NarsSerialCom.dataResult dataResult = comSystem.getData(register);
                        MessageBoxButtons buttons = MessageBoxButtons.OK;
                        if (dataResult.isString)
                        {
                            printOut("Got " + dataResult.dataString + " From " + register.ToString());
                            MessageBox.Show("Got " + dataResult.dataString + " From " + register.ToString(), "Error", buttons);
                        }
                        else
                        {
                            printOut("Got " + dataResult.dataLong.ToString() + " From " + register.ToString());
                            MessageBox.Show("Got " + dataResult.dataLong.ToString() + " From " + register.ToString(), "Error", buttons);
                        }
                        break;
                    case Sources.OPENNET:
                        NarsSerialComOpenNET.dataResult dataResulto = comOpenNET.getData(register);
                        MessageBoxButtons buttonso = MessageBoxButtons.OK;
                        if (dataResulto.isString)
                        {
                            printOut("Got " + dataResulto.dataString + " From " + register.ToString());
                            MessageBox.Show("Got " + dataResulto.dataString + " From " + register.ToString(), "Error", buttonso);
                        }
                        else
                        {
                            printOut("Got " + dataResulto.dataLong.ToString() + " From " + register.ToString());
                            MessageBox.Show("Got " + dataResulto.dataLong.ToString() + " From " + register.ToString(), "Error", buttonso);
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                MessageBox.Show("OUT_OF_RANGE", "Error", buttons);
            }
            
        }

        private void timestampsCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            timeStamps = timestampsCheckBox.Checked;
        }

        void sendRandom(Object sender, EventArgs eventArgs)
        {
            Random random = new Random();
            int register = random.Next(0, 65535);
            uint data = int.MaxValue + (uint)random.Next(0, int.MaxValue);
            switch (source)
            {
                case Sources.NONE:
                    break;
                case Sources.SYSTEM:
                    {
                        NarsSerialCom.result result = comSystem.sendData(register, data);
                        if (result.complete)
                        {
                            printOut($"Sent: {result.message}");
                        }
                        else
                        {
                            printOut($"Error: {result.error.ToString()}");
                        }
                    }
                    break;
                case Sources.OPENNET:
                    {
                        NarsSerialComOpenNET.result result = comOpenNET.sendData(register, data);
                        if (result.complete)
                        {
                            printOut($"Sent: {result.message}");
                        }
                        else
                        {
                            printOut($"Error: {result.error.ToString()}");
                        }
                    }
                    break;
                case Sources.USEFULS:
                    break;
                default:
                    break;
            }
        }

        void onOpenRecv(NarsSerialComOpenNET.message message)
        {
            outputTextBox.Invoke(new MethodInvoker(delegate { printOut($"Received {message.raw}"); }));
        }

        void onSystemRecv(NarsSerialCom.message message)
        {
            outputTextBox.Invoke(new MethodInvoker(delegate { printOut($"Received {message.raw}"); }));
        }

        private void printOut(string input)
        {
            if (timeStamps)
            {
                dateTime = DateTime.Now;
                if (outputTextBox.Lines.Length != 0)
                {
                    outputTextBox.AppendText(Environment.NewLine + dateTime.TimeOfDay.ToString() + ": " + input);
                }
                else
                {
                    outputTextBox.AppendText(dateTime.TimeOfDay.ToString() + ": " + input);
                }
            }
            else
            {
                if (outputTextBox.Lines.Length != 0)
                {
                    outputTextBox.AppendText(Environment.NewLine + input);
                }
                else
                {
                    outputTextBox.AppendText(input);
                }
            }
        }

        private void clearButton_Click(object sender, EventArgs e)
        {
            outputTextBox.Clear();
        }

        void onExit(object sender, FormClosingEventArgs e)
        {
            if (comOpenNET.state == NarsSerialComOpenNET.States.CONNECTED)
            {
                comOpenNET.disconnect();
            }
            if (comSystem.state == NarsSerialCom.States.CONNECTED)
            {
                comSystem.disconnect();
            }
            comSystem.serialPort.Dispose();
            comOpenNET.serialPort.Dispose();
        }
    }
}