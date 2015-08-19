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

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for MapPointHarbor.xaml
    /// </summary>
    public partial class MapPointHarbor : UserControl
    {
        public EventHandler f_Apply;
        public MapPointHarbor()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_Apply);
        }
        public void UIClear()
        {
            m_CommodityTextbox.Text = "";
        }
    }
}
