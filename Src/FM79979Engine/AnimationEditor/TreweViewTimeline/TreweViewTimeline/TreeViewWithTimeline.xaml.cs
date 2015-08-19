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
using System.Windows.Markup;
using System.Xml;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
namespace TreweViewTimeline
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class TreeViewWithTimeline : UserControl
    {
        //
        public TreeViewWithTimeline()
        {
            InitializeComponent();
            m_VisibleImage = new BitmapImage(new Uri("pack://application:,,,/TreweViewTimeline;component/Images/Eye.png"));
            m_InactiveImage = new BitmapImage(new Uri("pack://application:,,,/TreweViewTimeline;component/Images/Inactive.png"));
            MenuItem item1 = new MenuItem();
            MenuItem item2 = new MenuItem();
            MenuItem item3 = new MenuItem();
            MenuItem item4 = new MenuItem();

            item1.Header = "New"; item2.Header = "Copy"; item3.Header = "Cut"; item4.Header = "Paste";
            item1.Click += new RoutedEventHandler(NewObject); item2.Click += new RoutedEventHandler(CopyObject); item3.Click += new RoutedEventHandler(CutObject); item4.Click += new RoutedEventHandler(PasteObject);
            m_ContextMenu.Items.Add(item1); m_ContextMenu.Items.Add(item2); m_ContextMenu.Items.Add(item3); m_ContextMenu.Items.Add(item4);
            m_ObjectTreeView.ContextMenu = m_ContextMenu;
        }
        //for c++.
        public event EventHandler f_TreeViewItemVisibleClick;
        //
        ContextMenu m_ContextMenu = new ContextMenu();
        TreeViewItem m_CopyTreeViewItem = null;
        bool m_bCopyOtCut = false;//true for copy false for cut
        BitmapImage m_VisibleImage = null;
        BitmapImage m_InactiveImage = null;
        System.Collections.ArrayList m_AllVisibleTreeViewNode = new System.Collections.ArrayList();
        //the top visible
        TreeViewItem m_TopViewItem = null;
        TreeViewItem m_LastChangeTopViewItem = null;
        bool m_bForceReDraw = false;
        //
        public void DrawTimeLine()
        {
            if (m_TopViewItem != null)
            {
                if (m_LastChangeTopViewItem == null)
                {
                    m_LastChangeTopViewItem = m_TopViewItem;
                }
                else
                if (m_TopViewItem == m_LastChangeTopViewItem)
                {
                    if (m_bForceReDraw == false)
                        return;
                }
                DrawTimeLine();
            }
        }

        private TreeViewItem FindTreeViewSelectedItemContainer(ItemsControl root, object selection)
        {
            var item = root.ItemContainerGenerator.ContainerFromItem(selection) as TreeViewItem;
            if (item == null)
            {
                foreach (var subItem in root.Items)
                {
                    //ItemContainerGenerator only returns items that have been generated Visually.
                    //If the item is not in view (and I mean visible on the screen), 
                    //it might return a null since the item could be getting recycled.
                    item = FindTreeViewSelectedItemContainer((TreeViewItem)root.ItemContainerGenerator.ContainerFromItem(subItem), selection);
                    if (item != null)
                    {
                        break;
                    }
                }
            }

            return item;
        }

        private void FindTreeViewSelectedItemContainer(TreeViewItem e_TreeViewItem,int e_iTopY,int e_iBottomY)
        {
            if ( e_TreeViewItem != null )
            {
                Point myOffset = e_TreeViewItem.RenderTransform.Transform(new Point(0, 0));
                if( myOffset.Y >=e_iTopY && myOffset.Y <= e_iBottomY )
                {
                    m_AllVisibleTreeViewNode.Add(e_TreeViewItem);
                    if (  e_TreeViewItem.IsExpanded )
                    {
                        foreach (TreeViewItem l_subItem in e_TreeViewItem.Items)
                        {

                        }
                    }
                }
            }
        }

        void CollectAllTreeviewItemInTreeView()
        {
            m_AllVisibleTreeViewNode.Clear();
            foreach (TreeViewItem l_subItem in this.m_ObjectTreeView.Items)
            {
                FindTreeViewSelectedItemContainer(l_subItem, 0, 850);
            }
        }

        void image_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Image image = (Image)sender;
            if (image.Source == m_VisibleImage)
            {
                image.Source = m_InactiveImage;
            }
            else
            {
                image.Source = m_VisibleImage;
            }
            if (f_TreeViewItemVisibleClick != null)
            {
                f_TreeViewItemVisibleClick(sender, e);
            }
        }

        private void NewObject(object sender, RoutedEventArgs e)
        {//http://www.codeproject.com/Articles/124644/Basic-Understanding-of-Tree-View-in-WPF
            CommonWPFLib.InputNameDialog l_pInputNameDialog = new CommonWPFLib.InputNameDialog();
            if (l_pInputNameDialog.ShowDialog() == true)
            {
                if (l_pInputNameDialog.ResponseText.Length > 0)
                {
                    TreeViewItem l_ObjectTreeViewItem = new TreeViewItem();
                    TimeObjectData l_TimeObjectData = new TimeObjectData();
                    l_TimeObjectData.SetName(l_pInputNameDialog.ResponseText);
                    l_TimeObjectData.SetContextMenu(m_ContextMenu);

                    l_ObjectTreeViewItem.Header = l_TimeObjectData;
                    if (m_ObjectTreeView.SelectedItem == null)
                    {
                        m_ObjectTreeView.Items.Add(l_ObjectTreeViewItem);
                    }
                    else 
                    {
                        TreeViewItem l_SelectedTrreeViewItem = (TreeViewItem)m_ObjectTreeView.SelectedItem;
                        l_SelectedTrreeViewItem.Items.Add(l_ObjectTreeViewItem);
                        l_SelectedTrreeViewItem.IsExpanded = true;
                    }
                    this.m_ObjectTreeView.SelectedItemChanged += m_ObjectTreeView_SelectedItemChanged;
                    m_ObjectTreeView.MouseDoubleClick += m_ObjectTreeView_MouseDoubleClick;
                }
            }
            
        }

        void m_ObjectTreeView_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (m_ObjectTreeView.SelectedItem != null)
            {
                TreeViewItem tvi = m_ObjectTreeView.SelectedItem as TreeViewItem;
                tvi.IsSelected = false;
            }
        }

        void m_ObjectTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            TreeViewItem tvi = e.NewValue as TreeViewItem;
            if (tvi != null)
            {
                GeneralTransform myTransform = tvi.TransformToAncestor(m_ObjectTreeView);
                Point myOffset = myTransform.Transform(new Point(0, 0));
                m_Label.Content = myOffset.ToString();
            }
        }

        private void CopyObject(object sender, RoutedEventArgs e)
        {
            if (m_ObjectTreeView.SelectedItem != null)
            {
                m_CopyTreeViewItem = (TreeViewItem)m_ObjectTreeView.SelectedItem;
                m_bCopyOtCut = true;
            }
        }
        private void CutObject(object sender, RoutedEventArgs e)
        {
            if (m_ObjectTreeView.SelectedItem != null)
            {
                m_CopyTreeViewItem = (TreeViewItem)m_ObjectTreeView.SelectedItem;
                m_bCopyOtCut = false;
            }
        }
        //fuck here we have to clone all child and set parent as well
        //or if user control has set name and if its recursive it will occur crash because dupulicate name.
        private Object CloneUsingXaml(Object o)
        {//https://www.google.com.tw/search?q=Cannot+register+duplicate+name+'X'+in+this+scope&aq=f&oq=Cannot+register+duplicate+name+'X'+in+this+scope&aqs=chrome.0.57j62.241&sourceid=chrome&ie=UTF-8
            string xaml = System.Xaml.XamlServices.Save(o);
            return System.Xaml.XamlServices.Parse(xaml);
            //string xaml = XamlWriter.Save(o);
            return XamlReader.Load(new XmlTextReader(new StringReader(xaml)));
        }


        private Object CloneUsingSerialize(Object o)
        {//https://www.google.com.tw/search?q=Cannot+register+duplicate+name+'X'+in+this+scope&aq=f&oq=Cannot+register+duplicate+name+'X'+in+this+scope&aqs=chrome.0.57j62.241&sourceid=chrome&ie=UTF-8
            string  l_str = SerializeObject(o);
            if (l_str != null)
            {
                return DeserializeObject(l_str);
            }
            return null;
        }

        public static string SerializeObject(object o)
        {
            string l_strYypr = o.GetType().ToString();
            if (!o.GetType().IsSerializable)
            {
                return null;
            }

            using (MemoryStream stream = new MemoryStream())
            {
                new BinaryFormatter().Serialize(stream, o);
                return Convert.ToBase64String(stream.ToArray());
            }
        }

        public static object DeserializeObject(string str)
        {
            byte[] bytes = Convert.FromBase64String(str);

            using (MemoryStream stream = new MemoryStream(bytes))
            {
                return new BinaryFormatter().Deserialize(stream);
            }
        }

        public T Clone<T>(T from)
        {
            T clone = from;
            try
            {
                string objStr = XamlWriter.Save(from);
                StringReader stringReader = new StringReader(objStr);
                XmlReader xmlReader = XmlReader.Create(stringReader);
                clone = (T)XamlReader.Load(xmlReader);
            }
            catch (Exception e)
            {
                string l_str = e.ToString();
                int a = 0;
            }
            return clone;
        }

        private void PasteObject(object sender, RoutedEventArgs e)
        {//http://msdn.microsoft.com/zh-tw/library/ms746659.aspx u have setup all control's name
            if (m_CopyTreeViewItem != null)
            {
                TreeViewItem l_CopyTreeViewItem;
                TreeViewItem l_TargetTreeViewItem = (TreeViewItem)m_ObjectTreeView.SelectedItem;
                if( m_ObjectTreeView.SelectedItem != null )
                {
                    if (m_bCopyOtCut == false)
                    {//cut
                        l_CopyTreeViewItem = m_CopyTreeViewItem;
                    }
                    else
                    {
                        l_CopyTreeViewItem = Clone<TreeViewItem>(m_CopyTreeViewItem);
                        //l_CopyTreeViewItem = (TreeViewItem)CloneUsingXaml((object)m_CopyTreeViewItem);
                        //l_CopyTreeViewItem = DeepClone<TreeViewItem>(m_CopyTreeViewItem);
                        //http://stackoverflow.com/questions/12571008/how-do-i-copy-a-treeviewitem-onto-another-that-is-using-an-object-as-a-tag-and-h
                        //l_CopyTreeViewItem = (TreeViewItem)CloneUsingSerialize(m_CopyTreeViewItem);
                    }
                    if (l_CopyTreeViewItem != null)
                        l_TargetTreeViewItem.Items.Add(l_CopyTreeViewItem);
                }
                else
                {
                
                }
            }
        }
        private void RemoveTreeViewItem(TreeViewItem e_TreeViewItem)
        {
            TreeViewItem l_ObjectTreeViewItem = e_TreeViewItem;
            if (l_ObjectTreeViewItem != null)
            {
                if (l_ObjectTreeViewItem.Parent != null)
                {
                    TreeViewItem parent = (l_ObjectTreeViewItem as TreeViewItem).Parent as TreeViewItem;
                    if (parent != null)
                    {
                        parent.Items.Remove(l_ObjectTreeViewItem);
                        return;
                    }
                }
                this.m_ObjectTreeView.Items.Remove(l_ObjectTreeViewItem);
            }
        }
        private void ObjectTreeView_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Delete)
            {
                //TreeViewItem l_ObjectTreeViewItem = (TreeViewItem)this.m_ObjectTreeView.SelectedItem;
                RemoveTreeViewItem((TreeViewItem)this.m_ObjectTreeView.SelectedItem);                
            }
        }

        private void m_TimeLineCanvas_MouseDown(object sender, MouseButtonEventArgs e)
        {

        }

        private void m_TimeLineCanvas_MouseMove(object sender, MouseEventArgs e)
        {

        }

        private void m_TimeLineCanvas_MouseUp(object sender, MouseButtonEventArgs e)
        {

        }
    }
}
