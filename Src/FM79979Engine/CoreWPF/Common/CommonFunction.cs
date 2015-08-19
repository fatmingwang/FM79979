using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Navigation;
using System.Windows.Media;
using System.Windows.Markup;
using System.Xml;
namespace CoreWPF.Common
{
    public static class CommonFunction
    {
        public static string[]		DragDropEventWhileFileDrop(System.Windows.DragEventArgs  e)
        {
            string[]    l_strFileNames = (string[])e.Data.GetData(DataFormats.FileDrop, true);;
            return l_strFileNames;
        }

        public static void  EventDelegateExecute(object sender, EventArgs e,EventHandler e_EventHandler)
        {
            if( e_EventHandler != null )
            {
                e_EventHandler(sender,e);
            }
        }
        public static T Clone<T>(T from)
        {
            //// Create the Button.
            //Button origianlButton = new Button();
            //origianlButton.Height = 50;
            //origianlButton.Width = 100;
            //origianlButton.Background = Brushes.AliceBlue;
            //origianlButton.Content = "Click Me";

            //// Save the Button to a string.
            //string savedButton = XamlWriter.Save(origianlButton);

            //// Load the button
            //StringReader stringReader = new StringReader(savedButton);
            //XmlReader xmlReader = XmlReader.Create(stringReader);
            //Button readerLoadButton = (Button)XamlReader.Load(xmlReader);

            //string gridXaml = System.Windows.Markup.XamlWriter.Save(l_FMTextBox.m_TextBox);
            //System.IO.StringReader stringReader = new System.IO.StringReader(gridXaml);
            //System.Xml.XmlReader xmlReader = System.Xml.XmlReader.Create(stringReader);
            //TextBox l_TextBox = (TextBox)System.Windows.Markup.XamlReader.Load(xmlReader);

            string objStr = XamlWriter.Save(from);
            StringReader stringReader = new StringReader(objStr);
            XmlReader xmlReader = XmlReader.Create(stringReader);
            object clone = XamlReader.Load(xmlReader);
            return (T)clone;
        }
        //private TreeViewItem CreateTreeViewItem(string header,string iconFolder,string iconName)
        public static TreeViewItem CreateTreeViewItem(string header,string iconFolder,string iconName)
        {
            TreeViewItem child = new TreeViewItem();
            StackPanel pan = new StackPanel();
            if (iconName != null)
            {
                pan.Orientation = Orientation.Horizontal;
                System.Windows.Media.Imaging.IconBitmapDecoder icon = new System.Windows.Media.Imaging.IconBitmapDecoder(new Uri(System.IO.Path.Combine(iconFolder, iconName), UriKind.RelativeOrAbsolute), System.Windows.Media.Imaging.BitmapCreateOptions.None, System.Windows.Media.Imaging.BitmapCacheOption.OnLoad);
                //IconBitmapDecoder icon = new IconBitmapDecoder(m_VisibleImage.StreamSource, BitmapCreateOptions.None, BitmapCacheOption.OnLoad);
                Image image = new Image();
                image.Height = 16;
                image.Source = icon.Frames[0];
                pan.Children.Add(image);
            }
            pan.Children.Add(new TextBlock(new Run("  " + header)));
            child.Header = pan;
            return child;
        }

        static public void DragEnter(object sender, DragEventArgs e)
        {
            bool isCorrect = true;
            if (e.Data.GetDataPresent(DataFormats.FileDrop, true) == true)
            {
                string[] filenames = (string[])e.Data.GetData(DataFormats.FileDrop, true);
                foreach (string filename in filenames)
                {
                    if (File.Exists(filename) == false)
                    {
                        isCorrect = false;
                        break;
                    }
                    //FileInfo info = new FileInfo(filename);
                    //if (info.Extension != ".avi")
                    //{
                    //    isCorrect = false;
                    //    break;
                    //}
                }
            }
            if (isCorrect == true)
                e.Effects = DragDropEffects.All;
            else
                e.Effects = DragDropEffects.None;
            e.Handled = true;
        }

        static public void JumpToNode(TreeViewItem tvi, string NodeName)
        {
            if (tvi.Name == NodeName)
            {
                tvi.IsExpanded = true;
                tvi.BringIntoView();
                return;
            }
            else
                tvi.IsExpanded = false;

            if (tvi.HasItems)
            {
                foreach (var item in tvi.Items)
                {
                    TreeViewItem temp = item as TreeViewItem;
                    JumpToNode(temp, NodeName);
                }
            }
        }


        public static void GetTreeViewVisibleChrildren(System.Collections.ArrayList e_Collection, System.Windows.Controls.ItemCollection e_Control)
        {
            foreach (TreeViewItem l_Item in e_Control)
            {
                if(l_Item.IsVisible)
                {
                    e_Collection.Add(l_Item);
                    if(l_Item.IsExpanded)
                    {
                        GetTreeViewVisibleChrildren(e_Collection,l_Item.Items);
                    }
                }
            }
        }

        public static void RemoveChild(this DependencyObject parent, UIElement child)
        {
            var panel = parent as Panel;
            if (panel != null)
            {
                panel.Children.Remove(child);
                return;
            }

            var decorator = parent as Decorator;
            if (decorator != null)
            {
                if (decorator.Child == child)
                {
                    decorator.Child = null;
                }
                return;
            }

            var contentPresenter = parent as ContentPresenter;
            if (contentPresenter != null)
            {
                if (contentPresenter.Content == child)
                {
                    contentPresenter.Content = null;
                }
                return;
            }

            var contentControl = parent as ContentControl;
            if (contentControl != null)
            {
                if (contentControl.Content == child)
                {
                    contentControl.Content = null;
                }
                return;
            }

            TreeView l_ContentTreeView = parent as TreeView;
            if (l_ContentTreeView != null)
            {
                l_ContentTreeView.Items.Remove(child);
                return;
            }

            TreeViewItem l_ContentTreeViewItem = parent as TreeViewItem;
            if (l_ContentTreeViewItem != null)
            {
                l_ContentTreeViewItem.Items.Remove(child);
                return;
            }
            // maybe more
        }
        //ensure listboxitem is common type,0 for not contain,1 for contain,-1 for not support listboxitem type
        public static int IsListboxContainString(ListBox e_Listbox, String e_str)
        {
            for (int i = 0; i < e_Listbox.Items.Count; ++i)
            {
                ListBoxItem l_ListBoxItem = e_Listbox.Items[i] as ListBoxItem;
                if (l_ListBoxItem != null)
                {
                    if (l_ListBoxItem.Content.ToString().CompareTo(e_str) == 0)
                        return i;
                }
                else
                {
                    TextBox l_TextBox = e_Listbox.Items[i] as TextBox;
                    if (l_TextBox != null)
                    {
                        if (l_TextBox.Text.CompareTo(e_str) == 0)
                            return i;
                    }
                    else
                    {
                        String l_str = e_Listbox.Items[i] as String;
                        if (l_str != null)
                        {
                            if (l_str.CompareTo(e_str) == 0)
                                return i;
                        }
                        else
                            return -1;
                    }
                }
                //l_ListBoxItem.hea
            }
            return -1;
        }

        public static void ListboxEnableDrag(ListBox e_Listbox)
        {
            Style l_Style = new Style(typeof(ListBoxItem));
            l_Style.Setters.Add(new EventSetter(ListBoxItem.PreviewMouseLeftButtonDownEvent, new MouseButtonEventHandler(ListBoxMouseLeftButtonDown)));
            e_Listbox.ItemContainerStyle = l_Style;        
        }

        private static void ListBoxMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            String l_strSenderString = sender.ToString();
            int l_iIndex = l_strSenderString.IndexOf(':');
            if (l_iIndex == -1)
                return;
            ListBoxItem l_Sender = sender as ListBoxItem;
            if (l_Sender == null)
                return;
            
            l_iIndex += 2;
            int l_iLength = l_strSenderString.Length - l_iIndex;
            String l_strCompareString = l_strSenderString.Substring(l_iIndex, l_iLength);
            ListBox l_Parent = FindAncestor<ListBox>(l_Sender);
            if( l_Parent == null )
                return;
            int l_iSelectedIndex = IsListboxContainString(l_Parent,l_strCompareString);
            if (l_iSelectedIndex == -1)
            {
                return;
            }
            String l_strText;
            TextBox l_TextBox = l_Parent.Items[l_iSelectedIndex] as TextBox;
            if (l_TextBox != null)
            {
                l_strText = l_TextBox.Text;
            }
            else
            {
                l_strText = l_Parent.Items[l_iSelectedIndex] as string;
            }
            if (l_strText != null)
            {
                var l_DataObject = new DataObject(l_strText);
                l_DataObject.SetData("String", l_strText);
                DragDrop.DoDragDrop((System.Windows.DependencyObject)sender, l_DataObject, DragDropEffects.Copy);
            }
        }

        public static T FindAncestor<T>(DependencyObject from)
          where T : class
        {
            if (from == null)
            {
                return null;
            }

            T candidate = from as T;
            if (candidate != null)
            {
                return candidate;
            }

            return FindAncestor<T>(VisualTreeHelper.GetParent(from));
        }
    }
}
