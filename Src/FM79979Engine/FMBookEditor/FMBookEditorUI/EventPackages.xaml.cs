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
    /// Interaction logic for PagesData.xaml
    /// </summary>
    public partial class EventPackages : UserControl
    {
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        public event EventHandler f_Revert;

        public EventPackages()
        {
            InitializeComponent();
        }

        private void AddPackage_Click(object sender, RoutedEventArgs e)
        {
            if (f_Add != null)
            {
                f_Add(sender, e);
            }
        }

        private void DelPackage_Click(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)
            {
                f_Del(sender, e);
            }
        }

        private void RevertPackage_Click(object sender, RoutedEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }

        private void Packages_listBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (m_Packages_listBox.SelectedItem != null)
                m_PackageName_textBox.Text = m_Packages_listBox.SelectedItem.ToString();
        }

        private void Packages_listBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (f_Revert != null)
            {
                f_Revert(sender, e);
            }
        }
    }
}
