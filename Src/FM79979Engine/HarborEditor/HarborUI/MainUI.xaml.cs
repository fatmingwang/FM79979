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

//using ExcelLibrary.CompoundDocumentFormat;
//using ExcelLibrary.SpreadSheet;

//
//using Excel = Microsoft.Office.Interop.Excel;
using System.Data;

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for MainUI.xaml
    /// </summary>
    public partial class MainUI : UserControl
    {
        public EventHandler f_KeyDown;
        public EventHandler f_KeyUp;
        public EventHandler f_FileSave;
        public EventHandler f_FileOpen;
        public EventHandler f_ExcelConvert;
        public MainUI()
        {
            InitializeComponent();
            CoreWPF.Common.CommonFunction.ListboxEnableDrag(m_TradeRoutes.m_MapPointListbox);
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            //if (m_TradeRoutesMode == sender)
            //{
            //    m_HarborMode.IsChecked = !m_TradeRoutesMode.IsChecked;
            //    //m_TradeRoutesMode.IsChecked = !m_TradeRoutesMode.IsChecked;
            //}
            //else
            //if (sender == m_HarborMode)
            //{
            //    m_TradeRoutesMode.IsChecked = !m_HarborMode.IsChecked;
            //    //m_HarborMode.IsChecked = !m_HarborMode.IsChecked;
            //}
            //else
            if (sender == m_FileSave)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender,e,f_FileSave);
            }
            else
            if (sender == m_FileOpen)
            {
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_FileOpen);
            }
            else
            if (sender == m_Excel)
            {
                //ExcelTest();
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ExcelConvert);
            }
        }

        private void WindowsFormsHost_KeyDown(object sender, KeyEventArgs e)
        {

        }

        private void WindowsFormsHost_KeyUp(object sender, KeyEventArgs e)
        {

        }

        public void SetGoodSize()
        {
            m_HarborUIInfo.DockWidth = new GridLength(220);
        }

        private void CheckBox_Click(object sender, RoutedEventArgs e)
        {

        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Source is TabControl)
            {
                this.m_TradeRoutes.m_MapPointListbox.Items.Clear();
                foreach (TextBox l_TextBox in this.m_HarborInfo.m_HarborListbox.m_ListBox.Items)
                {
                    TextBox l_NewTextBox = new TextBox();
                    l_NewTextBox.Text = l_TextBox.Text;
                    l_NewTextBox.IsEnabled = false;
                    m_TradeRoutes.m_MapPointListbox.Items.Add(l_NewTextBox);
                }
            }
        }

    }
}
