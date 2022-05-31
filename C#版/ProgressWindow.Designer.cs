namespace WoWs_Officialmod_Packer
{
    partial class ProgressWindow
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
			this.progressBar = new System.Windows.Forms.ProgressBar();
			this.nowExecuting = new System.Windows.Forms.Label();
			this.mainProgressNum = new System.Windows.Forms.Label();
			this.cancelButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// progressBar
			// 
			this.progressBar.Location = new System.Drawing.Point(10, 10);
			this.progressBar.Name = "progressBar";
			this.progressBar.Size = new System.Drawing.Size(364, 20);
			this.progressBar.TabIndex = 0;
			// 
			// nowExecuting
			// 
			this.nowExecuting.Location = new System.Drawing.Point(10, 33);
			this.nowExecuting.Name = "nowExecuting";
			this.nowExecuting.Size = new System.Drawing.Size(384, 19);
			this.nowExecuting.TabIndex = 2;
			this.nowExecuting.Text = "正在";
			// 
			// mainProgressNum
			// 
			this.mainProgressNum.Location = new System.Drawing.Point(383, 10);
			this.mainProgressNum.Name = "mainProgressNum";
			this.mainProgressNum.Size = new System.Drawing.Size(100, 20);
			this.mainProgressNum.TabIndex = 3;
			this.mainProgressNum.Text = "0%";
			this.mainProgressNum.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// cancelButton
			// 
			this.cancelButton.Location = new System.Drawing.Point(287, 55);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(87, 25);
			this.cancelButton.TabIndex = 5;
			this.cancelButton.Text = "取消";
			this.cancelButton.UseVisualStyleBackColor = true;
			this.cancelButton.Click += new System.EventHandler(this.CancelButton_Click);
			// 
			// ProgressWindow
			// 
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
			this.ClientSize = new System.Drawing.Size(411, 85);
			this.ControlBox = false;
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.mainProgressNum);
			this.Controls.Add(this.nowExecuting);
			this.Controls.Add(this.progressBar);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ProgressWindow";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "正在生成...";
			this.ResumeLayout(false);

        }

		#endregion
		private ProgressBar progressBar;
        private Label nowExecuting;
        private Label mainProgressNum;
		private Button cancelButton;
	}
}
