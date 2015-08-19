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
    /// Interaction logic for PageObject.xaml
    /// </summary>
    public partial class EventInstanceList : UserControl
    {
        public event EventHandler f_Add;
        //instead setup this.m_EventList_listBox.f_DeleteSelectedItem
        //public event EventHandler f_Del;

        public event EventHandler f_Revert;
        public EventInstanceList()
        {
            InitializeComponent();
        }
        private void AddEvent_Click(object sender, RoutedEventArgs e)
        {
            if (f_Add != null)
            {
                f_Add(sender, e);
            }
        }

        private void DelEvent_Click(object sender, RoutedEventArgs e)
        {
            this.m_EventList_listBox.DeleteSelectedItems();
        }

        private void RevertEvent_Click(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }

        private void m_Event_listBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }
    }
}
