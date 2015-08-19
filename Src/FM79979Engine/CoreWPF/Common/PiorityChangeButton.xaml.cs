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

namespace CoreWPF.Common
{
    /// <summary>
    /// Interaction logic for PiorityChangeButton.xaml
    /// </summary>
    public partial class PiorityChangeButton : UserControl
    {
        public event EventHandler f_UpButtonClick;
        public event EventHandler f_DownButtonClick;
        public PiorityChangeButton()
        {
            InitializeComponent();
        }

        private void UpButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_UpButtonClick != null)
                f_UpButtonClick(sender, e);
        }

        private void DownButton_Click(object sender, RoutedEventArgs e)
        {
            if (f_DownButtonClick != null)
                f_DownButtonClick(sender, e);
        }
    }
}
