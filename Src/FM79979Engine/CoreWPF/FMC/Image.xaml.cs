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
    /// Interaction logic for Image.xaml
    /// </summary>
    public partial class Image : UserControl
    {
        public event EventHandler f_Add;
        public event EventHandler f_Del;
        public event EventHandler f_SelectedIndexChanged;
        public Image()
        {
            InitializeComponent();
        }

        private void AddImage_Click(object sender, RoutedEventArgs e)
        {
            if (f_Add != null)
            {
                f_Add(sender, e);
            }
        }

        private void DelImage_Click(object sender, RoutedEventArgs e)
        {
            if (f_Del != null)
            {
                f_Del(sender, e);
            }
        }

        private void Image_ListboxChangeSelection(object sender, SelectionChangedEventArgs e)
        {
            if (f_SelectedIndexChanged != null)
            {
                f_SelectedIndexChanged(sender, e);
            }
        }
    }
}
