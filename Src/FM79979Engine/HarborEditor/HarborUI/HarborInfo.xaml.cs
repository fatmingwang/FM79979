using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for HarborInfo.xaml
    /// </summary>
    public partial class HarborInfo : UserControl
    {
        public EventHandler f_BGImageButton;
        public EventHandler f_AddButton;
        public EventHandler f_MapPointTypeChange;
        //public EventHandler f_DelButton;
        //
        public bool        m_bDisableMapPointTypeChange = false;
        public HarborInfo()
        {
            InitializeComponent();
            //http://stackoverflow.com/questions/3350187/wpf-c-rearrange-items-in-listbox-via-drag-and-drop
            m_HarborListbox.EnableListboxDrag();
            m_HarborListbox.m_ListBox.SelectionMode = SelectionMode.Extended;
            //Style l_Style = new Style(typeof(ListBoxItem));
            //l_Style.Setters.Add(new EventSetter(ListBoxItem.PreviewMouseLeftButtonDownEvent,new MouseButtonEventHandler(m_HarborListbox_MouseLeftButtonDown)));
            //m_HarborListbox.m_ListBox.ItemContainerStyle = l_Style;
        }

        private void BGImageButton_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_BGImageButton);
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_AddButton);
        }

        private void DelButton_Click(object sender, RoutedEventArgs e)
        {
            this.m_HarborListbox.DeleteSelectedItems();
        }

        //private void m_HarborListbox_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        //{
        //    String l_strSenderString = sender.ToString();
        //    int l_iIndex = l_strSenderString.IndexOf(':');
        //    if (l_iIndex == -1)
        //        return;
        //    l_iIndex += 2;
        //    int l_iLength = l_strSenderString.Length-l_iIndex;
        //    String  l_strCompareString = l_strSenderString.Substring(l_iIndex,l_iLength);
        //    int l_iSelectedIndex = m_HarborListbox.IndexOf(l_strCompareString);
        //    if (l_iSelectedIndex == -1)
        //    {
        //        return;
        //    }
            
        //    TextBox l_TextBox = m_HarborListbox.m_ListBox.Items[l_iSelectedIndex] as TextBox;
        //    String l_strText = l_TextBox.Text;
        //    if( l_strText != null )
        //    {
        //        var l_DataObject = new DataObject(l_strText);
        //        l_DataObject.SetData("String", l_strText);
        //        DragDrop.DoDragDrop((System.Windows.DependencyObject)sender, l_DataObject, DragDropEffects.Copy);
        //    }
        //}

        private void MapPointTypeCombobox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if( m_bDisableMapPointTypeChange == false )
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_MapPointTypeChange);
            m_bDisableMapPointTypeChange = false;
        }
        public void UIClear()
        {
            m_MapPointHarbor.UIClear();
            m_MapPointRoutePoint.UIClear();
        }
    }
}
