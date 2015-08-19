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
    /// Interaction logic for UserData.xaml
    /// </summary>
    public partial class EventVariableManager : UserControl
    {
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        public event EventHandler f_Revert;

        public EventVariableManager()
        {
            InitializeComponent();
        }

        private void AddVariable_buttonClick(object sender, RoutedEventArgs e)
        {
            //String l_str = this.m_VariableName.Text;
            //if( l_str.Length >0 )
            //{
            //    m_EventVariable_listBox.Items.IndexOf
            //    if (m_EventVariable_listBox.Items.Contains(l_str))
            //    {
            //        if (System.Windows.Forms.DialogResult.Yes != System.Windows.Forms.MessageBox.Show(l_str +"exists","replace it!?", System.Windows.Forms.MessageBoxButtons.YesNo) )
            //            return;
            //    }
            //    if (m_EventVariable_listBox.Items.Contains(l_str))
            //    {
            //        int l_iIndex = m_EventVariable_listBox.Items.IndexOf(l_str);
            //        m_EventVariable_listBox.Items.RemoveAt(l_iIndex);
            //    }
            //    System.Windows.Controls.ListBoxItem l_pListBoxItem = new ListBoxItem();
            //    l_pListBoxItem.Content = l_str;
            //    m_EventVariable_listBox.Items.Add(l_pListBoxItem);
            if (f_Add != null)
                f_Add(sender, e);
        }

        private void Del(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)
                f_Del(sender, e);
        }

        private void Add(object sender, RoutedEventArgs e)
        {
            if (f_Add != null)
                f_Add(sender, e);
        }

        private void Revert(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
                f_Revert(sender, e);
        }

        private void EventVariable_listBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_Revert != null)
                f_Revert(sender, e);
        }
    }
}
