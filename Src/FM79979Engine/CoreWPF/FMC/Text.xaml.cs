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

namespace CoreWPF.FMC
{
    /// <summary>
    /// Interaction logic for Text.xaml
    /// </summary>
    public partial class Text : UserControl
    {
        public event EventHandler f_TextChanged;
        public event EventHandler f_AddTextFile;
        public event EventHandler f_DelTextFile;
        public Text()
        {
            InitializeComponent();
        }

        private void Text_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_TextChanged != null)
            {
                f_TextChanged(sender, args);
            }
        }

        private void AddTextFile_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddTextFile != null)
            {
                f_AddTextFile(sender, e);
            }
        }

        private void DelTextFile_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelTextFile != null)
            {
                f_DelTextFile(sender, e);
            }
        }
    }
}
