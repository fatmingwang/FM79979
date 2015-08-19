using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UITest_WpfApplication
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            //CommonWPFLib.InputNameDialog l_pInputNameDialog = new CommonWPFLib.InputNameDialog();
            //if (l_pInputNameDialog.ShowDialog() == true)
            //{
            //    string l_str = l_pInputNameDialog.ResponseText;
            //}
            //else
            //{
            //    string l_str = l_pInputNameDialog.ResponseText;
            //}
            TreweViewTimeline.TreeViewWithTimeline l_TreweViewTimeline = new TreweViewTimeline.TreeViewWithTimeline();
            m_MainGrid.Children.Add(l_TreweViewTimeline);
        }
    }
}
