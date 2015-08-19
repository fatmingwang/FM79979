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

namespace FMBookEditorUI
{
    /// <summary>
    /// Interaction logic for EventObjectInstance.xaml
    /// </summary>
    public partial class EventObjectInstanceList : UserControl
    {
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        //public event EventHandler f_Revert;

        public EventObjectInstanceList()
        {
            InitializeComponent();
            //m_EventObjectInstance_listBox.m_ListBox.MouseDoubleClick += new MouseButtonEventHandler(EventObjectInstance_listBox_MouseDoubleClick);
            //m_EventObjectInstance_listBox.MouseDoubleClick += new MouseButtonEventHandler(EventObjectInstance_listBox_MouseDoubleClick);
        }

        private void Add(object sender, RoutedEventArgs e)
        {
            if (f_Add != null)
            {
                f_Add(sender, e);
            }
        }
        private void Del(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)
            {
                f_Del(sender, e);
            }
        }

        private void m_EventObjectInstance_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ListBox l_ListBox = sender as ListBox;
            if (l_ListBox != null && l_ListBox.SelectedItem != null)
            {
                EventObjectInstanceContentForListBoxUI l_EventObjectInstanceContentForListBoxUI = l_ListBox.SelectedItem as EventObjectInstanceContentForListBoxUI;
                if (l_EventObjectInstanceContentForListBoxUI != null)
                {
                    m_EventObjectInstanceName_textBox.Text = l_EventObjectInstanceContentForListBoxUI.m_EventObjecInstanceName.Text;
                    m_EventInstance_textBox.Text = l_EventObjectInstanceContentForListBoxUI.m_EventInstance.Text;
                    m_EventStatus_textBox.Text = l_EventObjectInstanceContentForListBoxUI.m_EventObjecStatus.Text;
                    m_StatusEventVariable.Text = l_EventObjectInstanceContentForListBoxUI.m_StatusEventVariable.Text;
                }
            }
        }
        //private void Revert(object sender, RoutedEventArgs e)
        //{
        //    if (f_Revert != null)
        //    {
        //        f_Revert(sender, e);
        //    }
        //}

        //private void EventObjectInstance_listBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        //{
        //    if (f_Revert != null)
        //    {
        //        f_Revert(sender, e);
        //    }
        //}
    }
}
