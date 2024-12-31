namespace Bluetooth_Chess
{
    partial class Game
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("Paired Bluetooth devices", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("Non-paired Bluetooth devices", System.Windows.Forms.HorizontalAlignment.Left);
            this.button2 = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.desk = new System.Windows.Forms.Panel();
            this.deskFull = new System.Windows.Forms.Panel();
            this.GameTypePanel = new System.Windows.Forms.Panel();
            this.OfflineWithoutBot = new System.Windows.Forms.Button();
            this.Online = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.OfflineWithBot = new System.Windows.Forms.Button();
            this.ConnectDevsList = new System.Windows.Forms.ListView();
            this.nameHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.addressHeaher = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ConnetToOtherPanel = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.CancelConnetToOther = new System.Windows.Forms.Button();
            this.ConnetToOtherConfirm = new System.Windows.Forms.Button();
            this.CancelHostingButton = new System.Windows.Forms.Button();
            this.hostRoom = new System.Windows.Forms.Button();
            this.connectToOther = new System.Windows.Forms.Button();
            this.back = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.ConnectTypePanel = new System.Windows.Forms.Panel();
            this.GamePanel = new System.Windows.Forms.Panel();
            this.DisconnectB = new System.Windows.Forms.Button();
            this.RotateDeskB = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.GameTypePanel.SuspendLayout();
            this.ConnetToOtherPanel.SuspendLayout();
            this.ConnectTypePanel.SuspendLayout();
            this.GamePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.Transparent;
            this.button2.FlatAppearance.BorderSize = 0;
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.button2.Location = new System.Drawing.Point(850, 0);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(50, 27);
            this.button2.TabIndex = 0;
            this.button2.TabStop = false;
            this.button2.Text = "✖️";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.ExitButton_click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.WhiteSmoke;
            this.panel1.Controls.Add(this.button2);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(900, 30);
            this.panel1.TabIndex = 2;
            // 
            // desk
            // 
            this.desk.BackColor = System.Drawing.Color.Transparent;
            this.desk.BackgroundImage = global::Bluetooth_Chess.Properties.Resources.Desk;
            this.desk.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.desk.Location = new System.Drawing.Point(34, 64);
            this.desk.Name = "desk";
            this.desk.Size = new System.Drawing.Size(536, 536);
            this.desk.TabIndex = 1;
            // 
            // deskFull
            // 
            this.deskFull.BackColor = System.Drawing.Color.Transparent;
            this.deskFull.BackgroundImage = global::Bluetooth_Chess.Properties.Resources.ChessDesk;
            this.deskFull.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.deskFull.Location = new System.Drawing.Point(0, 30);
            this.deskFull.Name = "deskFull";
            this.deskFull.Size = new System.Drawing.Size(603, 603);
            this.deskFull.TabIndex = 0;
            // 
            // GameTypePanel
            // 
            this.GameTypePanel.BackColor = System.Drawing.Color.Transparent;
            this.GameTypePanel.Controls.Add(this.OfflineWithoutBot);
            this.GameTypePanel.Controls.Add(this.Online);
            this.GameTypePanel.Controls.Add(this.label1);
            this.GameTypePanel.Controls.Add(this.OfflineWithBot);
            this.GameTypePanel.Location = new System.Drawing.Point(609, 36);
            this.GameTypePanel.Name = "GameTypePanel";
            this.GameTypePanel.Size = new System.Drawing.Size(285, 149);
            this.GameTypePanel.TabIndex = 10;
            // 
            // OfflineWithoutBot
            // 
            this.OfflineWithoutBot.BackColor = System.Drawing.Color.Transparent;
            this.OfflineWithoutBot.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.OfflineWithoutBot.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.OfflineWithoutBot.Location = new System.Drawing.Point(69, 105);
            this.OfflineWithoutBot.Name = "OfflineWithoutBot";
            this.OfflineWithoutBot.Size = new System.Drawing.Size(152, 34);
            this.OfflineWithoutBot.TabIndex = 13;
            this.OfflineWithoutBot.TabStop = false;
            this.OfflineWithoutBot.Text = "Offline without bot";
            this.OfflineWithoutBot.UseVisualStyleBackColor = false;
            this.OfflineWithoutBot.Click += new System.EventHandler(this.OfflineWithoutBot_Click);
            // 
            // Online
            // 
            this.Online.BackColor = System.Drawing.Color.Transparent;
            this.Online.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.Online.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.Online.Location = new System.Drawing.Point(69, 25);
            this.Online.Name = "Online";
            this.Online.Size = new System.Drawing.Size(152, 34);
            this.Online.TabIndex = 11;
            this.Online.TabStop = false;
            this.Online.Text = "Online game";
            this.Online.UseVisualStyleBackColor = false;
            this.Online.Click += new System.EventHandler(this.OnlineGame_click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Cambria", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(74, 2);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(139, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Choose game type";
            // 
            // OfflineWithBot
            // 
            this.OfflineWithBot.BackColor = System.Drawing.Color.Transparent;
            this.OfflineWithBot.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.OfflineWithBot.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.OfflineWithBot.Location = new System.Drawing.Point(69, 65);
            this.OfflineWithBot.Name = "OfflineWithBot";
            this.OfflineWithBot.Size = new System.Drawing.Size(152, 34);
            this.OfflineWithBot.TabIndex = 12;
            this.OfflineWithBot.TabStop = false;
            this.OfflineWithBot.Text = "Offline vs bot";
            this.OfflineWithBot.UseVisualStyleBackColor = false;
            this.OfflineWithBot.Click += new System.EventHandler(this.OfflineWithBot_Click);
            // 
            // ConnectDevsList
            // 
            this.ConnectDevsList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.nameHeader,
            this.addressHeaher});
            listViewGroup1.Header = "Paired Bluetooth devices";
            listViewGroup1.Name = "pGroup";
            listViewGroup2.Header = "Non-paired Bluetooth devices";
            listViewGroup2.Name = "npGroup";
            this.ConnectDevsList.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1,
            listViewGroup2});
            this.ConnectDevsList.HideSelection = false;
            this.ConnectDevsList.Location = new System.Drawing.Point(4, 108);
            this.ConnectDevsList.Name = "ConnectDevsList";
            this.ConnectDevsList.Size = new System.Drawing.Size(275, 305);
            this.ConnectDevsList.TabIndex = 0;
            this.ConnectDevsList.TabStop = false;
            this.ConnectDevsList.UseCompatibleStateImageBehavior = false;
            this.ConnectDevsList.View = System.Windows.Forms.View.Details;
            // 
            // nameHeader
            // 
            this.nameHeader.Text = "Name";
            this.nameHeader.Width = 150;
            // 
            // addressHeaher
            // 
            this.addressHeaher.Text = "Address";
            this.addressHeaher.Width = 120;
            // 
            // ConnetToOtherPanel
            // 
            this.ConnetToOtherPanel.BackColor = System.Drawing.Color.Transparent;
            this.ConnetToOtherPanel.Controls.Add(this.ConnectDevsList);
            this.ConnetToOtherPanel.Controls.Add(this.label3);
            this.ConnetToOtherPanel.Controls.Add(this.CancelConnetToOther);
            this.ConnetToOtherPanel.Controls.Add(this.ConnetToOtherConfirm);
            this.ConnetToOtherPanel.Location = new System.Drawing.Point(609, 36);
            this.ConnetToOtherPanel.Name = "ConnetToOtherPanel";
            this.ConnetToOtherPanel.Size = new System.Drawing.Size(285, 423);
            this.ConnetToOtherPanel.TabIndex = 13;
            this.ConnetToOtherPanel.Visible = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Cambria", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label3.Location = new System.Drawing.Point(89, 1);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(112, 20);
            this.label3.TabIndex = 17;
            this.label3.Text = "Choose device";
            // 
            // CancelConnetToOther
            // 
            this.CancelConnetToOther.BackColor = System.Drawing.Color.Transparent;
            this.CancelConnetToOther.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CancelConnetToOther.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CancelConnetToOther.Location = new System.Drawing.Point(69, 68);
            this.CancelConnetToOther.Name = "CancelConnetToOther";
            this.CancelConnetToOther.Size = new System.Drawing.Size(152, 34);
            this.CancelConnetToOther.TabIndex = 16;
            this.CancelConnetToOther.TabStop = false;
            this.CancelConnetToOther.Text = "Go back";
            this.CancelConnetToOther.UseVisualStyleBackColor = false;
            this.CancelConnetToOther.Click += new System.EventHandler(this.CancelConnectToOther_Click);
            // 
            // ConnetToOtherConfirm
            // 
            this.ConnetToOtherConfirm.BackColor = System.Drawing.Color.Transparent;
            this.ConnetToOtherConfirm.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.ConnetToOtherConfirm.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ConnetToOtherConfirm.Location = new System.Drawing.Point(69, 28);
            this.ConnetToOtherConfirm.Name = "ConnetToOtherConfirm";
            this.ConnetToOtherConfirm.Size = new System.Drawing.Size(152, 34);
            this.ConnetToOtherConfirm.TabIndex = 14;
            this.ConnetToOtherConfirm.TabStop = false;
            this.ConnetToOtherConfirm.Text = "Confirm";
            this.ConnetToOtherConfirm.UseVisualStyleBackColor = false;
            this.ConnetToOtherConfirm.Click += new System.EventHandler(this.ConnectToOtherConfirm_Click);
            // 
            // CancelHostingButton
            // 
            this.CancelHostingButton.BackColor = System.Drawing.Color.Transparent;
            this.CancelHostingButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.CancelHostingButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.CancelHostingButton.Location = new System.Drawing.Point(63, 3);
            this.CancelHostingButton.Name = "CancelHostingButton";
            this.CancelHostingButton.Size = new System.Drawing.Size(152, 34);
            this.CancelHostingButton.TabIndex = 17;
            this.CancelHostingButton.TabStop = false;
            this.CancelHostingButton.Text = "Cancel hosting";
            this.CancelHostingButton.UseVisualStyleBackColor = false;
            // 
            // hostRoom
            // 
            this.hostRoom.BackColor = System.Drawing.Color.Transparent;
            this.hostRoom.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.hostRoom.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.hostRoom.Location = new System.Drawing.Point(69, 24);
            this.hostRoom.Name = "hostRoom";
            this.hostRoom.Size = new System.Drawing.Size(152, 34);
            this.hostRoom.TabIndex = 14;
            this.hostRoom.TabStop = false;
            this.hostRoom.Text = "Host room";
            this.hostRoom.UseVisualStyleBackColor = false;
            this.hostRoom.Click += new System.EventHandler(this.HostRoom_Click);
            // 
            // connectToOther
            // 
            this.connectToOther.BackColor = System.Drawing.Color.Transparent;
            this.connectToOther.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.connectToOther.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.connectToOther.Location = new System.Drawing.Point(69, 64);
            this.connectToOther.Name = "connectToOther";
            this.connectToOther.Size = new System.Drawing.Size(152, 34);
            this.connectToOther.TabIndex = 15;
            this.connectToOther.TabStop = false;
            this.connectToOther.Text = "Connect to other";
            this.connectToOther.UseVisualStyleBackColor = false;
            this.connectToOther.Click += new System.EventHandler(this.ConnectToOther_Click);
            // 
            // back
            // 
            this.back.BackColor = System.Drawing.Color.Transparent;
            this.back.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.back.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.back.Location = new System.Drawing.Point(69, 104);
            this.back.Name = "back";
            this.back.Size = new System.Drawing.Size(152, 34);
            this.back.TabIndex = 16;
            this.back.TabStop = false;
            this.back.Text = "Go back";
            this.back.UseVisualStyleBackColor = false;
            this.back.Click += new System.EventHandler(this.Back_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Cambria", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label2.Location = new System.Drawing.Point(59, 1);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(181, 20);
            this.label2.TabIndex = 17;
            this.label2.Text = "Choose connection type";
            // 
            // ConnectTypePanel
            // 
            this.ConnectTypePanel.Controls.Add(this.label2);
            this.ConnectTypePanel.Controls.Add(this.back);
            this.ConnectTypePanel.Controls.Add(this.connectToOther);
            this.ConnectTypePanel.Controls.Add(this.hostRoom);
            this.ConnectTypePanel.Location = new System.Drawing.Point(609, 36);
            this.ConnectTypePanel.Name = "ConnectTypePanel";
            this.ConnectTypePanel.Size = new System.Drawing.Size(285, 149);
            this.ConnectTypePanel.TabIndex = 12;
            this.ConnectTypePanel.Visible = false;
            // 
            // GamePanel
            // 
            this.GamePanel.Controls.Add(this.DisconnectB);
            this.GamePanel.Controls.Add(this.RotateDeskB);
            this.GamePanel.Location = new System.Drawing.Point(609, 36);
            this.GamePanel.Name = "GamePanel";
            this.GamePanel.Size = new System.Drawing.Size(285, 396);
            this.GamePanel.TabIndex = 15;
            this.GamePanel.Visible = false;
            // 
            // DisconnectB
            // 
            this.DisconnectB.BackColor = System.Drawing.Color.Transparent;
            this.DisconnectB.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.DisconnectB.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.DisconnectB.Location = new System.Drawing.Point(69, 356);
            this.DisconnectB.Name = "DisconnectB";
            this.DisconnectB.Size = new System.Drawing.Size(152, 34);
            this.DisconnectB.TabIndex = 17;
            this.DisconnectB.TabStop = false;
            this.DisconnectB.Text = "Disconnect";
            this.DisconnectB.UseVisualStyleBackColor = false;
            this.DisconnectB.Click += new System.EventHandler(this.DisconnectB_Click);
            // 
            // RotateDeskB
            // 
            this.RotateDeskB.BackColor = System.Drawing.Color.Transparent;
            this.RotateDeskB.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.RotateDeskB.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.RotateDeskB.Location = new System.Drawing.Point(69, 5);
            this.RotateDeskB.Name = "RotateDeskB";
            this.RotateDeskB.Size = new System.Drawing.Size(152, 34);
            this.RotateDeskB.TabIndex = 15;
            this.RotateDeskB.TabStop = false;
            this.RotateDeskB.Text = "Rotate desk";
            this.RotateDeskB.UseVisualStyleBackColor = false;
            this.RotateDeskB.Click += new System.EventHandler(this.RotateDeskB_Click);
            // 
            // Game
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.AliceBlue;
            this.ClientSize = new System.Drawing.Size(900, 630);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.desk);
            this.Controls.Add(this.deskFull);
            this.Controls.Add(this.ConnectTypePanel);
            this.Controls.Add(this.GameTypePanel);
            this.Controls.Add(this.ConnetToOtherPanel);
            this.Controls.Add(this.GamePanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Game";
            this.panel1.ResumeLayout(false);
            this.GameTypePanel.ResumeLayout(false);
            this.GameTypePanel.PerformLayout();
            this.ConnetToOtherPanel.ResumeLayout(false);
            this.ConnetToOtherPanel.PerformLayout();
            this.ConnectTypePanel.ResumeLayout(false);
            this.ConnectTypePanel.PerformLayout();
            this.GamePanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel deskFull;
        private System.Windows.Forms.Panel desk;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel GameTypePanel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button OfflineWithBot;
        private System.Windows.Forms.Button Online;
        private System.Windows.Forms.Button OfflineWithoutBot;
        private System.Windows.Forms.ListView ConnectDevsList;
        private System.Windows.Forms.ColumnHeader nameHeader;
        private System.Windows.Forms.ColumnHeader addressHeaher;
        private System.Windows.Forms.Panel ConnetToOtherPanel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button CancelConnetToOther;
        private System.Windows.Forms.Button ConnetToOtherConfirm;
        private System.Windows.Forms.Button CancelHostingButton;
        private System.Windows.Forms.Button hostRoom;
        private System.Windows.Forms.Button connectToOther;
        private System.Windows.Forms.Button back;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel ConnectTypePanel;
        private System.Windows.Forms.Panel GamePanel;
        private System.Windows.Forms.Button DisconnectB;
        private System.Windows.Forms.Button RotateDeskB;
    }
}

