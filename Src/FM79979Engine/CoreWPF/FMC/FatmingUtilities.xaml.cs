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
    /// Interaction logic for FatmingUtilities.xaml
    /// </summary>
    public partial class FatmingUtilities : UserControl
    {
        public event EventHandler f_TabControlSelectionChanged;
        public FatmingUtilities()
        {
            InitializeComponent();
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (f_TabControlSelectionChanged != null)
            {
                f_TabControlSelectionChanged(sender, e);
            }
        }
    }
}
