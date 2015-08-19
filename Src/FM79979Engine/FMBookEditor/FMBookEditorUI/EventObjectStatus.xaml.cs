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
    /// Interaction logic for ObjectStatus.xaml
    /// </summary>
    public partial class EventObjectStatus : UserControl
    {

        public event EventHandler f_AddStatus;
        public event EventHandler f_ApplyTestButton_Click;
        public event EventHandler f_Revert;
        public event EventHandler f_Del;
        public event EventHandler f_TabPageControlSelectChange;

        public EventObjectStatus()
        {
            InitializeComponent();
        }

        private void Status_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void ApplyTestButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_ApplyTestButton_Click != null)
            {
                f_ApplyTestButton_Click(sender, e);
            }
        }

        private void RevertButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }        

        private void AddStatusButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddStatus != null)
            {
                f_AddStatus(sender, e);
            }
        }

        private void m_Status_listBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }

        private void DelStatusButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)
            {
                f_Del(sender, e);
            }
        }

        private void m_TabPageControl_SelectionChanged_1(object sender, SelectionChangedEventArgs e)
        {
            if (f_TabPageControlSelectChange != null)
            {
                f_TabPageControlSelectChange(sender, e);
            }
        }

    }
}
