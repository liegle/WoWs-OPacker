namespace WoWs_Officialmod_Packer
{
	partial class StartMenu
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
			this.startText = new System.Windows.Forms.Label();
			this.startButton = new System.Windows.Forms.Button();
			this.foundModsBox = new System.Windows.Forms.CheckedListBox();
			this.executeButton = new System.Windows.Forms.Button();
			this.bgWorker = new System.ComponentModel.BackgroundWorker();
			this.SuspendLayout();
			// 
			// startText
			// 
			this.startText.Dock = System.Windows.Forms.DockStyle.Top;
			this.startText.Location = new System.Drawing.Point(0, 0);
			this.startText.Name = "startText";
			this.startText.Size = new System.Drawing.Size(284, 34);
			this.startText.TabIndex = 0;
			this.startText.Text = "版本：\r\n反馈BUG私信B站UID38112033";
			// 
			// startButton
			// 
			this.startButton.Location = new System.Drawing.Point(15, 151);
			this.startButton.Name = "startButton";
			this.startButton.Size = new System.Drawing.Size(254, 30);
			this.startButton.TabIndex = 1;
			this.startButton.Text = "开始搜索xml";
			this.startButton.UseVisualStyleBackColor = true;
			this.startButton.Click += new System.EventHandler(this.StartButton_Click);
			// 
			// foundModsBox
			// 
			this.foundModsBox.CheckOnClick = true;
			this.foundModsBox.FormattingEnabled = true;
			this.foundModsBox.Location = new System.Drawing.Point(-1, 34);
			this.foundModsBox.Name = "foundModsBox";
			this.foundModsBox.Size = new System.Drawing.Size(286, 112);
			this.foundModsBox.TabIndex = 2;
			// 
			// executeButton
			// 
			this.executeButton.Enabled = false;
			this.executeButton.Location = new System.Drawing.Point(15, 151);
			this.executeButton.Name = "executeButton";
			this.executeButton.Size = new System.Drawing.Size(254, 30);
			this.executeButton.TabIndex = 3;
			this.executeButton.Text = "处理选中xml";
			this.executeButton.UseVisualStyleBackColor = true;
			this.executeButton.Visible = false;
			this.executeButton.Click += new System.EventHandler(this.ExecuteButton_Click);
			// 
			// bgWorker
			// 
			this.bgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.BgWorker_DoWork);
			// 
			// StartMenu
			// 
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(284, 188);
			this.Controls.Add(this.executeButton);
			this.Controls.Add(this.foundModsBox);
			this.Controls.Add(this.startButton);
			this.Controls.Add(this.startText);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "StartMenu";
			this.ShowIcon = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "OPacker";
			this.ResumeLayout(false);

		}

		#endregion

		private Label startText;
		private Button startButton;
		private CheckedListBox foundModsBox;
        private Button executeButton;
		public System.ComponentModel.BackgroundWorker bgWorker;
	}
}
