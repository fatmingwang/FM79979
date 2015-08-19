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
    /// Interaction logic for EventObjectStatusList.xaml
    /// </summary>
    public partial class EventObjectStatusList : UserControl
    {
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        public event EventHandler f_Revert;
        public event EventHandler f_SelectionChanged;
        public EventObjectStatusList()
        {
            InitializeComponent();
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
        private void Revert(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }
        private void listBox_MouseDoubleClick(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }

        private void m_EventObjectStatusList_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_SelectionChanged != null)
            {
                f_SelectionChanged(sender, e);
            }
        }
    }
}
