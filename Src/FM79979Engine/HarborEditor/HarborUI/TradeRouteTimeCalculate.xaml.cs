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

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for TradeRouteCalcilateUI.xaml
    /// </summary>
    public partial class TradeRouteTimeCalculateUI : UserControl
    {
        public EventHandler f_AllRoute;
        public EventHandler f_Output;
        public EventHandler f_Excel;
        public EventHandler f_Apply;
        public EventHandler f_FileSave;
        public EventHandler f_FileOpen;
        public EventHandler f_ApplyEquation;
        public EventHandler f_ExcelToXML;
        //
        public EventHandler f_PointDropEvent;
        public EventHandler f_MapNameListbox_SelectionChanged;
        System.Collections.Hashtable m_FunctionAndObject = new System.Collections.Hashtable();
        public TradeRouteTimeCalculateUI()
        {
            InitializeComponent();

            Style l_Style = new Style(typeof(ListBoxItem));
            l_Style.Setters.Add(new EventSetter(ListBoxItem.PreviewMouseLeftButtonDownEvent, new MouseButtonEventHandler(MouseLeftButtonDown)));
            m_PortNameListbox.ItemContainerStyle = l_Style;
        }

        public int IndexOf(string e_strName)
        {
            int l_iIndex = 0;
            foreach (string l_TextBox in this.m_PortNameListbox.Items)
            {
                if (l_TextBox.CompareTo(e_strName) == 0)
                    return l_iIndex;
                ++l_iIndex;
            }
            return -1;
        }

        private void MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            String l_strSenderString = sender.ToString();
            int l_iIndex = l_strSenderString.IndexOf(':');
            if (l_iIndex == -1)
                return;
            l_iIndex += 2;
            int l_iLength = l_strSenderString.Length - l_iIndex;
            String l_strCompareString = l_strSenderString.Substring(l_iIndex, l_iLength);
            int l_iSelectedIndex = IndexOf(l_strCompareString);
            if (l_iSelectedIndex == -1)
            {
                return;
            }

            String l_strText = m_PortNameListbox.Items[l_iSelectedIndex] as String;
            if (l_strText != null)
            {
                var l_DataObject = new DataObject(l_strText);
                l_DataObject.SetData("String", l_strText);
                DragDrop.DoDragDrop((System.Windows.DependencyObject)sender, l_DataObject, DragDropEffects.Copy);
            }
        }
        public void SetFunction()
        {
            m_FunctionAndObject[m_FileOpen] = f_FileOpen;
            m_FunctionAndObject[m_FileSave] = f_FileSave;
            m_FunctionAndObject[m_Excel] = f_Excel;
            m_FunctionAndObject[m_AllRoute] = f_AllRoute;
            m_FunctionAndObject[m_Output] = f_Output;
            m_FunctionAndObject[m_MapNameListbox] = f_MapNameListbox_SelectionChanged;
            m_FunctionAndObject[m_ApplyEquation] = f_ApplyEquation;
            m_FunctionAndObject[m_ExcelToXML] = f_ExcelToXML;
            
        }

        private void m_FileOpen_Click(object sender, RoutedEventArgs e)
        {
            if (m_FunctionAndObject.ContainsKey(sender))
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, (EventHandler)m_FunctionAndObject[sender]);
            }
        }

        private void m_StartPortNameTextbox_PreviewDragOver(object sender, DragEventArgs e)
        {
            e.Handled = true;
            String l_Data = (String)e.Data.GetData("String");
            if (l_Data != null)
            {
                e.Effects = DragDropEffects.Copy;
            }
            else
                e.Effects = DragDropEffects.None;
        }

        private void m_StartPortNameTextbox_PreviewDrop(object sender, DragEventArgs e)
        {
            e.Handled = true;
            String l_Data = (String)e.Data.GetData("String");
            if (l_Data != null)
            {
                TextBox l_Textbox = sender as TextBox;
                l_Textbox.Text = l_Data;
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_PointDropEvent);
            }
        }

        private void MapNameListbox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (m_FunctionAndObject.ContainsKey(sender))
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, (EventHandler)m_FunctionAndObject[sender]);
            }
        }
    }
}
