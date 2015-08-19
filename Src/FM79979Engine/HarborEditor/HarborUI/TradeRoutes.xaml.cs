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

namespace HarborUI
{
    /// <summary>
    /// Interaction logic for TradeRoutes.xaml
    /// </summary>
    public partial class TradeRoutes : UserControl
    {
        public EventHandler f_Add;
        public EventHandler f_Delete;
        public EventHandler f_PointDropEvent;
        public EventHandler f_ListboxValueChanged;
        public TradeRoutes()
        {
            InitializeComponent();
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_Add);
        }
        private void DelButton_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_Delete);
        }

        private void m_StartName_DragOver(object sender, DragEventArgs e)
        {
            e.Handled = true;
            String l_Data = (String)e.Data.GetData("String");
            if ( l_Data != null )
            {
                e.Effects = DragDropEffects.Copy;
            }
            else
                e.Effects = DragDropEffects.None;
        }

        private void m_StartName_Drop(object sender, DragEventArgs e)
        {
            e.Handled = true;
            String l_Data = (String)e.Data.GetData("String");
            if (l_Data != null)
            {
                TextBox l_Textbox = sender as TextBox;
                l_Textbox.Text = l_Data;
                CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_PointDropEvent);
            }
        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_ListboxValueChanged);
        }
    }
}