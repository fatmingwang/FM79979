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

namespace CoreWPF.OpenGL
{
    /// <summary>
    /// Interaction logic for ColorBlending.xaml
    /// </summary>
    public partial class ColorBlending : UserControl
    {
        public event EventHandler f_EnableButtonClick;
        public ColorBlending()
        {
            InitializeComponent();
        }

        private void EnableCheckbox_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_EnableButtonClick);
        }
    }
}
