using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace WoWs_Officialmod_Packer
{
	public partial class StartMenu : Form
	{
        private int OPVersion = 2;
		protected ProgressWindow progressWindow;
		public StartMenu()
		{
			InitializeComponent();
			MyInitialize();
		}
		private void MyInitialize()
		{
			progressWindow = new ProgressWindow(this);
			startText.Text = "版本：" + OPVersion + "\r\n反馈BUG私信B站UID38112033";
			foundModsBox.Items.Clear();
			bgWorker.WorkerReportsProgress = true;
			bgWorker.WorkerSupportsCancellation = true;
			bgWorker.ProgressChanged += BgWorker_ProgressChanged;
			bgWorker.RunWorkerCompleted += BgWorker_RunWorkerCompleted;
		}
		private void StartButton_Click(object sender, EventArgs e)
		{
			DirectoryInfo officialmodsDir = new DirectoryInfo("res_unpack\\banks\\OfficialMods");

			if (!officialmodsDir.Exists)
			{
				MessageBox.Show("目标文件夹不存在");
			}
			else
			{
				DirectoryInfo[] modDirArray = officialmodsDir.GetDirectories();

				if (modDirArray.Length == 0)
				{
					MessageBox.Show("目标文件夹为空");
				}
				else
				{
					FindModsToBox(modDirArray);
				}
			}
		}
		private void FindModsToBox(DirectoryInfo[] modDirArray)
		{
			string noneModXmlDirs = "";
			int modXmlCount = 0;
			int noneModXmlCount = 0;

			startButton.Enabled = false;
			startButton.Visible = false;
			executeButton.Enabled = true;
			executeButton.Visible = true;
			foreach (DirectoryInfo modDir in modDirArray)
			{
				FileInfo modXml = new FileInfo(modDir.FullName + "\\mod.xml");
				if (modXml.Exists)
				{
					foundModsBox.Items.Add(modDir.Name);
					foundModsBox.SetItemChecked(foundModsBox.Items.Count - 1, true);
					++modXmlCount;
				}
				else
				{
					noneModXmlDirs += (noneModXmlDirs == "" ? "" : ", ") + modDir.Name;
					++noneModXmlCount;
				}
			}
			MessageBox.Show("已找到" + modXmlCount + "个mod.xml\n以下" + noneModXmlCount + "个文件夹未找到mod.xml\n" + noneModXmlDirs);
		}
		private void ExecuteButton_Click(object sender, EventArgs e)
        {
			if (foundModsBox.CheckedItems.Count == 0)
			{
				MessageBox.Show("请选择需要生成的Officialmod");
			}
			else if(!bgWorker.IsBusy)
			{
				executeButton.Enabled = false;
				bgWorker.RunWorkerAsync();
				progressWindow.ShowDialog();
			}
        }
		private void BgWorker_DoWork(object sender, DoWorkEventArgs e)
		{
			ProgressData data = new ProgressData();
			XmlDocument modXmlDoc = new XmlDocument();
			XmlNodeList NodeList;
			XmlNode Node;
			int executedModsNum = -1, executedCommsNum, executedNodesNum, executedFilesNum;

			foreach (string modName in foundModsBox.CheckedItems)
			{
				ArrayList commNameList = new ArrayList();

				if (bgWorker.CancellationPending == true)
				{
					e.Cancel = true;
					break;
				}
				//更新进度
				executedModsNum++;
				executedCommsNum = -1;
				data.Progress = 100 * executedModsNum / foundModsBox.CheckedItems.Count;
				data.CommName = modName;
				bgWorker.ReportProgress(0, data);
				//载入初始化角色列表
				modXmlDoc!.Load("res_unpack\\banks\\OfficialMods\\" + modName + "\\mod.xml");
				/*container*/Node = modXmlDoc.SelectSingleNode("/AudioModification.xml/AudioModification/ExternalEvent/Container");
				/*name*/NodeList = /*container*/Node.SelectNodes("Path/StateList/State[Name='CrewName']/Value");
				foreach (XmlNode nameNode in /*name*/NodeList)
				{
					commNameList.Add(nameNode.FirstChild.InnerText);
				}
				foreach (string commName in commNameList)
				{
					if (bgWorker.CancellationPending == true)
					{
						e.Cancel = true;
						break;
					}
					//重新载入
					modXmlDoc!.Load("res_unpack\\banks\\OfficialMods\\" + modName + "\\mod.xml");
					//更新进度
					executedCommsNum++;
					executedNodesNum = -1;
					executedFilesNum = -1;
					data.Progress = (100 * executedModsNum + 100 * executedCommsNum / commNameList.Count) / foundModsBox.CheckedItems.Count;
					data.CommName = modName + " " + commName;
					bgWorker.ReportProgress(1, data);
					//创建"mod名 角色名"文件夹
					DirectoryInfo outPutDir = new DirectoryInfo("bin\\banks\\Mods\\" + modName + ' ' + commName);
					if (!outPutDir.Exists)
					{
						outPutDir.Create();
					}
					//在mod.xml开头的Name结点写入角色名
					modXmlDoc.SelectSingleNode("/AudioModification.xml/AudioModification/Name").FirstChild.InnerText = modName + ' ' + commName;
					//搜索所有Path，如果其下的CrewName的Value不为commName，就将此Path删除
					/*path*/NodeList = modXmlDoc.GetElementsByTagName("Path");
					for (int i = 0; i < NodeList.Count; i++)
					{
						//更新进度
						executedNodesNum++;
						data.Progress = (100 * executedModsNum + (100 * executedCommsNum + 50 * executedNodesNum / NodeList.Count) / commNameList.Count) / foundModsBox.CheckedItems.Count;
						bgWorker.ReportProgress(2, data);
						Node = NodeList[i];
						if (Node.SelectSingleNode("StateList/State[Name='CrewName']/Value").HasChildNodes)
						{
							if (Node.SelectSingleNode("StateList/State[Name='CrewName']/Value").FirstChild.InnerText != commName)
							{
								Node.ParentNode.RemoveChild(Node);
								i--;
							}
							else
							{
								Node.SelectSingleNode("StateList").RemoveChild(Node.SelectSingleNode("StateList/State[Name='CrewName']"));
							}
						}
					}
					modXmlDoc.Save(("bin\\banks\\Mods\\" + modName + ' ' + commName + "\\mod.xml"));
					//复制文件
					NodeList = modXmlDoc.SelectNodes("/AudioModification.xml/AudioModification/ExternalEvent/Container/Path/FilesList/File/Name");
					foreach (XmlNode fileNameNode in NodeList)
					{
						if (bgWorker.CancellationPending == true)
						{
							e.Cancel = true;
							break;
						}
						Node = fileNameNode.FirstChild;
						//更新进度
						executedFilesNum++;
						data.Progress = (100 * executedModsNum + (100 * executedCommsNum + 50 + 50 * executedFilesNum / NodeList.Count) / commNameList.Count) / foundModsBox.CheckedItems.Count;
						data.CommName = Node.InnerText;
						bgWorker.ReportProgress(3, data);
						new FileInfo("res_unpack\\banks\\OfficialMods\\" + modName + "\\" + Node.InnerText).CopyTo("bin\\banks\\Mods\\" + modName + ' ' + commName + '\\' + Node.InnerText, true);
					}
				}
			}
		}
		private void BgWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
		{
			progressWindow.UpdateProgress(e);
		}
		private void BgWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			progressWindow.Close();
			startButton.Enabled = true;
			startButton.Visible = true;
			executeButton.Enabled = false;
			executeButton.Visible = false;
			if (e.Cancelled)
			{
				MessageBox.Show("已中止");
			}
			else
			{
				MessageBox.Show("已完成\nbanks文件夹生成于\\bin下\n请自行移动至bin\\版本编号\\res_mods中（放在readme.txt旁）");
			}
			foundModsBox.Items.Clear();
		}
	}
}
