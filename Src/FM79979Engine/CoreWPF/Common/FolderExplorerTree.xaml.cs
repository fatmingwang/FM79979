using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
namespace CoreWPF.Common
{
    /// <summary>
    /// Interaction logic for FolderExplorerTree.xaml
    /// </summary>
    public partial class FolderExplorerTree : UserControl
    {
        public event EventHandler f_Drop;
        public event EventHandler f_MouseDoubleClick;
        public event EventHandler f_KeyUp;
        private object dummyNode = null;
        public string[] m_strExtensionName;//string[] split = e_strDirectory.Split(new Char[] { '\\'});
                                           //SetWorkingProject("C:\\Users\\Fatming\\Desktop\\FM79979\\Media\\MR\\Tutorial");
        //double m_dbOriginalSize;
        public FolderExplorerTree()
        {
            InitializeComponent();
            RoutedEventArgs e = new RoutedEventArgs();
            Window_Loaded(this, e);
            //m_dbOriginalSize = 300;

        }
        public string SelectedImagePath { get; set; }


        //public string GetFullPath(TreeViewItem e_pTreeViewItem)
        //{
        //    TreeViewItem l_Parent = e_pTreeViewItem;
        //    Array l_FullString;
        //    l_FullString.SetValue(l_Parent.Header, l_iIndex);
        //    while (l_Parent.Parent)
        //    {
        //        l_FullString.SetValue(l_Parent.Header, l_iIndex);
        //        l_Parent = l_Parent.Parent;
        //    }
        //    int l_iIndex = 0;
        //    for (int i = l_FullString.Length - 1; i > -1; --i)
        //    {
        //        if (l_iIndex == 0)
        //        {
        //            ++l_iIndex;
        //        }
        //        else 
        //        {

        //        }
        //    }
        //}
        //0for non match ,1 for my object 2 for folder
        int IsExtensionNameMatched(string e_strExtensionName)
        {
            string l_strExtension = System.IO.Path.GetExtension(e_strExtensionName);
            if (l_strExtension.Length == 0)
                return 2;
            foreach (string l_strExtensionName in m_strExtensionName)
            {
                if (l_strExtensionName.Equals(l_strExtension))
                {//create node
                    return 1;
                }
            }
            return 0;
        }

        int IndexOfTreeViewItem(TreeViewItem e_Item, String e_Name)
        {
            for (int i = 0; i < e_Item.Items.Count; ++i)
            {
                TreeViewItem l_TreeViewItem = (TreeViewItem)e_Item.Items[i];
                if (l_TreeViewItem.Header.ToString().CompareTo(e_Name) == 0)
                    return i;
            }
            return -1;
        }

        void ChanheTreeViewItemName(TreeViewItem e_Item, String e_OldName, String e_NewName)
        {
            for (int i = 0; i < e_Item.Items.Count; ++i)
            {
                TreeViewItem l_TreeViewItem = (TreeViewItem)e_Item.Items[i];
                if (l_TreeViewItem.Header.ToString().CompareTo(e_OldName) == 0)
                {
                    l_TreeViewItem.Header = e_NewName;
                }
            }
        }
        // Define the event handlers. 
        //private void OnChanged(object state)
        private void OnChanged(object source)
        //private void OnChanged(object source, FileSystemEventArgs e)
        {
            FileSystemEventArgs e = (FileSystemEventArgs)source;
            bool l_bNodeExist = IsNodeExists(e.FullPath);
            try
            {
                if (e.ChangeType == WatcherChangeTypes.Created)
                {
                    if (!l_bNodeExist)
                    {
                        int l_ObjectIndex = IsExtensionNameMatched(e.FullPath);
                        {//create node
                            TreeViewItem l_TreeViewItem = GetParentNodeByFileName(e.FullPath);
                            if (l_ObjectIndex == 1)
                                AddNewObjectNode(l_TreeViewItem, e.FullPath);
                            else
                                if (l_ObjectIndex == 2)
                                    AddNewDirectoryNode(l_TreeViewItem, e.FullPath,true);
                        }
                    }
                }
                else
                    if (e.ChangeType == WatcherChangeTypes.Deleted)
                    {
                        TreeViewItem l_TreeViewItem = GetParentNodeByFileName(e.FullPath);
                        string l_strName = System.IO.Path.GetFileName(e.FullPath);
                        int l_iIndex = IndexOfTreeViewItem(l_TreeViewItem, l_strName);
                        if (l_iIndex != -1)
                            l_TreeViewItem.Items.RemoveAt(l_iIndex);
                    }
            }
            catch (Exception) { }
            // Specify what is done when a file is changed, created, or deleted.
            System.Diagnostics.Debug.WriteLine("File: " + e.FullPath + " " + e.ChangeType);
        }

        private void OnRenamed(object source)
        //private void OnRenamed(object source, RenamedEventArgs e)
        {
            RenamedEventArgs e = (RenamedEventArgs)source;
            bool l_bNodeExist = IsNodeExists(e.OldFullPath);
            TreeViewItem l_TreeViewItem = GetParentNodeByFileName(e.OldFullPath);
            string l_strNewName = System.IO.Path.GetFileName(e.FullPath);
            try
            {
                if (l_bNodeExist)
                {//delete old 
                    string l_strOldName = System.IO.Path.GetFileName(e.OldFullPath);
                    int l_iIndex = IndexOfTreeViewItem(l_TreeViewItem, l_strOldName);
                    if (l_iIndex != -1)
                    {
                        ChanheTreeViewItemName(l_TreeViewItem, l_strOldName, l_strNewName);
                        return;
                    }
                }
                else
                {
                    int l_ObjectIndex = IsExtensionNameMatched(e.FullPath);
                    if (l_ObjectIndex == 1)
                        AddNewObjectNode(l_TreeViewItem, e.FullPath);
                    else
                        if (l_ObjectIndex == 2)
                            AddNewDirectoryNode(l_TreeViewItem, e.FullPath, true);
                }
            }
            catch (Exception) { }
        }

        TreeViewItem GetParentNodeByFileName(string e_strFileName)
        {
            string[] split = e_strFileName.Split(new Char[] { '\\' });
            TreeViewItem l_CurrentNode = foldersItem.Items[0] as TreeViewItem;
            TreeViewItem l_TargetNode = null;
            //int l_iIndex = 0;             
            for (int i = 0; i < split.Length-1;++i )
            {
                string s = split[i];
                l_TargetNode = null;
                int l_iCount = l_CurrentNode.Items.Count; ;
                try
                {
                    foreach (TreeViewItem l_pData in l_CurrentNode.Items)
                    {
                        object l_Header = l_pData.Header;
                        if (l_pData != null && l_Header.ToString().CompareTo(s) == 0)
                        {
                            l_CurrentNode = l_pData;
                            l_TargetNode = l_pData;
                            break;
                        }
                    }
                }
                catch (Exception)
                {
                    return l_TargetNode;
                }
            }
            return l_TargetNode;
        }

        bool IsNodeExists(string e_strFileName)
        {
            string[] split = e_strFileName.Split(new Char[] { '\\' });
            TreeViewItem l_CurrentNode = foldersItem.Items[0] as TreeViewItem;
            TreeViewItem l_TargetNode = null;
            //int l_iIndex = 0;             
            foreach (string s in split)
            {
                l_TargetNode = null;
                int l_iCount = l_CurrentNode.Items.Count; ;
                try
                {
                    foreach (TreeViewItem l_pData in l_CurrentNode.Items)
                    {
                        object l_Header = l_pData.Header;
                        if (l_pData != null && l_Header.ToString().CompareTo(s) == 0)
                        {
                            l_CurrentNode = l_pData;
                            l_TargetNode = l_pData;
                            break;
                        }
                    }
                }
                catch (Exception e) 
                {
                    System.Diagnostics.Debug.WriteLine("\n");
                    System.Diagnostics.Debug.WriteLine(e.ToString());
                    return false;
                }
            }
            if ( l_TargetNode != null )
                return true;
            else
                return false;
        }

        void SetFileWatcher(string e_strDirectory)
        {
            //foreach (string l_strExtensionName in m_strExtensionName)
            {
                FileSystemWatcher watcher = new FileSystemWatcher();
                watcher.Path = e_strDirectory;
                /* Watch for changes in LastAccess and LastWrite times, and
                   the renaming of files or directories. */
                watcher.NotifyFilter = NotifyFilters.LastAccess | NotifyFilters.LastWrite
                   | NotifyFilters.FileName | NotifyFilters.DirectoryName;
                //String l_strFilter = "*";
                //l_strFilter += l_strExtensionName;
                //watcher.Filter = l_strFilter;

                // Add event handlers.
                //watcher.Changed += new FileSystemEventHandler(OnChanged);
                //watcher.Created += new FileSystemEventHandler(OnChanged);
                //watcher.Deleted += new FileSystemEventHandler(OnChanged);
                //watcher.Renamed += new RenamedEventHandler(OnRenamed);
                //InvokeRequired 
                var context = System.Threading.SynchronizationContext.Current;
                //set thread back to self or it crush
                watcher.Created += (s, e) => { context.Post(val => OnChanged(e), s); };
                watcher.Changed += (s, e) => { context.Post(val => OnChanged(e), s); };
                watcher.Deleted += (s, e) => { context.Post(val => OnChanged(e), s); };
                watcher.Renamed += (s, e) => { context.Post(val => OnRenamed(e), s); };

                // Begin watching.
                watcher.EnableRaisingEvents = true;
            }        
        }

        public void SetWorkingProject(string e_strDirectory)
        {
            this.m_SelectedFileNameTextBlock.Text = e_strDirectory;
            string[] split = e_strDirectory.Split(new Char[] { '\\'});
            TreeViewItem l_CurrentNode = foldersItem.Items[0] as TreeViewItem;
            RoutedEventArgs l_RoutedEventArgs = new RoutedEventArgs();
            foreach(TreeViewItem l_TreeViewItem in foldersItem.Items)
            {
                if (l_TreeViewItem.Header.ToString().ToLower().CompareTo(split[0].ToLower()) == 0 )
                {
                    l_CurrentNode = l_TreeViewItem;
                    folder_Expanded(l_CurrentNode, l_RoutedEventArgs);
                    break;
                }
                
            }
            //int l_iIndex = 0;             
            foreach (string s in split)
            {
                foreach (TreeViewItem l_pData in l_CurrentNode.Items)
                {
                    if (l_pData != null && l_pData.Header.ToString().CompareTo(s) == 0)
                    {
                        folder_Expanded(l_pData, l_RoutedEventArgs);
                        l_CurrentNode = l_pData;
                        break;
                    }
                }
            }
            l_CurrentNode.IsExpanded = true;
            l_CurrentNode.BringIntoView();
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            foreach (string s in Directory.GetLogicalDrives())
            {
                TreeViewItem item = new TreeViewItem();
                int l_iIndex = s.LastIndexOf("\\");
                item.Header = s.Substring(0,l_iIndex);
                item.Tag = s;
                item.FontWeight = FontWeights.Normal;
                item.Items.Add(dummyNode);
                item.Expanded += new RoutedEventHandler(folder_Expanded);
                foldersItem.Items.Add(item);
            }
        }

        void AddNewDirectoryNode(TreeViewItem parentNode,string e_strNewNodeName,bool e_bAddWatch)
        {
            string l_strHeader = e_strNewNodeName.Substring(e_strNewNodeName.LastIndexOf("\\") + 1);
            if (parentNode.Items.Count == 1 && parentNode.Items[0] == dummyNode)
            {
                parentNode.Items.Clear();
            }
            TreeViewItem subitem = new TreeViewItem();
            subitem.Header = l_strHeader;
            subitem.Tag = e_strNewNodeName;
            subitem.FontWeight = FontWeights.Normal;
            subitem.Items.Add(dummyNode);
            subitem.Expanded += new RoutedEventHandler(folder_Expanded);
            parentNode.Items.Add(subitem);
            if (e_bAddWatch)
                SetFileWatcher(e_strNewNodeName);
        }

        void AddNewObjectNode(TreeViewItem parentNode, string e_strNewNodeName)
        {
            if ( IsExtensionNameMatched(e_strNewNodeName) != 1 )
            {
                return;
            }
            if (parentNode.Items.Count == 1 && parentNode.Items[0] == dummyNode)
            {
                parentNode.Items.Clear();
            }
            string l_strHeader = e_strNewNodeName.Substring(e_strNewNodeName.LastIndexOf("\\") + 1);
            TreeViewItem subitem = new TreeViewItem();
            subitem.Header = l_strHeader;
            subitem.Tag = e_strNewNodeName;
            subitem.FontWeight = FontWeights.Normal;
            parentNode.Items.Add(subitem);
        }

        void folder_Expanded(object sender, RoutedEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            if (item.Items.Count == 1 && item.Items[0] == dummyNode)
            {
                item.Items.Clear();
                try
                {
                    foreach (string s in Directory.GetDirectories(item.Tag.ToString()))
                    {
                        AddNewDirectoryNode(item, s,false);
                    }
                    foreach (TreeViewItem l_TreeViewItem in item.Items)
                    {
                        SetFileWatcher(l_TreeViewItem.Tag.ToString());
                    }
                    foreach (string s in Directory.GetFiles(item.Tag.ToString()))
                    {
                        AddNewObjectNode(item, s);
                    }
                }
                catch (Exception) { }
            }
            //else
            //if (item.Items.Count >= 1 && item.Items[0] != dummyNode)
            //{
            //    bool l_bNew = false;
            //    bool l_bDelete = false;
            //    //Array<int> l_DeleteIndexArray;
            //    foreach (string s in Directory.GetDirectories(item.Tag.ToString()))
            //    {
            //        l_bNew = false;
            //        l_bDelete = true;
            //        //string  l_strTargetName;
            //        foreach (TreeViewItem subitem in item.Items)
            //        { 
            //            if(!subitem.Header.Equals(s.Substring(s.LastIndexOf("\\") + 1)))
            //            {
            //                l_bNew = true;    
            //                //l_strTargetName = ;
            //                break;
            //            }
            //            else
            //            {
            //                l_bDelete = false;
            //            }
            //        }
            //        if( l_bNew )
            //        {
            //            //item.Items.Add    
            //        }
            //        else
            //        if( l_bDelete )
            //        {
            //        }
            //    }
            //    foreach (string s in Directory.GetFiles(item.Tag.ToString()))
            //    {

            //    }
            //}
        }

        private void foldersItem_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeView tree = (TreeView)sender;
            TreeViewItem temp = ((TreeViewItem)tree.SelectedItem);

            if (temp == null)
                return;
            SelectedImagePath = "";
            string temp1 = "";
            string temp2 = "";
            while (true)
            {
                temp1 = temp.Header.ToString();
                if (temp1.Contains(@"\"))
                {
                    temp2 = "";
                }
                SelectedImagePath = temp1 + temp2 + SelectedImagePath;
                if (temp.Parent.GetType().Equals(typeof(TreeView)))
                {
                    break;
                }
                temp = ((TreeViewItem)temp.Parent);
                temp2 = @"\";
            }
            //show user selected path
            this.m_SelectedFileNameTextBlock.Text = SelectedImagePath;
        }

        private void foldersItem_DragEnter(object sender, DragEventArgs e)
        {
            CoreWPF.Common.CommonFunction.DragEnter(sender, e);
        }

        private void foldersItem_Drop(object sender, DragEventArgs e)
        {
            if (f_Drop != null)
            {
                f_Drop(sender, e);
            }
        }

        private void foldersItem_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_MouseDoubleClick != null)
            {
                f_MouseDoubleClick(sender, e);
            }
        }

        private void foldersItem_KeyUp(object sender, KeyEventArgs e)
        {
            if (f_KeyUp != null)
            {
                f_KeyUp(sender, e);
            }
        }
    }
}
