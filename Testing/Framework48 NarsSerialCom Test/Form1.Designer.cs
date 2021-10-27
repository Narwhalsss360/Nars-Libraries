namespace Framework48_NarsSerialCom_Test
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.refreshButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            this.getButton = new System.Windows.Forms.Button();
            this.sendButton = new System.Windows.Forms.Button();
            this.registerTextBox = new System.Windows.Forms.TextBox();
            this.dataTextBox = new System.Windows.Forms.TextBox();
            this.outputTextBox = new System.Windows.Forms.TextBox();
            this.portComboBox = new System.Windows.Forms.ComboBox();
            this.sourceComboBox = new System.Windows.Forms.ComboBox();
            this.timestampsCheckBox = new System.Windows.Forms.CheckBox();
            this.registerLabel = new System.Windows.Forms.Label();
            this.dataLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // refreshButton
            // 
            this.refreshButton.Location = new System.Drawing.Point(132, 10);
            this.refreshButton.Name = "refreshButton";
            this.refreshButton.Size = new System.Drawing.Size(53, 23);
            this.refreshButton.TabIndex = 0;
            this.refreshButton.Text = "Refresh";
            this.refreshButton.UseVisualStyleBackColor = true;
            this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(12, 67);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(75, 23);
            this.connectButton.TabIndex = 1;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // getButton
            // 
            this.getButton.Location = new System.Drawing.Point(181, 69);
            this.getButton.Name = "getButton";
            this.getButton.Size = new System.Drawing.Size(32, 23);
            this.getButton.TabIndex = 2;
            this.getButton.Text = "Get";
            this.getButton.UseVisualStyleBackColor = true;
            this.getButton.Click += new System.EventHandler(this.getButton_Click);
            // 
            // sendButton
            // 
            this.sendButton.Enabled = false;
            this.sendButton.Location = new System.Drawing.Point(217, 69);
            this.sendButton.Name = "sendButton";
            this.sendButton.Size = new System.Drawing.Size(40, 23);
            this.sendButton.TabIndex = 3;
            this.sendButton.Text = "Send";
            this.sendButton.UseVisualStyleBackColor = true;
            this.sendButton.Click += new System.EventHandler(this.sendButton_Click);
            // 
            // registerTextBox
            // 
            this.registerTextBox.Location = new System.Drawing.Point(61, 39);
            this.registerTextBox.Name = "registerTextBox";
            this.registerTextBox.Size = new System.Drawing.Size(38, 20);
            this.registerTextBox.TabIndex = 4;
            // 
            // dataTextBox
            // 
            this.dataTextBox.Location = new System.Drawing.Point(141, 39);
            this.dataTextBox.Name = "dataTextBox";
            this.dataTextBox.Size = new System.Drawing.Size(67, 20);
            this.dataTextBox.TabIndex = 5;
            // 
            // outputTextBox
            // 
            this.outputTextBox.Enabled = false;
            this.outputTextBox.Location = new System.Drawing.Point(12, 98);
            this.outputTextBox.Multiline = true;
            this.outputTextBox.Name = "outputTextBox";
            this.outputTextBox.Size = new System.Drawing.Size(245, 294);
            this.outputTextBox.TabIndex = 6;
            // 
            // portComboBox
            // 
            this.portComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.portComboBox.FormattingEnabled = true;
            this.portComboBox.Location = new System.Drawing.Point(191, 12);
            this.portComboBox.Name = "portComboBox";
            this.portComboBox.Size = new System.Drawing.Size(66, 21);
            this.portComboBox.TabIndex = 7;
            // 
            // sourceComboBox
            // 
            this.sourceComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.sourceComboBox.FormattingEnabled = true;
            this.sourceComboBox.Items.AddRange(new object[] {
            "System",
            "openNET"});
            this.sourceComboBox.Location = new System.Drawing.Point(12, 12);
            this.sourceComboBox.Name = "sourceComboBox";
            this.sourceComboBox.Size = new System.Drawing.Size(114, 21);
            this.sourceComboBox.TabIndex = 8;
            this.sourceComboBox.SelectedIndexChanged += new System.EventHandler(this.sourceComboBox_SelectedIndexChanged);
            // 
            // timestampsCheckBox
            // 
            this.timestampsCheckBox.AutoSize = true;
            this.timestampsCheckBox.Location = new System.Drawing.Point(93, 73);
            this.timestampsCheckBox.Name = "timestampsCheckBox";
            this.timestampsCheckBox.Size = new System.Drawing.Size(82, 17);
            this.timestampsCheckBox.TabIndex = 9;
            this.timestampsCheckBox.Text = "Timestamps";
            this.timestampsCheckBox.UseVisualStyleBackColor = true;
            this.timestampsCheckBox.CheckedChanged += new System.EventHandler(this.timestampsCheckBox_CheckedChanged);
            // 
            // registerLabel
            // 
            this.registerLabel.AutoSize = true;
            this.registerLabel.Location = new System.Drawing.Point(9, 42);
            this.registerLabel.Name = "registerLabel";
            this.registerLabel.Size = new System.Drawing.Size(46, 13);
            this.registerLabel.TabIndex = 10;
            this.registerLabel.Text = "Register";
            // 
            // dataLabel
            // 
            this.dataLabel.AutoSize = true;
            this.dataLabel.Location = new System.Drawing.Point(105, 42);
            this.dataLabel.Name = "dataLabel";
            this.dataLabel.Size = new System.Drawing.Size(30, 13);
            this.dataLabel.TabIndex = 11;
            this.dataLabel.Text = "Data";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(268, 406);
            this.Controls.Add(this.dataLabel);
            this.Controls.Add(this.registerLabel);
            this.Controls.Add(this.timestampsCheckBox);
            this.Controls.Add(this.sourceComboBox);
            this.Controls.Add(this.portComboBox);
            this.Controls.Add(this.outputTextBox);
            this.Controls.Add(this.dataTextBox);
            this.Controls.Add(this.registerTextBox);
            this.Controls.Add(this.sendButton);
            this.Controls.Add(this.getButton);
            this.Controls.Add(this.connectButton);
            this.Controls.Add(this.refreshButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Framework48 Tests";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button refreshButton;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.Button getButton;
        private System.Windows.Forms.Button sendButton;
        private System.Windows.Forms.TextBox registerTextBox;
        private System.Windows.Forms.TextBox dataTextBox;
        private System.Windows.Forms.TextBox outputTextBox;
        private System.Windows.Forms.ComboBox portComboBox;
        private System.Windows.Forms.ComboBox sourceComboBox;
        private System.Windows.Forms.CheckBox timestampsCheckBox;
        private System.Windows.Forms.Label registerLabel;
        private System.Windows.Forms.Label dataLabel;
    }
}

