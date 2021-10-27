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

namespace Framework48_NarsSerialCom_Test
{
    public partial class Form1 : Form
    {

        NarsSerialComOpenNET comOpenNET = new NarsSerialComOpenNET(); //global serialport for openNET
        NarsSerialCom comSystem = new NarsSerialCom(); //global serialport for System
        bool timeStamps; //global if timestamps is on
        DateTime dateTime; //datetime struct instance for timestamps

        enum Sources
        {
            NONE,
            SYSTEM,
            OPENNET
        } //sources for testing

        Sources source; //source instance
        string selectedPort; //selected port 

        public Form1()
        {
            InitializeComponent();
            sourceComboBox.SelectedIndex = 0;
            comSystem.addOnReceiveHandler(onSystemRecv);
            comOpenNET.addOnReceiveHandler(onOpenRecv);
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            switch (source)
            {
                case Sources.NONE:
                    printOut("Warning: Must Select Serial Port source.");
                    break;
                case Sources.SYSTEM:
                    break;
                case Sources.OPENNET:
                    break;
                default:
                    break;
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
                default:
                    break;
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

        private void connectButton_Click(object sender, EventArgs e)
        {
            switch (source)
            {
                case Sources.NONE:
                    printOut("Warning: Must Select Serial Port source.");
                    break;
                case Sources.SYSTEM:
                    if (comSystem.state == NarsSerialCom.States.DISCONNECTED)
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
                case Sources.OPENNET:
                    if (comOpenNET.state == NarsSerialComOpenNET.States.DISCONNECTED)
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
                default:
                    break;
            }
        }

        private void timestampsCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            timeStamps = timestampsCheckBox.Checked;
        }

        private void printOut(string input)
        {
            if (timeStamps)
            {
                dateTime = DateTime.Now;
                outputTextBox.AppendText(Environment.NewLine + dateTime.ToString() + ": " + input);
            }
            else
            {
                outputTextBox.AppendText(Environment.NewLine + input);
            }
        }

        private void sourceComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sourceComboBox.SelectedIndex == 0)
            {
                source = Sources.SYSTEM;
            }
            if (sourceComboBox.SelectedIndex == 1)
            {
                source = Sources.OPENNET;
                
            }
            printOut($"Info: Switched to {source.ToString()} source.");
            refreshButton_Click(null, null);
        }

        void onOpenRecv(NarsSerialComOpenNET.message message)
        {
            outputTextBox.Invoke(new MethodInvoker(delegate { printOut($"Received {message.raw}"); }));
        }

        void onSystemRecv(NarsSerialCom.message message)
        {
            outputTextBox.Invoke(new MethodInvoker(delegate { printOut($"Received {message.raw}"); }));
        }
    }
}
