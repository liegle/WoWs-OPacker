using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WoWs_Officialmod_Packer
{
	public partial class ProgressWindow : Form
    {
		private int receive;
		public StartMenu parentWindow;
		public ProgressWindow(StartMenu parentWindow)
		{
			InitializeComponent();
			this.parentWindow = parentWindow;
		}
		public void UpdateProgress(ProgressChangedEventArgs e)
		{
			ProgressData data = e.UserState as ProgressData;
			receive = data.Progress > 100 ? 100 : data.Progress < 0 ? 0 : data.Progress;
			progressBar.Value = receive;
			mainProgressNum.Text = receive + "%";
			switch (e.ProgressPercentage)
			{
				case 0:
				{
					nowExecuting.Text = "正在载入res_unpack\\banks\\OfficialMods\\" + data.CommName + "\\mod.xml";
					break;
				}
				case 1:
				{
					nowExecuting.Text = "正在创建bin\\banks\\Mods\\" + data.CommName;
					break;
				}
				case 2:
				{
					nowExecuting.Text = "正在写入bin\\banks\\Mods\\" + data.CommName + "\\mod.xml";
					break;
				}
				case 3:
				{
					nowExecuting.Text = "正在复制" + data.CommName;
					break;
				}
				default:
				{
					break;
				}
			}
		}

		private void CancelButton_Click(object sender, EventArgs e)
		{
			if (parentWindow.bgWorker.IsBusy)
			{
				parentWindow.bgWorker.CancelAsync();
			}
		}
	}
}
