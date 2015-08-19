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
    /// Interaction logic for PI.xaml
    /// </summary>
    public partial class PI : UserControl
    {
        public event EventHandler f_AddPIButtonClick;
        public event EventHandler f_DelPIButtonClick;
        public event EventHandler f_PIChangeSelection;
        public event EventHandler f_PIUnitChangeSelection;
        //files drop to PIListbox
        public event EventHandler f_PIListBoxDrop;

        public PI()
        {
            InitializeComponent();
        }
        private void AddPI_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_AddPIButtonClick);
        }

        private void DelPI_Click(object sender, RoutedEventArgs e)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_DelPIButtonClick);
        }

        private void PIList_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, args, f_PIChangeSelection);
        }

        private void PIUnit_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, args, f_PIUnitChangeSelection);
        }

        private void m_PIUnit_listBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            object item = m_PIUnit_listBox.SelectedItem;
            if (item != null)
                DragDrop.DoDragDrop(m_PIUnit_listBox, item, DragDropEffects.Move);
        }

        private void PIListBox_DragEnter(object sender, DragEventArgs e)
        {
            CoreWPF.Common.CommonFunction.DragEnter(sender, e);
        }

        private void PIListBox_Drop(object sender, DragEventArgs e)
        {
            //please not the drag events is System::Windows::DragEventArgs
            CoreWPF.Common.CommonFunction.EventDelegateExecute(sender, e, f_PIListBoxDrop);
        }
    }
}
